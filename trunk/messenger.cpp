#include <stdio.h>
#include "messenger.h"

Messenger::Messenger ()
{
        ids = new IdManager (100);
        for (RecipricantList::iterator i; i != rlist.end (); ++ i) {
                delete (*i).second;
        }
}

Messenger::~Messenger ()
{
        delete ids;
}

uint32_t Messenger::addRecipricant (Recipricant* recipricant)
{
        uint32_t id = ids->newId ();

        rlist.insert (std::pair<uint32_t, Recipricant*> (id, recipricant));
        return id;
}

void Messenger::deleteRecipricant (uint32_t rid)
{
        if (rlist.erase (rid) == 0) {
                printf ("messenger delete error: recipricant doesnt exist\n");
                return;
        }
        if (!ids->recycleId (rid)) {
                printf ("messenger delete error: ids not concurrent\n");
        }
}

void Messenger::sendMessage (uint32_t rid, const std::string& msg)
{
        RecipricantList::iterator i = rlist.find (rid);

        if (i == rlist.end ()) {
                printf ("messenger send error: recipricant does not exist\n");
        } else {
                (*i).second->func (msg);
        }
}

void Messenger::broadcastMessage (const std::string& msg)
{
        for (RecipricantList::iterator i; i != rlist.end (); ++ i) {
                (*i).second->func (msg);
        }
}

