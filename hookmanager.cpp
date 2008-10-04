#include "hook.h"
#include "hookmanager.h"
#include "tibiamessage.h"

/**********************************************************
 * HookManager
 **********************************************************/

HookManager::HookManager ()
{
        for (uint32_t i = 0; i < MAX_HOOKS; i ++) {
                wHooks[i] = NULL;
        }
        ids = new IdManager (100);
}

HookManager::~HookManager ()
{
        for (uint32_t i = 0; i < MAX_HOOKS; i ++) {
                delete wHooks[i];
                for (RHookList::iterator it = rHooks[i].begin ();
                        it != rHooks[i].end (); ++ it)
                {
                        delete (*it).second;
                }
        }
        delete ids;
}

uint32_t HookManager::addReadHook (uint8_t id, ReadHook* hook) {
        if (!hook) {
                printf ("warning: attempt to assign NULL read hook\n");
                return -1;
        }

        uint32_t hid = ids->newId ();
        rLookup.insert (std::pair<uint32_t, uint32_t> (hid, id));
        rHooks[id].insert (std::pair<uint32_t, ReadHook*> (hid, hook));
        printf ("insert: %p\n", hook);
        return hid;
}

uint32_t HookManager::addWriteHook (uint8_t id, WriteHook* hook)
{
        if (!hook) {
                printf ("warning: attempt to assign NULL write hook\n");
                return 0;
        }
        if (wHooks[id]) {
                printf ("error: write hook id %d is already in use\n", id);
                return 0;
        }
        wHooks[id] = hook;
        /* the hashing function here is very simple. If the message id
         * is not 0, the hook id is simply the message id. Because 0 is
         * reserved for errors, instead of 0 we return 256 */
        uint32_t hid = (id != 0) ? id : MAX_HOOKS;

        return hid;
}

void HookManager::deleteReadHook (uint32_t hid)
{
        HookMap::iterator i = rLookup.find (hid);
        if (i == rLookup.end ()) {
                printf ("hookmanager error: delete: non existant hook\n");
                return;
        }
        uint32_t mid = (*i).second;

        if (rHooks[mid].erase (hid) == 0) {
                printf ("hookmanager error: delete: concurrency error. \n\
                         This is really bad. Its not your fault but let \
                         know if it happens.\n");
                return;
        }
        ids->recycleId (hid);
}

void HookManager::deleteWriteHook (uint32_t hid)
{
        uint32_t mid = (hid == MAX_HOOKS) ? 0 : hid;
        delete wHooks[mid];
        wHooks[mid] = NULL;
}

void HookManager::hookReadMessage (TibiaMessage* tm, Client* client)
{
        uint8_t id = tm->getId ();
        for (RHookList::iterator it = rHooks[id].begin ();
                it != rHooks[id].end (); ++ it)
        {
                (*it).second->func (tm, client);
        }
}
        
TibiaMessage* HookManager::hookWriteMessage (TibiaMessage* tm, Client* client)
{
        uint8_t id = tm->getId ();
        if (!wHooks[id]) {
                return tm;
        }
        return wHooks[id]->func (tm, client);
}

