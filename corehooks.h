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

class GRHMagicEffect : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHTextMsg : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHGlobalLight : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHCreatureLight : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

/************************************************************************
 * Player Protocol Hooks
 ************************************************************************/

class GRHSelfInfo : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHPlayerStats : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHPlayerSkills : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHPlayerIcons : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

/************************************************************************
 * Map Protocol Hooks
 ************************************************************************/

class GRHMapInit : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapNorth : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapEast : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapSouth : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapWest : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapUp : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHMapDown : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

/************************************************************
 * Map Modification Messages
 ************************************************************/

class GRHTileSet : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHTileAdd : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHTileUpdate : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHTileRemove : public ReadHook
{
        public: virtual void func (TibiaMessage* tm, Client* client);
};

class GRHCreatureMove : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

/**************************************************************
 * Inventory Messages
 **************************************************************/

class GRHContainerOpen : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHContainerClose : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHContainerAdd : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHContainerUpdate : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHContainerRemove : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHSlotItem : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class GRHSlotClear : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};
#endif

