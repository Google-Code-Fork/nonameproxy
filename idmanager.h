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

#ifndef ID_MANAGER_H__
#define ID_MANAGER_H__

/********************************************************************
 * IdManager
 * IdManager is used for managing unique ids
 * IdManager will not assign 0 unless all other ids are used
 * which is insanely unlikely given that this class will run out of
 * addressable memory before then
 ********************************************************************/

#include <stdint.h>
#include <set>
#include <list>

class IdManager
{
        public:
                IdManager ();
                IdManager (uint32_t bin_size);

                /* request a new id */
                uint32_t newId ();
                /* recycle an id, will return false if id not used */
                bool     recycleId (uint32_t id);
        private:
                uint32_t _bin_size;
                std::set<uint32_t> used;
                std::list<uint32_t> recyclebin;
};

#endif
                
