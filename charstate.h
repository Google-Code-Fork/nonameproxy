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

#ifndef __CHAR_STATE_H
#define __CHAR_STATE_H

#include <stdint.h>

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

                uint32_t getManaMax ();
                void     setManaMax (uint32_t manamax);

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

                uint32_t getFistPercent ();
                void     setFistPercent (uint32_t fistpercent);

                uint32_t getClub ();
                void     setClub (uint32_t club);

                uint32_t getClubPercent ();
                void     setClubPercent (uint32_t clubpercent);

                uint32_t getSword ();
                void     setSword (uint32_t sword);

                uint32_t getSwordPercent ();
                void     setSwordPercent (uint32_t swordpercent);

                uint32_t getAxe ();
                void     setAxe (uint32_t axe);

                uint32_t getAxePercent ();
                void     setAxePercent (uint32_t axepercent);

                uint32_t getDistance ();
                void     setDistance (uint32_t distance);

                uint32_t getDistancePercent ();
                void     setDistancePercent (uint32_t distancepercent);

                uint32_t getShield ();
                void     setShield (uint32_t shield);

                uint32_t getShieldPercent ();
                void     setShieldPercent (uint32_t shieldpercent);

                uint32_t getFishing ();
                void     setFishing (uint32_t fishing);

                uint32_t getFishingPercent ();
                void     setFishingPercent (uint32_t fishingpercent);

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
                uint32_t _tibiaId;
                bool     _reportErrors;

                uint32_t _hp;
                uint32_t _hpmax;
                uint32_t _capacity;
                uint32_t _experience;
                uint32_t _level;
                uint32_t _levelPercent;
                uint32_t _mana;
                uint32_t _manamax;
                uint32_t _magicLevel;
                uint32_t _magicLevelPercent;
                uint32_t _soul;
                uint32_t _stamina;

                uint32_t _fist;
                uint32_t _fistpercent;
                uint32_t _club;
                uint32_t _clubpercent;
                uint32_t _sword;
                uint32_t _swordpercent;
                uint32_t _axe;
                uint32_t _axepercent;
                uint32_t _distance;
                uint32_t _distancepercent;
                uint32_t _shield;
                uint32_t _shieldpercent;
                uint32_t _fishing;
                uint32_t _fishingpercent;

                uint32_t _icons;
};

#endif

