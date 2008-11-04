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

#ifndef __CORE_HOOKS_H
#define __CORE_HOOKS_H

#include <stdint.h>
#include "hook.h"

//hook naming should be HW for a write hook and HR for a read hook,
//followed by the message name

class HWCharacterList : public WriteHook
{
        public:
                virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

class HWMOTD : public WriteHook
{
        public:
                virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

class HRLoginMsg : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class HRGameInit : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};


/**********************************************************************
 * Protocol hooks
 **********************************************************************/


/**********************************************************************
 * Game Recv 
 **********************************************************************/

class GRHSlotItem : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMagicEffect : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHTextMsg : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHGlobalLight : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHCreatureLight : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

/************************************************************************
 * Player Protocol Hooks
 ************************************************************************/

class GRHSelfInfo : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHPlayerStats : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHPlayerSkills : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHPlayerIcons : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

/************************************************************************
 * Map Protocol Hooks
 ************************************************************************/

class GRHMapInit : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapNorth : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapEast : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapSouth : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapWest : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapUp : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapDown : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

/************************************************************
 * Map Modification Messages
 ************************************************************/

class GRHCreatureMove : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

#endif

