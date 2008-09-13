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
                void setName (const std::string& name);
                const std::string& getName ();

        private:
                uint32_t        _account;
                std::string     _password;
                std::string     _name;
};
#endif

