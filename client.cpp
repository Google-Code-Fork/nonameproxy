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
#include "enums.h"

#define CONN_TIMEOUT 5000

Client::Client (LoginState* ls)
{
        lstate = ls;
        gstate = new GameState ();
        dat = new DatReader ();
        messenger = new Messenger ();

        serverConn = NULL;
        clientConn = NULL;

        crypt = new TibiaCrypt ();
        //i guess there is some redundancy having an RSA module
        //for each client, but who cares ?
        crypt->setRSAPublicKey (TIBKEY, TIBMOD);
        crypt->setRSAPrivateKey (OTKEY, OTMOD);

        sendHM = new HookManager ();
        recvHM = new HookManager ();
        recvProtocol = new HookManager ();
        connMgr = new ConnectionManager ();
        pluginManager = new PluginManager (messenger, recvHM, sendHM, 
                                           connMgr, this);

}        

Client::~Client ()
{
        delete pluginManager;

        delete gstate;
        delete dat;

        delete serverConn;
        delete clientConn;
        delete connMgr;

        delete messenger;

        delete recvHM;
        delete sendHM;
        delete recvProtocol;
        delete crypt;
}

bool Client::runLogin (Connection* acceptedConn)
{
        /* first initialize the core hooks, im not going to add support for
         * user defined login hooks. All we have to do is load the core hooks */
        sendHM->addReadHook (0x01, (ReadHook*)(new HRLoginMsg));
        recvHM->addWriteHook (0x64, (WriteHook*)(new HWCharacterList));

        /* im only adding this hook for fun */
        recvHM->addWriteHook (0x14, (WriteHook*)(new HWMOTD));

        /* set up the connection */
        Connection* clientConn = acceptedConn;
        Connection* serverConn = new Connection ();
        /* TODO need to add support for all login servers */
        if (!serverConn->connectTo ("login02.tibia.com", 7171)) {
                printf ("error: failed to connect to login server\n");
                return false;
        }
        connMgr->addConnection (clientConn);
        connMgr->addConnection (serverConn);

        /* This loop will only terminate after both connections have terminated
         * Although this is less than ideal, it should work */
        NetworkMessage* msg;
        while (serverConn->isConnected () && clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        //msg->show ();
                        LSMessageList* lsml = new LSMessageList (msg, gstate, dat);
                        while (!lsml->isEnd ()) {
                                TibiaMessage* tm = lsml->read ();
                                sendHM->hookReadMessage (tm, this);
                                tm = sendHM->hookWriteMessage (tm, this);
                                if (tm == NULL) {
                                        lsml->remove ();
                                } else {
                                        lsml->replace (tm);
                                        lsml->next ();
                                }
                        }
                        msg = lsml->put ();
                        //msg->show ();
                        crypt->encrypt (msg);
                        serverConn->putMsg (msg);
                        delete lsml;
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        //msg->show ();
                        crypt->decrypt (msg);
                        //msg->show ();
                        LRMessageList* lrml = new LRMessageList (msg, gstate, dat);
                        while (!lrml->isEnd ()) {
                                TibiaMessage* tm = lrml->read ();
                                recvHM->hookReadMessage (tm, this);
                                tm = recvHM->hookWriteMessage (tm, this);
                                if (tm == NULL) {
                                        lrml->remove ();
                                } else {
                                        lrml->replace (tm);
                                        lrml->next ();
                                }
                        }
                        msg = lrml->put ();
                        //msg->show ();
                        crypt->encrypt (msg);
                        //msg->show ();
                        clientConn->putMsg (msg);
                        delete lrml;
                        break;
                }
        }

        /* this is a dodgy hack to flush the msg left
         * over from the break statement */
        connMgr->selectConnections (125);

        return true;
}

bool Client::runGame (Connection* acceptedConn)
{
        /* first initialize the core hooks, im not going to add support for
         * user defined login hooks. All we have to do is load the core hooks */
        sendHM->addReadHook (0x0A, (ReadHook*)(new HRGameInit));
        addProtocolHooks ();

        /* set up the connection */
        Connection* clientConn = acceptedConn;
        Connection* serverConn = new Connection ();

        connMgr->addConnection (clientConn);

        /* TEST PLUGIN CODE */
        /*std::string dummypath = "./plugins/dummy/dummy.so";
        uint32_t dummyid = pluginManager->addPlugin (dummypath);
        sendMessage (dummyid, "this is a test, an epic test");
        pluginManager->deletePlugin (dummyid);

        std::string channelpath = "./plugins/channelmanager/channelmanager.so";
        uint32_t cmid = pluginManager->addPlugin (channelpath);
        sendMessage (cmid, "channelmanager add test 1234");
        */
        /* in order to connect to the game server we need to retrieve the
         * original ip address but in order to do that we need to first
         * recv a packet from the client */
        NetworkMessage* msg;
        while (clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        //msg->show ();
                        GSMessageList* gsml = new GSMessageList (msg, gstate, dat);
                        while (!gsml->isEnd ()) {
                                TibiaMessage* tm = gsml->read ();
                                sendHM->hookReadMessage (tm, this);
                                tm = sendHM->hookWriteMessage (tm, this);
                                if (tm == NULL) {
                                        gsml->remove ();
                                } else {
                                        gsml->replace (tm);
                                        gsml->next ();
                                }
                        }
                        msg = gsml->put ();
                        //msg->show ();
                        crypt->encrypt (msg);
                        /* now we want to send the message to the server, but
                         * we have to connect to the server first, so well
                         * save the message */
                        delete gsml;
                        break;
                }
        }
        LoginDetails* ld = lstate->getAccountDetails
                        (gstate->account->getAccount());

        TCharacter* connChar = ld->getCharByName
                (gstate->account->getName ());

        /* and now we can finally connect */
        if (!serverConn->connectTo (connChar->getIp (), connChar->getPort ())) {
                printf ("error: failed to connect to game server\n");
                return false;
        }

        /* and dont forget to add it to the connection manager */
        connMgr->addConnection (serverConn);
        serverConn->putMsg (msg);

        /* and finally the main loop */
        while (serverConn->isConnected () || clientConn->isConnected ()) {
                connMgr->selectConnections (125);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        msg->show ();
                        GSMessageList gsml (msg, gstate, dat);
                        while (!gsml.isEnd ()) {
                                TibiaMessage* tm = gsml.read ();
                                gsml.next ();

                                //recvProtocol->hookReadMessage (tm, this);
                                //recvHM->hookReadMessage (tm, this);
                                //tm = recvHM->hookWriteMessage (tm, this);
                        }
                        msg = gsml.put ();
                        crypt->encrypt (msg);
                        serverConn->putMsg (msg);
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        //msg->show ();
                        GRMessageList* grml = new GRMessageList (msg, gstate, dat);
                        while (!grml->isEnd ()) {
                                TibiaMessage* tm = grml->read ();

                                recvProtocol->hookReadMessage (tm, this);
                                recvHM->hookReadMessage (tm, this);
                                tm = recvHM->hookWriteMessage (tm, this);
                                if (tm == NULL) {
                                        grml->remove ();
                                } else {
                                        grml->replace (tm);
                                        grml->next ();
                                }
                        }
                        msg = grml->put ();
                        //msg->show ();
                        crypt->encrypt (msg);
                        clientConn->putMsg (msg);
                        delete grml;
                }
        }

        return true;
}

void Client::addProtocolHooks ()
{
        recvProtocol->addReadHook (GRM_MAP_INIT_ID, (ReadHook*)new GRHMapInit);
        recvProtocol->addReadHook (GRM_MAP_NORTH_ID, (ReadHook*)new GRHMapNorth);
        recvProtocol->addReadHook (GRM_MAP_EAST_ID, (ReadHook*)new GRHMapEast);
        recvProtocol->addReadHook (GRM_MAP_SOUTH_ID, (ReadHook*)new GRHMapSouth);
        recvProtocol->addReadHook (GRM_MAP_WEST_ID, (ReadHook*)new GRHMapWest);
        recvProtocol->addReadHook (GRM_MAP_UP_ID, (ReadHook*)new GRHMapUp);
        recvProtocol->addReadHook (GRM_MAP_DOWN_ID, (ReadHook*)new GRHMapDown);
}

/************************************************************************
 * Plugin Wrappers
 ************************************************************************/

void Client::sendMessage (uint32_t pid, const std::string& msg)
{
        pluginManager->sendMessage (pid, msg);
}

uint32_t Client::addRecvReadHook (uint32_t pid, uint8_t id, ReadHook* hook)
{
        return pluginManager->addRecvReadHook (pid, id, hook);
}

uint32_t Client::addRecvWriteHook (uint32_t pid, uint8_t id, WriteHook* hook)
{
        return pluginManager->addRecvWriteHook (pid, id, hook);
}

uint32_t Client::addSendReadHook (uint32_t pid, uint8_t id, ReadHook* hook)
{
        return pluginManager->addSendReadHook (pid, id, hook);
}

uint32_t Client::addSendWriteHook (uint32_t pid, uint8_t id, WriteHook* hook)
{
        return pluginManager->addSendWriteHook (pid, id, hook);
}

void Client::deleteRecvReadHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deleteRecvReadHook (pid, hid);
}

void Client::deleteRecvWriteHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deleteRecvWriteHook (pid, hid);
}

void Client::deleteSendReadHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deleteSendReadHook (pid, hid);
}

void Client::deleteSendWriteHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deleteSendWriteHook (pid, hid);
}

uint32_t Client::addRecipricant (uint32_t pid, Recipricant* recipricant)
{
        return pluginManager->addRecipricant (pid, recipricant);
}

void Client::deleteRecipricant (uint32_t pid, uint32_t rid)
{
        pluginManager->deleteRecipricant (pid, rid);
}

