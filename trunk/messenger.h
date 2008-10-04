#ifndef __MESSENGER_H
#define __MESSENGER_H

#include <stdint.h>
#include <map>
#include <string>

#include "idmanager.h"

class Recipricant
{
        public:
                virtual ~Recipricant () {};
                virtual void func (const std::string& msg) {};
};

typedef std::map<uint32_t, Recipricant> RecipricantList;

class Messenger
{
        public:
                Messenger ();
                virtual ~Messenger ();

                /* Add hook returns a unique id for the hook just added */
                uint32_t addRecipricant    (Recipricant* recipricant);
                void     deleteRecipricant (uint32_t rid);

                void sendMessage (uint32_t rid, const std::string& msg);
                void broadcastMessage (const std::string& msg);

        private:
                RecipricantList rlist;
                IdManager* ids;
};

#endif

