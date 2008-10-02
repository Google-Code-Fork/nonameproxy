#ifndef __MESSANGER_H
#define __MESSANGER_H

#include <stdint.h>
#include <map>
#include <string>

#include "idmanager.h"

class Recipricant
{
        public:
                virtual ~Recipricant () = 0;
                virtual void func (const std::string& msg);
};

typedef std::map<uint32_t, Recipricant*> RecipricantList;

class Messanger
{
        public:
                Messanger ();
                virtual ~Messanger ();

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

