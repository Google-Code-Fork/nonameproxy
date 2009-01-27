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

#ifndef __TIBIA_CRYPT_H
#define __TIBIA_CRYPT_H

#include <stdint.h>

class RSA;
class XTEA;
class NetworkMessage;

//automatically detects encryption type of tibia network messages
//and encrypts/decrypts them

class TibiaCrypt
{
        public:
                TibiaCrypt ();
                virtual ~TibiaCrypt ();

                void setRSAPublicKey (const char* e, const char* m);
                void setRSAPrivateKey (const char* e, const char* m);
                void setXTEAKey (const uint32_t* k);

                void encrypt (NetworkMessage* msg);
                void decrypt (NetworkMessage* msg);
        private:
                RSA* rsa;
                XTEA* xtea;
};

#endif

