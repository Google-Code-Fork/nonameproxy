#include "typeparser.h"

Pos TypeParser::toPos (const TPos& tpos)
{
        return Pos (tpos.x (), tpos.y (), tpos.z ());
}

TPos TypeParser::toTPos (const Pos& pos)
{
        return TPos (pos.x, pos.y, pos.z);
}

Item TypeParser::toItem (const TItem& titem)
{
        const TItem& i = (const TItem&)titem;
        return Item (i.getItemId ());
}

XItem TypeParser::toXItem (const TXItem& txitem)
{
        const TXItem& i = (const TXItem&)txitem;
        return XItem (i.getItemId (), i.getXByte ());
}

Creature TypeParser::toCreature (const TNewCreature& tcreature)
{
        return Creature (tcreature.getRemoveId (),
                         tcreature.getTibiaId (),
                         tcreature.getName (),
                         tcreature.getHp (),
                         toTurnDirection (tcreature.getDirection ()),

                         toOutfit (tcreature.getOutfit ()),

                         tcreature.getCreatureLight (). getColor (),
                         tcreature.getCreatureLight (). getRadius (),
                         tcreature.getSpeed (),
                         tcreature.getSkull (),
                         tcreature.getShield ());
}

Creature TypeParser::toCreature (const TOldCreature& tcreature, MapState* map)
{
        const Creature& updated = map->getCreature (tcreature.getTibiaId ());

        return Creature (0,
                         tcreature.getTibiaId (),
                         updated.getName (),
                         tcreature.getHp (),
                         toTurnDirection (tcreature.getDirection ()),

                         toOutfit (tcreature.getOutfit ()),

                         tcreature.getCreatureLight (). getColor (),
                         tcreature.getCreatureLight (). getRadius (),
                         tcreature.getSpeed (),
                         tcreature.getSkull (),
                         tcreature.getShield ());
}

Creature TypeParser::toCreature (const TCreatureTurn& tcreature, MapState* map)
{
        const Creature& updated = map->getCreature (tcreature.getTibiaId ());

        return Creature (0,
                         tcreature.getTibiaId (),
                         updated.getName (),
                         updated.getHp (),
                         toTurnDirection (tcreature.getDir ()),

                         updated.getOutfit (),
                         updated.getLightColor (),
                         updated.getLightRadius (),
                         updated.getSpeed (),
                         updated.getSkull (),
                         updated.getShield ());
}

Outfit_t TypeParser::toOutfit (const TOutfit& toutfit)
{
        Outfit_t ret;
        if (toutfit.getType () == TOutfit::itemoutfit) {
                ret.lookitem = ((TItemOutfit&)toutfit).getItemId ();

	        ret.looktype = 0;
	        ret.lookhead = 0;
	        ret.lookbody = 0;
	        ret.looklegs = 0;
	        ret.lookfeet = 0;
	        ret.addons = 0;
        } else if (toutfit.getType () == TOutfit::charoutfit) {
                const TCharOutfit& charoutfit = (TCharOutfit&)toutfit;

                ret.looktype = charoutfit.getLookType ();
                ret.lookhead = charoutfit.getHead ();
                ret.lookbody = charoutfit.getBody ();
                ret.looklegs = charoutfit.getLegs ();
                ret.lookfeet = charoutfit.getFeet ();
                ret.addons = charoutfit.getAddons ();

                ret.lookitem = 0;
        } else {
                printf ("toOutfit error: funny outfit type\n");
        }
        return ret;
}

direction_t TypeParser::toDirection (uint32_t dir)
{
        switch (dir) {
                case 0x01: return DIRECTION_EAST;
                case 0x02: return DIRECTION_NE;
                case 0x03: return DIRECTION_NORTH;
                case 0x04: return DIRECTION_NW;
                case 0x05: return DIRECTION_WEST;
                case 0x06: return DIRECTION_SW;
                case 0x07: return DIRECTION_SOUTH;
                case 0x08: return DIRECTION_SE;
        }
        printf ("toDirection error: invalid direction\n");
        return DIRECTION_UNUSED;;
}

turn_dir_t TypeParser::toTurnDirection (uint32_t dir)
{
        switch (dir) {
                case 0x00: return TURN_DIR_NORTH;
                case 0x01: return TURN_DIR_EAST;
                case 0x02: return TURN_DIR_SOUTH;
                case 0x03: return TURN_DIR_WEST;
        }
        printf ("toTurnDirection error: invalid direction %d\n", dir);
        printf (NULL);
        return TURN_DIR_UNUSED;
}

/**************************************************
 * Map Parsing Functions
 **************************************************/

bool TypeParser::parseMapDescription (GameState* gs, 
                                      TMapDescription& map,
                                      DatReader* dat)
{
        /* make sure we are looking at the beginning of the map */
        map.begin ();
        uint32_t skip = 0;

        uint32_t sz, ez;
        /* if the map is above ground we read the floors in reverse order */
        if (map.getStart ().z () == 0) {
                sz = map.getEnd ().z ();
                ez = map.getStart ().z ();
                /* ez - 1 is a nasty hack :( */
                for (uint32_t z = sz; z != (ez - 1); z --) {
                        if (!i_parseFloor (gs, map, z, skip, dat)) {
                                return false;
                        }
                }
        } else {
                sz = map.getStart ().z ();
                ez = map.getEnd ().z ();
                for (uint32_t z = sz; z <= ez; z ++) {
                        if (!i_parseFloor (gs, map, z, skip, dat)) {
                                return false;
                        }
                }
        }
        return true;
}

bool TypeParser::i_parseFloor (GameState* gs, 
                               TMapDescription& map, 
                               uint32_t z,
                               uint32_t& skip,
                               DatReader* dat)
{
        printf ("parsing floor %d\n", z);
        uint32_t sx = map.getStart ().x ();
        uint32_t ex = map.getEnd ().x ();
        uint32_t sy = map.getStart ().y ();
        uint32_t ey = map.getEnd ().y ();

        for (uint32_t x = sx; x <= ex; x ++) {
                for (uint32_t y = sy; y <= ey; y ++) {
                        if (skip == 0) {
                                Tile& t = gs->map->getIsoTile (x, y, z);
                                if (!i_parseTile (gs, map, t, skip, dat)) {
                                        return false;
                                }
                        } else {
                                skip --;
                        }
                }
        }
        return true;
}

bool TypeParser::i_parseTile (GameState* gs, 
                              TMapDescription& map, 
                              Tile& t,
                              uint32_t& skip,
                              DatReader* dat)
{
        uint32_t n = 0;
        t.clear ();
        const TThing* tthing = &map.getThing ();;
        while (tthing->getType () != TThing::skip) {
                if (!i_parseThing (gs, *tthing, t, dat)) {
                        printf ("i_parseTile error: i_parseThing failed\n");
                        return false;
                }
                map.next ();
                tthing = &map.getThing ();
                n ++;
                if (n > 10) {
                        printf ("i_parseTile error: too many things\n");
                        return false;
                }
        }
        skip = ((const TSkip*)tthing)->getN ();
        map.next ();
        return true;
}

bool TypeParser::i_parseThing (GameState* gs, 
                               const TThing& tthing, 
                               Tile& t,
                               DatReader* dat)
{
        MapState* map = gs->map;
        TThing::ThingType type = tthing.getType ();
        if (type == TThing::oldcreature)
        {
                Creature add = toCreature ((TOldCreature&)tthing, map);
                return t.addThing (add, dat);
        } else if (type == TThing::newcreature) {
                return t.addThing (toCreature ((TNewCreature&)tthing), dat);
        } else if (type == TThing::creatureturn) {
                Creature add = toCreature ((TCreatureTurn&)tthing, map);
                return t.addThing (add, dat);
        } else if (type == TThing::item) {
                return t.addThing (toItem ((TItem&)tthing), dat);
        } else if (type == TThing::xitem) {
                return t.addThing (toXItem ((TXItem&)tthing), dat);
        } else if (type == TThing::skip) {
                printf ("i_parseThing error: how did i get a skip?\n");
        }
        return false;
}
