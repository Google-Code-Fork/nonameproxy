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
        Pos& pos = client->gstate->map->getCurPos ();
        printf ("North: %d %d %d ", pos.x, pos.y, pos.z);
        pos.y --;
        printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapNorth*)tm)->getMap ();
        printf ("map from: "); map.getStart ().show (); printf ("\n");
        printf ("map to:   "); map.getEnd ().show (); printf ("\n");
}

/**********************************************************************
 * GRHMapEast 
 **********************************************************************/
void GRHMapEast::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        printf ("East: %d %d %d ", pos.x, pos.y, pos.z);
        pos.x ++;
        printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapEast*)tm)->getMap ();
        printf ("map from: "); map.getStart ().show (); printf ("\n");
        printf ("map to:   "); map.getEnd ().show (); printf ("\n");
}

/**********************************************************************
 * GRHMapSouth 
 **********************************************************************/
void GRHMapSouth::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        printf ("South: %d %d %d ", pos.x, pos.y, pos.z);
        pos.y ++;
        printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapSouth*)tm)->getMap ();
        printf ("map from: "); map.getStart ().show (); printf ("\n");
        printf ("map to:   "); map.getEnd ().show (); printf ("\n");
}

/**********************************************************************
 * GRHMapWest 
 **********************************************************************/
void GRHMapWest::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        printf ("West: %d %d %d ", pos.x, pos.y, pos.z);
        pos.x --;
        printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        TMapDescription& map = ((GRMMapWest*)tm)->getMap ();
        printf ("map from: "); map.getStart ().show (); printf ("\n");
        printf ("map to:   "); map.getEnd ().show (); printf ("\n");
}

/**********************************************************************
 * GRHMapUp 
 **********************************************************************/
void GRHMapUp::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        printf ("Up: %d %d %d ", pos.x, pos.y, pos.z);
        pos.z --;
        printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        if (((GRMMapUp*)tm)->hasMap ()) {
                TMapDescription& map = ((GRMMapUp*)tm)->getMap ();
                printf ("map from: "); map.getStart ().show (); printf ("\n");
                printf ("map to:   "); map.getEnd ().show (); printf ("\n");
        } else {
                printf ("no map\n");
        }
}

/**********************************************************************
 * GRHMapDown 
 **********************************************************************/
void GRHMapDown::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        printf ("Down: %d %d %d ", pos.x, pos.y, pos.z);
        pos.z ++;
        printf ("to %d %d %d\n", pos.x, pos.y, pos.z);

        if (((GRMMapUp*)tm)->hasMap ()) {
                TMapDescription& map = ((GRMMapDown*)tm)->getMap ();
                printf ("map from: "); map.getStart ().show (); printf ("\n");
                printf ("map to:   "); map.getEnd ().show (); printf ("\n");
        } else {
                printf ("no map\n");
        }
}

