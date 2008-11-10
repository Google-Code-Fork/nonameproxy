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

#include <stdio.h>
#include "pluginmanager.h"

PluginManager::PluginManager (Messenger* messenger,
                              HookManager* sendhm,
                              HookManager* recvhm,
                              PacketHookManager* sendphm,
                              PacketHookManager* recvphm,
                              ConnectionManager* connMgr,
                              Client* client)
{
        _client = client;
        _messenger = messenger;
        _sendhm = sendhm;
        _recvhm = recvhm;
        _sendphm = sendphm;
        _recvphm = recvphm;
        _connMgr = connMgr;

        ids = new IdManager (100);
}

PluginManager::~PluginManager ()
{
        PluginList::iterator i;
        for (i = plist.begin (); i != plist.end (); ++ i)
        {
                delete (*i).second;
        }
        delete ids;
}

uint32_t PluginManager::addPlugin (const std::string& path)
{
        uint32_t id = ids->newId ();

        Plugin* p = new Plugin ();

        /* we have to add the id before calling load */
        plist.insert (std::pair<uint32_t, Plugin*> (id, p));
        if (p->load (id, path, _client)) {
                /* the plugin is already loaded so we must count to 2 */
                uint32_t count = 0;
                const std::string& name = p->name ();
                PluginList::iterator i;
                for (i = plist.begin (); i != plist.end (); ++ i) {
                        if (name == (*i).second->name ()) {
                                count ++;
                        }
                }
                        
                if (count > 1) {
                        printf ("plugin manager error: ");
                        printf ("plugin already loaded\n");
                        p->unload ();
                        plist.erase (id);
                        ids->recycleId (id);
                        delete p;
                        return 0;
                }
                return id;
        } else {
                plist.erase (id);
                ids->recycleId (id);
                printf ("plugin manager error: could not load plugin\n");
                delete p;
                return 0;
        }
}

uint32_t PluginManager::addFakein (const std::string& name)
{
        uint32_t id = ids->newId ();

        Fakein* p = new Fakein (name);
        /* we have to add the id before calling load */
        plist.insert (std::pair<uint32_t, Plugin*> (id, p));
        if (p->load (id, "", _client)) {
                /* the plugin is already loaded so we must count to 2 */
                uint32_t count = 0;
                const std::string& name = p->name ();
                PluginList::iterator i;
                for (i = plist.begin (); i != plist.end (); ++ i) {
                        if (name == (*i).second->name ()) {
                                count ++;
                        }
                }
                        
                if (count > 1) {
                        printf ("fakein manager error: ");
                        printf ("fakein already loaded\n");
                        p->unload ();
                        plist.erase (id);
                        ids->recycleId (id);
                        delete p;
                        return 0;
                }
                return id;
        } else {
                plist.erase (id);
                ids->recycleId (id);
                printf ("plugin manager error: could not load fakein\n");
                return 0;
        }
}

bool PluginManager::deletePlugin (uint32_t pid)
{
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin delete error: plugin doesnt exist\n");
                return false;
        }
        if (!ids->recycleId (pid)) {
                printf ("plugin delete error: ids not concurrent\n");
        }
        delete (*i).second;
        plist.erase (pid);
        return true;
}

uint32_t PluginManager::getPluginByName (const std::string& name)
{
        for (PluginList::iterator i = plist.begin (); i != plist.end (); ++ i) {
                if (name == (*i).second->name ()) {
                        return (*i).first;
                }
        }
        return 0;
}

/************************************************************************
 * These functions provide the medium for the plugins to operate through
 * Note: all these functions work in terms of plugin id, the one
 *       notable function is send message which hides the recipricant
 *       id from the outside
 ************************************************************************/

Args PluginManager::sendMessage (uint32_t pid, const std::string& msg)
{
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: sendMessage: non existant plugin\n");
                return Args ();
        }
        uint32_t rid = (*i).second->getRecipricantId ();
        return _messenger->sendMessage (rid, msg);
}

Args PluginManager::broadcastMessage (const std::string& msg)
{
        ArgsParser ap (msg, _client);
        const Args& args = ap.getArgs ();

        if (args.size () == 0) {
                printf ("broadcast error: empty message\n");
                return Args ();
        }
        uint32_t pid = getPluginByName (args.front ());
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                /* a broadcast is allowed to fail */
                return Args ();
        }
        uint32_t rid = (*i).second->getRecipricantId ();
        return _messenger->sendMessage (rid, msg);
}

uint32_t PluginManager::addRecvReadHook (uint32_t pid, uint8_t id, 
                                         ReadHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addReadHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _recvhm->addReadHook (id, hook);
        (*i).second->addRecvReadHookId (hid);
        return hid;
}
        
uint32_t PluginManager::addRecvWriteHook (uint32_t pid, uint8_t id, 
                                          WriteHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addWriteHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _recvhm->addWriteHook (id, hook);
        (*i).second->addRecvWriteHookId (hid);
        return hid;
}
        
uint32_t PluginManager::addSendReadHook (uint32_t pid, uint8_t id, 
                                         ReadHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addReadHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _sendhm->addReadHook (id, hook);
        (*i).second->addSendReadHookId (hid);
        return hid;
}
        
uint32_t PluginManager::addSendWriteHook (uint32_t pid, uint8_t id, 
                                          WriteHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addWriteHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _sendhm->addWriteHook (id, hook);
        (*i).second->addSendWriteHookId (hid);
        return hid;
}

void PluginManager::deleteRecvReadHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: deleteReadHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _recvhm->deleteReadHook (hid);
        (*i).second->deleteRecvReadHookId (hid);
}

void PluginManager::deleteRecvWriteHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: deleteWriteHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _recvhm->deleteWriteHook (hid);
        (*i).second->deleteRecvWriteHookId (hid);
}

void PluginManager::deleteSendReadHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: deleteReadHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _sendhm->deleteReadHook (hid);
        (*i).second->deleteSendReadHookId (hid);
}

void PluginManager::deleteSendWriteHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: deleteWriteHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _sendhm->deleteWriteHook (hid);
        (*i).second->deleteSendWriteHookId (hid);
}

uint32_t PluginManager::addRecipricant (uint32_t pid, Recipricant* recipricant)
{
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: setRecipricant: non existant plugin\n");
                return 0;
        }
        if ((*i).second->getRecipricantId () != 0) {
                printf ("plugin manager error: plugins only allowed one ");
                printf ("recipricant at a time\n");
                return 0;
        }
        uint32_t rid = _messenger->addRecipricant (recipricant);
        (*i).second->setRecipricantId (rid);
        return rid;
}

void PluginManager::deleteRecipricant (uint32_t pid, uint32_t rid)
{
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: setRecipricant: non existant plugin\n");
                return;
        }
        if ((*i).second->getRecipricantId () == 0) {
                printf ("plugin manager error: attempt delete ");
                printf ("non existant plugin\n");
                return;
        }
        if ((*i).second->getRecipricantId () != rid) {
                printf ("plugin manager error: attempt delete ");
                printf ("recipricant that does not belong to plugin\n");
                return;
        }
        _messenger->deleteRecipricant (rid);
        (*i).second->setRecipricantId (0);
}
        
uint32_t PluginManager::addPreSendPacketHook (uint32_t pid, PacketHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addPreSendHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _sendphm->addPreHook (hook);
        (*i).second->addPreSendPacketHookId (hid);
        return hid;
}
        
uint32_t PluginManager::addPostSendPacketHook (uint32_t pid, PacketHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addPostSendHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _sendphm->addPostHook (hook);
        (*i).second->addPostSendPacketHookId (hid);
        return hid;
}
        
uint32_t PluginManager::addPreRecvPacketHook (uint32_t pid, PacketHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addPreRecvHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _recvphm->addPreHook (hook);
        (*i).second->addPreRecvPacketHookId (hid);
        return hid;
}
        
uint32_t PluginManager::addPostRecvPacketHook (uint32_t pid, PacketHook* hook)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: addPostRecvHook: non existant plugin\n");
                return 0;
        }
        uint32_t hid = _recvphm->addPostHook (hook);
        (*i).second->addPostRecvPacketHookId (hid);
        return hid;
}
        
void PluginManager::deletePreSendPacketHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: delete PreSendPacketHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _sendphm->deletePreHook (hid);
        (*i).second->deletePreSendPacketHookId (hid);
}

void PluginManager::deletePostSendPacketHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: delete PrePacketHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _sendphm->deletePostHook (hid);
        (*i).second->deletePostSendPacketHookId (hid);
}

void PluginManager::deletePreRecvPacketHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: delete PreRecvPacketHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _recvphm->deletePreHook (hid);
        (*i).second->deletePreRecvPacketHookId (hid);
}

void PluginManager::deletePostRecvPacketHook (uint32_t pid, uint32_t hid)
{
        /* before we add the hook we make sure the plugin actually exists */
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: delete PrePacketHook: ");
                printf ("non existant plugin\n");
                return;
        }
        _recvphm->deletePostHook (hid);
        (*i).second->deletePostRecvPacketHookId (hid);
}

