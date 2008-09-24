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
                
