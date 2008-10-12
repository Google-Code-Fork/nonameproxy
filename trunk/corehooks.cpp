#include <string>
#include "corehooks.h"
#include "tibiamessage.h"
#include "gamestate.h"
#include "client.h"

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
        Pos& pos = client->gstate->map->getCurPos ();
        const TPos& initPos = ((GRMMapInit*)tm)->getPos ();
        pos.x = initPos.x ();
        pos.y = initPos.y ();
        pos.z = initPos.z ();
}

/**********************************************************************
 * GRHMapNorth 
 **********************************************************************/
void GRHMapNorth::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        pos.y --;
}

/**********************************************************************
 * GRHMapEast 
 **********************************************************************/
void GRHMapEast::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        pos.x ++;
}

/**********************************************************************
 * GRHMapSouth 
 **********************************************************************/
void GRHMapSouth::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        pos.y ++;
}

/**********************************************************************
 * GRHMapWest 
 **********************************************************************/
void GRHMapWest::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        pos.x --;
}

/**********************************************************************
 * GRHMapUp 
 **********************************************************************/
void GRHMapUp::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        pos.z --;
}

/**********************************************************************
 * GRHMapDown 
 **********************************************************************/
void GRHMapDown::func (TibiaMessage* tm, Client* client)
{
        Pos& pos = client->gstate->map->getCurPos ();
        pos.z ++;
}

