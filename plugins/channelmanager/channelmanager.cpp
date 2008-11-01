#include <stdio.h>
#include "channelmanager.h"
#include "enums.h"

ChannelManager chanMgr;

TibiaMessage* ChannelListHook::func (TibiaMessage* tm, Client* client)
{
        return chanMgr.hookChannelList ((GRMChannelList*)tm, client);
}

TibiaMessage* ChannelOpenHook::func (TibiaMessage* tm, Client* client)
{
        return chanMgr.hookChannelOpen ((GSMChannelOpen*)tm, client);
}

TibiaMessage* ChannelCloseHook::func (TibiaMessage* tm, Client* client)
{
        return chanMgr.hookChannelClose ((GSMChannelClose*)tm, client);
}

TibiaMessage* SpeakHook::func (TibiaMessage* tm, Client* client)
{
        return chanMgr.hookSpeak ((GSMSpeak*)tm, client);
}

Args ChannelRecipricant::func (const Args& args)
{
        Args ret;
        Args::const_iterator i = args.begin ();
        if (args.size () < 2) {
                ret.push_back ("channelmanager: usage");
                ret.push_back ("channelmanager add channelname channelid");
                ret.push_back ("channelmanager remove channelid");
                ret.push_back ("channelmanager blacklist channelid");
                ret.push_back ("channelmanager whitelist channelid");
                return ret;
        }
        if ((*i) != "channelmanager") {
                return ret;
        }
        i ++;
        if ((*i) == "add") {
                if (args.size () != 4) {
                        ret.push_back ("channelmanager: add takes 4 args");
                        return ret;
                }
                i ++;
                const std::string& name = *i;
                i ++;
                uint32_t id = atoi ((*i).c_str ());
                if (id > 0xFFFF) {
                        ret.push_back ("channelmanager: warning id out of range");
                        id %= 0xFFFF;
                }
                chanMgr.addChannel (name, id);
                ret.push_back ("channelmanager: channel " + *i + " added");
        } else if ((*i) == "remove") {
                if (args.size () != 3) {
                        ret.push_back ("channelmanager: remove takes 3 args");
                        return ret;
                }
                i ++;
                uint32_t id = atoi ((*i).c_str ());
                if (id > 0xFFFF) {
                        ret.push_back ("channelmanager: warning id out of range");
                        id %= 0xFFFF;
                }
                chanMgr.removeChannel (id);
                ret.push_back ("channelmanager: channel " + *i + " removed");
        } else if ((*i) == "blacklist") {
                if (args.size () != 3) {
                        ret.push_back ("channel manager: blacklist takes 3 args");
                        return ret;
                }
                i ++;
                uint32_t id = atoi ((*i).c_str ());
                if (id > 0xFFFF) {
                        ret.push_back ("channelmanager: warning id out of range\n");
                        id %= 0xFFFF;
                }
                chanMgr.blackListChannel (id);
                ret.push_back ("channelmanager: channel " + *i + " blacklisted");
        } else if ((*i) == "whitelist") {
                if (args.size () != 3) {
                        ret.push_back ("channelmanager: whitelist takes 4 args");
                        return ret;
                }
                i ++;
                uint32_t id = atoi ((*i).c_str ());
                if (id > 0xFFFF) {
                        ret.push_back ("channelmanager: warning channel id out of range\n");
                        id %= 0xFFFF;
                }
                chanMgr.whiteListChannel (id);
                ret.push_back ("channelmanager: channel " + *i + " whitelisted");
        } else {
                ret.push_back ("channelmanager: unreconized command " + *i);
        }
        return ret;
}

void ChannelManager::addChannel (const std::string& name, uint32_t id)
{
        if (addList.count (id) != 0) {
                printf ("channelmanager add error: channel already added\n");
                return;
        }
        addList.insert (std::pair<uint32_t, std::string> (id, name));
}

void ChannelManager::removeChannel (uint32_t id)
{
        if (addList.erase (id) == 0) {
                printf ("channelmanager remove error: channel not added\n");
        }
}

void ChannelManager::blackListChannel (uint32_t id)
{
        if (addList.count (id) != 0) {
                printf ("channelmanager blacklist error: may not blacklist ");
                printf ("channels added by the manager\n");
                return;
        }
        std::pair<ChannelSet::iterator, bool> ret = blackList.insert (id);
        if (ret.second == false) {
                printf ("channelmanager blacklist error: channel already ");
                printf ("blacklisted\n");
        }
}

void ChannelManager::whiteListChannel (uint32_t id)
{
        if (blackList.erase (id) == 0) {
                printf ("channelmanager whitelist error: channel not blacklisted\n");
        }
}

GSMChannelOpen* ChannelManager::hookChannelOpen (GSMChannelOpen* co, Client* client)
{
        uint32_t channelId = co->getChannelId ();
        if (addList.count (channelId) != 0) {
                delete co;
                return NULL;
        }
        return co;
}

GSMChannelClose* ChannelManager::hookChannelClose (GSMChannelClose* cc, Client* client)
{
        uint32_t channelId = cc->getChannelId ();
        if (addList.count (channelId) != 0) {
                delete cc;
                return NULL;
        }
        return cc;
}

GSMSpeak* ChannelManager::hookSpeak (GSMSpeak* sp, Client* client)
{
        if (sp->getSpeakType () == GSMSpeak::channel) {
                uint32_t channelId = sp->getChannelId ();
                if (addList.count (channelId) != 0) {
                        delete sp;
                        return NULL;
                }
        }
        return sp;
}

GRMChannelList* ChannelManager::hookChannelList (GRMChannelList* cl, Client* client)
{
        /* copy the old channel list */
        TChannelList* channels = new TChannelList (cl->getChannelList ());
        channels->begin ();
        while (!channels->isEnd ()) {
                uint32_t channelId = channels->getChannel ().getChannelId ();
                if (blackList.count (channelId) != 0) {
                        channels->remove ();
                }
                channels->next ();
        }
        ChannelMap::iterator i;
        for (i = addList.begin (); i != addList.end (); i ++) {
                channels->add (new TChannel ((*i).first, (*i).second));
        }
        delete cl;
        return new GRMChannelList (channels);
}

void ChannelManager::iload (uint32_t pluginId, Client* client)
{
        _name = "channelmanager";
        _pluginId = pluginId;
        _client = client;

        _channellist_hid = _client->addRecvWriteHook (_pluginId, 
                                GRM_CHANNEL_LIST_ID, new ChannelListHook ());

        _channelopen_hid = _client->addSendWriteHook (_pluginId, 
                                GSM_CHANNEL_OPEN_ID, new ChannelOpenHook ());

        _channelclose_hid = _client->addSendWriteHook (_pluginId, 
                                GSM_CHANNEL_CLOSE_ID, new ChannelCloseHook ());

        _speak_hid = _client->addSendWriteHook (_pluginId, 
                                GSM_SPEAK_ID, new SpeakHook ());

        _rid = _client->addRecipricant (_pluginId, new ChannelRecipricant ());
}

void ChannelManager::iunload ()
{
        _client->deleteRecvWriteHook (_pluginId, _channellist_hid);
        _client->deleteSendWriteHook (_pluginId, _channelopen_hid);
        _client->deleteSendWriteHook (_pluginId, _channelclose_hid);
        _client->deleteSendWriteHook (_pluginId, _speak_hid);
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string& ChannelManager::iname ()
{
        return _name;
}

void load (uint32_t id, Client* client)
{
        chanMgr.iload (id, client);
}

void unload ()
{
        chanMgr.iunload ();
}

const std::string& name ()
{
        return chanMgr.iname ();
}

