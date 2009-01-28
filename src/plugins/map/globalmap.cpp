#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include <map>
#include <set>
#include <list>

#include "localmap.h"
#include "map.h"
#include "enums.h"

#define BUFFER_SIZE 256 

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define MIN_COST 100

typedef std::list<Pos> NodeList;
typedef std::map<uint32_t, NodeList> NodeMap;
typedef std::pair<uint32_t, NodeList> ListPair;
typedef std::set<Pos, bool (*) (const Pos&, const Pos&)> NodeSet;
typedef std::list<direction_t> Path;

class MoveHook : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};


Path* findPath (Pos start, Pos end, uint32_t[][18]);

void MoveHook::func (TibiaMessage* tm, Client* client)
{
        map.global.kick ();
}

GlobalMap::GlobalMap ()
{
        for (uint32_t i = 0; i < MAX_MOVE_HOOK; i ++) {
                _movehooks[i] = 0;
        }
        _walking = false;
}

bool GlobalMap::at_target ()
{
        return local.at_target ();
}

void GlobalMap::stop ()
{
        local.stop ();
}

bool GlobalMap::kick ()
{
        return local.kick ();
}

bool GlobalMap::walk ()
{
        return local.walk ();
}

uint32_t GlobalMap::calcPathCost (const Pos& from, const Pos& to)
{
        return local.calcPathCost (from, to);
}

bool GlobalMap::set_target (uint32_t x, uint32_t y, uint32_t z)
{
        return local.set_target (x, y, z);
}

bool GlobalMap::set_target (const Pos& pos)
{
        return set_target (pos.x, pos.y, pos.z);
}
        
void GlobalMap::i_load (uint32_t pluginId, Client* client)
{
        local.i_load (pluginId, client);

        _client = client;
        _pluginId = pluginId;

        _movehooks[0] = _client->addRecvReadHook (_pluginId, GRM_MAP_NORTH_ID,
                                                        new MoveHook);
        _movehooks[1] = _client->addRecvReadHook (_pluginId, GRM_MAP_EAST_ID,
                                                        new MoveHook);
        _movehooks[2] = _client->addRecvReadHook (_pluginId, GRM_MAP_SOUTH_ID,
                                                        new MoveHook);
        _movehooks[3] = _client->addRecvReadHook (_pluginId, GRM_MAP_WEST_ID,
                                                        new MoveHook);
}

void GlobalMap::i_unload ()
{
        local.i_unload ();

        for (uint32_t i = 0; i != MAX_MOVE_HOOK; i ++) {
                _client->deleteRecvReadHook (_pluginId, _movehooks[i]);
        }
}

