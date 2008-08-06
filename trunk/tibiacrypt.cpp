#include <stdio.h>
#include "rsa.h"
#include "networkmessage.h"
#include "tibiacrypt.h"
#include "xtea.h"

#define LOGIN_RSA_LEN 147
#define GAME_RSA_LEN 135

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
        msg->setPos (0);
        uint8_t* buffer = msg->getBuffer ();
        uint16_t len = *(uint16_t*)buffer + 2;
        if (len == LOGIN_RSA_LEN) {
                rsa->encrypt (&buffer[19], 128);
        } else if (len == GAME_RSA_LEN) {
                rsa->encrypt (&buffer[7], 128);
        } else if ((len - 2) % 8 == 0) {
                xtea->encrypt (&buffer[2], len - 2);
        } else {
                printf ("encryption error\n");
        }
}

void TibiaCrypt::decrypt (NetworkMessage* msg)
{
        uint8_t* buffer = msg->getBuffer ();
        uint16_t len = *(uint16_t*)buffer + 2;
        if (len == LOGIN_RSA_LEN) {
                rsa->decrypt (&buffer[19], 128);
                msg->setPos (2);
        } else if (len == GAME_RSA_LEN) {
                rsa->decrypt (&buffer[7], 128);
                msg->setPos (2);
        } else if ((len - 2) % 8 == 0) {
                xtea->decrypt (&buffer[2], len - 2);
                msg->setPos (4);
        } else {
                printf ("decryption error\n");
        }
}

