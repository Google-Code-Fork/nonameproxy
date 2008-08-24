#include "gamestate.h"

GameState::GameState ()
{
        account = new AccountState;
        character = new CharState;
}

GameState::~GameState ()
{
        delete account;
}

