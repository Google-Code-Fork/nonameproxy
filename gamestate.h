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

#ifndef __GAME_STATE_H
#define __GAME_STATE_H

#include "accountstate.h"
#include "charstate.h"
#include "mapstate.h"
//knock these off as the states are written
class CreatureListState;
class MessageState;
class InvetoryState;

//Gamestate simply manages all the smaller states
//note a ptr is for states if they are universal, such as login state
//these will have to be passed to gamestate in its constructor
class GameState
{
        public:
                GameState ();
                virtual ~GameState ();

                AccountState*           account;
                CharState*              character;
                MapState*               map;
                CreatureListState*      creaturelist;
                MessageState*           message;
                InvetoryState*          inventory;
                
        private:
};

#endif
                
