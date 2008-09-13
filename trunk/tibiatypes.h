#ifndef __TIBIA_TYPES_H
#define __TIBIA_TYPES_H

#include <stdint.h>
#include <string>
#include <list>

class NetworkMessage;
class DatReader;

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
                        creatureturn,
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
                TCreature (NetworkMessage* msg, DatReader* dat);
                TCreature (uint32_t tibiaId, const std::string name, uint8_t hp,
                                uint8_t direction, const TOutfit& outfit,
                                const TCreatureLight& light, uint16_t speed,
                                uint8_t skull, uint8_t shield);
                TCreature (const TCreature& clone);
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
                void get (NetworkMessage* msg, DatReader* dat);

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
                TOldCreature (NetworkMessage* msg, DatReader* dat);
                TOldCreature (uint32_t tibiaId, const std::string name, 
                                uint8_t hp, uint8_t direction, 
                                const TOutfit& outfit,
                                const TCreatureLight& light, 
                                uint16_t speed, uint8_t skull, uint8_t shield);

                TOldCreature (const TOldCreature& clone);
                virtual ~TOldCreature ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                //this function returns a clone of creatrue
                //meaning that the TOldCreature can be deleted
                TCreature* getCreature () const;

        private:
                void get (NetworkMessage* msg, DatReader* dat);

                TWord16*        _itemId;
                TCreature*      _creature;
};

class TNewCreature : public TThing
{
        public:
                TNewCreature (NetworkMessage* msg, DatReader* dat);
                TNewCreature (uint32_t removeId, uint32_t tibiaId, 
                                const std::string name, uint8_t hp,
                                uint8_t direction, const TOutfit& outfit,
                                const TCreatureLight& light, uint16_t speed,
                                uint8_t skull, uint8_t shield);

                TNewCreature (const TNewCreature& clone);
                virtual ~TNewCreature ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                uint32_t getRemoveId () const;

                //this function returns a clone of creatrue
                //meaning that the TNewCreature can be deleted
                TCreature* getCreature () const;

        private:
                void get (NetworkMessage* msg, DatReader* dat);

                TWord16*        _itemId;
                TWord32*        _removeId;
                TCreature*      _creature;
};

class TCreatureTurn : public TThing
{
        public:
                TCreatureTurn (NetworkMessage* msg);
                TCreatureTurn (uint32_t creatrueId, uint8_t dir);
                TCreatureTurn (const TCreatureTurn& clone);
                virtual ~TCreatureTurn ();

                virtual ThingType getType () const;
                
                virtual void show () const;
                virtual void put (NetworkMessage* msg) const;

                uint32_t getTibiaId () const;
                uint8_t  getDir () const;

        private:
                void get (NetworkMessage* msg);

                TWord16* _itemId;
                TWord32* _tibiaId;
                TWord8*  _dir;
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
 * TThingFactory
 ***************************************************************************/

class TThingFactory
{
        public:
                //use this constructor for reading from a network msg
                TThingFactory (NetworkMessage* msg, DatReader* dat);
                //this constructor is for cloning only
                TThingFactory ();

                TThing* getThing ();
                TThing* cloneThing (const TThing& thing);

        private:
                NetworkMessage* _msg;
                DatReader*      _dat;

                bool _readable;
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
                TItemOutfit (NetworkMessage* msg, DatReader* dat);
                TItemOutfit (const TThing& item);
                TItemOutfit (const TItemOutfit& clone);
                virtual ~TItemOutfit ();

                virtual TOutfit::OutfitType getType () const;
                virtual void put (NetworkMessage* msg) const;
                virtual void show () const;

        private:
                void get (NetworkMessage* msg, DatReader* dat);
                
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

/***************************************************************************
 * TOutfitFactory
 ***************************************************************************/

class TOutfitFactory
{
        public:
                TOutfitFactory (NetworkMessage* msg, DatReader* dat);
                TOutfitFactory ();
                
                TOutfit* getOutfit ();
                TOutfit* cloneOutfit (const TOutfit& clone);

        private:
                NetworkMessage* _msg;
                DatReader*      _dat;

                bool _readable;
};

/************************************************************************
 * TMapDescription
 * The Map is stored in the same way as it is presented in the packet
 * (ie a list of tiles separated by skips). However to ease usage
 * the start and end points of the map description are also stored
 * the start is the three lowest coordanates, and end is the 3 largest
 * coordanates
 * note this class is useless as const because it relies on an internal
 * iterator to keep track of the current tile. Therefore there are two
 * options, clone a map list every time you need to use it (expensive)
 * or to return no const references. I will take the later approach, but
 * in my code i will not abuse this. If you need to change a map, clone
 * it first.
 ************************************************************************/

typedef std::list<TThing*> MapList;
class TMapDescription
{
        public:
                TMapDescription (const TPos& start, const TPos& end,
                                NetworkMessage* msg, DatReader* dat);
                //if this constructor is used it is expected that the user will use
                //manipulation functions to add things
                TMapDescription (const TPos& start, const TPos& end);
                TMapDescription (const TMapDescription& clone);
                virtual ~TMapDescription ();

                void put (NetworkMessage* msg) const;
                void show () const;

                const TPos& getStart () const;
                const TPos& getEnd () const;

                //sets the iterator to the beginning of the map
                void begin ();
                //returns true if at end of map
                bool isEnd ();
                //move to the next thing
                void next ();
                //get the current thing 
                const TThing& getThing ();
                //insert thing BEFORE current thing
                void insert (TThing* thing);
                //replace current thing
                void replace (TThing* thing);
                //remove current thing and moves to the next thing
                void remove ();
                //adds a thing to the end of the map
                void add (TThing* thing);

                //TODO implement these nice functions
                //these functions are useful if one has to make small changes to
                //a cloned map description, keep in mind it runs in linear time
                //const TPos& getCurPos ();
                //uint32_t getStackPos ();
                //goto pos, returns true if pos was found, otherwise returns 
                //false and iterator will be left at end of map
                //bool gotoPos (const TPos& p, uint32_t stackPos);

        private:
                void get (const TPos& start, const TPos& end
                                , NetworkMessage* msg, DatReader* dat);

                TPos* _start;
                TPos* _end;
                TPos* _cur;
                MapList _map;
                MapList::iterator _it;
};

/*************************************************************************
 * TEffect
 *************************************************************************/

class TEffect
{
        public:
                TEffect (NetworkMessage* msg);
                TEffect (const TPos& pos, uint8_t effectId);
                TEffect (const TEffect& clone);
                virtual ~TEffect ();

                void put (NetworkMessage* msg) const;
                void show () const;

                const TPos& getPos () const;
                uint8_t getEffectId () const;

        private:
                void get (NetworkMessage* msg);

                TPos* _pos;
                TWord8* _effectId;
};

/*************************************************************************
 * TTextMsg
 *************************************************************************/

class TTextMsg
{
        public:
                TTextMsg (NetworkMessage* msg);
                TTextMsg (uint8_t msgType, const std::string msg);
                TTextMsg (const TTextMsg& clone);
                virtual ~TTextMsg ();

                void put (NetworkMessage* msg) const;
                void show () const;

                uint8_t getMsgType () const;
                const std::string getMsg () const;

        private:
                void get (NetworkMessage* msg);

                TWord8* _msgType;
                TString* _msg;
};

/*************************************************************************
 * TPlayerStats
 *************************************************************************/

class TPlayerStats
{
        public:
                TPlayerStats (NetworkMessage* msg);
                TPlayerStats (uint16_t getHp,
                                uint16_t getHpmax,
                                uint16_t getCapacity,
                                uint32_t getExperience,
                                uint16_t getLevel,
                                uint8_t getLevelPercent,
                                uint16_t getMana,
                                uint16_t getMaxmana,
                                uint8_t getMagicLevel,
                                uint8_t getMagicLevelPercent,
                                uint8_t getSoul,
                                uint16_t getStamina);

                TPlayerStats (const TPlayerStats& clone);
                virtual ~TPlayerStats ();

                void put (NetworkMessage* msg);
                void show ();

                uint16_t getHp () const;
                uint16_t getHpmax () const;
                uint16_t getCapacity () const;
                uint32_t getExperience () const;
                uint16_t getLevel () const;
                uint8_t getLevelPercent () const;
                uint16_t getMana () const;
                uint16_t getMaxmana () const;
                uint8_t getMagicLevel () const;
                uint8_t getMagicLevelPercent () const;
                uint8_t getSoul () const;
                uint16_t getStamina () const;

        private:
                void get (NetworkMessage* msg);

                TWord16*        _hp;
                TWord16*        _hpmax;
                TWord16*        _capacity;
                TWord32*        _experience;
                TWord16*        _level;
                TWord8*         _levelPercent;
                TWord16*        _mana;
                TWord16*        _maxmana;
                TWord8*         _magicLevel;
                TWord8*         _magicLevelPercent;
                TWord8*         _soul;
                TWord16*        _stamina;
};

/*************************************************************************
 * TPlayerSkill
 *************************************************************************/

class TPlayerSkill
{
        public:
                TPlayerSkill (NetworkMessage* msg);
                TPlayerSkill (uint8_t level, uint8_t percent);
                TPlayerSkill (const TPlayerSkill& clone);
                virtual ~TPlayerSkill ();

                void put (NetworkMessage* msg) const;
                void show () const;

                uint8_t getLevel () const;
                uint8_t getPercent () const;

        private:
                void get (NetworkMessage* msg);

                TWord8* _level;
                TWord8* _percent;
};

/*************************************************************************
 * TPlayerSkills
 *************************************************************************/

class TPlayerSkills
{
        public:
                TPlayerSkills (NetworkMessage* msg);
                TPlayerSkills ( const TPlayerSkill& fist,
                                const TPlayerSkill& club,
                                const TPlayerSkill& sword,
                                const TPlayerSkill& axe,
                                const TPlayerSkill& distance,
                                const TPlayerSkill& shield,
                                const TPlayerSkill& fishing);

                TPlayerSkills (const TPlayerSkills& clone);
                virtual ~TPlayerSkills ();

                void put (NetworkMessage* msg) const;
                void show () const;

                const TPlayerSkill& getFist () const;
                const TPlayerSkill& getClub () const;
                const TPlayerSkill& getSword () const;
                const TPlayerSkill& getAxe () const;
                const TPlayerSkill& getDistance () const;
                const TPlayerSkill& getShield () const;
                const TPlayerSkill& getFishing () const;

        private:
                void get (NetworkMessage* msg);

                TPlayerSkill* _fist;
                TPlayerSkill* _club;
                TPlayerSkill* _sword;
                TPlayerSkill* _axe;
                TPlayerSkill* _distance;
                TPlayerSkill* _shield;
                TPlayerSkill* _fishing;
};

/************************************************************************
 * TContainer
 * TContainer has the same problem as TMapDescription in that it uses
 * an internal iterator to keep track of the current item, meaning that
 * functions like begin () which moves the iterator to the begining of the
 * list can not be const, even though it does not really modify any
 * data.
 ************************************************************************/

typedef std::list<TThing*> ContainerList;
class TContainer
{
        public:
                TContainer (NetworkMessage* msg, DatReader* dat);
                //if this constructor is used it is expected that the user will use
                //manipulation functions to add things
                TContainer (const TThing& item, const std::string& name,
                                uint8_t capacity, uint8_t hasParent);
                TContainer (const TContainer& clone);
                virtual ~TContainer ();

                void put (NetworkMessage* msg) const;
                void show () const;

                const TThing& getItem () const;
                const std::string& getName () const;
                uint8_t getCapacity () const;
                uint8_t getHasParent () const;
                uint8_t getNItems () const;

                //sets the iterator to the beginning of the map
                void begin ();
                //returns true if at end of map
                bool isEnd ();
                //move to the next thing
                void next ();
                //get the current thing 
                const TThing& getThing ();
                //insert thing BEFORE current thing
                void insert (TThing* thing);
                //replace current thing
                void replace (TThing* thing);
                //remove current thing and moves to the next thing
                void remove ();
                //adds a thing to the end of the map
                void add (TThing* thing);

        private:
                void get (NetworkMessage* msg, DatReader* dat);

                TThing* _item;
                TString* _name;
                TWord8* _capacity;
                TWord8* _hasParent;
                TWord8* _nItems;
                ContainerList _items;
                ContainerList::iterator _it;
};

#endif

