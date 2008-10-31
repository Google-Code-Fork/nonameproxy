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
                return 0;
        }

        uint32_t hid = ids->newId ();
        rLookup.insert (std::pair<uint32_t, uint32_t> (hid, id));
        rHooks[id].insert (std::pair<uint32_t, ReadHook*> (hid, hook));
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
        HookMap::iterator i1 = rLookup.find (hid);
        if (i1 == rLookup.end ()) {
                printf ("hookmanager error: delete: non existant hook\n");
                return;
        }
        uint32_t mid = (*i1).second;

        RHookList::iterator i2 = rHooks[mid].find (hid);
        if (i2 == rHooks[mid].end ()) {
                printf ("hookmanager error: delete: concurrency error. \n\
                         This is really bad. Its not your fault but let \
                         know if it happens.\n");
                return;
        }
        delete (*i2).second;
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

