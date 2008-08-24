#ifndef __CHAR_STATE_H
#define __CHAR_STATE_H

#include <string>

class CharState
{
        public:
                void setName (const std::string& name);
                const std::string& getName ();
        private:
                std::string _name;
};
#endif

