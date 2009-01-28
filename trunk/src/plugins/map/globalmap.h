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

#include <stdint.h>

#include "mapgraph.h"
#include "localmap.h"

#include "hook.h"
#include "client.h"
#include "pos.h"

#define MAX_MOVE_HOOK 4

class GlobalMap
{
        public:

                GlobalMap ();

                bool set_target (uint32_t x, uint32_t y, uint32_t z);
                bool set_target (const Pos &pos);

                bool at_target ();

                /* kick start the walker, checks if the kick is needed first*/
                bool kick ();
                /* start the walker */
                bool walk ();

                void stop ();

                uint32_t calcPathCost (const Pos &from, const Pos &to);

                void i_load (uint32_t pluginId, Client* client);
                void i_unload ();

                LocalMap local;
        private:
                bool            _walking;

                MapGraph        _globalMap;

                uint32_t        _movehooks[MAX_MOVE_HOOK];
                uint32_t        _pluginId;
                Client*         _client;
};

#endif

