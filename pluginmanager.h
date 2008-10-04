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

typedef std::map<uint32_t, Plugin*> PluginList;

/* plugin manager doesnt know about client, this avoids circular dependancies */
class Client;

class PluginManager
{
        public:
                PluginManager (Messenger* messenger,
                               HookManager* recvhm,
                               HookManager* sendhm,
                               ConnectionManager* connMgr,
                               Client* client);

                virtual ~PluginManager ();

                /* Add plugin returns a unique id for the hook just added */
                uint32_t addPlugin    (const std::string& path);
                void     deletePlugin (uint32_t pid);

                uint32_t getPluginByName      (const std::string& name);

                /* Interfacing functions */
                void sendMessage (uint32_t pid, const std::string& msg);

                uint32_t addRecvReadHook (uint32_t pid, uint8_t id,
                                                        ReadHook* hook);
                uint32_t addRecvWriteHook (uint32_t pid, uint8_t id,
                                                        WriteHook* hook);
                uint32_t addSendReadHook (uint32_t pid, uint8_t id,
                                                        ReadHook* hook);
                uint32_t addSendWriteHook (uint32_t pid, uint8_t id,
                                                        WriteHook* hook);
                void deleteRecvReadHook (uint32_t pid, uint32_t hid);
                void deleteRecvWriteHook (uint32_t pid, uint32_t hid);
                void deleteSendReadHook (uint32_t pid, uint32_t hid);
                void deleteSendWriteHook (uint32_t pid, uint32_t hid);
        private:
                PluginList plist;
                IdManager* ids;

                Messenger*              _messenger;
                HookManager*            _recvhm;
                HookManager*            _sendhm;
                ConnectionManager*      _connMgr;
                Client*                 _client;
};

#endif

