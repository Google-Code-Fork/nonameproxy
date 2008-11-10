#ifndef __INFO
#define __INFO

#include <string>
#include <stdint.h>

#include "tibiamessage.h"
#include "networkmessage.h"
#include "hook.h"
#include "packethook.h"
#include "client.h"

/**************************************************************************
 * Info gives the user a way to view player stats in the console. It is
 * similar to debug, but info returns the requested values, so it could
 * also be used by other plugins. Also it prints to the console, not stdout
 *
 * Usage:
 *      info                            print nice table
 *      info field                      print single value
 **************************************************************************/

class InfoRecipricant : public Recipricant
{
        public:
                virtual Args func (const Args& args);
};

class Info
{
        public:

                Args usage ();

                Args getTable ();
                std::string getTibiaId ();
                std::string getReportErrors ();
                std::string getHp ();
                std::string getHpMax ();
                std::string getCapacity ();
                std::string getExperience ();
                std::string getLevel ();
                std::string getLevelPercent ();
                std::string getMana ();
                std::string getManaMax ();
                std::string getMagicLevel ();
                std::string getMagicLevelPercent ();
                std::string getSoul ();
                std::string getStamina ();
                std::string getFist ();
                std::string getFistPercent ();
                std::string getClub ();
                std::string getClubPercent ();
                std::string getSword ();
                std::string getSwordPercent ();
                std::string getAxe ();
                std::string getAxePercent ();
                std::string getDistance ();
                std::string getDistancePercent ();
                std::string getShield ();
                std::string getShieldPercent ();
                std::string getFishing ();
                std::string getFishingPercent ();
                std::string getIconPoison ();
                std::string getIconBurn ();
                std::string getIconEnergy ();
                std::string getIconDrunk ();
                std::string getIconManaShield ();
                std::string getIconParalyze ();
                std::string getIconHaste ();
                std::string getIconSwords ();
                std::string getIconDrowning ();
                std::string getIconFreezing ();
                std::string getIconDazzled ();
                std::string getIconCursed ();

                void iload (uint32_t pluginId, Client* client);
                void iunload ();
                const std::string& iname ();

        private:
                uint32_t i_calcLvlXp (uint32_t lvl);
                uint32_t i_calcSkillXp (uint32_t lvl);


                uint32_t _pluginId;
                Client*  _client;

                uint32_t _rid;

                std::string _name;
};

extern "C"
{
        void load (uint32_t id, Client* client);
        void unload ();
        const std::string& name ();
}

#endif

