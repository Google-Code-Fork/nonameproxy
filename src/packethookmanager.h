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

#ifndef __PACKET_HOOK_MANAGER_H
#define __PACKET_HOOK_MANAGER_H

#include <stdint.h>
#include <map>

#include "idmanager.h"

class PacketHook;
class NetworkMessage;

typedef std::map<uint32_t, PacketHook*> PHookMap;

class PacketHookManager
{
        public:
                PacketHookManager ();
                virtual ~PacketHookManager ();
                /* these functions return a hook id, so it can be 
                 * deleted later. 0 means failure to insert hook */
                uint32_t addPreHook (PacketHook* hook); 
                uint32_t addPostHook (PacketHook* hook); 
                void deletePreHook (uint32_t hid);
                void deletePostHook (uint32_t hid);
                void hookPrePacket (const NetworkMessage& msg);
                void hookPostPacket (const NetworkMessage& msg);
        private:
                IdManager*      preIds;
                IdManager*      postIds;
                PHookMap        preHooks;
                PHookMap        postHooks;
};
#endif

