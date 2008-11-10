#ifndef __CHANNEL_MANAGER_H
#define __CHANNEL_MANAGER_H

#include <string>
#include <set>
#include <map>
#include <stdint.h>

#include "tibiamessage.h"
#include "hook.h"
#include "client.h"

/**************************************************************************
 * channel contains a  recv write hook which waits for the List Channels 
 * Message. When this message is recieved, it will alter it according to a 
 * add list of channels to add, and a black list of channels to block. 
 *      channelmanager add channelname channelid
 *      channelmanager remove channelid
 *      channelmanager blacklist channelid
 *      channelmanager whitelist channelid
 * After a channel has been added, several other write hooks blocks all
 * messages sent to the server which use the added channel id.
 * These Include:
 *      GSMChannelOpen
 *      GSMChannelClose
 *      GSMSpeak
 * Remember that a read hook will still be able to respond to these 
 * messages
 *
 * Note: Write hooks are placed on
 * Recv:
 *      GRMChannelList
 * Send:
 *      GSMChannelOpen
 *      GSMChannelClose
 *      GSMSpeak
 **************************************************************************/

class ChannelListHook : public WriteHook
{
        virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

class ChannelOpenHook : public WriteHook
{
        virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

class ChannelCloseHook : public WriteHook
{
        virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

class SpeakHook : public WriteHook
{
        virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

class ChannelRecipricant : public Recipricant
{
        virtual Args func (const Args& args);
};

typedef std::map<uint32_t, std::string> ChannelMap;
typedef std::set<uint32_t> ChannelSet;

class ChannelManager
{
        public:
                void addChannel (const std::string& name, uint32_t id);
                void removeChannel (uint32_t id);

                void blackListChannel (uint32_t id);
                void whiteListChannel (uint32_t id);

                GRMChannelList*  hookChannelList (GRMChannelList* cl, Client* client);

                GSMChannelOpen*  hookChannelOpen (GSMChannelOpen* co, Client* client);
                GSMChannelClose* hookChannelClose (GSMChannelClose* cc, Client* client);
                GSMSpeak*        hookSpeak (GSMSpeak* co, Client* client);

                void iload (uint32_t pluginId, Client* client);
                void iunload ();
                const std::string& iname ();

        private:
                uint32_t _pluginId;
                uint32_t _channellist_hid;
                uint32_t _channelopen_hid;
                uint32_t _channelclose_hid;
                uint32_t _speak_hid;
                uint32_t _rid;
                Client*  _client;
                std::string _name;

                ChannelMap addList;
                ChannelSet blackList;
};

extern "C"
{
        void load (uint32_t id, Client* client);
        void unload ();
        const std::string& name ();
}

#endif

