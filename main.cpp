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
        
        LSMLoginMsg* loginmsg;

        while (serverConn->isConnected () || clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
                        //for now read the header and the msg id
                        uint8_t tmp[3];
                        msg->getN (tmp, 3);
                        //and now lets have a little fun
                        loginmsg = new LSMLoginMsg (msg);
                        loginmsg->show ();
                        crypt->setXTEAKey (loginmsg->getXTEA ());
                        crypt->encrypt (msg);
                        serverConn->putMsg (msg);
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
                        crypt->encrypt (msg);
                        clientConn->putMsg (msg);
                }
        }

        delete clientConn;
        delete serverConn;
        return 0;
}

