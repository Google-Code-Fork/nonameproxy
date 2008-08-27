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

                void put (NetworkMessage* msg) const;

                uint8_t getVal () const;
                void show () const;
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

                void put (NetworkMessage* msg) const;

                uint16_t getVal () const;
                void show () const;
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

                void put (NetworkMessage* msg) const;

                uint32_t getVal () const;
                void show () const;
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

                void put (NetworkMessage* msg) const;

                uint32_t getLen () const;
                const std::string& getString () const;
                void show () const;
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

                void put (NetworkMessage* msg) const;
                void show () const;
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

                void put (NetworkMessage* msg) const;
                
                const uint32_t* getKey () const;
                void show () const;
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

                const std::string& getName () const;
                const std::string& getWorld () const;
                uint32_t getIp () const;
                uint16_t getPort () const;

                void put (NetworkMessage* msg) const;
                void show () const;
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
                
                uint32_t getNChars () const;
                const CharList& getCharList () const;

                void put (NetworkMessage* msg) const;
                void show () const;
        private:
                void get (NetworkMessage* msg);
                TWord8* _nChars;
                CharList _charlist;
};

class TPos
{
        public:
                TPos (NetworkMessage* msg);
                TPos (uint16_t xPos, uint16_t yPos, uint8_t zPos);
                TPos (const TPos& clone);
                virtual ~TPos ();

                void put (NetworkMessage* msg) const;
                void show () const;

                uint16_t x () const;
                uint16_t y () const;
                uint8_t  z () const;

        private:
                void get (NetworkMessage* msg);

                TWord16* _x;
                TWord16* _y;
                TWord8*  _z;
};

//Tibia has two light sources, creature light and item light
//the difference is that creature light has TWord8s and 
//item light has TWord16s

class TItemLight
{
        public:
                TItemLight (NetworkMessage* msg);
                TItemLight (uint16_t color, uint16_t radius);
                TItemLight (const TItemLight&);
                virtual ~TItemLight ();

                uint16_t getColor () const;
                uint16_t getRadius () const;

                void put (NetworkMessage* msg) const;
                void show () const;

        private:
                void get (NetworkMessage* msg);

                TWord16* _color;
                TWord16* _radius;
};

class TCreatureLight
{
        public:
                TCreatureLight (NetworkMessage* msg);
                TCreatureLight (uint8_t color, uint8_t radius);
                TCreatureLight (const TCreatureLight&);
                virtual ~TCreatureLight ();

                uint8_t getColor () const;
                uint8_t getRadius () const;

                void put (NetworkMessage* msg) const;
                void show () const;

        private:
                void get (NetworkMessage* msg);

                TWord8* _color;
                TWord8* _radius;
};

/***************************************************************************
 * TThing are both used to interpret binary data as well as to store
 * information in gamestate. Therefore it must be able to handle all
 * item types found in a map description
 *      item:           standard TWord16 item
 *      xitem:          TWord16 + extra TWord8 
 *      creature:       creature inof
 *      olddcreatrue:   header + creature
 *      newcreature:    header + Remove ID + creature
 *      creatureturn:   creature ID + direction
 *      skiptiles:      0xFF followed by TWord8 n tiles
 * to do this we will use TThingFactory, a class that returns a class
 * derived from TThing. Unfortunately this make creating Items
 * cumbersome, however because items are rarely read by themselves, ie
 * they will probably be read from the constructor of a higher type,
 * this shouldnt be too much of an issue. The one problem is creature,
 * for storage we would like a stand alone creature type, but for parsing
 * we would like a old and new creature. Therefore both newcreature and
 * old creature have a function get creature, which returns a CLONE of
 * the creature. The wrapper class can then be deleted.
 ***************************************************************************/

class TOutfit;

class TThing
{
        public:
                enum ThingType
                {
                        item,
                        xitem,
                        creature,
                        oldcreature,
                        newcreature,
                        skip,
                };

                virtual ThingType getType () const = 0;
                virtual void put (NetworkMessage* msg) const = 0;
                virtual void show () const = 0;
};

class TItem : public TThing
{
        public:
                TItem (NetworkMessage* msg);
                TItem (uint16_t itemId);
                TItem (const TItem& clone);
                virtual ~TItem ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                uint16_t getItemId () const;

        private:
                void get (NetworkMessage* msg);

                TWord16* _itemId;
};

class TXItem : public TThing
{
        public:
                TXItem (NetworkMessage* msg);
                TXItem (uint16_t itemId, uint8_t xByte);
                TXItem (const TXItem& clone);
                virtual ~TXItem ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                uint16_t getItemId () const;
                uint8_t  getXByte () const;

        private:
                void get (NetworkMessage* msg);

                TWord16* _itemId;
                TWord8*  _xByte;
};

class TCreature : public TThing
{
        public:
                TCreature (NetworkMessage* msg);
                TCreature (uint32_t tibiaId, const std::string name, uint8_t hp,
                                uint8_t direction, const TOutfit& outfit,
                                const TCreatureLight& light, uint16_t speed,
                                uint8_t skull, uint8_t shield);
                TCreature (const TCreature& clone);

                TCreature (const TItem& clone);
                virtual ~TCreature ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                uint32_t                getTibiaId () const;
                const std::string&      getName () const;
                uint8_t                 getHp () const;
                uint8_t                 getDirection () const;
                const TOutfit&          getOutfit () const;
                const TCreatureLight&   getCreatureLight () const;
                uint16_t                getSpeed () const;
                uint8_t                 getSkull () const;
                uint8_t                 getShield () const;

        private:
                void get (NetworkMessage* msg);

                TWord32*        _tibiaId;
                TString*        _name;
                TWord8*         _hp;
                TWord8*         _direction;
                TOutfit*        _outfit;
                TCreatureLight* _light;
                TWord16*        _speed;
                TWord8*         _skull;
                TWord8*         _shield;

};

//Yes this could be derived from TCreature, but that makes thing complicated

class TOldCreature : public TThing
{
        public:
                TOldCreature (NetworkMessage* msg);
                TOldCreature (uint32_t tibiaId, const std::string name, 
                                uint8_t hp, uint8_t direction, 
                                const TOutfit& outfit,
                                const TCreatureLight& light, 
                                uint16_t speed, uint8_t skull, uint8_t shield);

                TOldCreature (const TOldCreature& clone);
                TOldCreature (const TItem& clone);
                virtual ~TOldCreature ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                //this function returns a clone of creatrue
                //meaning that the TOldCreature can be deleted
                TCreature* getCreature () const;

        private:
                void get (NetworkMessage* msg);

                TCreature*      _creature;
};

class TNewCreature : public TThing
{
        public:
                TNewCreature (NetworkMessage* msg);
                TNewCreature (uint32_t removeId, uint32_t tibiaId, 
                                const std::string name, uint8_t hp,
                                uint8_t direction, const TOutfit& outfit,
                                const TCreatureLight& light, uint16_t speed,
                                uint8_t skull, uint8_t shield);

                TNewCreature (const TNewCreature& clone);
                TNewCreature (const TItem& clone);
                virtual ~TNewCreature ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                uint32_t getRemoveId () const;

                //this function returns a clone of creatrue
                //meaning that the TNewCreature can be deleted
                TCreature* getCreature () const;

        private:
                void get (NetworkMessage* msg);

                TWord32*        _removeId;
                TCreature*      _creature;
};

class TSkip : public TThing
{
        public:
                TSkip (NetworkMessage* msg);
                TSkip (uint8_t n);
                TSkip (const TSkip& clone);

                virtual ~TSkip ();

                virtual ThingType getType () const;
                
                void show () const;
                void put (NetworkMessage* msg) const;

                uint8_t getN () const;

        private:
                void get (NetworkMessage* msg);

                TWord8* _ff;
                TWord8* _n;
};

/***************************************************************************
 * TOutfit
 * Like TThing TOutfit has multiple forms
 *      Item Outfit:    looktype = 0 Item
 *      Char Outfit:    looktype head body legs feet addons
 ***************************************************************************/
 
class TOutfit
{
        public:
                enum OutfitType
                {
                        itemoutfit,
                        charoutfit,
                };

                virtual OutfitType getType () const = 0;
                virtual void put (NetworkMessage* msg) const = 0;
                virtual void show () const = 0;
};

class TItemOutfit : public TOutfit
{
        public:
                TItemOutfit (NetworkMessage* msg);
                TItemOutfit (const TThing& item);
                TItemOutfit (const TItemOutfit& clone);
                virtual ~TItemOutfit ();

                virtual TOutfit::OutfitType getType () const;
                virtual void put (NetworkMessage* msg) const;
                virtual void show () const;

        private:
                void get (NetworkMessage* msg);
                
                TWord16* _lookType;
                TThing*  _item;
};

class TCharOutfit : public TOutfit
{
        public:
                TCharOutfit (NetworkMessage* msg);
                TCharOutfit (uint16_t lookType, uint8_t head, uint8_t body,
                                uint8_t legs, int8_t feet, uint8_t addons);
                TCharOutfit (const TCharOutfit& clone);
                virtual ~TCharOutfit ();

                virtual TOutfit::OutfitType getType () const;
                virtual void put (NetworkMessage* msg) const;
                virtual void show () const;

        private:
                void get (NetworkMessage* msg);
                
                TWord16* _lookType;
                TWord8*  _head;
                TWord8*  _body;
                TWord8*  _legs;
                TWord8*  _feet;
                TWord8*  _addons;
};
#endif

