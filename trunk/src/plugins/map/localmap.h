#ifndef __LOCAL_MAP_H
#define __LOCAL_MAP_H

/***********************************************************************
 * LocalMap deals with the 18x14 portion of the map which is loaded
 * into tibias memory
 ***********************************************************************/

#include "hook.h"
#include "client.h"
#include "pos.h"

#define MAX_MOVE_HOOK 4

class MoveHook : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

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
                /* force the walker to start */
                bool walk ();

                /* stops by setting target to players pos */
                void stop ();

                void i_load (uint32_t pluginId, Client* client);
                void i_unload ();

        private:
                Pos i_globalToLocal (const Pos& pos);
                void i_makeMap (uint32_t m[][18]);
                void i_move (direction_t dir);

                Pos             _to;

                /* we can send 10 moves at a time to the server
                 * so we must split the journey using tmpTo */
                Pos             _tmpTo;
                uint32_t        _movehooks[MAX_MOVE_HOOK];
                uint32_t        _pluginId;
                Client*         _client;
};

#endif

