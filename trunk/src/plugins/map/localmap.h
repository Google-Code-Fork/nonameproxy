#ifndef __LOCAL_MAP_H
#define __LOCAL_MAP_H

/***********************************************************************
 * LocalMap deals with the 18x14 portion of the map which is loaded
 * into tibias memory
 ***********************************************************************/

#include <stdint.h>

#include "hook.h"
#include "client.h"
#include "pos.h"

#define MAX_MOVE_HOOK 4

class LocalMap
{
        public:

                LocalMap ();

                /* returns false if target is out of bounds (screen) */
                bool set_target (uint32_t x, uint32_t y, uint32_t z);
                bool set_target (const Pos& pos);

                bool at_target ();

                /* kick start the walker, checks if the kick is needed first*/
                bool kick ();
                /* start the walker */
                bool walk ();

                void stop ();

                uint32_t calcPathCost (const Pos& from, const Pos& to);

                void i_load (uint32_t pluginId, Client* client);
                void i_unload ();

        private:
                Pos i_globalToLocal (const Pos& pos);

                /* can you believe this is the syntax for returning
                 * multi dimensional arrays ?!?! */
                uint32_t (*i_makeMap (const Pos& from, const Pos& to, 
                                bool ignoreCreatures = false))[MAP_X];
                void i_move (direction_t dir);
                void i_stop ();

                Pos             _to;

                /* in order to avoid creating a map each time we need it
                 * we update it each cycle. note there are two maps, one
                 * for creatures ond one for no creatures */
                uint32_t _c_cycle;
                uint32_t _nc_cycle;

                uint32_t _c_map[MAP_Y][MAP_X];
                uint32_t _nc_map[MAP_Y][MAP_X];

                /* we can send 10 moves at a time to the server
                 * so we must split the journey using tmpTo */
                Pos             _tmpTo;

                /* we dont always want to have the auto walker walking 
                 * so we need a way of turning the walker off */
                bool            _walking;

                uint32_t        _movehooks[MAX_MOVE_HOOK];
                uint32_t        _pluginId;
                Client*         _client;
};

#endif

