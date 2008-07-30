#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>

#include "server.h"
#include "connection.h"
#include "networkmessage.h"
#include "connectionmanager.h"

int main (uint32_t argc, char** argv)
{
        Server* server = new Server ();
        server->listenOn (7171);
        Connection* clientConn = server->acceptConnection ();

        sleep (1);
        
        ConnectionManager* connMgr = new ConnectionManager ();
        connMgr->addConnection (clientConn);

        NetworkMessage* msg;
        //while ((msg = clientConn->getMsg ()) == NULL) {
        //        connMgr->selectConnections (1000);
        //}
        //        
        //msg->show ();

        int i = 0;
        while (1) {
                connMgr->selectConnections (125);
                printf ("\n");
                if ((msg = clientConn->getMsg ()) != NULL) {
                        msg->show ();
                        clientConn->putMsg (msg);
                        connMgr->selectConnections (125);
                        break;
                }
                i ++;
        }

        delete clientConn;
        return 0;
}

