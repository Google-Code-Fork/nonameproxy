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

#ifndef __INVENTORY_STATE_H
#define __INVENTORY_STATE_H

#define MAX_CONTAINER 16

#include <string>
#include <vector>
#include <map>
#include <stdint.h>

#include "thing.h"
#include "datreader.h"
#include "enums.h"

typedef std::vector<Thing*> ThingVector;

class InventoryState;

class Container
{
        public:
                Container ();
                virtual ~Container ();
                void show () const;

                /* open a container and set the capacity and name*/
                bool open (uint32_t capacity, uint32_t itemid, bool hasparent,
                        const std::string& name);
                /* close the container and delete all things */
                bool close ();

                bool isOpen () const;
                bool hasParent () const;

                uint32_t getCapacity () const;
                uint32_t getThingCount () const;

                const std::string& getName ();

                /* the item id of the container
                 * were not interested in the x byte */
                uint32_t getItemId () const;

                bool insertThing (const Thing& thing, uint32_t index);
                bool removeThing (uint32_t index);
                bool addThing (const Thing& thing);
                bool addBackThing (const Thing& thing);

                Thing& getThing (uint32_t index);
                const Thing& getThing (uint32_t index) const;
        private:
                bool            _open;
                bool            _hasparent;
                uint32_t        _capacity;
                uint32_t        _itemid;
                std::string     _name;
                ThingVector     _things;
};
                
/*********************************************************************
 * InventoryState
 *********************************************************************/

class InventoryState
{
        public:
                InventoryState ();

                Container&      getContainer (uint32_t cid);
                Thing&          getSlot (slot_t slot);
                bool            emptySlot (slot_t slot);
                bool            setSlot (const Thing& thing, slot_t slot);

                void            showSlots ();
        private:
                Container       _containers[MAX_CONTAINER];
                Thing*          _slots[SLOT_LAST];
};
#endif

