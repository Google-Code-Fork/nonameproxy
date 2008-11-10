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

#include "packethookmanager.h"
#include "packethook.h"
#include "networkmessage.h"

/**********************************************************
 * HookManager
 **********************************************************/

PacketHookManager::PacketHookManager ()
{
        preIds = new IdManager (100);
        postIds = new IdManager (100);
}

PacketHookManager::~PacketHookManager ()
{
        PHookMap::iterator i;
        for (i = preHooks.begin (); i != preHooks.end (); ++ i) {
                delete (*i).second;
        }
        for (i = postHooks.begin (); i != postHooks.end (); ++ i) {
                delete (*i).second;
        }
        delete preIds;
        delete postIds;
}

uint32_t PacketHookManager::addPreHook (PacketHook* hook) {
        if (!hook) {
                printf ("warning: attempt to assign NULL prePacketHook\n");
                return 0;
        }

        uint32_t hid = preIds->newId ();
        preHooks.insert (std::pair<uint32_t, PacketHook*> (hid, hook));
        return hid;
}

uint32_t PacketHookManager::addPostHook (PacketHook* hook) {
        if (!hook) {
                printf ("warning: attempt to assign NULL postPacketHook\n");
                return 0;
        }

        uint32_t hid = postIds->newId ();
        postHooks.insert (std::pair<uint32_t, PacketHook*> (hid, hook));
        return hid;
}

void PacketHookManager::deletePreHook (uint32_t hid)
{
        PHookMap::iterator i = preHooks.find (hid);
        if (i == preHooks.end ()) {
                printf ("phookmanager error: delete: non existant pre hook\n");
                return;
        }
        preHooks.erase (i);
        preIds->recycleId ((*i).first);
        delete (*i).second;
}

void PacketHookManager::deletePostHook (uint32_t hid)
{
        PHookMap::iterator i = postHooks.find (hid);
        if (i == postHooks.end ()) {
                printf ("phookmanager error: delete: non existant post hook\n");
                return;
        }
        postHooks.erase (i);
        postIds->recycleId ((*i).first);
        delete (*i).second;
}

void PacketHookManager::hookPrePacket (const NetworkMessage& msg)
{
        PHookMap::iterator i;
        for (i = preHooks.begin (); i != preHooks.end (); ++ i)
        {
                (*i).second->func (msg);
        }
}
        
void PacketHookManager::hookPostPacket (const NetworkMessage& msg)
{
        PHookMap::iterator i;
        for (i = postHooks.begin (); i != postHooks.end (); ++ i)
        {
                (*i).second->func (msg);
        }
}

