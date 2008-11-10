/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

#include "inventorystate.h"

/*****************************************
 * Tile
 *****************************************/

Container::Container ()
{
        _open = false;
        _capacity = 0;
        _itemid = 0;
        _hasparent = false;
}

Container::~Container ()
{
        ThingVector::iterator i;
        for (i = _things.begin (); i != _things.end (); i ++) {
                delete (*i);
        }
        _things.clear ();
}

bool Container::open (uint32_t capacity, uint32_t itemid, bool hasparent,
        const std::string& name)
{
        if (_open) {
                printf ("open error: container already open\n");
                return false;
        }

        _hasparent      = hasparent;
        _capacity       = capacity;
        _itemid         = itemid;
        _name           = name;
        _open           = true;
        return true;
}

bool Container::close ()
{
        if (!_open) {
                printf ("close error: container not open\n");
                return false;
        }
        ThingVector::iterator i;
        for (i = _things.begin (); i != _things.end (); i ++) {
                delete (*i);
        }
        _things.clear ();
        
        _open = false;
        /* debuging purposes, can be removed later */
        _name = "closed";
        return true;
}

bool Container::isOpen () const
{
        return _open;
}

bool Container::hasParent () const
{
        return _hasparent;
}

uint32_t Container::getItemId () const
{
        return _itemid;
}

uint32_t Container::getCapacity () const
{
        return _capacity;
}

uint32_t Container::getThingCount () const
{
        return _things.size ();
}

bool Container::insertThing (const Thing& thing, uint32_t stackpos)
{
        if (getThingCount () == _capacity) {
                printf ("insertThing error: container full\n");
                return false;
        }
        if (thing.getType () == Thing::t_creature) {
                printf ("insertThing error: can't put creature in container\n");
                return false;
        }
        Thing* add;
        uint32_t itemid;
        if (thing.getType () == Thing::t_item) {
                itemid = ((Item&)thing).getItemId ();
                add = new Item ((Item&)thing);
        } else if (thing.getType () == Thing::t_xitem) {
                itemid = ((XItem&)thing).getItemId ();
                add = new XItem ((XItem&)thing);
        } else {
                printf ("insertThing error: this item has a funny type\n");
                return false;
        }
        ThingVector::iterator i = _things.begin ();
        uint32_t curpos;
        for (curpos = 0; curpos < stackpos; curpos ++) {
                if (i == _things.end ()) {
                        break;
                }
                i ++;
        }
        if (curpos != stackpos) {
                printf ("insertThing error: ");
                printf ("curpos %d != stackpos %d \n",
                        curpos, stackpos);
                show ();
                add->show ();
                *((uint32_t*)NULL) = 0;
                return false;
        }
        _things.insert (i, add);
        return true;
}
        
void Container::show () const
{
        uint32_t stackpos = 0;
        printf ("open=%d cap=%d hasparent=%d nitems=%d\n", 
                _open,
                _capacity,
                _hasparent,
                getThingCount ()); 
        ThingVector::const_iterator i;
        for (i = _things.begin (); i != _things.end (); ++ i) {
                printf ("%d: ", stackpos);
                stackpos ++;
                (*i)->show ();
        }
}
                
bool Container::addThing (const Thing& thing)
{
        return insertThing (thing, 0);
}

bool Container::addBackThing (const Thing& thing)
{
        return insertThing (thing, _things.size ());
}

bool Container::removeThing (uint32_t index)
{
        ThingVector::iterator i = _things.begin ();
        uint32_t curpos;
        for (curpos = 0; curpos < index; curpos ++) {
                i ++;
                if (i == _things.end ()) {
                        break;
                }
        }
        if (i == _things.end ()) {
                printf ("removeThing error: out of bounds\n");
                return false;
        }
        if (curpos != index) {
                printf ("removeThing error: curpos != index\n");
                return false;
        }
        /* if a creature leaves the map we need to keep it for later */
        delete (*i);
        _things.erase (i);
        return true;
}

Thing& Container::getThing (uint32_t index) 
{
        if (!(index < getThingCount ())) {
                printf ("getThing error: thing %d out of bounds\n", index);
                show ();
                *((uint32_t*)NULL) = 0;
        }
        return *_things[index];
}

const Thing& Container::getThing (uint32_t index) const 
{
        if (!(index < getThingCount ())) {
                printf ("getThing error: thing %d out of bounds\n", index);
                show ();
                *((uint32_t*)NULL) = 0;
        }
        return *_things[index];
}

/********************************************************************
 * InventoryState
 ********************************************************************/
InventoryState::InventoryState ()
{
        for (uint32_t i = SLOT_FIRST; i < SLOT_LAST; i ++) {
                _slots[i] = NULL;
        }
}

Container& InventoryState::getContainer (uint32_t cid)
{
        return _containers[cid];
}

Thing& InventoryState::getSlot (slot_t slot)
{
        if (!(SLOT_FIRST <= slot && slot < SLOT_LAST)) {
                printf ("getSlot error: %d out of bounds\n", slot);
        }
        if (!_slots[slot]) {
                printf ("getSlot error: %d empty\n", slot);
        }
        return *_slots[slot];
}

bool InventoryState::setSlot (const Thing& thing, slot_t slot)
{
        Thing* add;
        if (thing.getType () == Thing::t_creature) {
                printf ("setSlot error: can't put creature in slot\n");
                return false;
        } else if (thing.getType () == Thing::t_item) {
                add = new Item ((Item&)thing);
        } else if (thing.getType () == Thing::t_xitem) {
                add = new XItem ((XItem&)thing);
        } else {
                printf ("setSlot error: this item has a funny type\n");
                return false;
        }
        delete _slots[slot];
        _slots[slot] = add;
        return true;
}

bool InventoryState::emptySlot (slot_t slot)
{
        delete _slots[slot];
        _slots[slot] = NULL;
        return true;
}

void InventoryState::showSlots ()
{
        printf ("head : ");
        if (_slots[SLOT_HEAD]) {
                _slots[SLOT_HEAD]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("neck : ");
        if (_slots[SLOT_NECKLACE]) {
                _slots[SLOT_NECKLACE]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("pack : ");
        if (_slots[SLOT_BACKPACK]) {
                _slots[SLOT_BACKPACK]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("armor: ");
        if (_slots[SLOT_ARMOR]) {
                _slots[SLOT_ARMOR]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("right: ");
        if (_slots[SLOT_RIGHT]) {
                _slots[SLOT_RIGHT]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("left : ");
        if (_slots[SLOT_LEFT]) {
                _slots[SLOT_LEFT]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("legs : ");
        if (_slots[SLOT_LEGS]) {
                _slots[SLOT_LEGS]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("feet : ");
        if (_slots[SLOT_FEET]) {
                _slots[SLOT_FEET]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("ring : ");
        if (_slots[SLOT_RING]) {
                _slots[SLOT_RING]->show ();
        } else {
                printf ("empty\n");
        }
        printf ("ammo : ");
        if (_slots[SLOT_AMMO]) {
                _slots[SLOT_AMMO]->show ();
        } else {
                printf ("empty\n");
        }
}
        
