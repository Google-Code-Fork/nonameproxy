#include "hook.h"
#include "hookmanager.h"
#include "tibiamessage.h"

/**********************************************************
 * RHookInfor
 **********************************************************/

RHookInfo::RHookInfo (ReadHook* hook, uint64_t id)
{
        _hook = hook;
        _id = id;
}

RHookInfo::~RHookInfo ()
{
        delete _hook;
}

ReadHook* RHookInfo::getHook ()
{
        return _hook;
}

uint64_t RHookInfo::getId ()
{
        return _id;
}
        
/**********************************************************
 * HookManager
 **********************************************************/

HookManager::HookManager ()
{
        for (uint32_t i = 0; i < MAX_HOOKS; i ++) {
                wHooks[i] = NULL;
        }
}

HookManager::~HookManager ()
{
        for (uint32_t i = 0; i < MAX_HOOKS; i ++) {
                delete wHooks[i];
                for (rHookList::iterator it = rHooks[i].begin ();
                        it != rHooks[i].end (); ++ it)
                {
                        delete (*it);
                }
        }
}

uint64_t HookManager::addReadHook (uint8_t id, ReadHook* hook) {
        if (!hook) {
                printf ("warning: attempt to assign NULL read hook\n");
                return -1;
        }
        //the hashing function here is very simple. the first dword is the
        //message id. the second is the pointer to the hook. This ensures
        //that if someone wants to add more hooks than feasibly possible
        //they can. Note this is not 64-bit safe
        uint64_t hid = (uint64_t)hook << 32 + id;
        rHooks[id].push_back (new RHookInfo (hook, hid));
        return hid;
}

uint64_t HookManager::addWriteHook (uint8_t id, WriteHook* hook)
{
        if (!hook) {
                printf ("warning: attempt to assign NULL write hook\n");
                return -1;
        }
        if (wHooks[id]) {
                printf ("error: write hook id %d is already in use\n", id);
                return -1;
        }
        wHooks[id] = hook;
        //an even simple hashing algorithm
        return id;
}

void HookManager::deleteReadHook (uint64_t hid)
{
        uint8_t id = hid & 0xFF;
        for (rHookList::iterator it = rHooks[id].begin ();
                it != rHooks[id].end (); ++ it)
        {
                if (hid == (*it)->getId ()) {
                        delete (*it);
                        rHooks[id].erase (it);
                        break;
                }
        }
}

void HookManager::deleteWriteHook (uint64_t hid)
{
        uint8_t id = hid & 0xFF;
        delete wHooks[id];
        wHooks[id] = NULL;
}

void HookManager::hookReadMessage (TibiaMessage* tm, Client* client)
{
        uint8_t id = tm->getID ();
        for (rHookList::iterator it = rHooks[id].begin ();
                it != rHooks[id].end (); ++ it)
        {
                (*it)->getHook ()->func (tm, client);
        }
}
        
TibiaMessage* HookManager::hookWriteMessage (TibiaMessage* tm, Client* client)
{
        uint8_t id = tm->getID ();
        if (!wHooks[id]) {
                return tm;
        }
        return wHooks[id]->func (tm, client);
}

