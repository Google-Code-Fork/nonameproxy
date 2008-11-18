#ifndef __ACTION
#define __ACTION

#include <string>
#include <set>
#include <stdint.h>

#include "client.h"
#include "tibiamessage.h"

/******************************************************************************
 * Action provides an interface for sending actions through the messaging
 * system
 * Usage:
 *      action say 'string'
 *
 *****************************************************************************/

class ActionRecipricant : public Recipricant
{
        public:
                virtual Args func (const Args& args);
};

class Action
{
        public:
                Args usage ();

                void send_say (const std::string& msg);
                void i_load (uint32_t id, Client* client);
                void i_unload ();
                const std::string& i_name ();

        private:
                uint32_t        _pluginId;
                uint32_t        _rid;

                Client*         _client;

                std::string     _name;
};

extern "C"
{
        void load (uint32_t id, Client* client);
        void unload ();
        const std::string& name ();
}

#endif

