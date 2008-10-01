#ifndef __ACCOUNT_STATE_H
#define __ACCOUNT_STATE_H

#include <string>

class AccountState
{
        public:
                void setAccount (const std::string& account);
                void setPassword (const std::string& password);

                const std::string&      getAccount ();
                const std::string&      getPassword ();

                void setName (const std::string& name);
                const std::string& getName ();

        private:
                std::string     _account;
                std::string     _password;
                std::string     _name;
};
#endif

