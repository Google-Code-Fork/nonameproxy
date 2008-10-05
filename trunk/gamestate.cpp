#include "gamestate.h"

GameState::GameState ()
{
        account = new AccountState;
        character = new CharState;
        map = new MapState;
}

GameState::~GameState ()
{
        delete account;
        delete character;
        delete map;
}

