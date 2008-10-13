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

#ifndef __MAP_STATE_H
#define __MAP_STATE_H

#include <string>
#include <stdint.h>

class Pos
{
        public:
                Pos ();
                Pos (uint32_t xx, uint32_t yy, uint32_t zz);

                uint32_t x;
                uint32_t y;
                uint32_t z;
};

class MapState
{
        public:
                Pos& getCurPos ();
        private:
                Pos _curPos;
};
#endif

