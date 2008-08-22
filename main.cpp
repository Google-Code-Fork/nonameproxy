#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>

#include "rsakeys.h"
#include "tibiacrypt.h"
#include "server.h"
#include "connection.h"
#include "networkmessage.h"
#include "connectionmanager.h"
#include "tibiamessage.h"
#include "hook.h"
#include "corehooks.h"
#include "loginstate.h"
#include "gamestate.h"
#include "messagelist.h"

int main (uint32_t argc, char** argv)
{
        TibiaCrypt* crypt = new TibiaCrypt ();
        crypt->setRSAPublicKey (TIBKEY, TIBMOD);
        //crypt->setRSAPublicKey (TIBKEY, OTMOD);
        crypt->setRSAPrivateKey (OTKEY, OTMOD);
        Server* loginServer = new Server ();
        loginServer->listenOn (1337);
        Connection* clientConn = loginServer->acceptConnection ();
        Connection* serverConn = new Connection ();
        serverConn->connectTo ("login02.tibia.com", 7171);

        ConnectionManager* connMgr = new ConnectionManager ();
        connMgr->addConnection (clientConn);
        connMgr->addConnection (serverConn);

        LoginState* lstate = new LoginState ();
        GameState*  gstate = new GameState (lstate);

        NetworkMessage* msg;
        NetworkMessage* newmsg;
        
        LSMessageList* lsml;
        LRMessageList* lrml;

        TibiaMessage* tm;
        WriteHook* hook = new HWCharacterList;

        while (serverConn->isConnected () || clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        //and now lets have a little fun
                        //here we pass control of the msg to lsml
                        lsml = new LSMessageList (msg);
                        tm = lsml->read ();
                        tm->show ();
                        lsml->next ();
                        crypt->setXTEAKey (((LSMLoginMsg*)tm)->getXTEA ());
                        //there should only be the one login message
                        while (!lsml->isEnd ()) {
                                tm = lsml->read ();
                                tm->show ();
                                lsml->next ();
                        }
                        msg = lsml->put ();
                        crypt->encrypt (msg);
                        serverConn->putMsg (msg);
                        delete lsml;
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        //and some more fun
                        lrml = new LRMessageList (msg);
                        while (!lrml->isEnd ()) {
                                tm = lrml->read ();
                                if (tm->getID () == 0x64) {
                                        tm = hook->func (tm, gstate);
                                        lrml->replace (tm);
                                }
                                tm->show ();
                                lrml->next ();
                        }
                        msg = lrml->put ();
                        crypt->encrypt (msg);
                        clientConn->putMsg (msg);
                        delete lrml;
                }
        }

        delete clientConn;
        delete serverConn;
        return 0;
}

