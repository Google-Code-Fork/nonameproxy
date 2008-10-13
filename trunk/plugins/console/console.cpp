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
                GRMSpeak* out = new GRMSpeak ((uint32_t)0,
                                              ">", 
                                              (uint16_t)0, 
                                              SPEAK_CHANNEL_O,
                                              (uint16_t)_channelId,
                                              msg);

                grml.add (out);
                _client->sendToClient (grml);
        }
}

void Console::hookChannelOpen (GSMChannelOpen* co, Client* client)
{
        printf ("open console\n");
        uint32_t channelId = co->getChannelId ();
        if (channelId == _channelId) {
                printf ("ids match\n");
                GRMessageList grml;
                grml.add (new GRMChannelOpen (_channelId, "console"));
                _client->sendToClient (grml);
                printf ("console open sent\n");
                _open = true;
        }
}

void Console::hookChannelClose (GSMChannelClose* cc, Client* client)
{
        uint32_t channelId = cc->getChannelId ();
        if (channelId == _channelId) {
                printf ("console close\n");
                _open = false;
        }
}

void Console::hookSpeak (GSMSpeak* sp, Client* client)
{
        if (sp->getSpeakType () == GSMSpeak::channel) {
                uint32_t channelId = sp->getChannelId ();
                if (channelId == _channelId) {
                        GRMessageList grml;
                        const std::string& msg = sp->getMsg ();
                        GRMSpeak* out = new GRMSpeak ((uint32_t)0,
                                                      "$", 
                                                      (uint16_t)0, 
                                                      SPEAK_CHANNEL_Y,
                                                      (uint16_t)_channelId,
                                                      msg);

                        grml.add (out);
                        _client->sendToClient (grml);

                        _client->broadcastMessage (msg);
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
                                GSM_CHANNEL_CLOSE_ID, new ChannelCloseHook ());

        _speak_hid = _client->addSendReadHook (_pluginId, 
                                GSM_SPEAK_ID, new SpeakHook ());

        _rid = _client->addRecipricant (_pluginId, new ChannelRecipricant ());

        uint32_t _cMgrId = _client->getPluginByName ("channelmanager");
        _client->sendMessage (_cMgrId, "channelmanager add console 1234");
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

