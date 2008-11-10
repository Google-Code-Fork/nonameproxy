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

#ifndef __LOGIN_STATE_H
#define __LOGIN_STATE_H

#include <stdint.h>
#include <map>
#include <string>
#include "tibiatypes.h"

class LoginDetails
{
        public:
                LoginDetails (  const std::string& account, 
                                const std::string& pwd,
                                TCharacterList* charlist);

                virtual ~LoginDetails ();
                const std::string& getAccount ();
                const std::string& getPwd ();
                TCharacter* getCharByName (const std::string& name);
        private:
                std::string _account;
                std::string _pwd;
                TCharacterList* _charlist;
};

typedef std::map<std::string, LoginDetails*> LoginRecord;

class LoginState
{
        public:
                LoginDetails*   getAccountDetails (const std::string& account);
                void            putAccountDetails (LoginDetails* details);
                
        private:
                LoginRecord _loginrec;
};

#endif
                
                
