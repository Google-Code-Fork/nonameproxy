#include "charstate.h"

void CharState::setName (const std::string& name)
{
        _name = name;
}

const std::string& CharState::getName ()
{
        return _name;
}

