/********************************************************************
 * I guess if youre here you want to know how this works, so ill
 * give a quick explanation.
 * When the class is initialized, the set of used ids is empty 
 * Optionally a recycling bin can be used.
 *      When a recycling bin is used, when an item is recycled
 *      it is added to the recycling bin unless the bin is full.
 *      Next time a new id is needed, it will first check the 
 *      recycling bin. If it is empty, it will have to iterate 
 *      through the set until it finds an unused ID (nlogn time).
 *      
 *      If there is something in the recycling bin, then the id
 *      is taken from there in constant time
 *      If a recycling bin is used, then at inititializating the
 *      recycling bin is filled with "bin_size" elements
 ********************************************************************/

#include "idmanager.h"

IdManager::IdManager ()
{
        _bin_size = 0;
}

IdManager::IdManager (uint32_t bin_size)
{
        _bin_size = bin_size;
        
        for (uint32_t i = 1; i < bin_size + 1; i ++) {
                recyclebin.push_back (i);
        }
}

uint32_t IdManager::newId ()
{
        uint32_t id;
        if (recyclebin.size () != 0) {
                id = recyclebin.front ();
                recyclebin.pop_front ();
                used.insert (id);
        } else {
                std::pair <std::set<uint32_t>::iterator, bool> ret;
                id = 1;
                ret = used.insert (id);
                while (!ret.second) {
                        id ++;
                        ret = used.insert (id);
                }
        }
        return id;
}

bool IdManager::recycleId (uint32_t id)
{
        if (used.erase (id) == 0) {
                return false;
        }
        if (recyclebin.size () < _bin_size) {
                recyclebin.push_front (id);
        }
        return true;
}
        
