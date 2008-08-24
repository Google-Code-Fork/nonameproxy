#ifndef __LOGIN_STATE_H
#define __LOGIN_STATE_H

#include <stdint.h>
#include <map>
#include <string>
#include "tibiatypes.h"

class LoginDetails
{
        public:
                LoginDetails (uint32_t account, std::string pwd,
                        TCharacterList* charlist);
                virtual ~LoginDetails ();
                uint32_t getAccount ();
                const std::string& getPwd ();
                TCharacter* getCharByName (const std::string& name);
        private:
                uint32_t _account;
                std::string _pwd;
                TCharacterList* _charlist;
};

typedef std::map<uint32_t, LoginDetails*> LoginRecord;

class LoginState
{
        public:
                LoginDetails*   getAccountDetails (uint32_t account);
                void            putAccountDetails (LoginDetails* details);
                
        private:
                LoginRecord _loginrec;
};

#endif
                
                
