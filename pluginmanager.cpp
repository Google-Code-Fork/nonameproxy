#include <stdio.h>
#include "pluginmanager.h"

PluginManager::PluginManager (Messenger* messenger,
                              HookManager* recvhm,
                              HookManager* sendhm,
                              ConnectionManager* connMgr,
                              Client* client)
{
        _client = client;
        _messenger = messenger;
        _recvhm = recvhm;
        _sendhm = sendhm;
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
                printf ("inserted %d\n", id);
                return id;
        } else {
                plist.erase (id);
                ids->recycleId (id);
                printf ("plugin manager error: could not load plugin\n");
                return 0;
        }
}

void PluginManager::deletePlugin (uint32_t pid)
{
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin delete error: plugin doesnt exist\n");
                return;
        }
        if (!ids->recycleId (pid)) {
                printf ("plugin delete error: ids not concurrent\n");
        }
        delete (*i).second;
        plist.erase (pid);
}

uint32_t PluginManager::getPluginByName (const std::string& name)
{
        for (PluginList::iterator i; i != plist.end (); ++ i) {
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

void PluginManager::sendMessage (uint32_t pid, const std::string& msg)
{
        PluginList::iterator i = plist.find (pid);
        if (i == plist.end ()) {
                printf ("plugin manager: sendMessage: non existant plugin\n");
                return;
        }
        uint32_t rid = (*i).second->getRecipricantId ();
        _messenger->sendMessage (rid, msg);
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
        
