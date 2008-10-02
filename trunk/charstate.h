#ifndef __CHAR_STATE_H
#define __CHAR_STATE_H

#include <string>

class CharState
{
        public:
                /* Self Info */
                uint32_t getTibiaId ();
                void     setTibiaId (uint32_t tibiaId);

                bool     getReportErrors ();
                void     setReportErrors (bool reportErrors);

                /* Player Stats */
                uint32_t getHp ();
                void     setHp (uint32_t hp);

                uint32_t getHpMax ();
                void     setHpMax (uint32_t hpmax);

                uint32_t getCapacity ();
                void     setCapacity (uint32_t capacity);

                uint32_t getExperience ();
                void     setExperience (uint32_t experience);

                uint32_t getLevel ();
                void     setLevel (uint32_t level);

                uint32_t getLevelPercent ();
                void     setLevelPercent (uint32_t levelPercent);

                uint32_t getMana ();
                void     setMana (uint32_t mana);

                uint32_t getMaxMana ();
                void     setMaxMana (uint32_t maxmana);

                uint32_t getMagicLevel ();
                void     setMagicLevel (uint32_t magicLevel);

                uint32_t getMagicLevelPercent ();
                void     setMagicLevelPercent (uint32_t magicLevelPercent);

                uint32_t getSoul ();
                void     setSoul (uint32_t soul);

                uint32_t getStamina ();
                void     setStamina (uint32_t stamina);

                /* Player Skills */
                uint32_t getFist ();
                void     setFist (uint32_t fist);

                uint32_t getClub ();
                void     setClub (uint32_t club);

                uint32_t getSword ();
                void     setSword (uint32_t sword);

                uint32_t getAxe ();
                void     setAxe (uint32_t axe);

                uint32_t getDistance ();
                void     setDistance (uint32_t distance);

                uint32_t getShield ();
                void     setShield (uint32_t shield);

                uint32_t getFishing ();
                void     setFishing (uint32_t fishing);

                /* Player Icons/
                 * I don't see why anyone would ever need setIcon functions */
                bool getIconPoison ();
                bool getIconBurn ();
                bool getIconEnergy ();
                bool getIconDrunk ();
                bool getIconManaShield ();
                bool getIconParalyze ();
                bool getIconHaste ();
                bool getIconSwords ();
                bool getIconDrowning ();
                bool getIconFreezing ();
                bool getIconDazzled ();
                bool getIconCursed ();

                void setIcons (uint32_t icons);

        private:
                uint32_t _hp;
                uint32_t _hpmax;
                uint32_t _capacity;
                uint32_t _experience;
                uint32_t _level;
                uint32_t _levelPercent;
                uint32_t _mana;
                uint32_t _maxmana;
                uint32_t _magicLevel;
                uint32_t _magicLevelPercent;
                uint32_t _soul;
                uint32_t _stamina;

                uint32_t _fist;
                uint32_t _club;
                uint32_t _sword;
                uint32_t _axe;
                uint32_t _distance;
                uint32_t _shield;
                uint32_t _fishing;

                uint32_t _icons;
}

