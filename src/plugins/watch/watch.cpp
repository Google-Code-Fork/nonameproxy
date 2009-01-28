#include <stdio.h>
#include "watch.h"
#include "enums.h"

#define BUFFER_SIZE 256 

Watch watch;

int32_t WatchRecipricant::func (const Args& args, Args &out)
{
        Args::const_iterator i = args.begin ();
        if (args.size () == 0) {
                printf ("watch error: got emtpy message\n");
                return PLUGIN_FAILURE;
        }
        if (args.size () == 1) {
                if (*i == "watch") {
                        return watch.usage (out);
                } else {
                        printf ("watch error: got wrong message\n");
                        return PLUGIN_FAILURE;
                }
        }
        i ++;

        if (*i == "list") {
                i ++;
                if (args.size () == 2) {
                        return watch.listAll (out);
                } else {
                        for (; i != args.end (); ++ i) {
                                char* end;
                                uint32_t id = strtol ((*i).c_str (), &end, 0);
                                if (*end == '\0') {
                                        out.push_back (watch.list (id));
                                }
                        }
                }
        } else if (*i == "remove" && args.size () > 2) {
                i ++;
                if (*i == "all") {
                        out.push_back (watch.removeAll ());
                } else {
                        /* if we remove the ids in a random order the array
                         * size will change and ids will also change. to
                         * avoid this we sort the ids in reverse order and
                         * then remove them */
                        std::list<uint32_t> ids;
                        for (; i != args.end (); ++ i) {
                                char* end;
                                uint32_t id = strtol ((*i).c_str (), &end, 0);
                                if (*end == '\0') {
                                        ids.push_back (id);
                                }
                        }
                        ids.sort ();
                        ids.reverse ();
                        std::list<uint32_t>::iterator id;
                        for (id = ids.begin (); id != ids.end (); ++ id) {
                                out.push_back (watch.removeEntry (*id));
                        }
                }
        } else if (*i == "enable" && args.size () > 2) {
                i ++;
                if (*i == "all") {
                        out.push_back (watch.enableAll ());
                } else {
                        for (; i != args.end (); ++ i) {
                                char* end;
                                uint32_t id = strtol ((*i).c_str (), &end, 0);
                                if (*end == '\0') {
                                        out.push_back (watch.enableEntry (id));
                                }
                        }
                }
        } else if (*i == "disable" && args.size () > 2) {
                i ++;
                if (*i == "all") {
                        out.push_back (watch.disableAll ());
                } else {
                        for (; i != args.end (); ++ i) {
                                char* end;
                                uint32_t id = strtol ((*i).c_str (), &end, 0);
                                if (*end == '\0') {
                                        out.push_back (watch.disableEntry (id));
                                }
                        }
                }
        } else if (args.size () > 4) {
                /* if we get this far we assume we have a new test */
                const std::string& test = *i;
                i ++;
                const std::string& command = *i;
                i ++;
                uint32_t type;
                if (*i == "send") {
                        type = SEND;
                } else if (*i == "recv") {
                        type = RECV;
                } else {
                        return watch.usage (out);
                }

                i ++;
                        
                WEntry* entry = new WEntry (test, command, type);
                for (; i != args.end (); ++ i) {
                        char* end;
                        uint32_t mid = strtol ((*i).c_str (), &end, 0);
                        if (*end == '\0') {
                                entry->addMid (mid);
                        }
                }
                out.push_back (watch.addEntry (entry));
        }
        return PLUGIN_SUCCESS;
}

WatchHook::WatchHook (WEntry* entry)
{
        _entry = entry;
}

void WatchHook::func (TibiaMessage* tm, Client* client)
{
        if (!_entry->isEnabled ()) {
                return;
        }
        
        int32_t exit_status = client->broadcastMessage (_entry->getTest ());

        if (exit_status == 0) {
                client->broadcastMessage (_entry->getCommand ());
        }
}

WEntry::WEntry (const std::string& test,
                const std::string& command,
                uint32_t type)
{
        _test           = test;
        _command        = command;
        _type           = type;
        _cycle          = watch.getCycle ();
        _enabled        = true;
}

std::string WEntry::show ()
{
        std::string ret = _test + " " + _command;
        if (_type == SEND) {
                ret += " send";
        } else {
                ret += " recv";
        }

        char tmp[BUFFER_SIZE];
        for (MidSet::iterator i = _mids.begin (); i != _mids.end (); ++ i) {
                snprintf (tmp, BUFFER_SIZE, "0x%X", *i);
                ret += " ";
                ret += tmp;
        }

        if (_enabled) {
                ret += " enabled";
        } else {
                ret += " disabled";
        }
        return ret;
}

bool WEntry::newCycle (uint32_t cycle)
{
        if (_cycle != cycle) {
                _cycle = cycle;
                return true;
        } else {
                return false;
        }
}

uint32_t WEntry::getType ()
{
        return _type;
}

bool WEntry::enable ()
{
        if (_enabled) {
                return false;
        }
        _enabled = true;
        return true;
}

bool WEntry::disable ()
{
        if (!_enabled) {
                return false;
        }
        _enabled = false;
        return true;
}

bool WEntry::isEnabled ()
{
        return _enabled;
}

const std::string& WEntry::getTest ()
{
        return _test;
}

const std::string& WEntry::getCommand ()
{
        return _command;
}

void WEntry::addMid (uint32_t mid)
{
        _mids.insert (mid);
}
        
const MidSet& WEntry::getMids ()
{
        return _mids;
}
        
void WEntry::addHid (uint32_t hid)
{
        _hids.insert (hid);
}
        
const HidSet& WEntry::getHids ()
{
        return _hids;
}

int32_t Watch::usage (Args &out)
{
        out.push_back ("watch list");
        out.push_back ("watch list id [id2 id3 ...]");
        out.push_back ("watch 'test' 'command' [send|recv] mid [mid2 mid3 ...]");
        out.push_back ("watch remove [all|id [id2 id3 ...]");
        out.push_back ("watch enable [all|id [id2 id3 ...]");
        out.push_back ("watch disable [all|id [id2 id3 ...]");
        return PLUGIN_FAILURE;
}

int32_t Watch::listAll (Args &out)
{
        Args ret;
        char tmp[BUFFER_SIZE];
        uint32_t index = 0;

        WEntryVector::iterator i;
        for (i = _entries.begin (); i != _entries.end (); ++ i) {
                snprintf (tmp, BUFFER_SIZE, "%d: ", index);
                out.push_back (std::string(tmp) + (*i)->show ());
                index ++;
        }
        return PLUGIN_SUCCESS;
}

std::string Watch::list (uint32_t n)
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", n);

        if (n >= _entries.size ()) {
                return std::string (tmp) + " does not exist\n";
        }
        return std::string(tmp) + ": " + _entries[n]->show ();
}

std::string Watch::enableAll ()
{
        WEntryVector::iterator i;
        for (i = _entries.begin (); i != _entries.end (); ++ i) {
                (*i)->enable ();
        }
        return "watch: all entries enabled";
}

std::string Watch::enableEntry (uint32_t n)
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", n);

        if (n >= _entries.size ()) {
                return std::string (tmp) + " does not exist\n";
        }
        _entries[n]->enable ();
        return std::string(tmp) + " enabled";
}

std::string Watch::disableAll ()
{
        WEntryVector::iterator i;
        for (i = _entries.begin (); i != _entries.end (); ++ i) {
                (*i)->disable ();
        }
        return "watch: all entries disabled";
}

std::string Watch::disableEntry (uint32_t n)
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", n);

        if (n >= _entries.size ()) {
                return std::string (tmp) + " does not exist\n";
        }
        _entries[n]->disable ();
        return std::string(tmp) + " disabled";
}

std::string Watch::removeAll ()
{
        WEntryVector::iterator i;
        for (i = _entries.begin (); i != _entries.end (); ++ i) {
                uint32_t type = (*i)->getType ();
                const HidSet& hids = (*i)->getHids ();

                HidSet::iterator hid;
                for (hid = hids.begin (); hid != hids.end (); ++ hid) {
                        if (type == RECV) {
                                _client->deleteRecvReadHook (_pluginId, *hid);
                        } else {
                                _client->deleteSendReadHook (_pluginId, *hid);
                        }
                }
                delete (*i);
        }

        _entries.clear ();
        
        return "watch: all entries removed";
}

std::string Watch::removeEntry (uint32_t n)
{
        char tmp[BUFFER_SIZE];
        snprintf (tmp, BUFFER_SIZE, "%d", n);

        if (n >= _entries.size ()) {
                return std::string (tmp) + " does not exist\n";
        }

        WEntry* entry = _entries[n];

        uint32_t type = entry->getType ();
        const HidSet& hids = entry->getHids ();

        for (HidSet::iterator i = hids.begin (); i != hids.end (); ++ i) {
                if (type == RECV) {
                        _client->deleteRecvReadHook (_pluginId, *i);
                } else {
                        _client->deleteSendReadHook (_pluginId, *i);
                }
        }
        
        delete entry;
        _entries.erase (_entries.begin () + n);

        return std::string(tmp) + " removed";
}

std::string Watch::addEntry (WEntry* entry)
{
        const MidSet& mids = entry->getMids ();
        if (mids.size () == 0) {
                return "error: entry must have atleast one message id";
        }
        for (MidSet::iterator i = mids.begin (); i != mids.end (); ++ i) {
                uint32_t hid;
                if (entry->getType () == RECV) {
                        hid = _client->addRecvReadHook (_pluginId, *i,
                                                        new WatchHook (entry));
                } else {
                        hid = _client->addSendReadHook (_pluginId, *i,
                                                        new WatchHook (entry));
                }
                entry->addHid (hid);
        }
        _entries.push_back (entry);
        return "added";
}

uint32_t Watch::getCycle ()
{
        return _client->getCycle ();
}
                
void Watch::i_load (uint32_t pluginId, Client* client)
{
        _name = "watch";
        _pluginId = pluginId;
        _client = client;

        _rid = _client->addRecipricant (_pluginId, new WatchRecipricant ());
}

void Watch::i_unload ()
{
        removeAll ();
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string& Watch::i_name ()
{
        return _name;
}

void load (uint32_t id, Client* _client)
{
        watch.i_load (id, _client);
}

void unload ()
{
        watch.i_unload ();
}

const std::string& name ()
{
        return watch.i_name ();
}

