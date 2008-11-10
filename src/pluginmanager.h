/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

#ifndef __PLUGIN_MANAGER_H
#define __PLUGIN_MANAGER_H

#include <stdint.h>
#include <map>
#include <string>

#include "idmanager.h"
#include "plugin.h"
#include "hookmanager.h"
#include "packethookmanager.h"
#include "messenger.h"
#include "connectionmanager.h"

typedef std::map<uint32_t, Plugin*> PluginList;

/* plugin manager doesnt know about client, this avoids circular dependancies */
class Client;

class PluginManager
{
        public:
                PluginManager (Messenger* messenger,
                               HookManager* sendhm,
                               HookManager* recvhm,
                               PacketHookManager* sendphm,
                               PacketHookManager* recvphm,
                               ConnectionManager* connMgr,
                               Client* client);

                virtual ~PluginManager ();

                /* Add plugin returns a unique id for the hook just added */
                uint32_t addPlugin    (const std::string& path);
                bool     deletePlugin (uint32_t pid);

                /* a hack to allow the core to add a recipricant */
                uint32_t addFakein    (const std::string& name);

                uint32_t getPluginByName      (const std::string& name);

                /* Interfacing functions */
                /* messageing functions */
                Args sendMessage (uint32_t pid, const std::string& msg);
                Args broadcastMessage (const std::string& msg);

                uint32_t addRecipricant (uint32_t pid, Recipricant* recipricant);
                void     deleteRecipricant (uint32_t pid, uint32_t rid);

                /* tibia message hooks */
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

                /* packet hooks */
                uint32_t addPreSendPacketHook (uint32_t pid, PacketHook* hook);
                uint32_t addPostSendPacketHook (uint32_t pid, PacketHook* hook);
                uint32_t addPreRecvPacketHook (uint32_t pid, PacketHook* hook);
                uint32_t addPostRecvPacketHook (uint32_t pid, PacketHook* hook);

                void deletePreSendPacketHook (uint32_t pid, uint32_t hid);
                void deletePostSendPacketHook (uint32_t pid, uint32_t hid);
                void deletePreRecvPacketHook (uint32_t pid, uint32_t hid);
                void deletePostRecvPacketHook (uint32_t pid, uint32_t hid);
        private:
                PluginList plist;
                IdManager* ids;

                Messenger*              _messenger;
                HookManager*            _sendhm;
                HookManager*            _recvhm;
                PacketHookManager*      _sendphm;
                PacketHookManager*      _recvphm;
                ConnectionManager*      _connMgr;
                Client*                 _client;
};

#endif

