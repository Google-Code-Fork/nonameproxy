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

#include <string>
#include <vector>
#include <stdint.h>

#include "pos.h"
#include "thing.h"
#include "datreader.h"

typedef std::vector<Thing*> ThingVector;

class Tile 
{
        public:
                Tile ();
                virtual ~Tile ();

                void show () const;

                void clear ();
                uint32_t getThingCount () const;

                bool insertThing (const Thing& thing, uint32_t stackpos,
                        DatReader* dat);
                bool removeThing (uint32_t stackpos);
                bool addThing (const Thing& thing, DatReader* dat, 
                        bool push = false);

                Thing* getThingByStackPos (uint32_t stackpos);
                const Thing* getThingByStackPos (uint32_t stackpos) const;
                const Creature* getTopCreature () const;
                const Item* getGround () const;

        private:
                uint32_t i_getOrder (const Thing& thing, DatReader* dat);
                Thing*          _ground;
                ThingVector     _things;
};

class MapState
{
        public:
                Pos& getCurPos ();

                Tile& getTile (uint32_t x, uint32_t y, uint32_t z);
        private:
                Pos _curPos;

                Tile _map [18][14][8];
};
#endif

