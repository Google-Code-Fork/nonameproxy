#ifndef __ACCOUNT_STATE_H
#define __ACCOUNT_STATE_H

#include <string>

class AccountState
{
        public:
                void setAccount (uint32_t account);
                void setPassword (const std::string& password);

                uint32_t                getAccount ();
                const std::string&      getPassword ();
        private:
                uint32_t        _account;
                std::string     _password;
};
#endif

