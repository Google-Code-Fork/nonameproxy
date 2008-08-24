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
                virtual uint8_t getID ();
                virtual void show ();
                virtual void get (NetworkMessage* msg);
};

//for message naming, L = login, G = game, S = send, R = recv
class LSMLoginMsg : public TibiaMessage
{
        public:
                //note that this message reads 145 bytes from the buffer, ie it
                //reads the entire rsa buffer even if they are only random bytes
                LSMLoginMsg (NetworkMessage* msg);
                LSMLoginMsg (uint16_t OS, uint16_t version, uint32_t datsig,
                        uint32_t sprsig, uint32_t picsig, uint8_t u1,
                        uint32_t* xtea, uint32_t account, std::string password);

                virtual ~LSMLoginMsg ();
                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                uint16_t                getOS ();
                uint16_t                getVersion ();
                uint32_t                getDatsig ();
                uint32_t                getSprsig ();
                uint32_t                getPicSig ();
                uint8_t                 getU1 ();
                const uint32_t*         getXTEA ();
                uint32_t                getAccount ();
                const std::string&      getPassword (); 
                
                //im not exactly sure how deriving classes works, if this
                //can go into private it should
                virtual void get (NetworkMessage* msg);
        private:
                TWord8*   _id;
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

class LRMError : public TibiaMessage
{
        public:
                LRMError (NetworkMessage* msg);
                LRMError (std::string msg);

                virtual ~LRMError ();
                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg);
        private:

                TWord8*  _id;
                TString* _errormsg;
};

class LRMInfo : public TibiaMessage
{
        public:
                LRMInfo (NetworkMessage* msg);
                LRMInfo (std::string msg);

                virtual ~LRMInfo ();
                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg);
        private:

                TWord8*  _id;
                TString* _infomsg;
};

//message of the day
class LRMMOTD : public TibiaMessage
{
        public:
                LRMMOTD (NetworkMessage* msg);
                LRMMOTD (std::string msg);

                virtual ~LRMMOTD ();
                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg);
        private:

                TWord8*  _id;
                TString* _motd;
};

class LRMCharacterList : public TibiaMessage
{
        public:
                LRMCharacterList (NetworkMessage* msg);
                //this function takes control of the TCharacterList passed to it
                LRMCharacterList (TCharacterList* charlist, uint16_t daysprem);

                virtual ~LRMCharacterList ();

                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                TCharacterList* getCharList ();
                uint16_t getDaysPrem ();

                virtual void get (NetworkMessage* msg);
        private:
                TWord8*         _id;
                TCharacterList* _charlist;
                TWord16*        _daysprem;
};

class LRMPatchExe : public TibiaMessage
{
        public:
                LRMPatchExe (NetworkMessage* msg);
                LRMPatchExe ();

                virtual ~LRMPatchExe ();

                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg);
        private:
                TWord8* _id;
};

class LRMPatchDat : public TibiaMessage
{
        public:
                LRMPatchDat (NetworkMessage* msg);
                LRMPatchDat ();

                virtual ~LRMPatchDat ();

                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg);
        private:
                TWord8* _id;
};

class LRMPatchSpr : public TibiaMessage
{
        public:
                LRMPatchSpr (NetworkMessage* msg);
                LRMPatchSpr ();

                virtual ~LRMPatchSpr ();

                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg);
        private:
                TWord8* _id;
};

class LRMNewLoginServer : public TibiaMessage
{
        public:
                LRMNewLoginServer (NetworkMessage* msg);
                LRMNewLoginServer ();

                virtual ~LRMNewLoginServer ();

                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg);
        private:
                TWord8* _id;
};

class GSMGameInit : public TibiaMessage
{
        public:
                //note that this message reads 137 bytes from the buffer, ie it
                //reads the entire rsa buffer even if they are only random bytes
                GSMGameInit (NetworkMessage* msg);
                GSMGameInit (uint16_t OS, uint16_t version, uint8_t u1,
                        uint32_t* xtea, uint8_t isGM, uint32_t account,
                        std::string password, std::string name);

                virtual ~GSMGameInit ();
                virtual uint8_t getID ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                uint16_t                getOS ();
                uint16_t                getVersion ();
                uint8_t                 getU1 ();
                const uint32_t*         getXTEA ();
                uint8_t                 getIsGM ();
                uint32_t                getAccount ();
                const std::string&      getName (); 
                const std::string&      getPassword (); 
                
                //im not exactly sure how deriving classes works, if this
                //can go into private it should
                virtual void get (NetworkMessage* msg);
        private:
                TWord8*   _id;
                TWord16*  _OS;
                TWord16*  _version;
                TWord8*   _u1;
                TXTEAKey* _xtea;
                TWord8*   _isGM;
                TWord32*  _account;
                TString*  _name;
                TString*  _password;
                
                //and somewhere to store the remaining bytes
                TByteBuffer* _bytes;
};
#endif

