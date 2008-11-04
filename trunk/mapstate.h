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

/* It should be noted that a lot of this code is based off YACT */

#ifndef __MAP_STATE_H
#define __MAP_STATE_H

#define MAP_X 18
#define MAP_Y 14 
#define MAP_Z 8

#include <string>
#include <vector>
#include <map>
#include <stdint.h>

#include "pos.h"
#include "thing.h"
#include "datreader.h"

typedef std::vector<Thing*> ThingVector;

class MapState;

class Tile 
{
        public:
                Tile (MapState* map);
                virtual ~Tile ();

                void show () const;

                void clear ();
                uint32_t getThingCount () const;

                bool insertThing (const Thing& thing, uint32_t stackpos,
                        DatReader* dat);
                bool removeThing (uint32_t stackpos);
                bool addThing (const Thing& thing, DatReader* dat, 
                        bool push = false);

                Thing& getThing (uint32_t stackpos);
                const Thing& getThing (uint32_t stackpos) const;
                const Creature& getTopCreature () const;
                const Item& getGround () const;

        protected:
                /* c ++ sucks, therefore i cant initialize an array of tiles
                 * so ive had to add a setMap function to pass the mapstate* */
                Tile ();
                void setMap (MapState* map);
                friend class MapState;

        private:
                uint32_t i_getOrder (const Thing& thing, DatReader* dat);
                Thing*          _ground;
                ThingVector     _things;

                MapState*       _map;
};

/*********************************************************************
 * MapState
 *      MapState stores information about the map AND creatures.
 *      Originally there was a seperate CreatureState, but 
 *      creatures are really part of the map.
 *********************************************************************/

typedef std::map<uint32_t, Creature*> CreatureMap;
class MapState
{
        public:
                MapState ();
                /* map functions */
                Pos& getCurPos ();

                Tile& getTile (uint32_t x, uint32_t y, uint32_t z);
                Tile& getTile (const Pos& pos);

                /* creature functions */
                const Creature& getCreature (uint32_t tibiaId) const;
                const CreatureMap& getCreatures () const;

                void showCreatures () const;

        protected:
                bool addCreature (Creature* creature);
                /* note removeCreature will NOT delete the creature */
                bool removeCreature (uint32_t tibiaId);

                friend class Tile;
        private:
                Pos _curPos;

                Tile            _map [MAP_X][MAP_Y][MAP_Z];
                CreatureMap     _creatures;
};
#endif

