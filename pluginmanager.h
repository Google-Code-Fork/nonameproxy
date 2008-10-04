#ifndef __PLUGIN_MANAGER_H
#define __PLUGIN_MANAGER_H

#include <stdint.h>
#include <map>
#include <string>

#include "idmanager.h"
#include "plugin.h"
#include "hookmanager.h"
#include "messenger.h"
#include "connectionmanager.h"

typedef std::map<uint32_t, Plugin> PluginList;

class PluginManager
{
        public:
                PluginManager ();
                virtual ~PluginManager ();

                /* Add plugin returns a unique id for the hook just added */
                uint32_t addPlugin    (Plugin* plugin);
                void     deletePlugin (uint32_t pid);

                uint32_t getPluginByName      (const std::string& name);

        private:
                PluginList plist;
                IdManager* ids;

                Messenger*              messenger;
                HookManager*            recvhm;
                HookManager*            sendhm;
                ConnectionManager*      connMgr;
};

#endif

