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

int main (uint32_t argc, char** argv)
{
        TibiaCrypt* crypt = new TibiaCrypt ();
        crypt->setRSAPublicKey (TIBKEY, TIBMOD);
        crypt->setRSAPrivateKey (OTKEY, OTMOD);
        Server* loginServer = new Server ();
        loginServer->listenOn (1337);
        Connection* clientConn = loginServer->acceptConnection ();
        Connection* serverConn = new Connection ();
        serverConn->connectTo ("tibia01.cipsoft.com", 7171);

        ConnectionManager* connMgr = new ConnectionManager ();
        connMgr->addConnection (clientConn);
        connMgr->addConnection (serverConn);

        NetworkMessage* msg;
        
        LSMessageFactory* lsmf;
        LRMessageFactory* lrmf;
        TibiaMessage* tm;
        WriteHook* hook = new HWCharacterList;
        GameState* dummy;

        while (serverConn->isConnected () || clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
                        //and now lets have a little fun
                        lsmf = new LSMessageFactory (msg);
                        tm = lsmf->getMessage ();
                        tm->show ();
                        crypt->setXTEAKey (((LSMLoginMsg*)tm)->getXTEA ());
                        //there should only be the one login message
                        delete tm;
                        while ((tm = lsmf->getMessage ()) != NULL) {
                                tm->show ();
                                delete tm;
                        }
                        crypt->encrypt (msg);
                        serverConn->putMsg (msg);
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
                        //and some more fun
                        lrmf = new LRMessageFactory (msg);
                        while ((tm = lrmf->getMessage ()) != NULL) {
                                if (tm->getID () == 0x64) {
                                        tm = hook->func (tm, dummy);
                                }
                                tm->show ();
                                delete tm;
                        }
                        crypt->encrypt (msg);
                        clientConn->putMsg (msg);
                }
        }

        delete clientConn;
        delete serverConn;
        return 0;
}

