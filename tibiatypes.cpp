#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "tibiatypes.h"
#include "networkmessage.h"

//TWord8
TWord8::TWord8 (uint8_t val)
{
        _val = val;
}

TWord8::TWord8 (NetworkMessage* msg)
{
        get (msg);
}

TWord8::TWord8 (const TWord8& clone)
{
        _val = clone._val;
}

void TWord8::put (NetworkMessage* msg)
{
        msg->putU8 (_val); 
}

void TWord8::get (NetworkMessage* msg)
{
        msg->getU8 (_val);
}

uint8_t TWord8::getVal ()
{
        return _val;
}

void TWord8::show ()
{
        printf ("%u", _val);
}

//TWord16
TWord16::TWord16 (uint16_t val)
{
        _val = val;
}

TWord16::TWord16 (NetworkMessage* msg)
{
        get (msg);
}

TWord16::TWord16 (const TWord16& clone)
{
        _val = clone._val;
}

void TWord16::put (NetworkMessage* msg)
{
        msg->putU16 (_val); 
}

void TWord16::get (NetworkMessage* msg)
{
        msg->getU16 (_val);
}

uint16_t TWord16::getVal ()
{
        return _val;
}

void TWord16::show ()
{
        printf ("%u", _val);
}
//TWord32
TWord32::TWord32 (uint32_t val)
{
        _val = val;
}

TWord32::TWord32 (NetworkMessage* msg)
{
        get (msg);
}

TWord32::TWord32 (const TWord32& clone)
{
        _val = clone._val;
}

void TWord32::put (NetworkMessage* msg)
{
        msg->putU32 (_val); 
}

void TWord32::get (NetworkMessage* msg)
{
        msg->getU32 (_val);
}

uint32_t TWord32::getVal ()
{
        return _val;
}

void TWord32::show ()
{
        printf ("%u", _val);
}

//TString
TString::TString (NetworkMessage* msg)
{
        get (msg);
}

TString::TString (const std::string& str)
{
        _str = str;
}

TString::TString (const TString& clone)
{
        _str = clone._str;
}

const std::string& TString::getString ()
{
        return _str;
}

uint32_t TString::getLen ()
{
        return _str.length ();
}

void TString::put (NetworkMessage* msg)
{
        uint16_t len = _str.length ();
        const uint8_t* tmp = (const uint8_t*)_str.c_str ();
        msg->putU16 (len);
        msg->putN (tmp, len);
}
        
void TString::get (NetworkMessage* msg)
{
        uint16_t len;
        msg->getU16 (len);
        uint8_t tmp[len + 1];
        msg->getN (tmp, len);
        tmp[len] = '\0';
        _str.assign ((const char*)tmp);
}

void TString::show ()
{
        printf ("%s (len = %u)", _str.c_str (), _str.length ());
}

//TByteBuffer
TByteBuffer::TByteBuffer (uint32_t len, NetworkMessage* msg)
{
        get (len, msg);
}

TByteBuffer::TByteBuffer (uint32_t len)
{
        _buffer = new uint8_t[len];
        _len = len;
        //init the random number generator
        struct timeval tv;
        gettimeofday (&tv, NULL);
        srand (tv.tv_usec);
        //now fill the buffer, most of the time this is called
        //len will be < 8, so i havent bothered making it efficient
        for (uint32_t i = 0; i < len; i ++) {
                _buffer[i] = rand ();
        }
}

TByteBuffer::TByteBuffer (const TByteBuffer& clone)
{
        _len = clone._len;
        _buffer = new uint8_t[_len];
        memcpy (_buffer, clone._buffer, _len);
}

TByteBuffer::~TByteBuffer ()
{
        delete[] _buffer;
}

void TByteBuffer::put (NetworkMessage* msg)
{
        msg->putN (_buffer, _len);
}

void TByteBuffer::get (uint32_t len, NetworkMessage* msg)
{
        _buffer = new uint8_t[len];
        _len = len;
        msg->getN (_buffer, len);
}

void TByteBuffer::show ()
{
        printf ("%d random bytes", _len);
}

//TXTEAKey
TXTEAKey::TXTEAKey (NetworkMessage* msg)
{
        get (msg);
}

TXTEAKey::TXTEAKey (const uint32_t* key)
{
        memcpy (_keyBuffer, key, 16);
        for (uint32_t i = 0; i < 4; i ++) {
                _key[i] = new TWord32 (key[i]);
        }
}

TXTEAKey::TXTEAKey (const TXTEAKey& clone)
{
        memcpy (_keyBuffer, clone._keyBuffer, 16);
        for (uint32_t i = 0; i < 4; i ++) {
                _key[i] = new TWord32 (*clone._key[i]);
        }
}

TXTEAKey::~TXTEAKey ()
{
        for (uint32_t i = 0; i < 4; i ++) {
                delete _key[i]; 
        }
}

void TXTEAKey::put (NetworkMessage* msg)
{
        for (uint32_t i = 0; i < 4; i ++) {
                _key[i]->put (msg);
        }
}

const uint32_t* TXTEAKey::getKey ()
{
        return _keyBuffer;
}

void TXTEAKey::get (NetworkMessage* msg)
{
        for (uint32_t i = 0; i < 4; i ++) {
                _key[i] = new TWord32 (msg);
                _keyBuffer[i] = _key[i]->getVal ();
        }
}

void TXTEAKey::show ()
{
        for (uint32_t i = 0; i < 4; i ++) {
                _key[i]->show ();
                printf (" ");
        }
}
        
//TCharacter
TCharacter::TCharacter (NetworkMessage* msg)
{
        get (msg);
}

TCharacter::TCharacter (const std::string& name, const std::string& world,
        uint32_t ip, uint16_t port)
{
        _name = new TString (name);
        _world = new TString (world);
        _ip = new TWord32 (ip);
        _port = new TWord16 (port);
}

TCharacter::TCharacter (const TCharacter& clone)
{
        _name = new TString (*clone._name);
        _world = new TString (*clone._world);
        _ip = new TWord32 (*clone._ip);
        _port = new TWord16 (*clone._port);
        clone._port->show (); printf (" "); _port->show (); printf ("\n");
}

TCharacter::~TCharacter ()
{
        delete _name;
        delete _world;
        delete _ip;
        delete _port;
}

void TCharacter::put (NetworkMessage* msg)
{
        _name->put (msg);
        _world->put (msg);
        _ip->put (msg);
        _port->put (msg);
}
        
void TCharacter::get (NetworkMessage* msg)
{
        _name = new TString (msg);
        _world = new TString (msg);
        _ip = new TWord32 (msg);
        _port = new TWord16 (msg);
}

const std::string& TCharacter::getName ()
{
        return _name->getString ();
}

const std::string& TCharacter::getWorld ()
{
        return _world->getString ();
}

uint32_t TCharacter::getIp ()
{
        return _ip->getVal ();
}

uint16_t TCharacter::getPort ()
{
        return _port->getVal ();
}

void TCharacter::show ()
{
        printf ("name: "); _name->show (); printf (" ");
        printf ("world: "); _world->show (); printf (" ");
        printf ("ip: "); _ip->show (); printf (" ");
        printf ("port: "); _port->show (); printf (" ");
}

//TCharacterList

TCharacterList::TCharacterList (NetworkMessage* msg)
{
        get (msg);
}

TCharacterList::TCharacterList ()
{
        _nChars = new TWord8 ((uint8_t)0);
}

TCharacterList::TCharacterList (const TCharacterList& clone)
{
        _nChars = new TWord8 (*clone._nChars);
        //first make a copy of to avoid const iterators
        CharList tmp = clone._charlist;
        for (CharList::iterator i = tmp.begin ();
                i != tmp.end (); ++i) {
                _charlist.push_back (new TCharacter (*(*i)));
        }
}

TCharacterList::~TCharacterList ()
{
        delete _nChars;
        for (CharList::iterator i = _charlist.begin (); i != _charlist.end ();
                ++ i) {
                delete (*i);
        }
}

void TCharacterList::addChar (TCharacter* character)
{
        _charlist.push_back (character);
        uint32_t tmp = _nChars->getVal ();
        //i know this is ugly prehaps i will define the operators later
        delete _nChars;
        _nChars = new TWord8 (tmp + 1);
}

uint32_t TCharacterList::getNChars ()
{
        return _nChars->getVal ();
}

const CharList& TCharacterList::getCharList ()
{
        return _charlist;
}

void TCharacterList::get (NetworkMessage* msg)
{
        _nChars = new TWord8 (msg);
        uint32_t n = _nChars->getVal ();
        for (uint32_t i = 0; i < n; i ++) {
                _charlist.push_back (new TCharacter (msg));
        }
}

void TCharacterList::put (NetworkMessage* msg)
{
        _nChars->put (msg);
        for (CharList::iterator i = _charlist.begin (); i != _charlist.end ();
                ++ i) {
                (*i)->put (msg);
        }
}

void TCharacterList::show ()
{
        printf ("\t"); _nChars->show (); printf (" characters {\n");
        for (CharList::iterator i = _charlist.begin (); i != _charlist.end ();
                ++ i) {
                printf ("\t\t");
                (*i)->show ();
                printf ("\n");
        }
        printf ("\t}"); 
}

