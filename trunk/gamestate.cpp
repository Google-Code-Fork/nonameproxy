#include "gamestate.h"

GameState::GameState ()
{
        account = new AccountState;
}

GameState::~GameState ()
{
        delete account;
}

