#ifndef __WATCH
#define __WATCH

#include <string>
#include <set>
#include <stdint.h>

#include "tibiamessage.h"
#include "networkmessage.h"
#include "hook.h"
#include "packethook.h"
#include "client.h"

#define SEND 0
#define RECV 1

/**************************************************************************
 * Watch executes a command if a condition is true. condition is a string
 * which when executed, if it returns a 1 then the command will be issued
 * otherwise nothing happens
 * The test is only exectuted when one of the message ids are recved.
 *
 * You cannot set a watch on two messages from both protocols (send/recv)
 * If you need this functionality, this is free software, so add it
 *
 * Usage:
 *      watch 'condition' 'command' [send|recv] messageid [messageid2 ...]
 **************************************************************************/

class WEntry;

class WatchRecipricant : public Recipricant
{
        public:
                virtual Args func (const Args& args);
};

class WatchHook : public ReadHook
{
        public:
                WatchHook (WEntry* entry);

                virtual void func (TibiaMessage* tm, Client* client);

        private:
                WEntry* _entry;
};


typedef std::set<uint32_t> MidSet;
typedef std::set<uint32_t> HidSet;

class WEntry
{
        public:
                WEntry (const std::string& test, 
                        const std::string& command,
                        uint32_t type
                       );
                virtual ~WEntry () {}

                std::string show ();

                bool newCycle (uint32_t cycle);

                uint32_t getType ();

                bool enable ();
                bool disable ();
                bool isEnabled ();

                const std::string& getTest ();
                const std::string& getCommand ();

                void addMid (uint32_t mid);
                const MidSet& getMids ();

                void addHid (uint32_t hid);
                const HidSet& getHids ();


        private:
                uint32_t        _cycle;
                uint32_t        _type;
                bool            _enabled;

                std::string     _test;
                std::string     _command;

                MidSet          _mids;
                HidSet          _hids;
};

typedef std::vector<WEntry*> WEntryVector;
class Watch
{
        public:
                Args usage ();

                std::string list (uint32_t n);
                Args listAll ();

                std::string addEntry (WEntry* entry);

                std::string removeEntry (uint32_t n);
                std::string enableEntry (uint32_t n);
                std::string disableEntry (uint32_t n);

                std::string removeAll ();
                std::string enableAll ();
                std::string disableAll ();

                uint32_t getCycle ();

                void i_load (uint32_t pluginId, Client* client);
                void i_unload ();
                const std::string& i_name ();

        private:
                WEntryVector    _entries;

                uint32_t        _pluginId;
                uint32_t        _rid;

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

