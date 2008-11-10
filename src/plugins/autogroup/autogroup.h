#ifndef __CHANNEL_MANAGER_H
#define __CHANNEL_MANAGER_H

#include <string>
#include <set>
#include <stdint.h>

#include "tibiamessage.h"
#include "networkmessage.h"
#include "hook.h"
#include "packethook.h"
#include "client.h"

/**************************************************************************
 * AutoGroup is an awesome feature from TibiaAuto which automatically
 * groups items
 *
 * Usage:
 *      autogroup list                          list all open backpacks
 *      autogroup ids                           list added Cids
 *      autogroup add x y z                     add x y z to the Cids
 *      autogroup remove x y z                  remove x y z from the Cids
 *      autogroup all                           add all ids to the Cids
 *      autogroup none                          remove all ids from the Cids
 **************************************************************************/

class AutoGroupRecipricant : public Recipricant
{
        public:
                virtual Args func (const Args& args);
};

class ContainerAddHook : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class ContainerOpenHook : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

class ContainerUpdateHook : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

typedef std::set<uint32_t> CidSet;

class AutoGroup
{
        public:
                Args usage ();
                Args list_ids ();
                Args list_containers ();

                std::string all_ids ();
                std::string no_ids ();
                std::string add_id (const std::string& cid);
                std::string remove_id (const std::string& cid);

                bool isSet (uint32_t cid);
                bool groupContainer (const Container& c, uint32_t cid);
                bool groupItem (const Container& c,
                                uint32_t         cid,
                                const XItem&     x,
                                uint32_t         i);

                void i_load (uint32_t pluginId, Client* client);
                void i_unload ();
                const std::string& i_name ();

                /* returns true if the container has not yet
                 * been modified this cycle */
                bool newContainerCycle (uint32_t cid);

        private:
                void i_moveItem (uint32_t cid, 
                                 uint32_t itemid,
                                 uint32_t from,
                                 uint32_t to,
                                 uint32_t count);

                CidSet          _cids;

                uint32_t        _cycles[MAX_CONTAINER];

                uint32_t        _pluginId;
                uint32_t        _rid;

                uint32_t        _addhookid;
                uint32_t        _openhookid;
                uint32_t        _updatehookid;

                Client*         _client;

                std::string     _name;
};

extern "C"
{
        void load (uint32_t id, Client* client);
        void unload ();
        const std::string& name ();
}

#endif

