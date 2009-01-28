#ifndef __MAP_H
#define __MAP_H

/***********************************************************************
 * Map plugin is responsible for a number of things:
 *      1. Building a graph of the map as the user explores
 *      2. Path finding across both the graph and the screen
 *      3. Making sure that if the user attempts to interfere with
 *         the movement process all autowalking features will be
 *         paused so that the user regains control
 ***********************************************************************/

#include "hook.h"
#include "client.h"
#include "messenger.h"
#include "tibiamessage.h"

#include "globalmap.h"

#define MAX_STOP_HOOK 15

class MapRecipricant : public Recipricant
{
        public:
                virtual Args func (const Args& args);
};

/* set on all send messages which will cause the user to take control
 * from the bot */

class StopHook : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class Map
{
        public:
                enum state_t
                {
                        s_play,
                        s_stop,
                        s_pause,
                };

                Map ();

                Args usage ();

                /* used by recipricant */
                std::string     r_set_state (const std::string& state);
                std::string     r_get_state ();

                /* if state is set twice in a cycle set_state returns false */
                /* used internally */
                bool            set_state (state_t state);
                state_t         get_state ();

                bool            get_autodisable ();
                bool            enable_autodisable ();
                bool            disable_autodisable ();

                void i_load (uint32_t pluginId, Client* client);
                void i_unload ();
                const std::string& i_name ();

                LocalMap        global;
                
        private:
                state_t         _state;
                uint32_t        _statecycle;

                bool            _autodisable;

                uint32_t        _stophooks[MAX_STOP_HOOK];
                uint32_t        _pluginId;
                uint32_t        _rid;

                Client*         _client;

                std::string     _name;
};

extern Map map;

extern "C"
{
        void load (uint32_t id, Client* client);
        void unload ();
        const std::string& name ();
}

#endif

