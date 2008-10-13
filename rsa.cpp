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

#include "rsa.h"

#include <stdio.h>

RSA::RSA ()
{
        publicKeySet = false;
        privateKeySet = false;

        //init public key
        mpz_init2(m_e, 1024);
        mpz_init2(m_pubmod, 1024);
        //init private key
        mpz_init2(m_d, 1024);
        mpz_init2(m_prvmod, 1024);
}

RSA::~RSA ()
{
        //public key
        mpz_clear(m_e);
        mpz_clear(m_pubmod);
        //private key
        mpz_clear(m_d);
        mpz_clear(m_prvmod);
}

bool RSA::setPublicKey(const char* e, const char* m)
{
        int32_t res;
        res  = mpz_set_str(m_e, e, 10);
        res |= mpz_set_str(m_pubmod, m, 10);
        if (res == 0) {
                publicKeySet = true;
                return true;
        } else {
                return false;
        }
}

bool RSA::setPrivateKey(const char* d, const char* m)
{
        int32_t res;
        res  = mpz_set_str (m_d, d, 10);
        res |= mpz_set_str (m_prvmod, m, 10);
        if (res == 0) {
                privateKeySet = true;
                return true;
        } else {
                return false;
        }
}

bool RSA::encrypt(uint8_t* buffer, uint32_t size)
{
        if(publicKeySet) {
                mpz_t plain, crypt;
                mpz_init2 (plain, 1024);
                mpz_init2 (crypt, 1024);

                mpz_import (plain, 128, 1, 1, 0, 0, buffer);
                mpz_powm(crypt, plain, m_e, m_pubmod);
                
                //these 3 lines are taken directly from yact, i think i
                //can improve but its been a hassle to get this working
                //so for now it stays
                size_t count = (mpz_sizeinbase(crypt, 2) + 7)/8;
                memset(buffer, 0, size - count);
                mpz_export(&buffer[size - count], NULL, 1, 1, 0, 0, crypt);
                //mpz_export (buffer, NULL, 1, 1, 0, 0, crypt);

                mpz_clear(crypt);
                mpz_clear(plain);
                return true;
        } else {
                return false;
        }
}

bool RSA::decrypt(uint8_t* buffer, uint32_t size)
{
        if(privateKeySet) {
                mpz_t crypt, plain;
                mpz_init2 (plain, 1024);
                mpz_init2 (crypt, 1024);

                mpz_import(crypt, 128, 1, 1, 0, 0, buffer);

                mpz_powm (plain, crypt, m_d, m_prvmod);

                size_t count = (mpz_sizeinbase(plain, 2) + 7)/8;
                memset(buffer, 0, size - count);
                mpz_export(&buffer[size - count], NULL, 1, 1, 0, 0, plain); 
                //mpz_export (buffer, NULL, 1, 1, 0, 0, plain);

                mpz_clear(crypt);
                mpz_clear(plain);
                return true;
        } else {
                return false;
        }
}

