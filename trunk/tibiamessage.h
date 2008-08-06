#ifndef __TIBIA_MESSAGE_H
#define __TIBIA_MESSAGE_H

#include <stdint.h>
#include <string>
#include "tibiatypes.h"

class NetworkMessage;

class TibiaMessage
{
        public:
                virtual void put (NetworkMessage* msg);
                virtual uint32_t type ();
                virtual void show ();
        private:
                virtual void get (NetworkMessage* msg);

};

//for message naming, L = login, G = game, S = send, R = recv
class LSMLoginMsg : TibiaMessage
{
        public:
                //note that this message reads 145 bytes from the buffer, ie it
                //reads the entire rsa buffer even if they are only random bytes
                LSMLoginMsg (NetworkMessage* msg);
                LSMLoginMsg (uint16_t OS, uint16_t version, uint32_t datsig,
                        uint32_t sprsig, uint32_t picsig, uint8_t u1,
                        uint32_t* xtea, uint32_t account, std::string password);

                virtual ~LSMLoginMsg ();
                virtual uint32_t type ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                uint16_t        getOS ();
                uint16_t        getVersion ();
                uint32_t        getDatsig ();
                uint32_t        getSprsig ();
                uint32_t        getPicSig ();
                uint8_t         getU1 ();
                const uint32_t* getXTEA ();
                uint32_t        getAccount ();
                uint32_t        getPassword (); 
        private:
                virtual void get (NetworkMessage* msg);
                uint32_t _id;

                TWord16*  _OS;
                TWord16*  _version;
                TWord32*  _datsig;
                TWord32*  _sprsig;
                TWord32*  _picsig;
                TWord8*   _u1;
                TXTEAKey* _xtea;
                TWord32*  _account;
                TString*  _password;
                
                //and somewhere to store the remaining bytes
                TByteBuffer* _bytes;
};

class LSMessageFactory
{
        public:
                LSMessageFactory (NetworkMessage* msg);
                virtual ~LSMessageFactory ();
                TibiaMessage* getMessage ();
};

#endif

