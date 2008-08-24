#ifndef __CLIENT_H
#define __CLIENT_H

#include "tibiacrypt.h"
#include "connection.h"
#include "gamestate.h"
#include "loginstate.h"

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
                bool runGame ();
                //these definitions are part of the Client standard and
                //by declaring anything here you make a promise that 
                //these classes will be initialized when client is initialized.
                //The only exceptions are the connections which may be
                //initialized on a call to runLogin or runGame
                LoginState*     lstate;
                GameState*      gstate;

                Connection*     serverConn;
                Connection*     clientConn;

                TibiaCrypt*     crypt;
        private:
                ConnectionManager*      connMgr;
                HookManager*            recvHM;
                HookManager*            sendHM;
};
#endif

