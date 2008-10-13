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

#include <stdio.h>
#include "rsa.h"
#include "networkmessage.h"
#include "tibiacrypt.h"
#include "adler32.h"
#include "xtea.h"

#define LOGIN_RSA_LEN 151
#define GAME_RSA_LEN 139
#define RSA_LEN 128

TibiaCrypt::TibiaCrypt ()
{
        rsa = new RSA ();
        xtea = new XTEA ();
}

TibiaCrypt::~TibiaCrypt ()
{
        delete rsa;
        delete xtea;
}

void TibiaCrypt::setRSAPublicKey (const char* e, const char* m)
{
        rsa->setPublicKey (e, m);
}

void TibiaCrypt::setRSAPrivateKey (const char* d, const char* m)
{
        rsa->setPrivateKey (d, m);
}

void TibiaCrypt::setXTEAKey (const uint32_t* k)
{
        xtea->setKey (k);
}

void TibiaCrypt::encrypt (NetworkMessage* msg)
{
        Adler32 adler;
        uint32_t checksum;

        msg->setPos (0);
        uint8_t* buffer = msg->getBuffer ();
        uint16_t len = *(uint16_t*)buffer + 2;
        if (len == LOGIN_RSA_LEN) {
                rsa->encrypt (&buffer[LOGIN_RSA_LEN - RSA_LEN], RSA_LEN);

                checksum = adler.checksum (&buffer[6], len - 6);
                memcpy (&buffer[2], &checksum, 4);

        } else if (len == GAME_RSA_LEN) {
                rsa->encrypt (&buffer[GAME_RSA_LEN - RSA_LEN], RSA_LEN);

                checksum = adler.checksum (&buffer[6], len - 6);
                memcpy (&buffer[2], &checksum, 4);

        } else if ((len - 6) % 8 == 0) {
                xtea->encrypt (&buffer[6], len - 6);

                checksum = adler.checksum (&buffer[6], len - 6);
                memcpy (&buffer[2], &checksum, 4);

        } else {
                printf ("encryption error\n");
        }
}

void TibiaCrypt::decrypt (NetworkMessage* msg)
{
        uint8_t* buffer = msg->getBuffer ();
        uint16_t len = *(uint16_t*)buffer + 2;
        if (len == LOGIN_RSA_LEN) {
                rsa->decrypt (&buffer[LOGIN_RSA_LEN - RSA_LEN], RSA_LEN);
                msg->setPos (6);
        } else if (len == GAME_RSA_LEN) {
                rsa->decrypt (&buffer[GAME_RSA_LEN - RSA_LEN], RSA_LEN);
                msg->setPos (6);
        } else if ((len - 6) % 8 == 0) {
                xtea->decrypt (&buffer[6], len - 6);
                msg->setPos (8);
        } else {
                printf ("decryption error\n");
        }
}

