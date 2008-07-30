#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>

#include "rsakeys.h"
#include "tibiacrypt.h"
#include "server.h"
#include "connection.h"
#include "networkmessage.h"
#include "connectionmanager.h"

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

        while (serverConn->isConnected () || clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                printf ("\n");
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
                        crypt->encrypt (msg);
                        serverConn->putMsg (msg);
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        msg->show ();
                        clientConn->putMsg (msg);
                }
                printf ("loop\n");
        }

        delete clientConn;
        delete serverConn;
        return 0;
}

