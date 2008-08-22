#ifndef __GAME_STATE_H
#define __GAME_STATE_H

class LoginState;

//Gamestate simply manages all the smaller states
//note a ptr is for states if they are universal, such as login state
//these will have to be passed to gamestate in its constructor
class GameState
{
        public:
                GameState (LoginState* lstate);
                LoginState* getLoginState ();
        private:
                LoginState* _lstate;
};

#endif
                
