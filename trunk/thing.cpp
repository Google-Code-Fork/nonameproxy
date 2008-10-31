#include "thing.h"

/********************************************
 * Item
 *******************************************/

Item::Item (uint32_t itemid)
{
        _itemid = itemid;
}

Item::Item (const Item& clone)
{
        _itemid = clone._itemid;
}

Thing::thing_t Item::getType () const
{
        return Thing::t_item;
}

uint32_t Item::getItemId () const
{
        return _itemid;
}

void Item::setItemId (uint32_t itemid)
{
        _itemid = itemid;
}

/********************************************
 * XItem
 *******************************************/

XItem::XItem (uint32_t itemid, uint32_t xbyte)
{
        _itemid = itemid;
        _xbyte = _xbyte;
}

XItem::XItem (const XItem& clone)
{
        _itemid = clone._itemid;
        _xbyte = clone._xbyte;
}

Thing::thing_t XItem::getType () const
{
        return Thing::t_xitem;
}

uint32_t XItem::getItemId () const
{
        return _itemid;
}

void XItem::setItemId (uint32_t itemid)
{
        _itemid = itemid;
}

uint32_t XItem::getXByte () const
{
        return _xbyte;
}

void XItem::setXByte (uint32_t xbyte)
{
        _xbyte = xbyte;
}

/********************************************
 * Creature
 *******************************************/
Creature::Creature (uint32_t tibiaid, 
                    const std::string& name,
                    uint32_t hp, 
                    turn_dir_t direction, 
                    const Outfit_t& outfit, 
                    uint32_t lightcolor,
                    uint32_t lightradius,
                    uint32_t speed,
                    uint32_t skull, 
                    uint32_t shield)
{
        _tibiaid        = tibiaid;
        _name           = name;
        _hp             = hp;
        _direction      = direction;
        _outfit         = outfit;
        _lightcolor     = lightcolor;
        _lightradius    = lightradius;
        _speed          = speed;
        _skull          = skull;
        _shield         = shield;
}
        
Creature::Creature (const Creature& clone)
{
        _tibiaid        = clone._tibiaid;
        _name           = clone._name;
        _hp             = clone._hp;
        _direction      = clone._direction;
        _outfit         = clone._outfit;
        _lightcolor     = clone._lightcolor;
        _lightradius    = clone._lightradius;
        _speed          = clone._speed;
        _skull          = clone._skull;
        _shield         = clone._shield;
}
        
bool Creature::isPlayer () const 
{ 
        return (_tibiaid & 0x10000000) && !(_tibiaid & 0x20000000);
}

bool Creature::isMonster () const 
{ 
        return (_tibiaid & 0x40000000); 
}

bool Creature::isNPC () const 
{ 
        return (_tibiaid & 0x10000000) && (_tibiaid & 0x20000000);
}

Thing::thing_t Creature::getType () const
{
        return Thing::t_creature;
}

uint32_t Creature::getTibiaId () const
{
        return _tibiaid;
}

void Creature::setTibiaId (uint32_t tibiaid)
{
        _tibiaid = tibiaid;
}

const std::string& Creature::getName () const
{
        return _name;
}

void Creature::setName (const std::string& name)
{
        _name = name;
}

uint32_t Creature::getHp () const
{
        return _hp;
}

void Creature::setHp (uint32_t hp)
{
        _hp = hp;
}

turn_dir_t Creature::getDirection () const
{
        return _direction;
}

void Creature::setDirection (turn_dir_t direction)
{
        _direction = direction;
}

const Outfit_t& Creature::getOutfit () const
{
        return _outfit;
}

void Creature::setOutfit (const Outfit_t& outfit)
{
        _outfit = outfit;
}

uint32_t Creature::getLightColor () const
{
        return _lightcolor;
}

void Creature::setLightColor (uint32_t lightcolor)
{
        _lightcolor = lightcolor;
}

uint32_t Creature::getLightRadius () const
{
        return _lightradius;
}

void Creature::setLightRadius (uint32_t lightradius)
{
        _lightradius = lightradius;
}

uint32_t Creature::getSpeed () const
{
        return _speed;
}

void Creature::setSpeed (uint32_t speed)
{
        _speed = speed;
}

uint32_t Creature::getSkull () const
{
        return _skull;
}

void Creature::setSkull (uint32_t skull)
{
        _skull = skull;
}

uint32_t Creature::getShield () const
{
        return _shield;
}

void Creature::setShield (uint32_t shield)
{
        _shield = shield;
}

