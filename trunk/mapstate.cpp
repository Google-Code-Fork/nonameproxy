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

#include "mapstate.h"

#define ORDER_GROUND 0
#define ORDER_CREATURE 4

/*****************************************
 * Tile
 *****************************************/
Tile::Tile ()
{
        _ground = NULL;
}

Tile::~Tile ()
{
        delete _ground;

        ThingVector::iterator i;
        for (i = _things.begin (); i != _things.end (); i ++) {
                delete (*i);
        }
}

uint32_t Tile::getThingCount () const
{
        return _things.size () + (_ground ? 1 : 0);
}

bool Tile::insertThing (const Thing& thing, uint32_t stackpos, DatReader* dat)
{
        if (getThingCount () == 10) {
                return false;
        }
        if (thing.getType () == Thing::t_creature) {
                /* cannot insert a creature, only add */
                return false;
        }
        if (thing.getType () == Thing::t_item) {
                const Item& item = ((Item&)thing);
                uint32_t itemId = item.getItemId ();
                const ThingData& thingdata = dat->getItemData (itemId);
                if (thingdata.isGround ()) {
                        if (stackpos != 0) {
                                return false;
                        }
                        if (_ground != NULL) {
                                delete _ground;
                        }
                        _ground = new Item (item);
                        return true;
                } else {
                        if (stackpos == 0) {
                                return false;
                        }
                        ThingVector::iterator i = _things.begin ();
                        uint32_t curpos;
                        for (curpos = 1; curpos < stackpos; curpos ++) {
                                i ++;
                                if (i == _things.end ()) {
                                        break;
                                }
                        }
                        if (curpos != stackpos) {
                                return false;
                        }
                        _things.insert (i, new Item (item));
                        return true;
                }       
        } else if (thing.getType () == Thing::t_xitem) {
                const XItem& xitem = ((XItem&)thing);
                uint32_t xitemId = xitem.getItemId ();
                const ThingData& thingdata = dat->getItemData (xitemId);
                if (thingdata.isGround ()) {
                        if (stackpos != 0) {
                                return false;
                        }
                        if (_ground != NULL) {
                                delete _ground;
                        }
                        _ground = new XItem (xitem);
                        return true;
                } else {
                        if (stackpos == 0) {
                                return false;
                        }
                        ThingVector::iterator i = _things.begin ();
                        uint32_t curpos;
                        for (curpos = 1; curpos < stackpos; curpos ++) {
                                i ++;
                                if (i == _things.end ()) {
                                        break;
                                }
                        }
                        if (curpos != stackpos) {
                                return false;
                        }
                        _things.insert (i, new XItem (xitem));
                        return true;
                }       
        }
        printf ("insertThing error: this item has a funny type\n");
        return false;
}
        
/*bool Tile::addThing (const Thing& thing, DatReader* dat)
{
        if (thing.getType () == Thing::t_creature) {
                const Creature creature = (Creature&)thing;
                return i_addThing (new Creature (creature), ORDER_CREATURE);
        } else if (thing.getType () == Thing::t_item) {
                const Item& item = (Item&)thing;
                uint32_t itemId = item.getItemId ();
                uint32_t order = dat->getItemData (itemId).getOrder ();
                return i_addThing (new Item (item), order);
        } else if (thing.getType () == Thing::t_xitem) {
                const XItem& xitem = (XItem&)thing;
                uint32_t itemId = xitem.getItemId ();
                uint32_t order = dat->getItemData (itemId).getOrder ();
                return i_addThing (new XItem (xitem), order);
        }
        printf ("addThing error: this item has a funny type\n");
        return false;
}
*/
/*bool Tile::i_addThing (Thing* thing, uint32_t order, DatReader* dat)
{
        if (thing == NULL) {
                printf ("i_addThing error: somehow i got a NULL *\n");
                return false;
        }
        if (order == 0) {
                delete _ground;
                _ground = thing;
                return true;
        }
        if (getThingCount () == 10) {
                if (!removeThing (9)) {
                        printf ("i_addThing error: internal remove failed\n");
                        return false;
                }
                ThingVector::iterator i;
                for (i = _things.begin (); i != _things.end (); ++ i) {
                        uint32_t curId = (*i)
                        if (
        }
*/

bool Tile::removeThing (uint32_t stackpos)
{
        if (stackpos == 0) {
                if (_ground == NULL) {
                        return false;
                }
                delete _ground;
                _ground = NULL;
                return true;
        }

        ThingVector::iterator i = _things.begin ();
        uint32_t curpos;
        for (curpos = 1; curpos < stackpos; curpos ++) {
                i ++;
                if (i == _things.end ()) {
                        return false;
                }
        }
        if (i == _things.end ()) {
                return false;
        }
        delete (*i);
        _things.erase (i);
        return true;
}

Pos& MapState::getCurPos ()
{
        return _curPos;
}

