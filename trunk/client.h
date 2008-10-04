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

class Connection;
class ConnectionManager;
class TibiaCrypt;
class HookManager;

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

                /* these functions are wrappers for plugin management 
                 * functions accessable from external plugins */

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
                void addProtocolHooks ();

                ConnectionManager*      connMgr;
                Connection*             serverConn;
                Connection*             clientConn;


                Messenger*              messenger;

                HookManager*            recvHM;
                HookManager*            sendHM;
                HookManager*            recvProtocol;

                PluginManager*          pluginManager;
};
#endif

