#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>

#include "client.h"
#include "rsakeys.h"
#include "networkmessage.h"
#include "connectionmanager.h"
#include "tibiamessage.h"
#include "hook.h"
#include "corehooks.h"
#include "hookmanager.h"
#include "messagelist.h"

#define CONN_TIMEOUT 5000

Client::Client (LoginState* ls)
{
        lstate = ls;
        gstate = new GameState ();
        dat = new DatReader ();

        serverConn = NULL;
        clientConn = NULL;
        connMgr = NULL;

        crypt = new TibiaCrypt ();
        //i guess there is some redundancy having an RSA module
        //for each client, but who cares ?
        crypt->setRSAPublicKey (TIBKEY, TIBMOD);
        crypt->setRSAPrivateKey (OTKEY, OTMOD);

        recvHM = NULL;
        sendHM = NULL;
}        

Client::~Client ()
{
        delete gstate;
        delete dat;

        delete serverConn;
        delete clientConn;
        delete connMgr;

        delete recvHM;
        delete sendHM;
}

bool Client::runLogin (Connection* acceptedConn)
{
        //first initialize the core hooks, im not going to add support for
        //user defined login hooks. All we have to do is load the core hooks
        sendHM = new HookManager ();
        sendHM->addReadHook (0x01, (ReadHook*)(new HRLoginMsg));

        recvHM = new HookManager ();
        recvHM->addWriteHook (0x64, (WriteHook*)(new HWCharacterList));
        //im only adding this hook for fun
        recvHM->addWriteHook (0x14, (WriteHook*)(new HWMOTD));

        //set up the connection
        Connection* clientConn = acceptedConn;
        Connection* serverConn = new Connection ();
        //need to add support for all login servers
        if (!serverConn->connectTo ("login02.tibia.com", 7171)) {
                printf ("error: failed to connect to login server\n");
                return false;
        }

        ConnectionManager* connMgr = new ConnectionManager ();
        connMgr->addConnection (clientConn);
        connMgr->addConnection (serverConn);

        NetworkMessage* msg;
        //This loop will only terminate after both connections have terminated
        //Although this is less than ideal, it should work
        while (serverConn->isConnected () && clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        LSMessageList* lsml = new LSMessageList (msg, gstate, dat);
                        while (!lsml->isEnd ()) {
                                TibiaMessage* tm = lsml->read ();
                                sendHM->hookReadMessage (tm, this);
                                tm = sendHM->hookWriteMessage (tm, this);
                                lsml->replace (tm);
                                tm->show ();
                                lsml->next ();
                        }
                        msg = lsml->put ();
                        crypt->encrypt (msg);
                        serverConn->putMsg (msg);
                        delete lsml;
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        msg->show ();
                        crypt->decrypt (msg);
                        msg->show ();
                        LRMessageList* lrml = new LRMessageList (msg, gstate, dat);
                        while (!lrml->isEnd ()) {
                                TibiaMessage* tm = lrml->read ();
                                recvHM->hookReadMessage (tm, this);
                                tm = recvHM->hookWriteMessage (tm, this);
                                lrml->replace (tm);
                                tm->show ();
                                lrml->next ();
                        }
                        msg = lrml->put ();
                        msg->show ();
                        crypt->encrypt (msg);
                        msg->show ();
                        clientConn->putMsg (msg);
                        delete lrml;
                        break;
                }
        }

        //this is a dodgy hack to flush the msg left over from the break statement
        connMgr->selectConnections (125);

        delete connMgr;
        delete clientConn;
        delete serverConn;
        delete recvHM;
        delete sendHM;
        clientConn = NULL;
        serverConn = NULL;
        recvHM = NULL;
        sendHM = NULL;

        return true;
}

bool Client::runGame (Connection* acceptedConn)
{
        sendHM = new HookManager ();
        sendHM->addReadHook (0x0A, (ReadHook*)(new HRGameInit));

        recvHM = new HookManager ();
        //set up the connection
        Connection* clientConn = acceptedConn;
        Connection* serverConn = new Connection ();

        ConnectionManager* connMgr = new ConnectionManager ();
        connMgr->addConnection (clientConn);

        NetworkMessage* msg;

        //in order to connect to the game server we need to retrieve the
        //original ip address but in order to do that we need to first
        //recv a packet from the client
        while (clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
                        GSMessageList* gsml = new GSMessageList (msg, gstate, dat);
                        while (!gsml->isEnd ()) {
                                TibiaMessage* tm = gsml->read ();
                                sendHM->hookReadMessage (tm, this);
                                tm = sendHM->hookWriteMessage (tm, this);
                                gsml->replace (tm);
                                tm->show ();
                                gsml->next ();
                        }
                        msg = gsml->put ();
                        msg->show ();
                        crypt->encrypt (msg);
                        //now we want to send the message to the server, but
                        //we have to connect to the server first, so well
                        //save the message
                        delete gsml;
                        break;
                }
        }
        LoginDetails* ld = lstate->getAccountDetails
                        (gstate->account->getAccount());

        TCharacter* connChar = ld->getCharByName
                (gstate->character->getName ());

        //and now we can finally connect
        if (!serverConn->connectTo (connChar->getIp (), connChar->getPort ())) {
                printf ("error: failed to connect to game server\n");
                return false;
        }

        //and dont forget to add it to the connection manager
        connMgr->addConnection (serverConn);
        serverConn->putMsg (msg);

        //and finally the main loop
        while (serverConn->isConnected () || clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
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
        delete recvHM;
        delete sendHM;
        clientConn = NULL;
        serverConn = NULL;
        recvHM = NULL;
        sendHM = NULL;

        return true;
}

