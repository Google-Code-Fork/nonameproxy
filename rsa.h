#ifndef RSA_H_GUARD
#define RSA_H_GUARD

#include <gmp.h>
#include <stdint.h>

class RSA {
	public:
		RSA();
		~RSA();

		bool setPublicKey(const char* e, const char* m);
		bool setPrivateKey(const char* d, const char* m);

		bool encrypt(uint8_t* buffer, uint32_t size);
		bool decrypt(uint8_t* buffer, uint32_t size);
	private:
		//public key
		mpz_t m_e, m_pubmod;
		bool publicKeySet;
		//private key
		mpz_t m_d, m_prvmod;
		bool privateKeySet;
};

#endif

