#ifndef __TIBIA_TYPES_H
#define __TIBIA_TYPES_H

#include <stdint.h>
#include <string>
#include <list>

class NetworkMessage;

class TWord8
{
        public:
                TWord8 (NetworkMessage* msg);
                TWord8 (uint8_t val);
                TWord8 (const TWord8& clone); 

                void put (NetworkMessage* msg);

                uint8_t getVal ();
                void show ();
        private:
                void get (NetworkMessage* msg);
                uint8_t _val;
};
                
class TWord16
{
        public:
                TWord16 (NetworkMessage* msg);
                TWord16 (uint16_t val);
                TWord16 (const TWord16& clone);

                void put (NetworkMessage* msg);

                uint16_t getVal ();
                void show ();
        private:
                void get (NetworkMessage* msg);
                uint16_t _val;
};
                
class TWord32
{
        public:
                TWord32 (NetworkMessage* msg);
                TWord32 (uint32_t val);
                TWord32 (const TWord32& clone);

                void put (NetworkMessage* msg);

                uint32_t getVal ();
                void show ();
        private:
                void get (NetworkMessage* msg);
                uint32_t _val;
};
                
class TString
{
        public:
                TString (NetworkMessage* msg);
                TString (const std::string& str);
                TString (const TString& clone);

                void put (NetworkMessage* msg);

                uint32_t getLen ();
                const std::string& getString ();
                void show ();
        private:
                void get (NetworkMessage* msg);
                std::string _str;
};

//this type is only used for reading the remainder of the RSA buffers that
//are random bytes. This is because the login packet header reflects the
//length of the entire packet, and our method of reading messages relies
//on the length of all messages summing to the length of the packet
class TByteBuffer 
{
        public:
                TByteBuffer (uint32_t len, NetworkMessage* msg);
                TByteBuffer (uint32_t len);
                TByteBuffer (const TByteBuffer& clone);
                virtual ~TByteBuffer ();

                void put (NetworkMessage* msg);
                void show ();
        private:
                void get (uint32_t len, NetworkMessage* msg);

                uint32_t _len;
                uint8_t* _buffer;
};

class TXTEAKey
{
        public:
                TXTEAKey (NetworkMessage* msg);
                TXTEAKey (const uint32_t* key);
                TXTEAKey (const TXTEAKey& clone);
                virtual ~TXTEAKey ();

                void put (NetworkMessage* msg);
                
                const uint32_t* getKey ();
                void show ();
        private:
                void get (NetworkMessage* msg);
                TWord32* _key[4];
                uint32_t _keyBuffer[4];
};

class TCharacter
{
        public:
                TCharacter (NetworkMessage* msg);
                TCharacter (const std::string& name, const std::string& world,
                        uint32_t ip, uint16_t port);
                TCharacter (const TCharacter& clone);
                virtual ~TCharacter ();

                const std::string& getName ();
                const std::string& getWorld ();
                uint32_t getIp ();
                uint16_t getPort ();

                void put (NetworkMessage* msg);
                void show ();
        private:
                void get (NetworkMessage* msg);
                TString* _name;
                TString* _world;
                TWord32* _ip;
                TWord16* _port;
};


typedef std::list<TCharacter*> CharList;
class TCharacterList
{
        public:
                TCharacterList (NetworkMessage* msg);
                TCharacterList ();
                TCharacterList (const TCharacterList& clone);
                virtual ~TCharacterList ();
                void addChar (TCharacter* character);
                
                uint32_t getNChars ();
                const CharList& getCharList ();

                void put (NetworkMessage* msg);
                void show ();
        private:
                void get (NetworkMessage* msg);
                TWord8* _nChars;
                CharList _charlist;
};

#endif

