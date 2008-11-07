/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

#include <string>
#include "corehooks.h"
#include "tibiamessage.h"
#include "gamestate.h"
#include "client.h"
#include "typeparser.h"

void HRLoginMsg::func (TibiaMessage* tm, Client* client)
{
        LSMLoginMsg* li = (LSMLoginMsg*)tm;
        client->gstate->account->setAccount (li->getAccount ());
        client->gstate->account->setPassword (li->getPassword ());
        client->crypt->setXTEAKey (li->getXTEA ());
}

TibiaMessage* HWCharacterList::func (TibiaMessage* tm, Client* client)
{
        LRMCharacterList* cl = (LRMCharacterList*)tm;

        //first save the original login details in login state
        LoginDetails* ld = new LoginDetails (
                client->gstate->account->getAccount (),
                client->gstate->account->getPassword (),
                cl->getCharList ());
        client->lstate->putAccountDetails (ld);

        //and now make a new char list
        TCharacterList* fixedCharList = new TCharacterList ();
        TCharacter*  newChar;
        CharList charlist = cl->getCharList ()->getCharList ();
        for (CharList::iterator i = charlist.begin ();
                i != charlist.end (); ++ i) {
                const std::string& name = (*i)->getName ();
                const std::string& world = (*i)->getWorld ();
                newChar = new TCharacter (name, world, 0x0100007F, 7331);
                fixedCharList->addChar (newChar);
        }

        LRMCharacterList* newcl
                = new LRMCharacterList (fixedCharList, cl->getDaysPrem ());
        /* because we do not return the original message it must be deleted */
        delete cl;
        return newcl;
}

TibiaMessage* HWMOTD::func (TibiaMessage* tm, Client* client)
{
        delete tm;
        return new LRMMOTD (std::string ("450\nLol isnt this awesome\n"));
}

void HRGameInit::func (TibiaMessage* tm, Client* client)
{
        GSMGameInit* gi = (GSMGameInit*)tm;
        client->gstate->account->setAccount (gi->getAccount ());
        client->gstate->account->setPassword (gi->getPassword ());
        client->gstate->account->setName (gi->getName ());
        client->crypt->setXTEAKey (gi->getXTEA ());
}

/**********************************************************************
 * Protocol hooks
 **********************************************************************/


/**********************************************************************
 * Game Recv 
 **********************************************************************/
 
/**********************************************************************
 * GRHSelfInfo 
 **********************************************************************/
void GRHSelfInfo::func (TibiaMessage* tm, Client* client)
{
        GRMSelfInfo* si = (GRMSelfInfo*)tm;
        GameState* gs = client->gstate;

        gs->character->setTibiaId (si->getTibiaId ());
        gs->character->setReportErrors (si->getReportErrors ());
}

/**********************************************************************
 * GRHSlotItem 
 **********************************************************************/
void GRHSlotItem::func (TibiaMessage* tm, Client* client)
{
}

/**********************************************************************
 * GRHMagicEffect 
 **********************************************************************/
void GRHMagicEffect::func (TibiaMessage* tm, Client* client)
{
}

/**********************************************************************
 * GRHTextMsg 
 **********************************************************************/
void GRHTextMsg::func (TibiaMessage* tm, Client* client)
{
}

/**********************************************************************
 * GRHGlobalLight 
 **********************************************************************/
void GRHGlobalLight::func (TibiaMessage* tm, Client* client)
{
}

/**********************************************************************
 * GRHCreatureLight 
 **********************************************************************/
void GRHCreatureLight::func (TibiaMessage* tm, Client* client)
{
}

/**********************************************************************
 * GRHPlayerStats 
 **********************************************************************/
void GRHPlayerStats::func (TibiaMessage* tm, Client* client)
{
        GRMPlayerStats* ps = (GRMPlayerStats*)tm;
        CharState* character = client->gstate->character;

        const TPlayerStats& stats = ps->getStats ();

        character->setHp (stats.getHp ());
        character->setHpMax (stats.getHpMax ());
        character->setCapacity (stats.getCapacity ());
        character->setExperience (stats.getExperience ());
        character->setLevel (stats.getLevel ());
        character->setLevelPercent (stats.getLevelPercent ());
        character->setMana (stats.getMana ());
        character->setManaMax (stats.getManaMax ());
        character->setMagicLevel (stats.getMagicLevel ());
        character->setMagicLevelPercent (stats.getMagicLevelPercent ());
        character->setSoul (stats.getSoul ());
        character->setStamina (stats.getStamina ());
}

/**********************************************************************
 * GRHPlayerSkills 
 **********************************************************************/
void GRHPlayerSkills::func (TibiaMessage* tm, Client* client)
{
        GRMPlayerSkills* ps = (GRMPlayerSkills*)tm;
        CharState* chr = client->gstate->character;
        
        chr->setFist (ps->getSkills ().getFist ().getLevel ());
        chr->setFistPercent (ps->getSkills ().getFist ().getPercent ());

        chr->setClub (ps->getSkills ().getClub ().getLevel ());
        chr->setClubPercent (ps->getSkills ().getClub ().getPercent ());

        chr->setSword (ps->getSkills ().getSword ().getLevel ());
        chr->setSwordPercent (ps->getSkills ().getSword ().getPercent ());

        chr->setAxe (ps->getSkills ().getAxe ().getLevel ());
        chr->setAxePercent (ps->getSkills ().getAxe ().getPercent ());

        chr->setDistance (ps->getSkills ().getDistance ().getLevel ());
        chr->setDistancePercent (ps->getSkills ().getDistance ().getPercent ());

        chr->setShield (ps->getSkills ().getShield ().getLevel ());
        chr->setShieldPercent (ps->getSkills ().getShield ().getPercent ());

        chr->setFishing (ps->getSkills ().getFishing ().getLevel ());
        chr->setFishingPercent (ps->getSkills ().getFishing ().getPercent ());

}

/**********************************************************************
 * GRHMapInit 
 **********************************************************************/
void GRHMapInit::func (TibiaMessage* tm, Client* client)
{
        GRMMapInit* mi = (GRMMapInit*)tm;

        Pos& pos = client->gstate->map->getCurPos (); 
        const TPos& initPos = mi->getPos ();
        pos.x = initPos.x ();
        pos.y = initPos.y ();
        pos.z = initPos.z ();

        TypeParser tp;
        tp.parseMapDescription (client->gstate, mi->getMap (), client->dat);
}

/**********************************************************************
 * GRHMapNorth 
 **********************************************************************/
void GRHMapNorth::func (TibiaMessage* tm, Client* client)
{
        GRMMapNorth* mn = (GRMMapNorth*)tm;

        Pos& pos = client->gstate->map->getCurPos ();
        //printf ("North: %d %d %d ", pos.x, pos.y, pos.z);
        pos.y --;
        //printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapNorth*)tm)->getMap ();
        //printf ("map from: "); map.getStart ().show (); printf ("\n");
        //printf ("map to:   "); map.getEnd ().show (); printf ("\n");

        TypeParser tp;
        tp.parseMapDescription (client->gstate, mn->getMap (), client->dat);
}

/**********************************************************************
 * GRHMapEast 
 **********************************************************************/
void GRHMapEast::func (TibiaMessage* tm, Client* client)
{
        GRMMapEast* me = (GRMMapEast*)tm;

        Pos& pos = client->gstate->map->getCurPos ();
        //printf ("East: %d %d %d ", pos.x, pos.y, pos.z);
        pos.x ++;
        //printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapEast*)tm)->getMap ();
        //printf ("map from: "); map.getStart ().show (); printf ("\n");
        //printf ("map to:   "); map.getEnd ().show (); printf ("\n");

        TypeParser tp;
        tp.parseMapDescription (client->gstate, me->getMap (), client->dat);
}

/**********************************************************************
 * GRHMapSouth 
 **********************************************************************/
void GRHMapSouth::func (TibiaMessage* tm, Client* client)
{
        GRMMapSouth* ms = (GRMMapSouth*)tm;

        Pos& pos = client->gstate->map->getCurPos ();
        //printf ("South: %d %d %d ", pos.x, pos.y, pos.z);
        pos.y ++;
        //printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapSouth*)tm)->getMap ();
        //printf ("map from: "); map.getStart ().show (); printf ("\n");
        //printf ("map to:   "); map.getEnd ().show (); printf ("\n");

        TypeParser tp;
        tp.parseMapDescription (client->gstate, ms->getMap (), client->dat);
}

/**********************************************************************
 * GRHMapWest 
 **********************************************************************/
void GRHMapWest::func (TibiaMessage* tm, Client* client)
{
        GRMMapWest* mw = (GRMMapWest*)tm;

        Pos& pos = client->gstate->map->getCurPos ();
        //printf ("West: %d %d %d ", pos.x, pos.y, pos.z);
        pos.x --;
        //printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapWest*)tm)->getMap ();
        //printf ("map from: "); map.getStart ().show (); printf ("\n");
        //printf ("map to:   "); map.getEnd ().show (); printf ("\n");

        TypeParser tp;
        tp.parseMapDescription (client->gstate, mw->getMap (), client->dat);
}

/**********************************************************************
 * GRHMapUp 
 **********************************************************************/
void GRHMapUp::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        //printf ("Up: %d %d %d ", pos.x, pos.y, pos.z);
        pos.z --;
        pos.x ++;
        pos.y ++;
        //printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        if (((GRMMapUp*)tm)->hasMap ()) {
                TMapDescription& map = ((GRMMapUp*)tm)->getMap ();
                //printf ("map from: "); map.getStart ().show (); printf ("\n");
                //printf ("map to:   "); map.getEnd ().show (); printf ("\n");
        } else {
                //printf ("no map\n");
        }
}

/**********************************************************************
 * GRHMapDown 
 **********************************************************************/
void GRHMapDown::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        //printf ("Down: %d %d %d ", pos.x, pos.y, pos.z);
        pos.z ++;
        pos.x --;
        pos.y --;
        //printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        if (((GRMMapUp*)tm)->hasMap ()) {
                TMapDescription& map = ((GRMMapDown*)tm)->getMap ();
                //printf ("map from: "); map.getStart ().show (); printf ("\n");
                //printf ("map to:   "); map.getEnd ().show (); printf ("\n");
        } else {
                //printf ("no map\n");
        }
}

/**********************************************************************
 * GRHTileSet 
 **********************************************************************/
void GRHTileSet::func (TibiaMessage* tm, Client* client)
{
        GRMTileSet* ts = (GRMTileSet*)tm;

        //ts->show ();

        MapState* map = client->gstate->map;

        TMapDescription& tmap = ts->getMap ();

        TypeParser tp;

        Pos pos = tp.toPos (ts->getPos ());

        /* tibia does this weird thing where if we are just meant to
         * clear the tile the first thing in the map description will
         * be a skip 1. Other wise it is just a normal map description */
        tmap.begin ();
        if (tmap.getThing ().getType () == TThing::skip) {
                map->getTile (pos).clear ();
        } else {
                tp.parseMapDescription (client->gstate, tmap, client->dat);
        }
}

/**********************************************************************
 * GRHTileAdd
 **********************************************************************/
void GRHTileAdd::func (TibiaMessage* tm, Client* client)
{
        GRMTileAdd* ta = (GRMTileAdd*)tm;
        
        //ta->show ();

        MapState* map = client->gstate->map;

        const TThing& tthing = ta->getThing ();

        TypeParser tp;

        Pos pos = tp.toPos (ta->getPos ());

        Tile& t = map->getTile (pos);
        if (tthing.getType () == TThing::item) {
                t.addThing (tp.toItem ((TItem&)tthing), 
                        client->dat, true);
        } else if (tthing.getType () == TThing::xitem) {
                t.addThing (tp.toXItem ((TXItem&)tthing),
                        client->dat, true);
        } else if (tthing.getType () == TThing::newcreature) {
                t.addThing (tp.toCreature ((TNewCreature&)tthing),
                        client->dat, false);
        } else if (tthing.getType () == TThing::oldcreature) {
                Creature add = tp.toCreature ((TOldCreature&)tthing, map);
                t.addThing (add, client->dat, false);
        } else if (tthing.getType () == TThing::creatureturn) {
                Creature add = tp.toCreature ((TCreatureTurn&)tthing, map);
                t.addThing (add, client->dat, false);
        } else {
                printf ("GRHTileAdd error: you want me to add a ");
                if (tthing.getType () == TThing::skip) {
                        printf ("skip?\n");
                } else {
                        printf ("wtf??????\n");
                }
                ta->show ();
        }
}

/**********************************************************************
 * GRHTileUpdate
 **********************************************************************/
void GRHTileUpdate::func (TibiaMessage* tm, Client* client)
{
        GRMTileUpdate* tu = (GRMTileUpdate*)tm;

        //tu->show ();

        MapState* map = client->gstate->map;

        const TThing& tthing = tu->getThing ();

        TypeParser tp;

        Pos pos = tp.toPos (tu->getPos ());
        uint32_t stackpos = tu->getStackPos ();

        Tile& t = map->getTile (pos);

        if (tthing.getType () == TThing::creatureturn) {
                TCreatureTurn& turn = (TCreatureTurn&)tthing;
                Thing& thing = t.getThing (stackpos);
                if (thing.getType () != Thing::t_creature) {
                        printf ("GRHTileUpdate error: creature turn on item\n");
                        return;
                }
                Creature& creature = (Creature&)thing;
                printf ("turns not yet implemented\n");
                return;
                if (creature.getTibiaId () != turn.getTibiaId ()) {
                        printf ("GRHTileUpdate error: ");
                        printf ("creature ids do not match\n");
                }
                creature.setDirection (tp.toTurnDirection (turn.getDir ()));
                return;
        }

        if (!t.removeThing (stackpos)) {
                printf ("GRHTileUpdate error: could not remove thing\n");
                return;
        }

        if (tthing.getType () == TThing::item) {
                t.insertThing (tp.toItem ((TItem&)tthing), stackpos, 
                        client->dat);
        } else if (tthing.getType () == TThing::xitem) {
                t.insertThing (tp.toXItem ((TXItem&)tthing), stackpos, 
                        client->dat);
        } else {
                printf ("GRHTileUpdate error: you want me to update a ");
                if (tthing.getType () == TThing::oldcreature) {
                        printf ("old creature?\n");
                } else if (tthing.getType () == TThing::newcreature) {
                        printf ("new creature?\n");
                } else if (tthing.getType () == TThing::skip) {
                        printf ("skip?\n");
                } else {
                        printf ("wtf??????\n");
                }
        }
}

/**********************************************************************
 * GRHTileRemove
 **********************************************************************/
void GRHTileRemove::func (TibiaMessage* tm, Client* client)
{
        GRMTileRemove* tr = (GRMTileRemove*)tm;

        //tr->show ();

        MapState* map = client->gstate->map;

        TypeParser tp;

        Pos pos = tp.toPos (tr->getPos ());
        uint32_t stackpos = tr->getStackPos ();

        Tile& t = map->getTile (pos);

        if (!t.removeThing (stackpos)) {
                printf ("GRHTileRemove error: could not remove thing\n");
                tr->show ();
                return;
        }
}

/**********************************************************************
 * GRHCreatureMove 
 **********************************************************************/
void GRHCreatureMove::func (TibiaMessage* tm, Client* client)
{
        //tm->show ();
        GRMCreatureMove* cm = (GRMCreatureMove*)tm;
        MapState* map = client->gstate->map;

        const TPos& from = cm->getFrom ();
        uint32_t stack = cm->getStackPos ();
        const TPos& to = cm->getTo ();

        if (stack > 9) {
                printf ("GRHMCreatureMove error: stackpos = %d > 9\n", stack);
                return;
        }

        TypeParser tp;

        Tile& fromTile = map->getTile (tp.toPos (from));
        Tile& toTile = map->getTile (tp.toPos (to));

        //fromTile.show ();
        //toTile.show ();

        //map->showCreatureMap (0, 7);

        Thing& thing = fromTile.getThing (stack);
        if (thing.getType () != Thing::t_creature) {
                printf ("GRHCreatureMove error: no creature at %d %d %d %d\n",
                        from.x (), from.y (), from.z (), stack);
                return;
        }
        Creature& creature = (Creature&)thing;

        /* we must modify the creature before adding it because add inserts a 
         * new copy */

        
        if (from.x () > to.x ()) {
                creature.setDirection (TURN_DIR_WEST);
        } else if (from.x () < to.x ()) {
                creature.setDirection (TURN_DIR_EAST);
        } else if (from.y () > to.y ()) {
                creature.setDirection (TURN_DIR_NORTH);
        } else if (from.y () < to.y ()) {
                creature.setDirection (TURN_DIR_SOUTH);
        }

        fromTile.removeThing (stack);
        toTile.addThing (creature, client->dat);
}

