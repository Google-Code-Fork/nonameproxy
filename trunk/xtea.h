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

#ifndef __XTEA_H
#define __XTEA_H

#include <stdint.h>

class XTEA {
        public:
                XTEA ();

                bool setKey (const uint32_t* key);

                bool encrypt (uint8_t* buffer, uint32_t size);
                bool decrypt (uint8_t* buffer, uint32_t size);
        private:
                uint32_t xkey[4];
                bool keySet;
};

#endif

