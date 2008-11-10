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

#ifndef __PACKET_HOOK_H
#define __PACKET_HOOK_H

#include <stdlib.h>
#include <list>

class NetworkMessage;
class Client;

/* These are stub classes. Hooks can either be defined in corehooks.h
 * or a module can define a hook in its own code. */

class PacketHook
{
        public:
                virtual ~PacketHook () {}
                virtual void func (const NetworkMessage& msg) {}
};

#endif

