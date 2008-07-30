#ifndef __TIBIA_CRYPT_H
#define __TIBIA_CRYPT_H

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
                void setXTEAKey (uint32_t* k);

                void encrypt (NetworkMessage* msg);
                void decrypt (NetworkMessage* msg);
        private:
                RSA* rsa;
                XTEA* xtea;
};

#endif

