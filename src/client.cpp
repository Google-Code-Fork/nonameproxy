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

#include <stdint.h>

#include "client.h"
#include "rsakeys.h"
#include "networkmessage.h"
#include "connectionmanager.h"
#include "tibiamessage.h"
#include "hook.h"
#include "corehooks.h"
#include "hookmanager.h"
#include "enums.h"

#define CONN_TIMEOUT 5000
#define SELECT_SLEEP 0

Client::Client (LoginState* ls)
{
        lstate = ls;
        gstate = new GameState ();
        dat = new DatReader ();
        messenger = new Messenger (this);

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
        sendPHM = new PacketHookManager ();
        recvPHM = new PacketHookManager ();
        connMgr = new ConnectionManager ();

        pluginManager = new PluginManager (messenger, sendHM, recvHM, sendPHM,
                                           recvPHM, connMgr, this);

        _consoleId = 0;
        _cycle = 0;
}        

Client::~Client ()
{
        delete pluginManager;

        delete gstate;
        delete dat;

        /* connMgr deletes all its connections */
        delete connMgr;

        delete messenger;

        delete sendHM;
        delete recvHM;
        delete sendPHM;
        delete recvPHM;
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
        clientConn = acceptedConn;
        serverConn = new Connection ();
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
                _cycle ++;
                connMgr->selectConnections (SELECT_SLEEP);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
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
                        /* msg may be null if there are now messages */
                        if (msg) {
                                crypt->encrypt (msg);
                                serverConn->putMsg (msg);
                        }
                        delete lsml;
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
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
                        /* msg may be null if there are no messages */
                        if (msg) {
                                crypt->encrypt (msg);
                                clientConn->putMsg (msg);
                        }
                        delete lrml;
                        break;
                }
        }

        /* this is a dodgy hack to flush the msg left
         * over from the break statement */
        connMgr->selectConnections (SELECT_SLEEP);

        return true;
}

bool Client::runGame (Connection* acceptedConn)
{
        /* first initialize the core hooks, im not going to add support for
         * user defined login hooks. All we have to do is load the core hooks */
        sendHM->addReadHook (0x0A, (ReadHook*)(new HRGameInit));
        addProtocolHooks ();

        /* set up the connection */
        clientConn = acceptedConn;
        serverConn = new Connection ();

        connMgr->addConnection (clientConn);

        /* TEST PLUGIN CODE */
        /*std::string dummypath = "./plugins/dummy/dummy.so";
        uint32_t dummyid = pluginManager->addPlugin (dummypath);
        sendMessage (dummyid, "this is a test, an epic test");
        pluginManager->deletePlugin (dummyid);*/

        uint32_t coreid = pluginManager->addFakein ("core");
        addRecipricant (coreid, new CoreRecipricant (this));

#ifdef WIN32
        std::string channelpath = "channelmanager.dll";
        uint32_t cmid = pluginManager->addPlugin (channelpath);

        std::string consolepath = "console.dll";
        _consoleId = pluginManager->addPlugin (consolepath);
#else
        std::string channelpath = "./plugins/channelmanager/channelmanager.so";
        uint32_t cmid = pluginManager->addPlugin (channelpath);
        
        std::string consolepath = "./plugins/console/console.so";
        _consoleId = pluginManager->addPlugin (consolepath);
#endif
        
        /* in order to connect to the game server we need to retrieve the
         * original ip address but in order to do that we need to first
         * recv a packet from the client */
        NetworkMessage* msg;
        while (clientConn->isConnected ()) {
                _cycle ++;
                connMgr->selectConnections (SELECT_SLEEP);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
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
        
        /* msg may be null if there are no messages */
        if (!msg) {
                printf ("client error: first game recv packet ");
                printf ("empty\n");
                return false;
        }
        serverConn->putMsg (msg);

        /* and finally the main loop */
        while (serverConn->isConnected () || clientConn->isConnected ()) {
                _cycle ++;
                connMgr->selectConnections (SELECT_SLEEP);
                if ((msg = clientConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        sendPHM->hookPrePacket (*msg);
                        GSMessageList gsml (msg, gstate, dat);
                        while (!gsml.isEnd ()) {
                                TibiaMessage* tm = gsml.read ();

                                sendHM->hookReadMessage (tm, this);
                                tm = sendHM->hookWriteMessage (tm, this);
                                if (tm == NULL) {
                                        gsml.remove ();
                                } else {
                                        gsml.replace (tm);
                                        gsml.next ();
                                }
                        }
                        msg = gsml.put ();
                        /* msg may be null if there are no messages */
                        if (msg) {
                                sendPHM->hookPostPacket (*msg);
                                crypt->encrypt (msg);
                                serverConn->putMsg (msg);
                        }
                }
                if ((msg = serverConn->getMsg ()) != NULL) {
                        crypt->decrypt (msg);
                        recvPHM->hookPrePacket (*msg);
                        
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
                        /* msg may be null if there are no messages */
                        if (msg) {
                                recvPHM->hookPostPacket (*msg);
                                crypt->encrypt (msg);
                                clientConn->putMsg (msg);
                        }
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

        recvProtocol->addReadHook (GRM_TILE_SET_ID,
                                (ReadHook*)new GRHTileSet);
        recvProtocol->addReadHook (GRM_TILE_ADD_ID,
                                (ReadHook*)new GRHTileAdd);
        recvProtocol->addReadHook (GRM_TILE_UPDATE_ID,
                                (ReadHook*)new GRHTileUpdate);
        recvProtocol->addReadHook (GRM_TILE_REMOVE_ID,
                                (ReadHook*)new GRHTileRemove);
        recvProtocol->addReadHook (GRM_CREATURE_MOVE_ID, 
                                (ReadHook*)new GRHCreatureMove);

        recvProtocol->addReadHook (GRM_PLAYER_STATS_ID,
                                (ReadHook*)new GRHPlayerStats);
        recvProtocol->addReadHook (GRM_PLAYER_SKILLS_ID,
                                (ReadHook*)new GRHPlayerSkills);

        recvProtocol->addReadHook (GRM_CONTAINER_OPEN_ID,
                                (ReadHook*)new GRHContainerOpen);
        recvProtocol->addReadHook (GRM_CONTAINER_CLOSE_ID,
                                (ReadHook*)new GRHContainerClose);
        recvProtocol->addReadHook (GRM_CONTAINER_ADD_ID,
                                (ReadHook*)new GRHContainerAdd);
        recvProtocol->addReadHook (GRM_CONTAINER_UPDATE_ID,
                                (ReadHook*)new GRHContainerUpdate);
        recvProtocol->addReadHook (GRM_CONTAINER_REMOVE_ID,
                                (ReadHook*)new GRHContainerRemove);

        recvProtocol->addReadHook (GRM_SLOT_ITEM_ID, 
                                (ReadHook*)new GRHSlotItem);
        recvProtocol->addReadHook (GRM_SLOT_CLEAR_ID, 
                                (ReadHook*)new GRHSlotClear);
}

/************************************************************************
 * Plugin Wrappers
 ************************************************************************/

uint32_t Client::getPluginByName (const std::string& msg)
{
        return pluginManager->getPluginByName (msg);
}

int32_t Client::sendMessage (uint32_t pid, const std::string& msg)
{
        Args out;
        return pluginManager->sendMessage (pid, msg, out);
}

int32_t Client::sendMessage (uint32_t pid, const std::string& msg, Args &out)
{
        return pluginManager->sendMessage (pid, msg, out);
}

int32_t Client::broadcastMessage (const std::string& msg, Args &out)
{
        return pluginManager->broadcastMessage (msg, out);
}

int32_t Client::broadcastMessage (const std::string& msg)
{
        Args out;
        return pluginManager->broadcastMessage (msg, out);
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

uint32_t Client::addPreSendPacketHook (uint32_t pid, PacketHook* hook)
{
        return pluginManager->addPreSendPacketHook (pid, hook);
}

uint32_t Client::addPostSendPacketHook (uint32_t pid, PacketHook* hook)
{
        return pluginManager->addPostSendPacketHook (pid, hook);
}

uint32_t Client::addPreRecvPacketHook (uint32_t pid, PacketHook* hook)
{
        return pluginManager->addPreRecvPacketHook (pid, hook);
}

uint32_t Client::addPostRecvPacketHook (uint32_t pid, PacketHook* hook)
{
        return pluginManager->addPostRecvPacketHook (pid, hook);
}

void Client::deletePreSendPacketHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deletePreSendPacketHook (pid, hid);
}

void Client::deletePostSendPacketHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deletePostSendPacketHook (pid, hid);
}

void Client::deletePreRecvPacketHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deletePreRecvPacketHook (pid, hid);
}

void Client::deletePostRecvPacketHook (uint32_t pid, uint32_t hid)
{
        pluginManager->deletePostRecvPacketHook (pid, hid);
}

uint32_t Client::addRecipricant (uint32_t pid, Recipricant* recipricant)
{
        return pluginManager->addRecipricant (pid, recipricant);
}

void Client::deleteRecipricant (uint32_t pid, uint32_t rid)
{
        pluginManager->deleteRecipricant (pid, rid);
}

void Client::sendToClient (GRMessageList& msgs)
{
        NetworkMessage* msg = msgs.put ();
        /* msg may be null if there are no messages */
        if (msg) {
                crypt->encrypt (msg);
                clientConn->putMsg (msg);
        }
}

void Client::sendToServer (GSMessageList& msgs)
{
        NetworkMessage* msg = msgs.put ();
        /* msg may be null if there are no messages */
        if (msg) {
                crypt->encrypt (msg);
                serverConn->putMsg (msg);
        }
}

uint32_t Client::getConsoleId ()
{
        return _consoleId;
}

uint32_t Client::getCycle ()
{
        return _cycle;
}

