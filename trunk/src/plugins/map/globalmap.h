#ifndef __GLOBAL_MAP_H
#define __GLOBAL_MAP_H

/***********************************************************************
 * GlobalMap deals with the 18x14 portion of the map which is loaded
 * into tibias memory
 ***********************************************************************/

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
                uint32_t        _movehooks[MAX_MOVE_HOOK];
                uint32_t        _pluginId;
                Client*         _client;
};

#endif

