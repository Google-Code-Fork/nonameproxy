/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

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
        
