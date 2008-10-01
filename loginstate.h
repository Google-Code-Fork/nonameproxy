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
                
                
