#include <stdio.h>
#include "info.h"
#include "enums.h"

#define BUFFER_SIZE 256 

Info info;

int32_t InfoRecipricant::func (const Args& args, Args &out)
{
        Args::const_iterator i = args.begin ();
        if (args.size () == 1) {
                if (*i == "info") {
                        return info.getTable (out);
                } else {
                        printf ("info error: got wrong message\n");
                        return PLUGIN_FAILURE;
                }
        }
        i ++;
        for (; i != args.end (); i ++) {
                if (*i == "tibiaid") {
                        out.push_back (info.getTibiaId ());
                } else if (*i == "reporterrors") {
                        out.push_back (info.getReportErrors ());
                } else if (*i == "hp") {
                        out.push_back (info.getHp ());
                } else if (*i == "hpmax") {
                        out.push_back (info.getHpMax ());
                } else if (*i == "capacity") {
                        out.push_back (info.getCapacity ());
                } else if (*i == "experience") {
                        out.push_back (info.getExperience ());
                } else if (*i == "level") {
                        out.push_back (info.getLevel ());
                } else if (*i == "levelpercent") {
                        out.push_back (info.getLevelPercent ());
                } else if (*i == "mana") {
                        out.push_back (info.getMana ());
                } else if (*i == "manamax") {
                        out.push_back (info.getManaMax ());
                } else if (*i == "magiclevel") {
                        out.push_back (info.getMagicLevel ());
                } else if (*i == "magiclevelpercent") {
                        out.push_back (info.getMagicLevelPercent ());
                } else if (*i == "soul") {
                        out.push_back (info.getSoul ());
                } else if (*i == "stamina") {
                        out.push_back (info.getStamina ());
                } else if (*i == "fist") {
                        out.push_back (info.getFist ());
                } else if (*i == "fistpercent") {
                        out.push_back (info.getFistPercent ());
                } else if (*i == "club") {
                        out.push_back (info.getClub ());
                } else if (*i == "clubpercent") {
                        out.push_back (info.getClubPercent ());
                } else if (*i == "sword") {
                        out.push_back (info.getSword ());
                } else if (*i == "swordpercent") {
                        out.push_back (info.getSwordPercent ());
                } else if (*i == "axe") {
                        out.push_back (info.getAxe ());
                } else if (*i == "axepercent") {
                        out.push_back (info.getAxePercent ());
                } else if (*i == "distance") {
                        out.push_back (info.getDistance ());
                } else if (*i == "distancepercent") {
                        out.push_back (info.getDistancePercent ());
                } else if (*i == "shield") {
                        out.push_back (info.getShield ());
                } else if (*i == "shieldpercent") {
                        out.push_back (info.getShieldPercent ());
                } else if (*i == "fishing") {
                        out.push_back (info.getFishing ());
                } else if (*i == "fishingpercent") {
                        out.push_back (info.getFishingPercent ());
                } else if (*i == "poison") {
                        out.push_back (info.getIconPoison ());
                } else if (*i == "burn") {
                        out.push_back (info.getIconBurn ());
                } else if (*i == "energy") {
                        out.push_back (info.getIconEnergy ());
                } else if (*i == "drunk") {
                        out.push_back (info.getIconDrunk ());
                } else if (*i == "manashield") {
                        out.push_back (info.getIconManaShield ());
                } else if (*i == "paralyze") {
                        out.push_back (info.getIconParalyze ());
                } else if (*i == "haste") {
                        out.push_back (info.getIconHaste ());
                } else if (*i == "swords") {
                        out.push_back (info.getIconSwords ());
                } else if (*i == "drowning") {
                        out.push_back (info.getIconDrowning ());
                } else if (*i == "freezing") {
                        out.push_back (info.getIconFreezing ());
                } else if (*i == "dazzled") {
                        out.push_back (info.getIconDazzled ());
                } else if (*i == "cursed") {
                        out.push_back (info.getIconCursed ());
                }
        }
        return PLUGIN_SUCCESS;
}

uint32_t Info::i_calcLvlXp (uint32_t lvl)
{
        uint32_t l2 = lvl * lvl;
        uint32_t l3 = l2 * lvl;
        return (50 * l3 + 850 * lvl) / 3 - 100 * l2 - 200;
}

int32_t Info::getTable (Args &out)
{
        char tmp[BUFFER_SIZE];

        /* level xp stuff */
        uint32_t lvl = _client->gstate->character->getLevel ();
        uint32_t xp  = _client->gstate->character->getExperience ();
        uint32_t nlvl = i_calcLvlXp (lvl + 1);
        uint32_t clvl = i_calcLvlXp (lvl);
        snprintf (tmp, BUFFER_SIZE, "lvl: %d xp: %d/%d %d%%", 
                lvl, xp, nlvl, 100 - ((xp - clvl) * 100) / (nlvl - clvl));
        out.push_back (tmp);

        return PLUGIN_SUCCESS;
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

