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

#include "adler32.h"

#define MOD_ADLER 65521

//copy/pasted from wikipedia
uint32_t Adler32::checksum (uint8_t* buffer, uint32_t len)
{
        uint32_t a = 1, b = 0;
 
        while (len > 0) 
        {
                uint32_t tlen = len > 5552 ? 5552 : len;
                len -= tlen;
                do 
                {
                        a += *buffer++;
                        b += a;
                } while (--tlen);
 
                a %= MOD_ADLER;
                b %= MOD_ADLER;
        }
 
        return (b << 16) | a;
 
}

