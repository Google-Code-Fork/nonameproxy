#ifndef __PLUGIN_H
#define __PLUGIN_H

#include <stdint.h>
#include <string>
#include <set>

class Client;

typedef const std::string& (*namefunc) ();
typedef void (*loadfunc) (uint32_t, Client*);
typedef void (*unloadfunc) ();

typedef std::set <uint32_t> idset;

class Plugin
{
        public:
                Plugin ();
                virtual ~Plugin ();

                bool load (uint32_t pluginId, const std::string& path, 
                                Client* client);

                bool unload ();
                const std::string& name ();

                uint32_t getRecipricantId ();
                void     setRecipricantId (uint32_t rid);

                bool     addHookId (uint32_t hid);
                bool     removeHookId (uint32_t hid);

                bool     addConnectionId (uint32_t cid);
                bool     removeConnectionId (uint32_t cid);

        private:
#ifdef WIN32
#else
                void* _handle;
#endif
                /* we keep the client for cleanup purposes */
                Client* _client;

                namefunc   _name;
                loadfunc   _load;
                unloadfunc _unload;

                uint32_t _recipricantId;
                idset _hooks;
                idset _connections;
};
        
#endif

