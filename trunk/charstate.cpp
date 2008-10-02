#include "charstate.h"
#include "enums.h"

/* Self Info */
uint32_t CharState::getTibiaId ()
{
        return _tibiaId;
}

void CharState::setTibiaId (uint32_t tibiaId)
{
        _tibiaId = tibiaId;
}

bool CharState::getReportErrors ()
{
        return _reportErrors;
}

void CharState::setReportErrors (bool reportErrors)
{
        _reportErrors = reportErrors;
}

/* Player Stats */
uint32_t CharState::getHp ()
{
        return _hp;
}

void CharState::setHp (uint32_t hp)
{
        _hp = hp;
}

uint32_t CharState::getHpMax ()
{
        return _hpmax;
}

void CharState::setHpMax (uint32_t hpmax)
{
        _hpmax = hpmax;
}

uint32_t CharState::getCapacity ()
{
        return _capacity;
}

void CharState::setCapacity (uint32_t capacity)
{
        _capacity = capacity;
}

uint32_t CharState::getExperience ()
{
        return _experience;
}

void CharState::setExperience (uint32_t experience)
{
        _experience = experience;
}

uint32_t CharState::getLevel ()
{
        return _level;
}

void CharState::setLevel (uint32_t level)
{
        _level = level;
}

uint32_t CharState::getLevelPercent ()
{
        return _levelPercent;
}

void CharState::setLevelPercent (uint32_t levelPercent)
{
        _levelPercent = levelPercent;
}

uint32_t CharState::getMana ()
{
        return _mana;
}

void CharState::setMana (uint32_t mana)
{
        _mana = mana;
}

uint32_t CharState::getManaMax ()
{
        return _manamax;
}

void CharState::setManaMax (uint32_t manamax)
{
        _manamax = manamax;
}

uint32_t CharState::getMagicLevel ()
{
        return _magicLevel;
}

void CharState::setMagicLevel (uint32_t magicLevel)
{
        _magicLevel = magicLevel;
}

uint32_t CharState::getMagicLevelPercent ()
{
        return _magicLevelPercent;
}

void CharState::setMagicLevelPercent (uint32_t magicLevelPercent)
{
        _magicLevelPercent = magicLevelPercent;
}

uint32_t CharState::getSoul ()
{
        return _soul;
}

void CharState::setSoul (uint32_t soul)
{
        _soul = soul;
}

uint32_t CharState::getStamina ()
{
        return _stamina;
}

void CharState::setStamina (uint32_t stamina)
{
        _stamina = stamina;
}

/* Player Skills */
uint32_t CharState::getFist ()
{
        return _fist;
}

void CharState::setFist (uint32_t fist)
{
        _fist = fist;
}

uint32_t CharState::getFistPercent ()
{
        return _fistpercent;
}

void CharState::setFistPercent (uint32_t fistpercent)
{
        _fistpercent = fistpercent;
}

uint32_t CharState::getClub ()
{
        return _club;
}

void CharState::setClub (uint32_t club)
{
        _club = club;
}

uint32_t CharState::getClubPercent ()
{
        return _clubpercent;
}

void CharState::setClubPercent (uint32_t clubpercent)
{
        _clubpercent = clubpercent;
}

uint32_t CharState::getSword ()
{
        return _sword;
}

void CharState::setSword (uint32_t sword)
{
        _sword = sword;
}

uint32_t CharState::getSwordPercent ()
{
        return _swordpercent;
}

void CharState::setSwordPercent (uint32_t swordpercent)
{
        _swordpercent = swordpercent;
}

uint32_t CharState::getAxe ()
{
        return _axe;
}

void CharState::setAxe (uint32_t axe)
{
        _axe = axe;
}

uint32_t CharState::getAxePercent ()
{
        return _axepercent;
}

void CharState::setAxePercent (uint32_t axepercent)
{
        _axepercent = axepercent;
}

uint32_t CharState::getDistance ()
{
        return _distance;
}

void CharState::setDistance (uint32_t distance)
{
        _distance = distance;
}

uint32_t CharState::getDistancePercent ()
{
        return _distancepercent;
}

void CharState::setDistancePercent (uint32_t distancepercent)
{
        _distancepercent = distancepercent;
}

uint32_t CharState::getShield ()
{
        return _shield;
}

void CharState::setShield (uint32_t shield)
{
        _shield = shield;
}

uint32_t CharState::getShieldPercent ()
{
        return _shieldpercent;
}

void CharState::setShieldPercent (uint32_t shieldpercent)
{
        _shieldpercent = shieldpercent;
}

uint32_t CharState::getFishing ()
{
        return _fishing;
}

void CharState::setFishing (uint32_t fishing)
{
        _fishing = fishing;
}

uint32_t CharState::getFishingPercent ()
{
        return _fishingpercent;
}

void CharState::setFishingPercent (uint32_t fishingpercent)
{
        _fishingpercent = fishingpercent;
}

/* Player Icons/
 * I don't see why anyone would ever need setIcon functions */
bool CharState::getIconPoison ()
{
        return _icons & ICON_POISON;
}

bool CharState::getIconBurn ()
{
        return _icons & ICON_BURN;
}

bool CharState::getIconEnergy ()
{
        return _icons & ICON_ENERGY;
}

bool CharState::getIconDrunk ()
{
        return _icons & ICON_DRUNK;
}

bool CharState::getIconManaShield ()
{
        return _icons & ICON_MANASHIELD;
}

bool CharState::getIconParalyze ()
{
        return _icons & ICON_PARALYZE;
}

bool CharState::getIconHaste ()
{
        return _icons & ICON_HASTE;
}

bool CharState::getIconSwords ()
{
        return _icons & ICON_SWORDS;
}

bool CharState::getIconDrowning ()
{
        return _icons & ICON_DROWNING;
}

bool CharState::getIconFreezing ()
{
        return _icons & ICON_FREEZING;
}

bool CharState::getIconDazzled ()
{
        return _icons & ICON_DAZZLED;
}

bool CharState::getIconCursed ()
{
        return _icons & ICON_CURSED;
}


void CharState::setIcons (uint32_t icons)
{
        _icons = icons;
}

