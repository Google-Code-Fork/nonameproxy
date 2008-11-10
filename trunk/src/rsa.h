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

#ifndef __RSA_H
#define __RSA_H

#include <gmp.h>
#include <stdint.h>

class RSA {
	public:
		RSA ();
		virtual ~RSA ();

		bool setPublicKey (const char* e, const char* m);
		bool setPrivateKey (const char* d, const char* m);

		bool encrypt (uint8_t* buffer, uint32_t size);
		bool decrypt (uint8_t* buffer, uint32_t size);
	private:
		//public key
		mpz_t m_e, m_pubmod;
		bool publicKeySet;
		//private key
		mpz_t m_d, m_prvmod;
		bool privateKeySet;
};

#endif

