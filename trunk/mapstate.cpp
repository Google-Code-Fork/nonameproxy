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

/*****************************************
 * Tile
 *****************************************/
Tile::Tile (MapState* map)
{
        _ground = NULL;
        _map = map;
}

Tile::Tile ()
{
        _ground = NULL;
}

void Tile::setMap (MapState* map)
{
        _map = map;
}

Tile::~Tile ()
{
        delete _ground;
        _ground = NULL;

        ThingVector::iterator i;
        for (i = _things.begin (); i != _things.end (); i ++) {
                delete (*i);
        }
        _things.clear ();
}

void Tile::clear ()
{
        delete _ground;
        _ground = NULL;

        ThingVector::iterator i;
        for (i = _things.begin (); i != _things.end (); i ++) {
                delete (*i);
        }
        _things.clear ();
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
        
void Tile::show () const
{
        uint32_t stackpos = 0;
        printf ("tile has %d things\n", getThingCount ()); 
        if (_ground) {
                printf ("%d: ", stackpos);
                _ground->show ();
                stackpos ++;
        }
        ThingVector::const_iterator i = _things.end ();
        while (i != _things.begin ()) {
                i --;
                printf ("%d: ", stackpos);
                stackpos ++;
                (*i)->show ();
        }
}
                
bool Tile::addThing (const Thing& thing, DatReader* dat, bool push/*= false*/)
{
        Thing* add;
        if (thing.getType () == Thing::t_creature) {
                const Creature creature = (Creature&)thing;
                add = new Creature (creature);
        } else if (thing.getType () == Thing::t_item) {
                const Item& item = (Item&)thing;
                add = new Item (item);
        } else if (thing.getType () == Thing::t_xitem) {
                const XItem& xitem = (XItem&)thing;
                add = new XItem (xitem);
        } else {
                printf ("addThing error: this item has a funny type\n");
                return false;
        }
        uint32_t order = i_getOrder (thing, dat);
        if (order == 0) {
                if (add->getType () == Thing::t_creature) {
                        /* this is here for paranoia purposes */
                        printf ("addThing error: creature has order 0\n");
                        return false;
                }
                delete _ground;
                _ground = add;
                return true;
        } else {
                if (getThingCount () == 10) {
                        if (!removeThing (9)) {
                                printf ("addThing error: remove failed\n");
                                return false;
                        }
                }
                ThingVector::iterator i;
                for (i = _things.begin (); i != _things.end (); ++ i) {
                        if (push) {
                                if (order <= i_getOrder (**i, dat)) {
                                        break;
                                }
                        } else {
                                if (order < i_getOrder (**i, dat)) {
                                        break;
                                }
                        }
                }
                _things.insert (i, add);
                /* we only insert the creature on a success */
                if (add->getType () == Thing::t_creature) {
                        _map->addCreature ((Creature*)add);
                }
                return true;
        }
}

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
        if ((*i)->getType () == Thing::t_creature) {
                uint32_t tibiaId = ((Creature*)(*i))->getTibiaId ();
                _map->removeCreature (tibiaId);
        }
        delete (*i);
        _things.erase (i);
        return true;
}

Thing& Tile::getThing (uint32_t stackpos) 
{
        uint32_t i = stackpos;
        if (_ground) {
                if (stackpos == 0) {
                        return *_ground;
                } else {
                        i --;
                }
        }
        if (!(stackpos < getThingCount ())) {
                printf ("getThing error: thing %d out of bounds\n", stackpos);
                show ();
        }
        return *_things[i];
}

/* this type of functionality will probably eventually be implemnted
 * in some external library for plugins to use, but for internal use
 * this will do */

uint32_t Tile::i_getOrder (const Thing& thing, DatReader* dat)
{
        if (thing.getType () == Thing::t_creature) {
                return ORDER_CREATURE;
        }
        uint32_t itemId;
        if (thing.getType () == Thing::t_item) {
                itemId = ((Item&)thing).getItemId ();
        } else if (thing.getType () == Thing::t_xitem) {
                itemId = ((XItem&)thing).getItemId ();
        } else {
                printf ("i_getOrder error: bad thing type\n");
                return 0;
        }
        return dat->getItemData (itemId).getOrder ();
}
               
/********************************************************************
 * MapState
 ********************************************************************/
MapState::MapState ()
{
        for (uint32_t x = 0; x < MAP_X; x ++) {
                for (uint32_t y = 0; y < MAP_Y; y ++) {
                        for (uint32_t z = 0; z < MAP_Y; z ++) {
                                _map[x][y][z].setMap (this);
                        }
                }
        }
}

Pos& MapState::getCurPos ()
{
        return _curPos;
}

Tile& MapState::getTile (uint32_t x, uint32_t y, uint32_t z)
{
        return _map [x % 18][y % 14][z % 8];
}

Tile& MapState::getTile (const Pos& pos)
{
        return _map [pos.x % 18][pos.y % 14][pos.z % 8];
}

const Creature& MapState::getCreature (uint32_t tibiaId) const
{
        CreatureMap::const_iterator i = _creatures.find (tibiaId);
        if (i == _creatures.end ()) {
                printf ("getCreature error: creature %d does not exist\n", 
                        tibiaId);
        }
        return *((*i).second);
}

const CreatureMap& MapState::getCreatures () const
{
        return _creatures;
}

bool MapState::addCreature (Creature* creature)
{
        uint32_t tibiaId = creature->getTibiaId ();
        _creatures.insert (std::pair<uint32_t, Creature*> (tibiaId, creature));
        return true;
}

bool MapState::removeCreature (uint32_t tibiaId)
{
        if (_creatures.erase (tibiaId) == 0) {
                printf ("removeCreature error: creature %d does not exist\n",
                        tibiaId);
                return false;
        } else {
                return true;
        }
}

void MapState::showCreatures () const
{
        CreatureMap::const_iterator i;
        for (i = _creatures.begin (); i != _creatures.end (); i ++) {
                (*i).second->show ();
        }
}

