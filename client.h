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

#ifndef __CLIENT_H
#define __CLIENT_H

#include "tibiacrypt.h"
#include "connection.h"
#include "gamestate.h"
#include "loginstate.h"
#include "datreader.h"
#include "messenger.h"
#include "pluginmanager.h"
#include "plugin.h"
#include "messagelist.h"
#include "corerecipricant.h"

class Connection;
class ConnectionManager;
class TibiaCrypt;
class HookManager;
class PacketHookManager;

class Client
{
        public:
                Client (LoginState* ls);
                virtual ~Client ();
                bool runLogin (Connection* acceptedConn);
                bool runGame (Connection* acceptedConn);

                /* these definitions are part of the Client standard and
                 * by declaring anything here you make a promise that 
                 * these classes will be initialized when client is initialized.
                 * The only exceptions are the connections which may be
                 * initialized on a call to runLogin or runGame */

                LoginState*     lstate;
                GameState*      gstate;
                DatReader*      dat;

                /* Tibia crypt stores certain infomataton, namely the xtea
                 * key, therefore its in the same 'league' as gamestate */

                TibiaCrypt*     crypt;

                /* some plugins will require them to reconize when they are
                 * in a particular select cycle. For example, if multiple
                 * hooks are set and we only want at most one to be called
                 * in a single cycle. Cycle ids loop every 4294967296 cycles */
                uint32_t getCycle ();

                /* these functions are wrappers for plugin management 
                 * functions accessable from external plugins */

                uint32_t getPluginByName (const std::string& msg);

                Args sendMessage (uint32_t pid, const std::string& msg);

                /* sends a message to the plugin whos name is the first arg */
                Args broadcastMessage (const std::string& msg);

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

                uint32_t addPreSendPacketHook (uint32_t pid, PacketHook* hook);
                uint32_t addPostSendPacketHook (uint32_t pid, PacketHook* hook);
                uint32_t addPreRecvPacketHook (uint32_t pid, PacketHook* hook);
                uint32_t addPostRecvPacketHook (uint32_t pid, PacketHook* hook);

                void deletePreSendPacketHook (uint32_t pid, uint32_t hid);
                void deletePostSendPacketHook (uint32_t pid, uint32_t hid);
                void deletePreRecvPacketHook (uint32_t pid, uint32_t hid);
                void deletePostRecvPacketHook (uint32_t pid, uint32_t hid);

                uint32_t addRecipricant (uint32_t pid, Recipricant* recipricant);
                void     deleteRecipricant (uint32_t pid, uint32_t rid);

                void sendToClient (GRMessageList& msgs);
                void sendToServer (GSMessageList& msgs);

                /* since console will be a core plugin i think it makes sense
                 * make it available to other plugins. */
                uint32_t getConsoleId ();

                friend class CoreRecipricant;
        private:
                void addProtocolHooks ();

                ConnectionManager*      connMgr;
                Connection*             serverConn;
                Connection*             clientConn;

                Messenger*              messenger;

                HookManager*            sendHM;
                HookManager*            recvHM;
                HookManager*            recvProtocol;

                PacketHookManager*      sendPHM;
                PacketHookManager*      recvPHM;

                PluginManager*          pluginManager;

                uint32_t                _cycle;
                uint32_t                _consoleId;
};
#endif

