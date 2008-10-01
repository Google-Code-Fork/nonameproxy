#include "loginstate.h"

//LoginDetails
LoginDetails::LoginDetails (const std::string& account, const std::string& pwd,
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

const std::string& LoginDetails::getAccount ()
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
        std::string key = details->getAccount ();
        _loginrec.insert (std::pair<std::string, LoginDetails*>(key, details));
}

LoginDetails* LoginState::getAccountDetails (const std::string& account)
{
        LoginRecord::iterator i = _loginrec.find (account);
        if (i == _loginrec.end ()) {
                return NULL;
        } else {
                return ((*i).second);
        }
}
        
