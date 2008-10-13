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

#ifndef __HOOK_H
#define __HOOK_H

#include <stdlib.h>
#include <list>

class TibiaMessage;
class Client;

//These are stub classes. Hooks can either be defined in corehooks.h
//or a module can define a hook in its own code.
class ReadHook
{
        public:
                virtual ~ReadHook () {};
                virtual void func (TibiaMessage* tm, Client* client) {}
};

class WriteHook
{
        public:
                virtual ~WriteHook () {};
                virtual TibiaMessage* func (TibiaMessage* tm, Client* client)
                        {return NULL;}
};
#endif

