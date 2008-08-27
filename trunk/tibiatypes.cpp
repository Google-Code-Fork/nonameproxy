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

//TCreature

TCreature::TCreature (NetworkMessage* msg)
{
        get (msg);
}

TCreature::TCreature (uint32_t tibiaId, const std::string name, uint8_t hp,
                uint8_t direction, const TOutfit& outfit,
                const TCreatureLight& light, uint16_t speed,
                uint8_t skull, uint8_t shield)
{
        _tibiaId =      new TWord32 (tibiaId);
        _name =         new TString (name);
        _hp =           new TWord8 (hp);
        _direction =    new TWord8 (direction);
        //_outfit =       new TOutfit (outfit);
        _light =        new TCreatureLight (light);
        _speed =        new TWord16 (speed);
        _skull =        new TWord8 (skull);
        _shield =       new TWord8 (shield);
}

TCreature::TCreature (const TCreature& clone)
{
        _tibiaId =      new TWord32 (*clone._tibiaId);
        _name =         new TString (*clone._name);
        _hp =           new TWord8 (*clone._hp);
        _direction =    new TWord8 (*clone._direction);
        //_outfit =       new TOutfit (*clone._outfit);
        _light =        new TCreatureLight (*clone._light);
        _speed =        new TWord16 (*clone._speed);
        _skull =        new TWord8 (*clone._skull);
        _shield =       new TWord8 (*clone._shield);
}

TCreature::~TCreature ()
{
        delete _tibiaId;
        delete _name;
        delete _hp;
        delete _direction;
        //delete _outfit;
        delete _light;
        delete _speed;
        delete _skull;
        delete _shield;
}

TThing::ThingType TCreature::getType () const
{
        return TThing::creature;
}
                
void TCreature::show () const
{
        printf ("\tTCreature {\n");
        printf ("\t\t_tibiaId: "); _tibiaId->show (); printf ("\n");
        printf ("\t\t_name: "); _name->show (); printf ("\n");
        printf ("\t\t_hp: "); _hp->show (); printf ("\n");
        printf ("\t\t_direction: "); _direction->show (); printf ("\n");
        //printf ("\t\t_outfit: "); _outfit->show (); printf ("\n");
        printf ("\t\t_light: "); _light->show (); printf ("\n");
        printf ("\t\t_speed: "); _speed->show (); printf ("\n");
        printf ("\t\t_skull: "); _skull->show (); printf ("\n");
        printf ("\t\t_shield: "); _shield->show (); printf ("\n");
        printf ("}\n");
}

void TCreature::put (NetworkMessage* msg) const
{
        _tibiaId->put (msg);
        _name->put (msg);
        _hp->put (msg);
        _direction->put (msg);
        //_outfit->put (msg);
        _light->put (msg);
        _speed->put (msg);
        _skull->put (msg);
        _shield->put (msg);
}

uint32_t TCreature::getTibiaId () const
{
        return _tibiaId->getVal ();
}

const std::string& TCreature::getName () const
{
        return _name->getString ();
}

uint8_t TCreature::getHp () const
{
        return _hp->getVal ();
}

uint8_t TCreature::getDirection () const
{
        return _direction->getVal ();
}

const TOutfit& TCreature::getOutfit () const
{
        return *_outfit;
}

const TCreatureLight& TCreature::getCreatureLight () const
{
        return *_light;
}

uint16_t TCreature::getSpeed () const
{
        return _speed->getVal ();
}

uint8_t TCreature::getSkull () const
{
        return _skull->getVal ();
}

uint8_t TCreature::getShield () const
{
        return _shield->getVal ();
}

void TCreature::get (NetworkMessage* msg)
{
        _tibiaId = new TWord32 (msg); 
        _name = new TString (msg); 
        _hp = new TWord8 (msg); 
        _direction = new TWord8 (msg); 
        //_outfit = new TOutfit (msg); 
        _light = new TCreatureLight (msg); 
        _speed = new TWord16 (msg); 
        _skull = new TWord8 (msg); 
        _shield = new TWord8 (msg); 

}

//Yes this could be derived from TCreature, but that makes thing complicated
// TOldCreature

TOldCreature::TOldCreature (NetworkMessage* msg)
{
        get (msg);
}

TOldCreature::TOldCreature (uint32_t tibiaId, const std::string name, 
                uint8_t hp, uint8_t direction, const TOutfit& outfit, 
                const TCreatureLight& light, uint16_t speed,
                uint8_t skull, uint8_t shield)
{
        _creature = new TCreature (tibiaId, name, hp, direction, outfit, light,
                                        speed, skull, shield);
}

TOldCreature::TOldCreature (const TOldCreature& clone)
{
        _creature = new TCreature (*clone._creature);
}

TOldCreature::~TOldCreature ()
{
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
        _creature->put (msg);
}

TCreature* TOldCreature::getCreature () const
{
        return new TCreature (*_creature);
}

void TOldCreature::get (NetworkMessage* msg)
{
        _creature = new TCreature (msg);
};

// TNewCreature
TNewCreature::TNewCreature (NetworkMessage* msg)
{
        get (msg);
}

TNewCreature::TNewCreature (uint32_t removeId, uint32_t tibiaId, 
                const std::string name, uint8_t hp, uint8_t direction, 
                const TOutfit& outfit, const TCreatureLight& light, 
                uint16_t speed, uint8_t skull, uint8_t shield)
{
        _removeId = new TWord32 (removeId);
        _creature = new TCreature (tibiaId, name, hp, direction, outfit, light,
                                        speed, skull, shield);
}

TNewCreature::TNewCreature (const TNewCreature& clone)
{
        _removeId = new TWord32 (*clone._removeId);
        _creature = new TCreature (*clone._creature);
}

TNewCreature::~TNewCreature ()
{
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

void TNewCreature::get (NetworkMessage* msg)
{
        _removeId = new TWord32 (msg);
        _creature = new TCreature (msg);
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
        _ff = new TWord8 (*clone._ff);
        _n = new TWord8 (*clone._n);
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
        _ff->put (msg);
        _n->put (msg);
}

uint8_t TSkip::getN () const
{
        return _n->getVal ();
}

void TSkip::get (NetworkMessage* msg)
{
        _ff = new TWord8 (msg);
        _n = new TWord8 (msg);
}

/**************************************************************
 * TOutfit
 **************************************************************/

// TItemOutift
TItemOutfit::TItemOutfit (NetworkMessage* msg)
{
        get (msg);
}

TItemOutfit::TItemOutfit (const TThing& item)
{
        _lookType = new TWord16 ((uint16_t)0x0000);
        //new Thing requires factory
}

TItemOutfit::TItemOutfit (const TItemOutfit& clone)
{
        _lookType = new TWord16 (*clone._lookType);
        //factory
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

void TItemOutfit::get (NetworkMessage* msg)
{
        _lookType = new TWord16 (msg);
        // factory
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

