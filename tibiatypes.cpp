#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "tibiatypes.h"
#include "networkmessage.h"
#include "datreader.h"
#include "enums.h"

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

void TWord8::put (NetworkMessage* msg) const
{
        msg->putU8 (_val); 
}

void TWord8::get (NetworkMessage* msg)
{
        msg->getU8 (_val);
}

uint8_t TWord8::getVal () const
{
        return _val;
}

void TWord8::show () const
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

void TWord16::put (NetworkMessage* msg) const
{
        msg->putU16 (_val); 
}

void TWord16::get (NetworkMessage* msg)
{
        msg->getU16 (_val);
}

uint16_t TWord16::getVal () const
{
        return _val;
}

void TWord16::show () const
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

void TWord32::put (NetworkMessage* msg) const
{
        msg->putU32 (_val); 
}

void TWord32::get (NetworkMessage* msg)
{
        msg->getU32 (_val);
}

uint32_t TWord32::getVal () const
{
        return _val;
}

void TWord32::show () const
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

const std::string& TString::getString () const
{
        return _str;
}

uint32_t TString::getLen () const
{
        return _str.length ();
}

void TString::put (NetworkMessage* msg) const
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

void TString::show () const
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

void TByteBuffer::put (NetworkMessage* msg) const
{
        msg->putN (_buffer, _len);
}

void TByteBuffer::get (uint32_t len, NetworkMessage* msg)
{
        _buffer = new uint8_t[len];
        _len = len;
        msg->getN (_buffer, len);
}

void TByteBuffer::show () const
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

void TXTEAKey::put (NetworkMessage* msg) const
{
        for (uint32_t i = 0; i < 4; i ++) {
                _key[i]->put (msg);
        }
}

const uint32_t* TXTEAKey::getKey () const
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

void TXTEAKey::show () const
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

void TCharacter::put (NetworkMessage* msg) const
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

const std::string& TCharacter::getName () const
{
        return _name->getString ();
}

const std::string& TCharacter::getWorld () const
{
        return _world->getString ();
}

uint32_t TCharacter::getIp () const
{
        return _ip->getVal ();
}

uint16_t TCharacter::getPort () const
{
        return _port->getVal ();
}

void TCharacter::show () const
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

uint32_t TCharacterList::getNChars () const
{
        return _nChars->getVal ();
}

const CharList& TCharacterList::getCharList () const
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

void TCharacterList::put (NetworkMessage* msg) const
{
        _nChars->put (msg);
        //for these functions to be const we must make a copy of the
        //char list before iterating through it. Its not like this will
        //be in a tight loop so we take this option

        CharList tmp = _charlist;
        for (CharList::iterator i = tmp.begin (); i != tmp.end (); ++ i) {
                (*i)->put (msg);
        }
}

void TCharacterList::show () const
{
        printf ("\t"); _nChars->show (); printf (" characters {\n");

        CharList tmp = _charlist;
        for (CharList::iterator i = tmp.begin (); i != tmp.end (); ++ i) {
                printf ("\t\t");
                (*i)->show ();
                printf ("\n");
        }
        printf ("\t}"); 
}

/******************************************************************
 * TPos
 ******************************************************************/

TPos::TPos (NetworkMessage* msg)
{
        get (msg);
}

TPos::TPos (uint16_t xPos, uint16_t yPos, uint8_t zPos)
{
        _x = new TWord16 (xPos);
        _y = new TWord16 (yPos);
        _z = new TWord8  (zPos);
}

TPos::TPos (const TPos& clone)
{
        _x = new TWord16 (*clone._x);
        _y = new TWord16 (*clone._y);
        _z = new TWord8  (*clone._z);
}

TPos::~TPos ()
{
        delete _x;
        delete _y;
        delete _z;
}

void TPos::put (NetworkMessage* msg) const
{
        _x->put (msg);
        _y->put (msg);
        _z->put (msg);
}

void TPos::show () const
{
        printf ("\tTPos {x: "); _x->show ();
        printf (" y: "); _y->show ();
        printf (" z: "); _z->show ();
        printf ("}\n");
}

uint16_t TPos::x () const
{
        return (_x->getVal ());
}

uint16_t TPos::y () const
{
        return (_y->getVal ());
}

uint8_t TPos::z () const
{
        return (_z->getVal ());
}

void TPos::get (NetworkMessage* msg)
{
        _x = new TWord16 (msg);
        _y = new TWord16 (msg);
        _z = new TWord8  (msg);
}

/******************************************************************
 * TItemLight
 ******************************************************************/

TItemLight::TItemLight (NetworkMessage* msg)
{
        get (msg);
}

TItemLight::TItemLight (uint16_t color, uint16_t radius)
{
         _color = new TWord16 (color);
         _radius = new TWord16 (radius);
}

TItemLight::TItemLight (const TItemLight& clone)
{
        _color = new TWord16 (*clone._color);
        _radius = new TWord16 (*clone._radius);
}

TItemLight::~TItemLight ()
{
        delete _color;
        delete _radius;
}

uint16_t TItemLight::getColor () const
{
        return (_color->getVal ());
}

uint16_t TItemLight::getRadius () const
{
        return (_radius->getVal ());
}

void TItemLight::put (NetworkMessage* msg) const
{
        _color->put (msg);
        _radius->put (msg);
}

void TItemLight::show () const
{
        printf ("\tTItemLight {color: "); _color->show ();
        printf (" raduis"); _radius->show (); printf ("}\n");
}

void TItemLight::get (NetworkMessage* msg)
{
        _color = new TWord16 (msg);
        _radius = new TWord16 (msg);
}

/******************************************************************
 * TCreatureLight
 ******************************************************************/

TCreatureLight::TCreatureLight (NetworkMessage* msg)
{
        get (msg);
}

TCreatureLight::TCreatureLight (uint8_t color, uint8_t radius)
{
         _color = new TWord8 (color);
         _radius = new TWord8 (radius);
}

TCreatureLight::TCreatureLight (const TCreatureLight& clone)
{
        _color = new TWord8 (*clone._color);
        _radius = new TWord8 (*clone._radius);
}

TCreatureLight::~TCreatureLight ()
{
        delete _color;
        delete _radius;
}

uint8_t TCreatureLight::getColor () const
{
        return (_color->getVal ());
}

uint8_t TCreatureLight::getRadius () const
{
        return (_radius->getVal ());
}

void TCreatureLight::put (NetworkMessage* msg) const
{
        _color->put (msg);
        _radius->put (msg);
}

void TCreatureLight::show () const
{
        printf ("\tTCreatureLight {color: "); _color->show ();
        printf (" raduis"); _radius->show (); printf ("}\n");
}

void TCreatureLight::get (NetworkMessage* msg)
{
        _color = new TWord8 (msg);
        _radius = new TWord8 (msg);
}

/**************************************************************
 * TThing - this is a long one
 **************************************************************/

//TItem

TItem::TItem (NetworkMessage* msg)
{
        get (msg);
}

TItem::TItem (uint16_t itemId)
{
        _itemId = new TWord16 (itemId);
}

TItem::TItem (const TItem& clone)
{
        _itemId = new TWord16 (*clone._itemId);
}

TItem::~TItem ()
{
        delete _itemId;
}

TItem::ThingType TItem::getType () const
{
        return TThing::item;
}
                
void TItem::show () const
{
        printf ("\tItem {id = "); _itemId->show (); printf ("}\n)");
}

void TItem::put (NetworkMessage* msg) const
{
        _itemId->put (msg);
}
        
uint16_t TItem::getItemId () const
{
        return _itemId->getVal ();
}

void TItem::get (NetworkMessage* msg)
{
        _itemId = new TWord16 (msg);
}

//TXItem

TXItem::TXItem (NetworkMessage* msg)
{
        get (msg);
}

TXItem::TXItem (uint16_t itemId, uint8_t xByte)
{
        _itemId = new TWord16 (itemId);
        _xByte = new TWord8 (xByte);
}

TXItem::TXItem (const TXItem& clone)
{
        _itemId = new TWord16 (*clone._itemId);
        _xByte = new TWord8 (*clone._xByte);
}

TXItem::~TXItem ()
{
        delete _itemId;
        delete _xByte;
}

TThing::ThingType TXItem::getType () const
{
        return TThing::xitem;
}
                
void TXItem::show () const
{
        printf ("\tXItem {id = "); _itemId->show (); 
        printf ("xByte = "); _xByte->show (); printf ("}\n)");
}

void TXItem::put (NetworkMessage* msg) const
{
        _itemId->put (msg);
        _xByte->put (msg);
}
        

uint16_t TXItem::getItemId () const
{
        return _itemId->getVal ();
}

uint8_t TXItem::getXByte () const
{
        return _xByte->getVal ();
}

void TXItem::get (NetworkMessage* msg)
{
        _itemId = new TWord16 (msg);
        _xByte = new TWord8 (msg);
}

//TOldCreature

TOldCreature::TOldCreature (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
}

TOldCreature::TOldCreature (uint32_t tibiaId, uint8_t hp,
                uint8_t direction, const TOutfit& outfit,
                const TCreatureLight& light, uint16_t speed,
                uint8_t skull, uint8_t shield)
{
        _itemId =       new TWord16 ((uint16_t)0x6200);
        _tibiaId =      new TWord32 (tibiaId);
        _hp =           new TWord8 (hp);
        _direction =    new TWord8 (direction);
        _light =        new TCreatureLight (light);
        _speed =        new TWord16 (speed);
        _skull =        new TWord8 (skull);
        _shield =       new TWord8 (shield);
        
        TOutfitFactory of;
        _outfit =       of.cloneOutfit (outfit);
}

TOldCreature::TOldCreature (const TOldCreature& clone)
{
        _itemId =       new TWord16 (*clone._itemId);
        _tibiaId =      new TWord32 (*clone._tibiaId);
        _hp =           new TWord8 (*clone._hp);
        _direction =    new TWord8 (*clone._direction);
        _light =        new TCreatureLight (*clone._light);
        _speed =        new TWord16 (*clone._speed);
        _skull =        new TWord8 (*clone._skull);
        _shield =       new TWord8 (*clone._shield);
        
        TOutfitFactory of;
        _outfit =       of.cloneOutfit (*clone._outfit);
}

TOldCreature::~TOldCreature ()
{
        delete _itemId;
        delete _tibiaId;
        delete _hp;
        delete _direction;
        delete _outfit;
        delete _light;
        delete _speed;
        delete _skull;
        delete _shield;
}

TThing::ThingType TOldCreature::getType () const
{
        return TThing::oldcreature;
}
                
void TOldCreature::show () const
{
        printf ("\tTOldCreature {\n");
        printf ("\t\t_tibiaId: "); _tibiaId->show (); printf ("\n");
        printf ("\t\t_hp: "); _hp->show (); printf ("\n");
        printf ("\t\t_direction: "); _direction->show (); printf ("\n");
        printf ("\t\t_outfit: "); _outfit->show (); printf ("\n");
        printf ("\t\t_light: "); _light->show (); printf ("\n");
        printf ("\t\t_speed: "); _speed->show (); printf ("\n");
        printf ("\t\t_skull: "); _skull->show (); printf ("\n");
        printf ("\t\t_shield: "); _shield->show (); printf ("\n");
        printf ("}\n");
}

void TOldCreature::put (NetworkMessage* msg) const
{
        _itemId->put (msg);
        _tibiaId->put (msg);
        _hp->put (msg);
        _direction->put (msg);
        _outfit->put (msg);
        _light->put (msg);
        _speed->put (msg);
        _skull->put (msg);
        _shield->put (msg);
}

uint16_t TOldCreature::getItemId () const
{
        return _itemId->getVal ();
}

uint32_t TOldCreature::getTibiaId () const
{
        return _tibiaId->getVal ();
}

uint8_t TOldCreature::getHp () const
{
        return _hp->getVal ();
}

uint8_t TOldCreature::getDirection () const
{
        return _direction->getVal ();
}

const TOutfit& TOldCreature::getOutfit () const
{
        return *_outfit;
}

const TCreatureLight& TOldCreature::getCreatureLight () const
{
        return *_light;
}

uint16_t TOldCreature::getSpeed () const
{
        return _speed->getVal ();
}

uint8_t TOldCreature::getSkull () const
{
        return _skull->getVal ();
}

uint8_t TOldCreature::getShield () const
{
        return _shield->getVal ();
}

void TOldCreature::get (NetworkMessage* msg, DatReader* dat)
{
        _itemId = new TWord16 (msg);
        _tibiaId = new TWord32 (msg); 
        _hp = new TWord8 (msg); 
        _direction = new TWord8 (msg); 

        TOutfitFactory of (msg, dat);
        _outfit = of.getOutfit ();

        _light = new TCreatureLight (msg); 
        _speed = new TWord16 (msg); 
        _skull = new TWord8 (msg); 
        _shield = new TWord8 (msg); 

}

//TNewCreature

TNewCreature::TNewCreature (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
}

TNewCreature::TNewCreature (uint32_t removeId, uint32_t tibiaId,
                const std::string& name, uint8_t hp,
                uint8_t direction, const TOutfit& outfit,
                const TCreatureLight& light, uint16_t speed,
                uint8_t skull, uint8_t shield)
{
        _itemId =       new TWord16 ((uint16_t)0x0061);
        _removeId =     new TWord32 (removeId);
        _tibiaId =      new TWord32 (tibiaId);
        _name =         new TString (name);
        _hp =           new TWord8 (hp);
        _direction =    new TWord8 (direction);
        _light =        new TCreatureLight (light);
        _speed =        new TWord16 (speed);
        _skull =        new TWord8 (skull);
        _shield =       new TWord8 (shield);
        
        TOutfitFactory of;
        _outfit =       of.cloneOutfit (outfit);
}

TNewCreature::TNewCreature (const TNewCreature& clone)
{
        _itemId =       new TWord16 (*clone._itemId);
        _removeId =     new TWord32 (*clone._removeId);
        _tibiaId =      new TWord32 (*clone._tibiaId);
        _name =         new TString (*clone._name);
        _hp =           new TWord8 (*clone._hp);
        _direction =    new TWord8 (*clone._direction);
        _light =        new TCreatureLight (*clone._light);
        _speed =        new TWord16 (*clone._speed);
        _skull =        new TWord8 (*clone._skull);
        _shield =       new TWord8 (*clone._shield);
        
        TOutfitFactory of;
        _outfit =       of.cloneOutfit (*clone._outfit);
}

TNewCreature::~TNewCreature ()
{
        delete _itemId;
        delete _removeId;
        delete _tibiaId;
        delete _name;
        delete _hp;
        delete _direction;
        delete _outfit;
        delete _light;
        delete _speed;
        delete _skull;
        delete _shield;
}

TThing::ThingType TNewCreature::getType () const
{
        return TThing::newcreature;
}
                
void TNewCreature::show () const
{
        printf ("\tTNewCreature {\n");
        printf ("\t\t_removeId: "); _removeId->show (); printf ("\n");
        printf ("\t\t_tibiaId: "); _tibiaId->show (); printf ("\n");
        printf ("\t\t_name: "); _name->show (); printf ("\n");
        printf ("\t\t_hp: "); _hp->show (); printf ("\n");
        printf ("\t\t_direction: "); _direction->show (); printf ("\n");
        printf ("\t\t_outfit: "); _outfit->show (); printf ("\n");
        printf ("\t\t_light: "); _light->show (); printf ("\n");
        printf ("\t\t_speed: "); _speed->show (); printf ("\n");
        printf ("\t\t_skull: "); _skull->show (); printf ("\n");
        printf ("\t\t_shield: "); _shield->show (); printf ("\n");
        printf ("}\n");
}

void TNewCreature::put (NetworkMessage* msg) const
{
        _itemId->put (msg);
        _removeId->put (msg);
        _tibiaId->put (msg);
        _name->put (msg);
        _hp->put (msg);
        _direction->put (msg);
        _outfit->put (msg);
        _light->put (msg);
        _speed->put (msg);
        _skull->put (msg);
        _shield->put (msg);
}

uint16_t TNewCreature::getItemId () const
{
        return _itemId->getVal ();
}

uint32_t TNewCreature::getRemoveId () const
{
        return _removeId->getVal ();
}

uint32_t TNewCreature::getTibiaId () const
{
        return _tibiaId->getVal ();
}

const std::string& TNewCreature::getName () const
{
        return _name->getString ();
}

uint8_t TNewCreature::getHp () const
{
        return _hp->getVal ();
}

uint8_t TNewCreature::getDirection () const
{
        return _direction->getVal ();
}

const TOutfit& TNewCreature::getOutfit () const
{
        return *_outfit;
}

const TCreatureLight& TNewCreature::getCreatureLight () const
{
        return *_light;
}

uint16_t TNewCreature::getSpeed () const
{
        return _speed->getVal ();
}

uint8_t TNewCreature::getSkull () const
{
        return _skull->getVal ();
}

uint8_t TNewCreature::getShield () const
{
        return _shield->getVal ();
}

void TNewCreature::get (NetworkMessage* msg, DatReader* dat)
{
        _itemId = new TWord16 (msg);
        _removeId = new TWord32 (msg); 
        _tibiaId = new TWord32 (msg); 
        _name = new TString (msg);
        _hp = new TWord8 (msg); 
        _direction = new TWord8 (msg); 

        TOutfitFactory of (msg, dat);
        _outfit = of.getOutfit ();

        _light = new TCreatureLight (msg); 
        _speed = new TWord16 (msg); 
        _skull = new TWord8 (msg); 
        _shield = new TWord8 (msg); 

}

//Yes this could be derived from TCreature, but that makes thing complicated
// TOldCreature
/*
TOldCreature::TOldCreature (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
}

TOldCreature::TOldCreature (uint32_t tibiaId, const std::string name, 
                uint8_t hp, uint8_t direction, const TOutfit& outfit, 
                const TCreatureLight& light, uint16_t speed,
                uint8_t skull, uint8_t shield)
{
        _itemId = new TWord16 ((uint16_t)0x0062);
        _creature = new TCreature (tibiaId, name, hp, direction, outfit, light,
                                        speed, skull, shield);
}

TOldCreature::TOldCreature (const TOldCreature& clone)
{
        _itemId = new TWord16 (*clone._itemId);
        _creature = new TCreature (*clone._creature);
}

TOldCreature::~TOldCreature ()
{
        delete _itemId;
        delete _creature;
}

TThing::ThingType TOldCreature::getType () const
{
        return TThing::oldcreature;
}
                
void TOldCreature::show () const
{
        printf ("\tOldCreature {\n");
        _creature->show ();
        printf ("}\n");
}

void TOldCreature::put (NetworkMessage* msg) const
{
        _itemId->put (msg);
        _creature->put (msg);
}

TCreature* TOldCreature::getCreature () const
{
        return new TCreature (*_creature);
}

void TOldCreature::get (NetworkMessage* msg, DatReader* dat)
{
        _itemId = new TWord16 (msg);
        _creature = new TCreature (msg, dat);
};

// TNewCreature
TNewCreature::TNewCreature (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
}

TNewCreature::TNewCreature (uint32_t removeId, uint32_t tibiaId, 
                const std::string name, uint8_t hp, uint8_t direction, 
                const TOutfit& outfit, const TCreatureLight& light, 
                uint16_t speed, uint8_t skull, uint8_t shield)
{
        _itemId = new TWord16 ((uint16_t)0x0061);
        _removeId = new TWord32 (removeId);
        _creature = new TCreature (tibiaId, name, hp, direction, outfit, light,
                                        speed, skull, shield);
}

TNewCreature::TNewCreature (const TNewCreature& clone)
{
        _itemId = new TWord16 (*clone._itemId);
        _removeId = new TWord32 (*clone._removeId);
        _creature = new TCreature (*clone._creature);
}

TNewCreature::~TNewCreature ()
{
        delete _itemId;
        delete _removeId;
        delete _creature;
}

TThing::ThingType TNewCreature::getType () const
{
        return TThing::newcreature;
}
                
void TNewCreature::show () const
{
        printf ("\tNewCreature {removeId: "); _removeId->show (); printf ("\n");
        _creature->show ();
        printf ("}\n");
}

void TNewCreature::put (NetworkMessage* msg) const
{
        _itemId->put (msg);
        _removeId->put (msg);
        _creature->put (msg);
}

uint32_t TNewCreature::getRemoveId () const
{
        return _removeId->getVal () ;
}

TCreature* TNewCreature::getCreature () const
{
        return new TCreature (*_creature);
}

void TNewCreature::get (NetworkMessage* msg, DatReader* dat)
{
        _itemId = new TWord16 (msg);
        _removeId = new TWord32 (msg);
        _creature = new TCreature (msg, dat);
};
*/
//TCreatureTurn
TCreatureTurn::TCreatureTurn (NetworkMessage* msg)
{
        get (msg);
}

TCreatureTurn::TCreatureTurn (uint32_t tibiaId, uint8_t dir)
{
        _itemId = new TWord16 ((uint16_t)0x0063);
        _tibiaId = new TWord32 (tibiaId);
        _dir = new TWord8 (dir);
}

TCreatureTurn::TCreatureTurn (const TCreatureTurn& clone)
{
        _itemId = new TWord16 (*clone._itemId);
        _tibiaId = new TWord32 (*clone._tibiaId);
        _dir = new TWord8 (*clone._dir);
}

TCreatureTurn::~TCreatureTurn ()
{
        delete _itemId;
        delete _tibiaId;
        delete _dir;
}

TThing::ThingType TCreatureTurn::getType () const
{
        return TThing::creatureturn;
}
                
void TCreatureTurn::show () const
{
        printf ("\tTCreatureTurn {tibiaId: "); _tibiaId->show ();
        printf (" dir: "); _dir->show (); printf ("}\n");
}

void TCreatureTurn::put (NetworkMessage* msg) const
{
        _itemId->put (msg);
        _tibiaId->put (msg);
        _dir->put (msg);
}

uint32_t TCreatureTurn::getTibiaId () const
{
        return _tibiaId->getVal ();
}

uint8_t TCreatureTurn::getDir () const
{
        return _dir->getVal ();
}

void TCreatureTurn::get (NetworkMessage* msg)
{
        _itemId = new TWord16 (msg);
        _tibiaId = new TWord32 (msg);
        _dir = new TWord8 (msg);
};

//TSkip
TSkip::TSkip (NetworkMessage* msg)
{
        get (msg);
}

TSkip::TSkip (uint8_t n)
{
        _ff = new TWord8 ((uint8_t)0xFF);
        _n = new TWord8 (n);
}

TSkip::TSkip (const TSkip& clone)
{
        _n = new TWord8 (*clone._n);
        _ff = new TWord8 (*clone._ff);
}

TSkip::~TSkip ()
{
        delete _ff;
        delete _n;
}

TThing::ThingType TSkip::getType () const
{
        return TThing::skip;
}

void TSkip::show () const
{
        printf ("\tTSkip {n: "); _n->show (); printf ("}\n");
}

void TSkip::put (NetworkMessage* msg) const
{
        _n->put (msg);
        _ff->put (msg);
}

uint8_t TSkip::getN () const
{
        return _n->getVal ();
}

void TSkip::get (NetworkMessage* msg)
{
        _n = new TWord8 (msg);
        _ff = new TWord8 (msg);
}

/***************************************************************************
 * TThingFactory
 ***************************************************************************/

TThingFactory::TThingFactory (NetworkMessage* msg, DatReader* dat)
{
        _msg = msg;
        _dat = dat;
        _readable = 1;
}

TThingFactory::TThingFactory ()
{
        _readable = 0;
}

TThing* TThingFactory::getThing ()
{
        if (!_readable) {
                printf ("error: TThingFactory not initialized to read\n");
                return NULL;
        }
        uint16_t id;
        _msg->peekU16 (id);

        if (id == 0x0061) {
                return new TNewCreature (_msg, _dat);
        } else if (id == 0x0062) {
                return new TOldCreature (_msg, _dat);
        } else if (id == 0x0063) {
                return new TCreatureTurn (_msg);
        } else if (id >= 0xFF00) {
                return new TSkip (_msg);
        } else {
                if (_dat->getItemData (id)->isXItem ()) {
                        return new TXItem (_msg);
                } else {
                        return new TItem (_msg);
                }
        }
}

TThing* TThingFactory::cloneThing (const TThing& thing)
{
        TThing::ThingType tt = thing.getType ();
        if (tt == TThing::item) {
                return new TItem ((const TItem&)thing);
        } else if (tt == TThing::xitem) {
                return new TXItem ((const TXItem&)thing);
        } else if (tt == TThing::oldcreature) {
                return new TOldCreature ((const TOldCreature&)thing);
        } else if (tt == TThing::newcreature) {
                return new TNewCreature ((const TNewCreature&)thing);
        } else if (tt == TThing::skip) {
                return new TSkip ((const TSkip&)thing);
        } else {
                printf ("error: unreconized ThingType %d\n", tt);
                return NULL;
        }
}

/**************************************************************
 * TOutfit
 **************************************************************/

// TItemOutift
TItemOutfit::TItemOutfit (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
}

TItemOutfit::TItemOutfit (const TThing& item)
{
        _lookType = new TWord16 ((uint16_t)0x0000);
        TThingFactory tf;
        _item = tf.cloneThing (item);
}

TItemOutfit::TItemOutfit (const TItemOutfit& clone)
{
        _lookType = new TWord16 (*clone._lookType);
        TThingFactory tf;
        _item = tf.cloneThing (*clone._item);
}

TItemOutfit::~TItemOutfit ()
{
        delete _lookType;
        delete _item;
}

TOutfit::OutfitType TItemOutfit::getType () const
{
        return TOutfit::itemoutfit;
}

void TItemOutfit::put (NetworkMessage* msg) const
{
        _lookType->put (msg);
        _item->put (msg);
}

void TItemOutfit::show () const
{
        printf ("TItemOutfit {"); _item->show (); printf ("}\n");
}

void TItemOutfit::get (NetworkMessage* msg, DatReader* dat)
{
        _lookType = new TWord16 (msg);
        TThingFactory tf (msg, dat);
        _item = tf.getThing ();
}

//TCharOutfit

TCharOutfit::TCharOutfit (NetworkMessage* msg)
{
        get (msg);
}

TCharOutfit::TCharOutfit (uint16_t lookType, uint8_t head, uint8_t body,
                uint8_t legs, int8_t feet, uint8_t addons)
{
        _lookType = new TWord16 (lookType);
        _head = new TWord8 (head);
        _body = new TWord8 (body);
        _legs = new TWord8 (legs);
        _feet = new TWord8 (feet);
        _addons = new TWord8 (addons);
}

TCharOutfit::TCharOutfit (const TCharOutfit& clone)
{
        _lookType = new TWord16 (*clone._lookType);
        _head = new TWord8 (*clone._head);
        _body = new TWord8 (*clone._body);
        _legs = new TWord8 (*clone._legs);
        _feet = new TWord8 (*clone._feet);
        _addons = new TWord8 (*clone._addons);
}

TCharOutfit::~TCharOutfit ()
{
        delete _lookType;
        delete _head;
        delete _body;
        delete _legs;
        delete _feet;
        delete _addons;
}

TOutfit::OutfitType TCharOutfit::getType () const
{
        return TOutfit::charoutfit;
}

void TCharOutfit::put (NetworkMessage* msg) const
{
        _lookType->put (msg);
        _head->put (msg);
        _body->put (msg);
        _legs->put (msg);
        _feet->put (msg);
        _addons->put (msg);
}

void TCharOutfit::show () const
{
        printf ("\tTCharOutfit {\n");
        printf ("\t\tlookType: "); _lookType->show (); printf ("\n");
        printf ("\t\thead: "); _head->show (); printf ("\n");
        printf ("\t\tbody: "); _body->show (); printf ("\n");
        printf ("\t\tlegs: "); _legs->show (); printf ("\n");
        printf ("\t\tfeet: "); _feet->show (); printf ("\n");
        printf ("\t\taddons: "); _addons->show (); printf ("\n");
        printf ("}\n");
}

void TCharOutfit::get (NetworkMessage* msg)
{
        _lookType = new TWord16 (msg);
        _head = new TWord8 (msg);
        _body = new TWord8 (msg);
        _legs = new TWord8 (msg);
        _feet = new TWord8 (msg);
        _addons = new TWord8 (msg);
}

/********************************************************************
 * TOutfitFactory
 ********************************************************************/

TOutfitFactory::TOutfitFactory (NetworkMessage* msg, DatReader* dat)
{
        _msg = msg;
        _dat = dat;
        _readable = 1;
}

TOutfitFactory::TOutfitFactory ()
{
        _readable = 0;
}

TOutfit* TOutfitFactory::cloneOutfit (const TOutfit& clone)
{
        TOutfit::OutfitType ot = clone.getType ();
        if (ot == TOutfit::itemoutfit) {
                return new TItemOutfit((const TItemOutfit&)clone);
        } else if (ot == TOutfit::charoutfit) {
                return new TCharOutfit((const TCharOutfit&)clone);
        } else {
                printf ("error: unreconized outfit type %d\n", ot);
                return NULL;
        }
}
        
TOutfit* TOutfitFactory::getOutfit ()
{
        if (!_readable) {
                printf ("error: outfit factory is not initialized to read\n");
                return NULL;
        }
        uint16_t looktype;
        _msg->peekU16 (looktype);
        if (looktype == 0x0000) {
                return new TItemOutfit (_msg, _dat);
        } else {
                return new TCharOutfit (_msg);
        }
}

/************************************************************************
 * TMapDescription
 ************************************************************************/

TMapDescription::TMapDescription (const TPos& start, const TPos& end,
                NetworkMessage* msg, DatReader* dat)
{
        _start = new TPos (start);
        _cur = new TPos (start);
        _end = new TPos (end);
        _it = _map.begin ();

        get (start, end, msg, dat);
}
                
        
TMapDescription::TMapDescription (const TPos& start, const TPos& end)
{
        _start = new TPos (start);
        _cur = new TPos (start);
        _end = new TPos (end);
        _it = _map.begin ();
}

TMapDescription::TMapDescription (const TMapDescription& clone)
{
        _start = new TPos (*clone._start);
        _cur = new TPos (*clone._start);
        _end = new TPos (*clone._end);
        //use _it to go through clone list, then well assign
        //it afterwards
        TThingFactory tf;

        MapList::const_iterator i;
        for (i = clone._map.begin (); i != clone._map.end (); ++i) {
                _map.push_back (tf.cloneThing (*(*i)));
        }
        _it = _map.begin ();
}

TMapDescription::~TMapDescription ()
{
        delete _start;
        delete _cur;
        delete _end;

        for (_it = _map.begin (); _it != _map.end (); ++_it) {
                delete (*_it);
        }
}

void TMapDescription::put (NetworkMessage* msg) const
{
        MapList::const_iterator i;
        for (i = _map.begin (); i != _map.end (); ++i) {
                (*i)->put (msg);
        }
}

void TMapDescription::show () const
{
        printf ("\tTMapDescription {\n");
        MapList::const_iterator i;
        for (i = _map.begin (); i != _map.end (); ++i) {
                printf ("\t"); (*i)->show (); printf ("\n");
        }
        printf ("}\n");
}
        
const TPos& TMapDescription::getStart () const
{
        return *_start;
}

const TPos& TMapDescription::getEnd () const
{
        return *_end;
}


void TMapDescription::begin ()
{
        _it = _map.begin ();
}

bool TMapDescription::isEnd ()
{
        if (_it == _map.end ()) {
                return true;
        } else {
                return false;
        }
}

void TMapDescription::next ()
{
        if (_it == _map.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TThing& TMapDescription::getThing ()
{
        return *(*_it);
}

void TMapDescription::insert (TThing* tm)
{
        _map.insert (_it, tm);
}

void TMapDescription::replace (TThing* tm)
{
        if (_map.size () == 0) {
                printf ("map error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _map.end ()) {
                printf ("map error: attemp to replace \"end\"\n");
                return;
        }
        _it = _map.erase (_it);
        _map.insert (_it, tm);
        _it --;
} 

void TMapDescription::remove ()
{
        _it = _map.erase (_it);
}

void TMapDescription::add (TThing* thing)
{
        _map.push_back (thing);
}

void TMapDescription::get (const TPos& start, const TPos& end,
                NetworkMessage* msg, DatReader* dat)
{
        uint32_t nThings = (end.x () - start.x () + 1) *
                         (end.y () - start.y () + 1) *
                         (end.z () - start.z () + 1);

        TThingFactory tf (msg, dat);
        TThing* thing;
        
        while (nThings != 0) {
                thing = tf.getThing ();
                if (!thing) {
                        printf ("null thing\n");
                }
                if (thing->getType () == TThing::skip) {
                        //we alway skip one byte
                        nThings --;
                        nThings -= ((TSkip*)thing)->getN ();
                }
                _map.push_back (thing);
                if (nThings < 0) {
                        printf ("map: wrong number of tiles\n");
                }
        }
}

/*************************************************************************
 * TEffect
 *************************************************************************/

TEffect::TEffect (NetworkMessage* msg)
{
        get (msg);
}

TEffect::TEffect (const TPos& pos, uint8_t effectId)
{
        _pos = new TPos (pos);
        _effectId = new TWord8 (effectId);
}

TEffect::TEffect (const TEffect& clone)
{
        _pos = new TPos (*clone._pos);
        _effectId = new TWord8 (*clone._effectId);
}
        
TEffect::~TEffect ()
{
        delete _pos;
        delete _effectId;
}

void TEffect::put (NetworkMessage* msg) const
{
        _pos->put (msg);
        _effectId->put (msg);
}

void TEffect::show () const
{
        printf ("TPos {"); _pos->show ();
        printf (", Effect:"); _effectId->show();
        printf ("}");
}

const TPos& TEffect::getPos () const
{
        return *_pos;
}

uint8_t TEffect::getEffectId () const
{
        return _effectId->getVal ();
}

void TEffect::get (NetworkMessage* msg)
{
        _pos = new TPos (msg);
        _effectId = new TWord8 (msg);
}

/*************************************************************************
 * TTextMsg
 *************************************************************************/

TTextMsg::TTextMsg (NetworkMessage* msg)
{
        get (msg);
}

TTextMsg::TTextMsg (uint8_t msgType, const std::string msg)
{
        _msgType = new TWord8 (msgType);
        _msg = new TString (msg);
}

TTextMsg::TTextMsg (const TTextMsg& clone)
{
        _msgType = new TWord8 (*clone._msgType);
        _msg = new TString (*clone._msg);
}

TTextMsg::~TTextMsg ()
{
        delete _msgType;
        delete _msg;
}

void TTextMsg::put (NetworkMessage* msg) const
{
        _msgType->put (msg);
        _msg->put (msg);
}

void TTextMsg::show () const
{
        printf ("TTextMsg {type: "); _msgType->show ();
        printf ("msg: "); _msg->show ();
        printf ("}\n");
}

uint8_t TTextMsg::getMsgType () const
{
        return _msgType->getVal ();
}

const std::string TTextMsg::getMsg () const
{
        return _msg->getString ();
}

void TTextMsg::get (NetworkMessage* msg)
{
        _msgType = new TWord8 (msg);
        _msg = new TString (msg);
}

/*************************************************************************
 * TPlayerStats
 *************************************************************************/

TPlayerStats::TPlayerStats (NetworkMessage* msg)
{
        get (msg);
}

TPlayerStats::TPlayerStats (uint16_t hp,
                        uint16_t hpmax,
                        uint32_t capacity,
                        uint32_t experience,
                        uint16_t level,
                        uint8_t levelPercent,
                        uint16_t mana,
                        uint16_t maxmana,
                        uint8_t magicLevel,
                        uint8_t magicLevelPercent,
                        uint8_t soul,
                        uint16_t stamina)
{
        _hp =                  new TWord16 (hp);
        _hpmax =               new TWord16 (hpmax);
        _capacity =            new TWord32 (capacity);
        _experience =          new TWord32 (experience);
        _level =               new TWord16 (level);
        _levelPercent =        new TWord8 (levelPercent);
        _mana =                new TWord16 (mana);
        _maxmana =             new TWord16 (maxmana);
        _magicLevel =          new TWord8 (magicLevel);
        _magicLevelPercent =   new TWord8 (magicLevelPercent);
        _soul =                new TWord8 (soul);
        _stamina =             new TWord16 (stamina);
}

TPlayerStats::TPlayerStats (const TPlayerStats& clone)
{
        _hp =                  new TWord16 (*clone._hp);
        _hpmax =               new TWord16 (*clone._hpmax);
        _capacity =            new TWord32 (*clone._capacity);
        _experience =          new TWord32 (*clone._experience);
        _level =               new TWord16 (*clone._level);
        _levelPercent =        new TWord8 (*clone._levelPercent);
        _mana =                new TWord16 (*clone._mana);
        _maxmana =             new TWord16 (*clone._maxmana);
        _magicLevel =          new TWord8 (*clone._magicLevel);
        _magicLevelPercent =   new TWord8 (*clone._magicLevelPercent);
        _soul =                new TWord8 (*clone._soul);
        _stamina =             new TWord16 (*clone._stamina);
}

TPlayerStats::~TPlayerStats ()
{
        delete _hp;
        delete _hpmax;
        delete _capacity;
        delete _experience;
        delete _level;
        delete _levelPercent;
        delete _mana;
        delete _maxmana;
        delete _magicLevel;
        delete _magicLevelPercent;
        delete _soul;
        delete _stamina;
}

void TPlayerStats::put (NetworkMessage* msg)
{
        _hp->put (msg);
        _hpmax->put (msg);
        _capacity->put (msg);
        _experience->put (msg);
        _level->put (msg);
        _levelPercent->put (msg);
        _mana->put (msg);
        _maxmana->put (msg);
        _magicLevel->put (msg);
        _magicLevelPercent->put (msg);
        _soul->put (msg);
        _stamina->put (msg);
}

void TPlayerStats::show ()
{
        printf ("TPlayerStats {\n");
        printf ("\t\thp: "); _hp->show (); printf ("\n");
        printf ("\t\thpmax: "); _hpmax->show (); printf ("\n");
        printf ("\t\tcapacity: "); _capacity->show (); printf ("\n");
        printf ("\t\texperience: "); _experience->show (); printf ("\n");
        printf ("\t\tlevel: "); _level->show (); printf ("\n");
        printf ("\t\tlevelPercent: "); _levelPercent->show (); printf ("\n");
        printf ("\t\tmana: "); _mana->show (); printf ("\n");
        printf ("\t\tmaxmana: "); _maxmana->show (); printf ("\n");
        printf ("\t\tmagicLevel: "); _magicLevel->show (); printf ("\n");
        printf ("\t\tmagicLevelPercent: "); _magicLevelPercent->show (); printf ("\n");
        printf ("\t\tsoul: "); _soul->show (); printf ("\n");
        printf ("\t\tstamina: "); _stamina->show (); printf ("\n");
        printf ("}\n");
}

uint16_t TPlayerStats::getHp () const
{
        return _hp->getVal ();
}

uint16_t TPlayerStats::getHpmax () const
{
        return _hpmax->getVal ();
}

uint32_t TPlayerStats::getCapacity () const
{
        return _capacity->getVal ();
}

uint32_t TPlayerStats::getExperience () const
{
        return _experience->getVal ();
}

uint16_t TPlayerStats::getLevel () const
{
        return _level->getVal ();
}

uint8_t TPlayerStats::getLevelPercent () const
{
        return _levelPercent->getVal ();
}

uint16_t TPlayerStats::getMana () const
{
        return _mana->getVal ();
}

uint16_t TPlayerStats::getMaxmana () const
{
        return _maxmana->getVal ();
}

uint8_t TPlayerStats::getMagicLevel () const
{
        return _magicLevel->getVal ();
}

uint8_t TPlayerStats::getMagicLevelPercent () const
{
        return _magicLevelPercent->getVal ();
}

uint8_t TPlayerStats::getSoul () const
{
        return _soul->getVal ();
}

uint16_t TPlayerStats::getStamina () const
{
        return _stamina->getVal ();
}

void TPlayerStats::get (NetworkMessage* msg)
{
        _hp =                  new TWord16 (msg);
        _hpmax =               new TWord16 (msg);
        _capacity =            new TWord32 (msg);
        _experience =          new TWord32 (msg);
        _level =               new TWord16 (msg);
        _levelPercent =        new TWord8 (msg);
        _mana =                new TWord16 (msg);
        _maxmana =             new TWord16 (msg);
        _magicLevel =          new TWord8 (msg);
        _magicLevelPercent =   new TWord8 (msg);
        _soul =                new TWord8 (msg);
        _stamina =             new TWord16 (msg);
} 

/*************************************************************************
 * TPlayerSkill
 *************************************************************************/

TPlayerSkill::TPlayerSkill (NetworkMessage* msg)
{
        get (msg);
}

TPlayerSkill::TPlayerSkill (uint8_t level, uint8_t percent)
{
        _level = new TWord8 (level);
        _percent = new TWord8 (percent);
}

TPlayerSkill::TPlayerSkill (const TPlayerSkill& clone)
{
        _level = new TWord8 (*clone._level);
        _percent = new TWord8 (*clone._percent);
}

TPlayerSkill::~TPlayerSkill ()
{
        delete _level;
        delete _percent;
}

void TPlayerSkill::put (NetworkMessage* msg) const
{
        _level->put (msg);
        _percent->put (msg);
}

void TPlayerSkill::show () const
{
        printf ("TPlayerSkill {lvl: "); _level->show ();
        printf ("percent: "); _percent->show (); printf ("}");
}
        
uint8_t TPlayerSkill::getLevel () const
{
        return _level->getVal ();
}

uint8_t TPlayerSkill::getPercent () const
{
        return _percent->getVal ();
}

void TPlayerSkill::get (NetworkMessage* msg)
{
        _level = new TWord8 (msg);
        _percent = new TWord8 (msg);
}

/*************************************************************************
 * TPlayerSkills
 *************************************************************************/

TPlayerSkills::TPlayerSkills (NetworkMessage* msg)
{
        get (msg);
}

TPlayerSkills::TPlayerSkills ( const TPlayerSkill& fist,
                        const TPlayerSkill& club,
                        const TPlayerSkill& sword,
                        const TPlayerSkill& axe,
                        const TPlayerSkill& distance,
                        const TPlayerSkill& shield,
                        const TPlayerSkill& fishing)
{
        _fist =         new TPlayerSkill (fist);
        _club =         new TPlayerSkill (club);
        _sword =        new TPlayerSkill (sword);
        _axe =          new TPlayerSkill (axe);
        _distance =     new TPlayerSkill (distance);
        _shield =       new TPlayerSkill (shield);
        _fishing =      new TPlayerSkill (fishing);
}

TPlayerSkills::TPlayerSkills (const TPlayerSkills& clone)
{
        _fist =         new TPlayerSkill (*clone._fist);
        _club =         new TPlayerSkill (*clone._club);
        _sword =        new TPlayerSkill (*clone._sword);
        _axe =          new TPlayerSkill (*clone._axe);
        _distance =     new TPlayerSkill (*clone._distance);
        _shield =       new TPlayerSkill (*clone._shield);
        _fishing =      new TPlayerSkill (*clone._fishing);
}

TPlayerSkills::~TPlayerSkills ()
{
        delete _fist;
        delete _club;
        delete _sword;
        delete _axe;
        delete _distance;
        delete _shield;
        delete _fishing;
}

void TPlayerSkills::put (NetworkMessage* msg) const
{
        _fist->put (msg);
        _club->put (msg);
        _sword->put (msg);
        _axe->put (msg);
        _distance->put (msg);
        _shield->put (msg);
        _fishing->put (msg);
}

void TPlayerSkills::show () const
{
        printf ("TPlayerSkills {\n");
        printf ("\t\tfist: "); _fist->show (); printf ("\n");
        printf ("\t\tclub: "); _club->show (); printf ("\n");
        printf ("\t\tsword: "); _sword->show (); printf ("\n");
        printf ("\t\taxe: "); _axe->show (); printf ("\n");
        printf ("\t\tdistance: "); _distance->show (); printf ("\n");
        printf ("\t\tshield: "); _shield->show (); printf ("\n");
        printf ("\t\tfishing: "); _fishing->show (); printf ("\n");
}

const TPlayerSkill& TPlayerSkills::getFist () const
{
        return *_fist;
}

const TPlayerSkill& TPlayerSkills::getClub () const
{
        return *_club;
}

const TPlayerSkill& TPlayerSkills::getSword () const
{
        return *_sword;
}

const TPlayerSkill& TPlayerSkills::getAxe () const
{
        return *_axe;
}

const TPlayerSkill& TPlayerSkills::getDistance () const
{
        return *_distance;
}

const TPlayerSkill& TPlayerSkills::getShield () const
{
        return *_shield;
}

const TPlayerSkill& TPlayerSkills::getFishing () const
{
        return *_fishing;
}

void TPlayerSkills::get (NetworkMessage* msg)
{
        _fist = new TPlayerSkill (msg);
        _club = new TPlayerSkill (msg);
        _sword = new TPlayerSkill (msg);
        _axe = new TPlayerSkill (msg);
        _distance = new TPlayerSkill (msg);
        _shield = new TPlayerSkill (msg);
        _fishing = new TPlayerSkill (msg);
}

/************************************************************************
 * TContainer
 ************************************************************************/

TContainer::TContainer (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
        _it = _items.begin ();
}


TContainer::TContainer (const TThing& item, const std::string& name,
                        uint8_t capacity, uint8_t hasParent)
{
        
        TThingFactory tf;
        _item = tf.cloneThing (item);
        _name = new TString (name);
        _capacity = new TWord8 (capacity);
        _hasParent = new TWord8 (hasParent);
        _nItems = new TWord8 ((uint8_t)0);
        _it = _items.begin ();
}
        
TContainer::TContainer (const TContainer& clone)
{
        TThingFactory tf;
        _item = tf.cloneThing (*clone._item);

        _name = new TString (*clone._name);
        _capacity = new TWord8 (*clone._capacity);
        _hasParent = new TWord8 (*clone._hasParent);
        _nItems = new TWord8 (*clone._nItems);

        ContainerList::const_iterator i;
        for (i = clone._items.begin (); i != clone._items.end (); ++ i) {
                _items.push_back (tf.cloneThing (**i));
        }

        _it = _items.begin ();
}
        
TContainer::~TContainer ()
{
        delete _item;
        delete _name;
        delete _capacity;
        delete _hasParent;
        delete _nItems;

        for (_it = _items.begin (); _it != _items.end (); ++ _it) {
                delete (*_it);
        }
}
        

void TContainer::put (NetworkMessage* msg) const
{
        _item->put (msg);
        _name->put (msg);
        _capacity->put (msg);
        _hasParent->put (msg);
        _nItems->put (msg);

        ContainerList::const_iterator i;
        for (i = _items.begin (); i != _items.end (); ++ i) {
                (*i)->put (msg);
        }
}
        
void TContainer::show () const
{
        printf ("\tTContainer {\n");
        printf ("\t\titem: "); _item->show (); printf ("\n");
        printf ("\t\tname: "); _name->show (); printf ("\n");
        printf ("\t\tcapacity: "); _capacity->show (); printf ("\n");
        printf ("\t\thasParent: "); _hasParent->show (); printf ("\n");
        printf ("\t\tnItems: "); _nItems->show (); printf ("\n");

        printf ("\t\tItems:\n");

        ContainerList::const_iterator i;
        for (i = _items.begin (); i != _items.end (); ++ i) {
                printf ("\t\t"); (*i)->show (); printf ("\n");
        }
        printf ("\t}\n");
}

const TThing& TContainer::getItem () const
{
        return *_item;
}

const std::string& TContainer::getName () const
{
        return _name->getString () ;
}

uint8_t TContainer::getCapacity () const
{
        return _capacity->getVal ();
}

uint8_t TContainer::getHasParent () const
{
        return _hasParent->getVal ();
}

uint8_t TContainer::getNItems () const
{
        return _nItems->getVal ();
}

void TContainer::begin ()
{
        _it = _items.begin ();
}

bool TContainer::isEnd ()
{
        if (_it == _items.end ()) {
                return true;
        } else {
                return false;
        }
}

void TContainer::next ()
{
        if (_it == _items.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TThing& TContainer::getThing ()
{
        return *(*_it);
}

void TContainer::insert (TThing* tm)
{
        _items.insert (_it, tm);
        uint8_t tmp = _nItems->getVal ();
        delete _nItems;
        _nItems = new TWord8 (tmp + 1);
}

void TContainer::replace (TThing* tm)
{
        if (_items.size () == 0) {
                printf ("map error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _items.end ()) {
                printf ("map error: attemp to replace \"end\"\n");
                return;
        }
        _it = _items.erase (_it);
        _items.insert (_it, tm);
        _it --;
} 

void TContainer::remove ()
{
        _it = _items.erase (_it);
        uint8_t tmp = _nItems->getVal ();
        delete _nItems;
        _nItems = new TWord8 (tmp - 1);
}

void TContainer::add (TThing* thing)
{
        _items.push_back (thing);
        uint8_t tmp = _nItems->getVal ();
        delete _nItems;
        _nItems = new TWord8 (tmp + 1);
}

void TContainer::get (NetworkMessage* msg, DatReader* dat)
{
        TThingFactory tf (msg, dat);
        _item = tf.getThing ();

        _name = new TString (msg);
        _capacity = new TWord8 (msg);
        _hasParent = new TWord8 (msg);
        _nItems = new TWord8 (msg);

        uint32_t n = _nItems->getVal ();

        for (uint32_t i = 0; i < n; i ++) {
                _items.push_back (tf.getThing ());
        }

        _it = _items.begin ();
}

/************************************************************************
 * TTradeContainer
 ************************************************************************/

TTradeContainer::TTradeContainer (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
        _it = _items.begin ();
}


TTradeContainer::TTradeContainer (const std::string& name, uint8_t capacity)
{
        //TODO change constant when item lookup complete
        _item = new TItem ((uint16_t)0x0B25);
        _name = new TString (name);
        _capacity = new TWord8 (capacity);
        _hasParent = new TWord8 ((uint8_t)0);
        _nItems = new TWord8 ((uint8_t)0);
        _it = _items.begin ();
}
        
TTradeContainer::TTradeContainer (const TTradeContainer& clone)
{
        TThingFactory tf;
        _item = tf.cloneThing (*clone._item);

        _name = new TString (*clone._name);
        _capacity = new TWord8 (*clone._capacity);
        _hasParent = new TWord8 (*clone._hasParent);
        _nItems = new TWord8 (*clone._nItems);

        ContainerList::const_iterator i;
        for (i = clone._items.begin (); i != clone._items.end (); ++ i) {
                _items.push_back (tf.cloneThing (**i));
        }

        _it = _items.begin ();
}
        
TTradeContainer::~TTradeContainer ()
{
        delete _item;
        delete _name;
        delete _capacity;
        delete _hasParent;
        delete _nItems;

        for (_it = _items.begin (); _it != _items.end (); ++ _it) {
                delete (*_it);
        }
}
        

void TTradeContainer::put (NetworkMessage* msg) const
{
        _name->put (msg);
        _nItems->put (msg);

        ContainerList::const_iterator i;
        for (i = _items.begin (); i != _items.end (); ++ i) {
                (*i)->put (msg);
        }
}
        
void TTradeContainer::show () const
{
        printf ("\tTTradeContainer {\n");
        printf ("\t\titem: "); _item->show (); printf ("\n");
        printf ("\t\tname: "); _name->show (); printf ("\n");
        printf ("\t\tcapacity: "); _capacity->show (); printf ("\n");
        printf ("\t\thasParent: "); _hasParent->show (); printf ("\n");
        printf ("\t\tnItems: "); _nItems->show (); printf ("\n");

        printf ("\t\tItems:\n");

        ContainerList::const_iterator i;
        for (i = _items.begin (); i != _items.end (); ++ i) {
                printf ("\t\t"); (*i)->show (); printf ("\n");
        }
        printf ("\t}\n");
}

const TThing& TTradeContainer::getItem () const
{
        return *_item;
}

const std::string& TTradeContainer::getName () const
{
        return _name->getString () ;
}

uint8_t TTradeContainer::getCapacity () const
{
        return _capacity->getVal ();
}

uint8_t TTradeContainer::getHasParent () const
{
        return _hasParent->getVal ();
}

uint8_t TTradeContainer::getNItems () const
{
        return _nItems->getVal ();
}

void TTradeContainer::begin ()
{
        _it = _items.begin ();
}

bool TTradeContainer::isEnd ()
{
        if (_it == _items.end ()) {
                return true;
        } else {
                return false;
        }
}

void TTradeContainer::next ()
{
        if (_it == _items.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TThing& TTradeContainer::getThing ()
{
        return *(*_it);
}

void TTradeContainer::insert (TThing* tm)
{
        _items.insert (_it, tm);
        uint8_t tmp = _nItems->getVal ();
        delete _nItems;
        _nItems = new TWord8 (tmp + 1);
}

void TTradeContainer::replace (TThing* tm)
{
        if (_items.size () == 0) {
                printf ("map error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _items.end ()) {
                printf ("map error: attemp to replace \"end\"\n");
                return;
        }
        _it = _items.erase (_it);
        _items.insert (_it, tm);
        _it --;
} 

void TTradeContainer::remove ()
{
        _it = _items.erase (_it);
        uint8_t tmp = _nItems->getVal ();
        delete _nItems;
        _nItems = new TWord8 (tmp - 1);
}

void TTradeContainer::add (TThing* thing)
{
        _items.push_back (thing);
        uint8_t tmp = _nItems->getVal ();
        delete _nItems;
        _nItems = new TWord8 (tmp + 1);
}

void TTradeContainer::get (NetworkMessage* msg, DatReader* dat)
{
        //TODO replace with item lookup
        _item = new TItem ((uint16_t)0x0B25);
        _name = new TString (msg);
        _capacity = new TWord8 (msg);
        _hasParent = new TWord8 ((uint8_t)0);
        _nItems = new TWord8 (*_capacity);

        TThingFactory tf (msg, dat);
        uint32_t n = _nItems->getVal ();

        for (uint32_t i = 0; i < n; i ++) {
                _items.push_back (tf.getThing ());
        }

        _it = _items.begin ();
}


/************************************************************************
 * TShopItem
 ************************************************************************/

TShopItem::TShopItem (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
}


TShopItem::TShopItem (const TThing& item, uint8_t xbyte, 
        const std::string& name, uint32_t weight,
        uint32_t buyprice, uint32_t sellprice)
{
        TThingFactory tf;
        _item = tf.cloneThing (item);

        if (_item->getType () == TThing::xitem) {
                _xbyte = new TWord8 (((TXItem*)_item)->getXByte ());
        } else {
                _xbyte = new TWord8 (xbyte);
        }

        _name = new TString (name);
        _weight = new TWord32 (weight);
        _buyprice = new TWord32 (buyprice);
        _sellprice = new TWord32 (sellprice);
}
        
TShopItem::TShopItem (const TShopItem& clone)
{
        TThingFactory tf;
        _item = tf.cloneThing (*clone._item);

        _xbyte = new TWord8 (*clone._xbyte);
        _name = new TString (*clone._name);
        _weight = new TWord32 (*clone._weight);
        _buyprice = new TWord32 (*clone._buyprice);
        _sellprice = new TWord32 (*clone._sellprice);
}
        
TShopItem::~TShopItem ()
{
        delete _item;
        delete _xbyte;
        delete _name;
        delete _weight;
        delete _buyprice;
        delete _sellprice;
}
        

void TShopItem::put (NetworkMessage* msg) const
{
        _item->put (msg);
        if (_item->getType () != TThing::xitem) {
                _xbyte->put (msg);
        }
        _name->put (msg);
        _weight->put (msg);
        _buyprice->put (msg);
        _sellprice->put (msg);
}
        
void TShopItem::show () const
{
        printf ("\tTShopItem {\n");
        printf ("\t\titem: "); _item->show (); printf ("\n");
        printf ("\t\txbyte: "); _xbyte->show (); printf ("\n");
        printf ("\t\tname: "); _name->show (); printf ("\n");
        printf ("\t\tweight: "); _weight->show (); printf ("\n");
        printf ("\t\tbuyprice: "); _buyprice->show (); printf ("\n");
        printf ("\t\tsellprice: "); _sellprice->show (); printf ("\n");
        printf ("\t}\n");
}

const TThing& TShopItem::getItem () const
{
        return *_item;
}

uint8_t TShopItem::getXByte () const
{
        return _xbyte->getVal ();
}

const std::string& TShopItem::getName () const
{
        return _name->getString () ;
}

uint32_t TShopItem::getWeight () const
{
        return _weight->getVal ();
}

uint32_t TShopItem::getBuyPrice () const
{
        return _buyprice->getVal ();
}

uint32_t TShopItem::getSellPrice () const
{
        return _sellprice->getVal ();
}

void TShopItem::get (NetworkMessage* msg, DatReader* dat)
{
        TThingFactory tf (msg, dat);
        _item = tf.getThing ();

        if (_item->getType () == TThing::xitem) {
                _xbyte = new TWord8 (((TXItem*)_item)->getXByte ());
        } else {
                _xbyte = new TWord8 (msg);
        }

        _name = new TString (msg);
        _weight = new TWord32 (msg);
        _buyprice = new TWord32 (msg);
        _sellprice = new TWord32 (msg);
}

/************************************************************************
 * TShopList
 ************************************************************************/

TShopList::TShopList (NetworkMessage* msg, DatReader* dat)
{
        get (msg, dat);
        _it = _items.begin ();
}


TShopList::TShopList (uint8_t nitems)
{
        _nitems = new TWord8 ((uint8_t)0);
        _it = _items.begin ();
}
        
TShopList::TShopList (const TShopList& clone)
{
        _nitems = new TWord8 (*clone._nitems);

        ShopList::const_iterator i;
        for (i = clone._items.begin (); i != clone._items.end (); ++ i) {
                _items.push_back (new TShopItem (**i));
        }

        _it = _items.begin ();
}
        
TShopList::~TShopList ()
{
        delete _nitems;

        for (_it = _items.begin (); _it != _items.end (); ++ _it) {
                delete (*_it);
        }
}
        

void TShopList::put (NetworkMessage* msg) const
{
        _nitems->put (msg);

        ShopList::const_iterator i;
        for (i = _items.begin (); i != _items.end (); ++ i) {
                (*i)->put (msg);
        }
}
        
void TShopList::show () const
{
        printf ("\tTShopList {\n");
        printf ("\t\tnitems: "); _nitems->show (); printf ("\n");

        printf ("\t\tShopItems:\n");

        ShopList::const_iterator i;
        for (i = _items.begin (); i != _items.end (); ++ i) {
                printf ("\t\t"); (*i)->show (); printf ("\n");
        }
        printf ("\t}\n");
}

uint8_t TShopList::getNItems () const
{
        return _nitems->getVal ();
}

void TShopList::begin ()
{
        _it = _items.begin ();
}

bool TShopList::isEnd ()
{
        if (_it == _items.end ()) {
                return true;
        } else {
                return false;
        }
}

void TShopList::next ()
{
        if (_it == _items.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TShopItem& TShopList::getShopItem ()
{
        return *(*_it);
}

void TShopList::insert (TShopItem* shopitem)
{
        _items.insert (_it, shopitem);
        uint8_t tmp = _nitems->getVal ();
        delete _nitems;
        _nitems = new TWord8 (tmp + 1);
}

void TShopList::replace (TShopItem* shopitem)
{
        if (_items.size () == 0) {
                printf ("shoplist error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _items.end ()) {
                printf ("shoplist error: attemp to replace \"end\"\n");
                return;
        }
        _it = _items.erase (_it);
        _items.insert (_it, shopitem);
        _it --;
} 

void TShopList::remove ()
{
        _it = _items.erase (_it);
        uint8_t tmp = _nitems->getVal ();
        delete _nitems;
        _nitems = new TWord8 (tmp - 1);
}

void TShopList::add (TShopItem* shopitem)
{
        _items.push_back (shopitem);
        uint8_t tmp = _nitems->getVal ();
        delete _nitems;
        _nitems = new TWord8 (tmp + 1);
}

void TShopList::get (NetworkMessage* msg, DatReader* dat)
{
        _nitems = new TWord8 (msg);

        uint32_t n = _nitems->getVal ();

        for (uint32_t i = 0; i < n; i ++) {
                _items.push_back (new TShopItem (msg, dat));
        }

        _it = _items.begin ();
}

/************************************************************************
 * TOutfitSelection
 ************************************************************************/

TOutfitSelection::TOutfitSelection (NetworkMessage* msg)
{
        get (msg);
}


TOutfitSelection::TOutfitSelection (uint16_t looktype, const std::string& name,
                                        uint8_t addons)
{
        _looktype = new TWord16 (looktype);
        _name = new TString (name);
        _addons = new TWord8 (addons);
}
        
TOutfitSelection::TOutfitSelection (const TOutfitSelection& clone)
{
        _looktype = new TWord16 (*clone._looktype);
        _name = new TString (*clone._name);
        _addons = new TWord8 (*clone._addons);
}
        
TOutfitSelection::~TOutfitSelection ()
{
        delete _looktype;
        delete _name;
        delete _addons;
}

void TOutfitSelection::put (NetworkMessage* msg) const
{
        _looktype->put (msg);
        _name->put (msg);
        _addons->put (msg);
}
        
void TOutfitSelection::show () const
{
        printf ("\tTOutfitSelection {\n");
        printf ("\t\tlooktype: "); _looktype->show (); printf ("\n");
        printf ("\t\tname: "); _name->show (); printf ("\n");
        printf ("\t\taddons: "); _addons->show (); printf ("\n");
        printf ("\t}\n");
}

uint16_t TOutfitSelection::getLookType () const
{
        return _looktype->getVal ();
}

const std::string& TOutfitSelection::getName () const
{
        return _name->getString () ;
}

uint8_t TOutfitSelection::getAddons () const
{
        return _addons->getVal ();
}

void TOutfitSelection::get (NetworkMessage* msg)
{
        _looktype = new TWord16 (msg);
        _name = new TString (msg);
        _addons = new TWord8 (msg);
}

/************************************************************************
 * TOutfitList
 ************************************************************************/

TOutfitList::TOutfitList (NetworkMessage* msg)
{
        get (msg);
        _it = _outfits.begin ();
}


TOutfitList::TOutfitList (uint8_t nitems)
{
        _nitems = new TWord8 ((uint8_t)0);
        _it = _outfits.begin ();
}
        
TOutfitList::TOutfitList (const TOutfitList& clone)
{
        _nitems = new TWord8 (*clone._nitems);

        OutfitList::const_iterator i;
        for (i = clone._outfits.begin (); i != clone._outfits.end (); ++ i) {
                _outfits.push_back (new TOutfitSelection (**i));
        }

        _it = _outfits.begin ();
}
        
TOutfitList::~TOutfitList ()
{
        delete _nitems;

        for (_it = _outfits.begin (); _it != _outfits.end (); ++ _it) {
                delete (*_it);
        }
}
        

void TOutfitList::put (NetworkMessage* msg) const
{
        _nitems->put (msg);

        OutfitList::const_iterator i;
        for (i = _outfits.begin (); i != _outfits.end (); ++ i) {
                (*i)->put (msg);
        }
}
        
void TOutfitList::show () const
{
        printf ("\tTOutfitList {\n");
        printf ("\t\tnitems: "); _nitems->show (); printf ("\n");

        printf ("\t\tOutfits:\n");

        OutfitList::const_iterator i;
        for (i = _outfits.begin (); i != _outfits.end (); ++ i) {
                printf ("\t\t"); (*i)->show (); printf ("\n");
        }
        printf ("\t}\n");
}

uint8_t TOutfitList::getNItems () const
{
        return _nitems->getVal ();
}

void TOutfitList::begin ()
{
        _it = _outfits.begin ();
}

bool TOutfitList::isEnd ()
{
        if (_it == _outfits.end ()) {
                return true;
        } else {
                return false;
        }
}

void TOutfitList::next ()
{
        if (_it == _outfits.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TOutfitSelection& TOutfitList::getOutfitSelection ()
{
        return *(*_it);
}

void TOutfitList::insert (TOutfitSelection* outfit)
{
        _outfits.insert (_it, outfit);
        uint8_t tmp = _nitems->getVal ();
        delete _nitems;
        _nitems = new TWord8 (tmp + 1);
}

void TOutfitList::replace (TOutfitSelection* outfit)
{
        if (_outfits.size () == 0) {
                printf ("outfitlist error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _outfits.end ()) {
                printf ("outfitlist error: attemp to replace \"end\"\n");
                return;
        }
        _it = _outfits.erase (_it);
        _outfits.insert (_it, outfit);
        _it --;
} 

void TOutfitList::remove ()
{
        _it = _outfits.erase (_it);
        uint8_t tmp = _nitems->getVal ();
        delete _nitems;
        _nitems = new TWord8 (tmp - 1);
}

void TOutfitList::add (TOutfitSelection* outfit)
{
        _outfits.push_back (outfit);
        uint8_t tmp = _nitems->getVal ();
        delete _nitems;
        _nitems = new TWord8 (tmp + 1);
}

void TOutfitList::get (NetworkMessage* msg)
{
        _nitems = new TWord8 (msg);

        uint32_t n = _nitems->getVal ();

        for (uint32_t i = 0; i < n; i ++) {
                _outfits.push_back (new TOutfitSelection (msg));
        }

        _it = _outfits.begin ();
}


/************************************************************************
 * TQuest
 ************************************************************************/

TQuest::TQuest (NetworkMessage* msg)
{
        get (msg);
}


TQuest::TQuest (uint16_t questid, const std::string& name,
                                        uint8_t state)
{
        _questid = new TWord16 (questid);
        _name = new TString (name);
        _state = new TWord8 (state);
}
        
TQuest::TQuest (const TQuest& clone)
{
        _questid = new TWord16 (*clone._questid);
        _name = new TString (*clone._name);
        _state = new TWord8 (*clone._state);
}
        
TQuest::~TQuest ()
{
        delete _questid;
        delete _name;
        delete _state;
}

void TQuest::put (NetworkMessage* msg) const
{
        _questid->put (msg);
        _name->put (msg);
        _state->put (msg);
}
        
void TQuest::show () const
{
        printf ("\tTQuest {\n");
        printf ("\t\tquestid: "); _questid->show (); printf ("\n");
        printf ("\t\tname: "); _name->show (); printf ("\n");
        printf ("\t\tstate: "); _state->show (); printf ("\n");
        printf ("\t}\n");
}

uint16_t TQuest::getQuestId () const
{
        return _questid->getVal ();
}

const std::string& TQuest::getName () const
{
        return _name->getString () ;
}

uint8_t TQuest::getState () const
{
        return _state->getVal ();
}

void TQuest::get (NetworkMessage* msg)
{
        _questid = new TWord16 (msg);
        _name = new TString (msg);
        _state = new TWord8 (msg);
}

/************************************************************************
 * TQuestList
 ************************************************************************/

TQuestList::TQuestList (NetworkMessage* msg)
{
        get (msg);
        _it = _quests.begin ();
}


TQuestList::TQuestList (uint16_t nquests)
{
        _nquests = new TWord16 ((uint16_t)0);
        _it = _quests.begin ();
}
        
TQuestList::TQuestList (const TQuestList& clone)
{
        _nquests = new TWord16 (*clone._nquests);

        QuestList::const_iterator i;
        for (i = clone._quests.begin (); i != clone._quests.end (); ++ i) {
                _quests.push_back (new TQuest (**i));
        }

        _it = _quests.begin ();
}
        
TQuestList::~TQuestList ()
{
        delete _nquests;

        for (_it = _quests.begin (); _it != _quests.end (); ++ _it) {
                delete (*_it);
        }
}
        

void TQuestList::put (NetworkMessage* msg) const
{
        _nquests->put (msg);

        QuestList::const_iterator i;
        for (i = _quests.begin (); i != _quests.end (); ++ i) {
                (*i)->put (msg);
        }
}
        
void TQuestList::show () const
{
        printf ("\tTQuestList {\n");
        printf ("\t\tnquests: "); _nquests->show (); printf ("\n");

        printf ("\t\tOutfits:\n");

        QuestList::const_iterator i;
        for (i = _quests.begin (); i != _quests.end (); ++ i) {
                printf ("\t\t"); (*i)->show (); printf ("\n");
        }
        printf ("\t}\n");
}

uint16_t TQuestList::getNQuests () const
{
        return _nquests->getVal ();
}

void TQuestList::begin ()
{
        _it = _quests.begin ();
}

bool TQuestList::isEnd ()
{
        if (_it == _quests.end ()) {
                return true;
        } else {
                return false;
        }
}

void TQuestList::next ()
{
        if (_it == _quests.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TQuest& TQuestList::getOutfitSelection ()
{
        return *(*_it);
}

void TQuestList::insert (TQuest* quest)
{
        _quests.insert (_it, quest);
        uint16_t tmp = _nquests->getVal ();
        delete _nquests;
        _nquests = new TWord16 (tmp + 1);
}

void TQuestList::replace (TQuest* quest)
{
        if (_quests.size () == 0) {
                printf ("questlist error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _quests.end ()) {
                printf ("questlist error: attemp to replace \"end\"\n");
                return;
        }
        _it = _quests.erase (_it);
        _quests.insert (_it, quest);
        _it --;
} 

void TQuestList::remove ()
{
        _it = _quests.erase (_it);
        uint16_t tmp = _nquests->getVal ();
        delete _nquests;
        _nquests = new TWord16 (tmp - 1);
}

void TQuestList::add (TQuest* quest)
{
        _quests.push_back (quest);
        uint16_t tmp = _nquests->getVal ();
        delete _nquests;
        _nquests = new TWord16 (tmp + 1);
}

void TQuestList::get (NetworkMessage* msg)
{
        _nquests = new TWord16 (msg);

        uint32_t n = _nquests->getVal ();

        for (uint32_t i = 0; i < n; i ++) {
                _quests.push_back (new TQuest (msg));
        }

        _it = _quests.begin ();
}

/************************************************************************
 * TSubQuest
 ************************************************************************/

TSubQuest::TSubQuest (NetworkMessage* msg)
{
        get (msg);
}


TSubQuest::TSubQuest (const std::string& description, const std::string& name)
{
        _name = new TString (name);
        _description = new TString (description);
}
        
TSubQuest::TSubQuest (const TSubQuest& clone)
{
        _name = new TString (*clone._name);
        _description = new TString (*clone._description);
}
        
TSubQuest::~TSubQuest ()
{
        delete _name;
        delete _description;
}

void TSubQuest::put (NetworkMessage* msg) const
{
        _name->put (msg);
        _description->put (msg);
}
        
void TSubQuest::show () const
{
        printf ("\tTSubQuest {\n");
        printf ("\t\tname: "); _name->show (); printf ("\n");
        printf ("\t\tdescription: "); _description->show (); printf ("\n");
        printf ("\t}\n");
}

const std::string& TSubQuest::getName () const
{
        return _name->getString () ;
}

const std::string& TSubQuest::getDescription () const
{
        return _description->getString ();
}

void TSubQuest::get (NetworkMessage* msg)
{
        _name = new TString (msg);
        _description = new TString (msg);
}

/************************************************************************
 * TSubQuestList
 ************************************************************************/

TSubQuestList::TSubQuestList (NetworkMessage* msg)
{
        get (msg);
        _it = _subquests.begin ();
}


TSubQuestList::TSubQuestList (uint8_t nsubquests)
{
        _nsubquests = new TWord8 ((uint8_t)0);
        _it = _subquests.begin ();
}
        
TSubQuestList::TSubQuestList (const TSubQuestList& clone)
{
        _nsubquests = new TWord8 (*clone._nsubquests);

        SubQuestList::const_iterator i;
        for (i = clone._subquests.begin (); i != clone._subquests.end (); ++ i) {
                _subquests.push_back (new TSubQuest (**i));
        }

        _it = _subquests.begin ();
}
        
TSubQuestList::~TSubQuestList ()
{
        delete _nsubquests;

        for (_it = _subquests.begin (); _it != _subquests.end (); ++ _it) {
                delete (*_it);
        }
}
        

void TSubQuestList::put (NetworkMessage* msg) const
{
        _nsubquests->put (msg);

        SubQuestList::const_iterator i;
        for (i = _subquests.begin (); i != _subquests.end (); ++ i) {
                (*i)->put (msg);
        }
}
        
void TSubQuestList::show () const
{
        printf ("\tTSubQuestList {\n");
        printf ("\t\tnsubquests: "); _nsubquests->show (); printf ("\n");

        printf ("\t\tOutfits:\n");

        SubQuestList::const_iterator i;
        for (i = _subquests.begin (); i != _subquests.end (); ++ i) {
                printf ("\t\t"); (*i)->show (); printf ("\n");
        }
        printf ("\t}\n");
}

uint8_t TSubQuestList::getNSubQuests () const
{
        return _nsubquests->getVal ();
}

void TSubQuestList::begin ()
{
        _it = _subquests.begin ();
}

bool TSubQuestList::isEnd ()
{
        if (_it == _subquests.end ()) {
                return true;
        } else {
                return false;
        }
}

void TSubQuestList::next ()
{
        if (_it == _subquests.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TSubQuest& TSubQuestList::getSubQuest ()
{
        return *(*_it);
}

void TSubQuestList::insert (TSubQuest* subquest)
{
        _subquests.insert (_it, subquest);
        uint8_t tmp = _nsubquests->getVal ();
        delete _nsubquests;
        _nsubquests = new TWord8 (tmp + 1);
}

void TSubQuestList::replace (TSubQuest* subquest)
{
        if (_subquests.size () == 0) {
                printf ("subquestlist error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _subquests.end ()) {
                printf ("subquestlist error: attemp to replace \"end\"\n");
                return;
        }
        _it = _subquests.erase (_it);
        _subquests.insert (_it, subquest);
        _it --;
} 

void TSubQuestList::remove ()
{
        _it = _subquests.erase (_it);
        uint8_t tmp = _nsubquests->getVal ();
        delete _nsubquests;
        _nsubquests = new TWord8 (tmp - 1);
}

void TSubQuestList::add (TSubQuest* subquest)
{
        _subquests.push_back (subquest);
        uint8_t tmp = _nsubquests->getVal ();
        delete _nsubquests;
        _nsubquests = new TWord8 (tmp + 1);
}

void TSubQuestList::get (NetworkMessage* msg)
{
        _nsubquests = new TWord8 (msg);

        uint32_t n = _nsubquests->getVal ();

        for (uint32_t i = 0; i < n; i ++) {
                _subquests.push_back (new TSubQuest (msg));
        }

        _it = _subquests.begin ();
}

/************************************************************************
 * TChannel
 ************************************************************************/

TChannel::TChannel (NetworkMessage* msg)
{
        get (msg);
}


TChannel::TChannel (uint16_t channelid, const std::string& name)
{
        _channelid = new TWord16 (channelid);
        _name = new TString (name);
}
        
TChannel::TChannel (const TChannel& clone)
{
        _channelid = new TWord16 (*clone._channelid);
        _name = new TString (*clone._name);
}
        
TChannel::~TChannel ()
{
        delete _channelid;
        delete _name;
}

void TChannel::put (NetworkMessage* msg) const
{
        _channelid->put (msg);
        _name->put (msg);
}
        
void TChannel::show () const
{
        printf ("\tTChannel {\n");
        printf ("\t\tchannelid: "); _channelid->show (); printf ("\n");
        printf ("\t\tname: "); _name->show (); printf ("\n");
        printf ("\t}\n");
}

uint16_t TChannel::getChannelId () const
{
        return _channelid->getVal ();
}

const std::string& TChannel::getName () const
{
        return _name->getString () ;
}

void TChannel::get (NetworkMessage* msg)
{
        _channelid = new TWord16 (msg);
        _name = new TString (msg);
}

/************************************************************************
 * TChannelList
 ************************************************************************/

TChannelList::TChannelList (NetworkMessage* msg)
{
        get (msg);
        _it = _channels.begin ();
}


TChannelList::TChannelList (uint8_t nchannels)
{
        _nchannels = new TWord8 ((uint8_t)0);
        _it = _channels.begin ();
}
        
TChannelList::TChannelList (const TChannelList& clone)
{
        _nchannels = new TWord8 (*clone._nchannels);

        ChannelList::const_iterator i;
        for (i = clone._channels.begin (); i != clone._channels.end (); ++ i) {
                _channels.push_back (new TChannel (**i));
        }

        _it = _channels.begin ();
}
        
TChannelList::~TChannelList ()
{
        delete _nchannels;

        for (_it = _channels.begin (); _it != _channels.end (); ++ _it) {
                delete (*_it);
        }
}
        

void TChannelList::put (NetworkMessage* msg) const
{
        _nchannels->put (msg);

        ChannelList::const_iterator i;
        for (i = _channels.begin (); i != _channels.end (); ++ i) {
                (*i)->put (msg);
        }
}
        
void TChannelList::show () const
{
        printf ("\tTChannelList {\n");
        printf ("\t\tnchannels: "); _nchannels->show (); printf ("\n");

        printf ("\t\tChannels:\n");

        ChannelList::const_iterator i;
        for (i = _channels.begin (); i != _channels.end (); ++ i) {
                printf ("\t\t"); (*i)->show (); printf ("\n");
        }
        printf ("\t}\n");
}

uint8_t TChannelList::getNChannels () const
{
        return _nchannels->getVal ();
}

void TChannelList::begin ()
{
        _it = _channels.begin ();
}

bool TChannelList::isEnd ()
{
        if (_it == _channels.end ()) {
                return true;
        } else {
                return false;
        }
}

void TChannelList::next ()
{
        if (_it == _channels.end ()) {
                printf ("error: attempt to seek past end of map\n");
        } else {
                _it ++;
        }
}

const TChannel& TChannelList::getChannel ()
{
        return *(*_it);
}

void TChannelList::insert (TChannel* channel)
{
        _channels.insert (_it, channel);
        uint8_t tmp = _nchannels->getVal ();
        delete _nchannels;
        _nchannels = new TWord8 (tmp + 1);
}

void TChannelList::replace (TChannel* channel)
{
        if (_channels.size () == 0) {
                printf ("channellist error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _channels.end ()) {
                printf ("channellist error: attemp to replace \"end\"\n");
                return;
        }
        _it = _channels.erase (_it);
        _channels.insert (_it, channel);
        _it --;
} 

void TChannelList::remove ()
{
        _it = _channels.erase (_it);
        uint8_t tmp = _nchannels->getVal ();
        delete _nchannels;
        _nchannels = new TWord8 (tmp - 1);
}

void TChannelList::add (TChannel* channel)
{
        _channels.push_back (channel);
        uint8_t tmp = _nchannels->getVal ();
        delete _nchannels;
        _nchannels = new TWord8 (tmp + 1);
}

void TChannelList::get (NetworkMessage* msg)
{
        _nchannels = new TWord8 (msg);

        uint32_t n = _nchannels->getVal ();

        for (uint32_t i = 0; i < n; i ++) {
                _channels.push_back (new TChannel (msg));
        }

        _it = _channels.begin ();
}

/***************************************************************************
 * TSpeak
 ***************************************************************************/
 
TSpeak::TSpeak (NetworkMessage* msg)
{
        get (msg);
}

TSpeak::TSpeak (uint32_t                u1, 
                const std::string&      name,
                uint16_t                level, 
                uint8_t                 type)
{
        _u1 =   new TWord32 (u1);
        _name = new TString (name);
        _level =  new TWord16 (level);
        _type = new TWord8 (type);
}

TSpeak::TSpeak (const TSpeak& clone)
{
        _u1 =   new TWord32 (*clone._u1);
        _name = new TString (*clone._name);
        _level =  new TWord16 (*clone._level);
        _type = new TWord8 (*clone._type);
}

TSpeak::~TSpeak ()
{
        delete _u1;
        delete _name;
        delete _level;
        delete _type;
}

uint32_t TSpeak::getU1 () const
{
        return _u1->getVal ();
}

const std::string& TSpeak::getName () const
{
        return _name->getString ();
}

uint16_t TSpeak::getLevel () const
{
        return _level->getVal ();
}

uint8_t TSpeak::getType () const
{
        return _type->getVal ();
}

void TSpeak::put (NetworkMessage* msg) const
{
        _u1->put (msg);
        _name->put (msg);
        _level->put (msg);
        _type->put (msg);
} 

void TSpeak::get (NetworkMessage* msg)
{
        _u1 =   new TWord32 (msg);
        _name = new TString (msg);
        _level =  new TWord16 (msg);
        _type = new TWord8 (msg);
} 

/***************************************************************************
 * TPublicSpeak
 ***************************************************************************/

TPublicSpeak::TPublicSpeak (NetworkMessage* msg) : TSpeak (msg)
{
        get (msg);
}

TPublicSpeak::TPublicSpeak (uint32_t                    u1, 
                            const std::string&          name,
                            uint16_t                    level, 
                            uint8_t                     type,
                            const TPos&                 pos,  
                            const std::string&          msg)
: TSpeak (u1, name, level, type)
{
        
        _pos = new TPos (pos);
        _msg = new TString (msg);
}

TPublicSpeak::TPublicSpeak (const TPublicSpeak& clone) : TSpeak (clone)
{
        _pos = new TPos (*clone._pos);
        _msg = new TString (*clone._msg);
}

TPublicSpeak::~TPublicSpeak ()
{
        delete _pos;
        delete _msg;
}

const TPos& TPublicSpeak::getPos () const
{
        return *_pos;
}

const std::string& TPublicSpeak::getMsg () const
{
        return _msg->getString ();
}

TSpeak::SpeakType TPublicSpeak::getSpeakType () const
{
        return TSpeak::pub;
}

void TPublicSpeak::put (NetworkMessage* msg) const
{
        TSpeak::put (msg);

        _pos->put (msg);
        _msg->put (msg);
}

void TPublicSpeak::get (NetworkMessage* msg)
{
        //note TSpeaks constructor has already called TSpeak::get ()
        _pos = new TPos (msg);
        _msg = new TString (msg);
}

/***************************************************************************
 * TChannelSpeak
 ***************************************************************************/

TChannelSpeak::TChannelSpeak (NetworkMessage* msg) : TSpeak (msg)
{
        get (msg);
}

TChannelSpeak::TChannelSpeak (uint32_t                    u1, 
                              const std::string&          name,
                              uint16_t                    level, 
                              uint8_t                     type,
                              uint16_t                    channelid,
                              const std::string&          msg)
: TSpeak (u1, name, level, type)
{
        
        _channelid = new TWord16 (channelid);
        _msg = new TString (msg);
}

TChannelSpeak::TChannelSpeak (const TChannelSpeak& clone) : TSpeak (clone)
{
        _channelid = new TWord16 (*clone._channelid);
        _msg = new TString (*clone._msg);
}

TChannelSpeak::~TChannelSpeak ()
{
        delete _channelid;
        delete _msg;
}

uint16_t TChannelSpeak::getChannelId () const
{
        return _channelid->getVal ();
}

const std::string& TChannelSpeak::getMsg () const
{
        return _msg->getString ();
}

TSpeak::SpeakType TChannelSpeak::getSpeakType () const
{
        return TSpeak::channel;
}

void TChannelSpeak::put (NetworkMessage* msg) const
{
        TSpeak::put (msg);

        _channelid->put (msg);
        _msg->put (msg);
}

void TChannelSpeak::get (NetworkMessage* msg)
{
        //note TSpeaks constructor has already called TSpeak::get ()
        _channelid = new TWord16 (msg);
        _msg = new TString (msg);
}

/***************************************************************************
 * TPrivateSpeak
 ***************************************************************************/

TPrivateSpeak::TPrivateSpeak (NetworkMessage* msg) : TSpeak (msg)
{
        get (msg);
}

TPrivateSpeak::TPrivateSpeak (uint32_t                    u1, 
                              const std::string&          name,
                              uint16_t                    level, 
                              uint8_t                     type,
                              const std::string&          msg)
: TSpeak (u1, name, level, type)
{
        
        _msg = new TString (msg);
}

TPrivateSpeak::TPrivateSpeak (const TPrivateSpeak& clone) : TSpeak (clone)
{
        _msg = new TString (*clone._msg);
}

TPrivateSpeak::~TPrivateSpeak ()
{
        delete _msg;
}

const std::string& TPrivateSpeak::getMsg () const
{
        return _msg->getString ();
}

TSpeak::SpeakType TPrivateSpeak::getSpeakType () const
{
        return TSpeak::priv;
}

void TPrivateSpeak::put (NetworkMessage* msg) const
{
        TSpeak::put (msg);

        _msg->put (msg);
}

void TPrivateSpeak::get (NetworkMessage* msg)
{
        //note TSpeaks constructor has already called TSpeak::get ()
        _msg = new TString (msg);
}

/***************************************************************************
 * TRuleNumberSpeak
 ***************************************************************************/

TRuleNumberSpeak::TRuleNumberSpeak (NetworkMessage* msg) : TSpeak (msg)
{
        get (msg);
}

TRuleNumberSpeak::TRuleNumberSpeak (uint32_t                    u1, 
                              const std::string&          name,
                              uint16_t                    level, 
                              uint8_t                     type,
                              uint32_t                    rulenumber,
                              const std::string&          msg)
: TSpeak (u1, name, level, type)
{
        
        _rulenumber = new TWord32 (rulenumber);
        _msg = new TString (msg);
}

TRuleNumberSpeak::TRuleNumberSpeak (const TRuleNumberSpeak& clone) : TSpeak (clone)
{
        _rulenumber = new TWord32 (*clone._rulenumber);
        _msg = new TString (*clone._msg);
}

TRuleNumberSpeak::~TRuleNumberSpeak ()
{
        delete _rulenumber;
        delete _msg;
}

uint32_t TRuleNumberSpeak::getRuleNumber () const
{
        return _rulenumber->getVal ();
}

const std::string& TRuleNumberSpeak::getMsg () const
{
        return _msg->getString ();
}

TSpeak::SpeakType TRuleNumberSpeak::getSpeakType () const
{
        return TSpeak::channel;
}

void TRuleNumberSpeak::put (NetworkMessage* msg) const
{
        TSpeak::put (msg);

        _rulenumber->put (msg);
        _msg->put (msg);
}

void TRuleNumberSpeak::get (NetworkMessage* msg)
{
        //note TSpeaks constructor has already called TSpeak::get ()
        _rulenumber = new TWord32 (msg);
        _msg = new TString (msg);
}

/***************************************************************************
 * TRuleSpeak
 ***************************************************************************/

TRuleSpeak::TRuleSpeak (NetworkMessage* msg) : TSpeak (msg)
{
        get (msg);
}

TRuleSpeak::TRuleSpeak (uint32_t                    u1, 
                              const std::string&          name,
                              uint16_t                    level, 
                              uint8_t                     type,
                              const std::string&          msg)
: TSpeak (u1, name, level, type)
{
        
        _msg = new TString (msg);
}

TRuleSpeak::TRuleSpeak (const TRuleSpeak& clone) : TSpeak (clone)
{
        _msg = new TString (*clone._msg);
}

TRuleSpeak::~TRuleSpeak ()
{
        delete _msg;
}

const std::string& TRuleSpeak::getMsg () const
{
        return _msg->getString ();
}

TSpeak::SpeakType TRuleSpeak::getSpeakType () const
{
        return TSpeak::priv;
}

void TRuleSpeak::put (NetworkMessage* msg) const
{
        TSpeak::put (msg);

        _msg->put (msg);
}

void TRuleSpeak::get (NetworkMessage* msg)
{
        //note TSpeaks constructor has already called TSpeak::get ()
        _msg = new TString (msg);
}

/***************************************************************************
 * TSpeakFactory
 ***************************************************************************/

TSpeakFactory::TSpeakFactory (NetworkMessage* msg)
{
        _msg = msg;
        _readable = true;
}

TSpeakFactory::TSpeakFactory ()
{
        _readable = false;
}
                
TSpeak* TSpeakFactory::getSpeak ()
{
        if (!_readable) {
                printf ("error: TSpeakFactory not initialized to read\n");
                return NULL;
        }
        //get the message type
        uint32_t oldpos = _msg->getPos ();
        TWord32 unk (_msg);
        TString name (_msg);
        TWord16 level (_msg);
        uint8_t type = TWord8 (_msg).getVal ();
        _msg->setPos (oldpos);

        switch (type)
        {
                case SPEAK_SAY:
                case SPEAK_WHISPER:
                case SPEAK_YELL:
                case SPEAK_MONSTER_SAY:
                case SPEAK_MONSTER_YELL:
                case SPEAK_PRIVATE_NP:
                case SPEAK_PRIVATE_PN:
                {
                        return new TPublicSpeak (_msg);
                }

                case SPEAK_CHANNEL_R1:
                case SPEAK_CHANNEL_R2:
                case SPEAK_CHANNEL_O:
                case SPEAK_CHANNEL_Y:
                {
                        return new TChannelSpeak (_msg);
                }
                case SPEAK_PRIVATE:
                case SPEAK_BROADCAST:
                case SPEAK_PRIVATE_RED:
                {
                        return new TPrivateSpeak (_msg);
                }
                /*case SPEAK_CHANNEL_UNK6:
                {
                        return new TRuleNumberSpeak (_msg);
                }
                case SPEAK_CHANNEL_UNK7:
                case SPEAK_CHANNEL_UNK8:
                {
                        return new TRuleSpeak (_msg);
                        break;
                }*/
                default:
                {
                        uint32_t tmp = type;
                        printf ("TSpeakFactory: unknown speak type %d\n", tmp);
                        return NULL;
                }
        }
}
TSpeak* TSpeakFactory::cloneSpeak (const TSpeak& clone)
{
        uint8_t type = clone.getType ();
        switch (type)
        {
                case SPEAK_SAY:
                case SPEAK_WHISPER:
                case SPEAK_YELL:
                case SPEAK_MONSTER_SAY:
                case SPEAK_MONSTER_YELL:
                case SPEAK_PRIVATE_NP:
                case SPEAK_PRIVATE_PN:
                {
                        return new TPublicSpeak (_msg);
                }

                case SPEAK_CHANNEL_R1:
                case SPEAK_CHANNEL_R2:
                case SPEAK_CHANNEL_O:
                case SPEAK_CHANNEL_Y:
                {
                        return new TChannelSpeak (_msg);
                }
                case SPEAK_PRIVATE:
                case SPEAK_BROADCAST:
                case SPEAK_PRIVATE_RED:
                {
                        return new TPrivateSpeak (_msg);
                }
                /*case SPEAK_CHANNEL_UNK6:
                {
                        return new TRuleNumberSpeak (_msg);
                }
                case SPEAK_CHANNEL_UNK7:
                case SPEAK_CHANNEL_UNK8:
                {
                        return new TRuleSpeak (_msg);
                        break;
                }*/
                default:
                {
                        uint32_t tmp = type;
                        printf ("TSpeakFactory: unknown speak type %d\n", tmp);
                        return NULL;
                }
        }
}
        
