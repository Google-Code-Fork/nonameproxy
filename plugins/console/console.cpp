#include <stdio.h>
#include "console.h"
#include "enums.h"

Console console;

void ChannelOpenHook::func (TibiaMessage* tm, Client* client)
{
        console.hookChannelOpen ((GSMChannelOpen*)tm, client);
}

void ChannelCloseHook::func (TibiaMessage* tm, Client* client)
{
        console.hookChannelClose ((GSMChannelClose*)tm, client);
}

void SpeakHook::func (TibiaMessage* tm, Client* client)
{
        console.hookSpeak ((GSMSpeak*)tm, client);
}

void ChannelRecipricant::func (const Args& args)
{
        Args::const_iterator i = args.begin ();
        if (args.size () < 2) {
                return;
        }
        if ((*i) != "console") {
                return;
        }
        i ++;
        console.output (*i);
}

void Console::output (const std::string& msg)
{
        if (_open) {
                GRMessageList grml;
                TChannelSpeak out ((uint32_t)0,
                                   std::string("#"), 
                                   (uint16_t)0, 
                                   SPEAK_CHANNEL_O,
                                   (uint16_t)_channelId,
                                   msg);

                grml.add (new GRMSpeak (out));
                _client->sendToClient (grml);
        }
}

void Console::hookChannelOpen (GSMChannelOpen* co, Client* client)
{
        uint32_t channelId = co->getChannelId ();
        if (channelId == _channelId) {
                GRMessageList grml;
                grml.add (new GRMChannelOpen (_channelId, "console"));
                _client->sendToClient (grml);
                _open = true;
        }
}

void Console::hookChannelClose (GSMChannelClose* cc, Client* client)
{
        uint32_t channelId = cc->getChannelId ();
        if (channelId == _channelId) {
                _open = false;
        }
}

void Console::hookSpeak (GSMSpeak* sp, Client* client)
{
        if (sp->getSpeakType () == GSMSpeak::channel) {
                uint32_t channelId = sp->getChannelId ();
                if (channelId == _channelId) {
                        printf ("%s\n", sp->getMsg ().c_str ());
                }
        }
}

void Console::iload (uint32_t pluginId, Client* client)
{
        _name = "console";
        _pluginId = pluginId;
        _client = client;

        _open = false;
        _channelId = 1234;

        _channelopen_hid = _client->addSendReadHook (_pluginId, 
                                GSM_CHANNEL_OPEN_ID, new ChannelOpenHook ());

        _channelclose_hid = _client->addSendReadHook (_pluginId, 
                                GSM_CHANNEL_CLOSE_ID, new ChannelOpenHook ());

        _speak_hid = _client->addSendReadHook (_pluginId, 
                                GSM_SPEAK_ID, new SpeakHook ());

        _rid = _client->addRecipricant (_pluginId, new ChannelRecipricant ());
}

void Console::iunload ()
{
        _client->deleteSendWriteHook (_pluginId, _channelopen_hid);
        _client->deleteSendWriteHook (_pluginId, _channelclose_hid);
        _client->deleteSendWriteHook (_pluginId, _speak_hid);
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string& Console::iname ()
{
        return _name;
}

void load (uint32_t id, Client* client)
{
        console.iload (id, client);
}

void unload ()
{
        console.iunload ();
}

const std::string& name ()
{
        return console.iname ();
}

