#include "accountstate.h"

void AccountState::setAccount (const std::string& account)
{
        _account = account;
}

void AccountState::setPassword (const std::string& password)
{
        _password = password;
}

void AccountState::setName (const std::string& name)
{
        _name = name;
}

const std::string& AccountState::getAccount ()
{
        return _account;
}

const std::string& AccountState::getPassword ()
{
        return _password;
}

const std::string& AccountState::getName ()
{
        return _name;
}

