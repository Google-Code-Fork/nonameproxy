#include "loginstate.h"

//LoginDetails
LoginDetails::LoginDetails (uint32_t account, std::string pwd,
        TCharacterList* charlist)
{
        _account = account;
        _pwd = pwd;
        _charlist = new TCharacterList (*charlist);
}

LoginDetails::~LoginDetails ()
{
        delete _charlist;
}

uint32_t LoginDetails::getAccount ()
{
        return _account;
}

const std::string& LoginDetails::getPwd ()
{
        return _pwd;
}

TCharacter* LoginDetails::getCharByName (const std::string& name)
{
        const CharList& cl = _charlist->getCharList ();
        for (CharList::const_iterator i = cl.begin (); i != cl.end (); ++ i) {
                if (name == (*i)->getName ()) {
                        return (*i);
                }
        }
        return NULL;
}

//LoginState
void LoginState::putAccountDetails (LoginDetails* details)
{
        uint32_t key = details->getAccount ();
        _loginrec.insert (std::pair<uint32_t, LoginDetails>(key, *details));
}

LoginDetails* LoginState::getAccountDetails (uint32_t account)
{
        LoginRecord::iterator i = _loginrec.find (account);
        if (i == _loginrec.end ()) {
                return NULL;
        } else {
                return (&((*i).second));
        }
}
        
