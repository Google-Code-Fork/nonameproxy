#ifndef __THING_H
#define __THING_H

#include <stdint.h>
#include <string>
#include "enums.h"

class Thing
{
        public:
                virtual ~Thing () {};
                
                enum thing_t
                {
                        t_item,
                        t_xitem,
                        t_creature,
                };

                virtual thing_t getType () const = 0;
};

class Item : public Thing
{
        public:
                Item (uint32_t itemid);
                Item (const Item& clone);
                virtual ~Item () {};

                virtual thing_t getType () const;
                uint32_t getItemId () const;
                void setItemId (uint32_t itemid);

        private:
                uint32_t _itemid;
};

class XItem : public Thing
{
        public:
                XItem (uint32_t itemid, uint32_t xbyte);
                XItem (const XItem& clone);
                virtual ~XItem () {};

                virtual thing_t getType () const;
                
                uint32_t getItemId () const;
                void setItemId (uint32_t itemid);

                uint32_t getXByte () const;
                void setXByte (uint32_t xbyte);

        private:
                uint32_t _itemid;
                uint32_t _xbyte;
};

struct Outfit_t
{
	uint32_t looktype;
	uint32_t lookhead;
	uint32_t lookbody;
	uint32_t looklegs;
	uint32_t lookfeet;
	uint32_t lookitem;
	uint32_t addons;
};

class Creature : public Thing
{
        public:
                Creature (uint32_t tibiaid, 
                          const std::string& name,
                          uint32_t hp, 
                          turn_dir_t direction, 
                          const Outfit_t& outfit, 
                          uint32_t lightcolor,
                          uint32_t lightradius,
                          uint32_t speed,
                          uint32_t skull, 
                          uint32_t shield);

                Creature (const Creature& clone);
                virtual ~Creature () {};

                virtual thing_t getType () const;

                bool isPlayer () const;
                bool isMonster () const;
                bool isNPC () const;

                uint32_t                getItemId () const;
                void                    setItemId (uint32_t itemid);

                uint32_t                getTibiaId () const;
                void                    setTibiaId (uint32_t tibiaid);

                const std::string&      getName () const;
                void                    setName (const std::string& name);

                uint32_t                getHp () const;
                void                    setHp (uint32_t hp);

                turn_dir_t              getDirection () const;
                void                    setDirection (turn_dir_t direction);

                const Outfit_t&         getOutfit () const;
                void                    setOutfit (const Outfit_t& outfit);

                uint32_t                getLightColor () const;
                void                    setLightColor (uint32_t lightcolor);

                uint32_t                getLightRadius () const;
                void                    setLightRadius (uint32_t lightradius);

                uint32_t                getSpeed () const;
                void                    setSpeed (uint32_t speed);

                uint32_t                getSkull () const;
                void                    setSkull (uint32_t skull);

                uint32_t                getShield () const;
                void                    setShield (uint32_t shield);

        private:

                uint32_t        _tibiaid;
                std::string     _name;
                uint32_t        _hp;
                turn_dir_t      _direction;
                Outfit_t        _outfit;
                uint32_t        _lightcolor;
                uint32_t        _lightradius;
                uint32_t        _speed;
                uint32_t        _skull;
                uint32_t        _shield;
};

#endif

