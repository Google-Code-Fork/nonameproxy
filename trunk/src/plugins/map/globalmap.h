#ifndef __GLOBAL_MAP_H
#define __GLOBAL_MAP_H

/***********************************************************************
 * GlobalMap deals with the larger map. It does this by storing nodes
 * as the player moves around. on average each screen will have 3 or so
 * nodes. When the player wants to travel to a position across several
 * screens, global map finds a path through the nodes from the closest
 * node to the player to the closest node to the target. It then relies
 * on LocalMap to travel between these nodes.
 ***********************************************************************/

#include "mapgraph.h"

#include "hook.h"
#include "client.h"
#include "pos.h"

#define MAX_MOVE_HOOK 4

class GlobalMap
{
        public:

                GlobalMap ();

                void i_load (uint32_t pluginId, Client* client);
                void i_unload ();

        private:
                MapGraph        _globalMap;

                uint32_t        _movehooks[MAX_MOVE_HOOK];
                uint32_t        _pluginId;
                Client*         _client;
};

#endif

