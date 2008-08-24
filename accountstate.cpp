#include "accountstate.h"

void AccountState::setAccount (uint32_t account)
{
        _account = account;
}

void AccountState::setPassword (const std::string& password)
{
        _password = password;
}

uint32_t AccountState::getAccount ()
{
        return _account;
}

const std::string& AccountState::getPassword ()
{
        return _password;
}

