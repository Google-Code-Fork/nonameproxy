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
        uint32_t sx = map.getStart ().x ();
        uint32_t ex = map.getEnd ().x ();
        uint32_t sy = map.getStart ().y ();
        uint32_t ey = map.getEnd ().y ();

        for (uint32_t x = sx; x <= ex; x ++) {
                for (uint32_t y = sy; y <= ey; y ++) {
                        if (skip == 0) {
                                Tile& t = gs->map->getTile (x, y, z);
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
        TThing::ThingType type = tthing.getType ();
        if (type == TThing::oldcreature
         || type == TThing::newcreature)
        {
                /* TODO add creature parsing code */
                return true;
        } else if (type == TThing::creatureturn) {
                /* TODO add creature turn code */
                return true;
        } else if (type == TThing::item) {
                return t.addThing (toItem ((TItem&)tthing), dat);
        } else if (type == TThing::xitem) {
                return t.addThing (toXItem ((TXItem&)tthing), dat);
        } else if (type == TThing::skip) {
                printf ("i_parseThing error: how did i get a skip?\n");
        }
        return false;
}
