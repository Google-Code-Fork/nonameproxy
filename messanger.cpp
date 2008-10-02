#include <stdio.h>
#include "messanger.h"

Messanger::Messanger ()
{
        ids = new IdManager (100);
        for (RecipricantList::iterator i; i != rlist.end (); ++ i) {
                delete (*i).second;
        }
}

Messanger::~Messanger ()
{
        delete ids;
}

uint32_t Messanger::addRecipricant (Recipricant* recipricant)
{
        uint32_t id = ids->newId ();

        rlist.insert (std::pair<uint32_t, Recipricant*> (id, recipricant));
        return id;
}

void Messanger::deleteRecipricant (uint32_t rid)
{
        if (rlist.erase (rid) == 0) {
                printf ("messanger delete error: recipricant doesnt exist\n");
                return;
        }
        if (!ids->recycleId (rid)) {
                printf ("messanger delete error: ids not concurrent\n");
        }
}

void Messanger::sendMessage (uint32_t rid, const std::string& msg)
{
        RecipricantList::iterator i = rlist.find (rid);

        if (i == rlist.end ()) {
                printf ("messanger send error: recipricant does not exist\n");
        } else {
                (*i).second->func (msg);
        }
}

void Messanger::broadcastMessage (const std::string& msg)
{
        for (RecipricantList::iterator i; i != rlist.end (); ++ i) {
                (*i).second->func (msg);
        }
}

