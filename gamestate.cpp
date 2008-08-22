#include "gamestate.h"

GameState::GameState (LoginState* lstate)
{
        _lstate = lstate;
}

LoginState* GameState::getLoginState ()
{
        return _lstate;
}

