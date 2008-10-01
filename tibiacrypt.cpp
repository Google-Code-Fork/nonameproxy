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

