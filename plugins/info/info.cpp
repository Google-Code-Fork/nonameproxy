#include <stdio.h>
#include "info.h"
#include "enums.h"

#define BUFFER_SIZE 256 

Info info;

Args InfoRecipricant::func (const Args& args)
{
        Args::const_iterator i = args.begin ();
        if (args.size () == 1) {
                if (*i == "info") {
                        return info.getTable ();
                } else {
                        printf ("info error: got wrong message\n");
                        return Args ();
                }
        }
        Args ret;
        i ++;
        for (; i != args.end (); i ++) {
                if (*i == "tibiaid") {
                        ret.push_back (info.getTibiaId ());
                } else if (*i == "reporterrors") {
                        ret.push_back (info.getReportErrors ());
                } else if (*i == "hp") {
                        ret.push_back (info.getHp ());
                } else if (*i == "hpmax") {
                        ret.push_back (info.getHpMax ());
                } else if (*i == "capacity") {
                        ret.push_back (info.getCapacity ());
                } else if (*i == "experience") {
                        ret.push_back (info.getExperience ());
                } else if (*i == "level") {
                        ret.push_back (info.getLevel ());
                } else if (*i == "levelpercent") {
                        ret.push_back (info.getLevelPercent ());
                } else if (*i == "mana") {
                        ret.push_back (info.getMana ());
                } else if (*i == "manamax") {
                        ret.push_back (info.getManaMax ());
                } else if (*i == "magiclevel") {
                        ret.push_back (info.getMagicLevel ());
                } else if (*i == "magiclevelpercent") {
                        ret.push_back (info.getMagicLevelPercent ());
                } else if (*i == "soul") {
                        ret.push_back (info.getSoul ());
                } else if (*i == "stamina") {
                        ret.push_back (info.getStamina ());
                } else if (*i == "fist") {
                        ret.push_back (info.getFist ());
                } else if (*i == "fistpercent") {
                        ret.push_back (info.getFistPercent ());
                } else if (*i == "club") {
                        ret.push_back (info.getClub ());
                } else if (*i == "clubpercent") {
                        ret.push_back (info.getClubPercent ());
                } else if (*i == "sword") {
                        ret.push_back (info.getSword ());
                } else if (*i == "swordpercent") {
                        ret.push_back (info.getSwordPercent ());
                } else if (*i == "axe") {
                        ret.push_back (info.getAxe ());
                } else if (*i == "axepercent") {
                        ret.push_back (info.getAxePercent ());
                } else if (*i == "distance") {
                        ret.push_back (info.getDistance ());
                } else if (*i == "distancepercent") {
                        ret.push_back (info.getDistancePercent ());
                } else if (*i == "shield") {
                        ret.push_back (info.getShield ());
                } else if (*i == "shieldpercent") {
                        ret.push_back (info.getShieldPercent ());
                } else if (*i == "fishing") {
                        ret.push_back (info.getFishing ());
                } else if (*i == "fishingpercent") {
                        ret.push_back (info.getFishingPercent ());
                } else if (*i == "poison") {
                        ret.push_back (info.getIconPoison ());
                } else if (*i == "burn") {
                        ret.push_back (info.getIconBurn ());
                } else if (*i == "energy") {
                        ret.push_back (info.getIconEnergy ());
                } else if (*i == "drunk") {
                        ret.push_back (info.getIconDrunk ());
                } else if (*i == "manashield") {
                        ret.push_back (info.getIconManaShield ());
                } else if (*i == "paralyze") {
                        ret.push_back (info.getIconParalyze ());
                } else if (*i == "haste") {
                        ret.push_back (info.getIconHaste ());
                } else if (*i == "swords") {
                        ret.push_back (info.getIconSwords ());
                } else if (*i == "drowning") {
                        ret.push_back (info.getIconDrowning ());
                } else if (*i == "freezing") {
                        ret.push_back (info.getIconFreezing ());
                } else if (*i == "dazzled") {
                        ret.push_back (info.getIconDazzled ());
                } else if (*i == "cursed") {
                        ret.push_back (info.getIconCursed ());
                }
        }
        return ret;
}

uint32_t Info::i_calcLvlXp (uint32_t lvl)
{
        uint32_t l2 = lvl * lvl;
        uint32_t l3 = l2 * lvl;
        return (50 * l3 + 850 * lvl) / 3 - 100 * l2 - 200;
}

Args Info::getTable ()
{
        Args ret;
        char tmp[BUFFER_SIZE];

        /* level xp stuff */
        uint32_t lvl = _client->gstate->character->getLevel ();
        uint32_t xp  = _client->gstate->character->getExperience ();
        uint32_t nlvl = i_calcLvlXp (lvl + 1);
        uint32_t clvl = i_calcLvlXp (lvl);
        snprintf (tmp, BUFFER_SIZE, "lvl: %d xp: %d/%d %d%%", 
                lvl, xp, nlvl, 100 - ((xp - clvl) * 100) / (nlvl - clvl));
        ret.push_back (tmp);
        

        return ret;
}

std::string Info::getTibiaId ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getTibiaId ());
        return tmp;
}

std::string Info::getReportErrors ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getReportErrors ());
        return tmp;
}

std::string Info::getHp ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getHp ());
        return tmp;
}

std::string Info::getHpMax ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getHpMax ());
        return tmp;
}

std::string Info::getCapacity ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getCapacity ());
        return tmp;
}

std::string Info::getExperience ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getExperience ());
        return tmp;
}

std::string Info::getLevel ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getLevel ());
        return tmp;
}

std::string Info::getLevelPercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getLevelPercent ());
        return tmp;
}

std::string Info::getMana ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getMana ());
        return tmp;
}

std::string Info::getManaMax ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getManaMax ());
        return tmp;
}

std::string Info::getMagicLevel ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getMagicLevel ());
        return tmp;
}

std::string Info::getMagicLevelPercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getMagicLevelPercent ());
        return tmp;
}

std::string Info::getSoul ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getSoul ());
        return tmp;
}

std::string Info::getStamina ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getStamina ());
        return tmp;
}

std::string Info::getFist ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getFist ());
        return tmp;
}

std::string Info::getFistPercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getFistPercent ());
        return tmp;
}

std::string Info::getClub ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getClub ());
        return tmp;
}

std::string Info::getClubPercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getClubPercent ());
        return tmp;
}

std::string Info::getSword ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getSword ());
        return tmp;
}

std::string Info::getSwordPercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getSwordPercent ());
        return tmp;
}

std::string Info::getAxe ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getAxe ());
        return tmp;
}

std::string Info::getAxePercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getAxePercent ());
        return tmp;
}

std::string Info::getDistance ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getDistance ());
        return tmp;
}

std::string Info::getDistancePercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getDistancePercent ());
        return tmp;
}

std::string Info::getShield ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getShield ());
        return tmp;
}

std::string Info::getShieldPercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getShieldPercent ());
        return tmp;
}

std::string Info::getFishing ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getFishing ());
        return tmp;
}

std::string Info::getFishingPercent ()
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", 
                _client->gstate->character->getFishingPercent ());
        return tmp;
}

std::string Info::getIconPoison ()
{
        if (_client->gstate->character->getIconPoison ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconBurn ()
{
        if (_client->gstate->character->getIconBurn ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconEnergy ()
{
        if (_client->gstate->character->getIconEnergy ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconDrunk ()
{
        if (_client->gstate->character->getIconDrunk ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconManaShield ()
{
        if (_client->gstate->character->getIconManaShield ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconParalyze ()
{
        if (_client->gstate->character->getIconParalyze ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconHaste ()
{
        if (_client->gstate->character->getIconHaste ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconSwords ()
{
        if (_client->gstate->character->getIconSwords ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconDrowning ()
{
        if (_client->gstate->character->getIconDrowning ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconFreezing ()
{
        if (_client->gstate->character->getIconFreezing ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconDazzled ()
{
        if (_client->gstate->character->getIconDazzled ()) {
                return "1";
        } else {
                return "0";
        }
}

std::string Info::getIconCursed ()
{
        if (_client->gstate->character->getIconCursed ()) {
                return "1";
        } else {
                return "0";
        }
}

void Info::iload (uint32_t pluginId, Client* client)
{
        _name = "info";
        _pluginId = pluginId;
        _client = client;
        _rid = _client->addRecipricant (_pluginId, new InfoRecipricant ());
}

void Info::iunload ()
{
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string& Info::iname ()
{
        return _name;
}

void load (uint32_t id, Client* _client)
{
        info.iload (id, _client);
}

void unload ()
{
        info.iunload ();
}

const std::string& name ()
{
        return info.iname ();
}

