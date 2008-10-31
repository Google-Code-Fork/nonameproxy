#ifndef __CHANNEL_MANAGER_H
#define __CHANNEL_MANAGER_H

#include <string>
#include <stdint.h>

#include "tibiamessage.h"
#include "hook.h"
#include "client.h"

/**************************************************************************
 * Console adds a channel to the channels list called console. Anything
 * sent to this channel will be interpreted as a command and sent over
 * the messaging system. In this way one can control the proxy from within
 * tibia
 * Other plugins can also output to the console using the following command
 * Useage:
 *      console 'message'
 **************************************************************************/

class ChannelOpenHook : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class ChannelCloseHook : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class SpeakHook : public ReadHook
{
        virtual void func (TibiaMessage* tm, Client* client);
};

class ConsoleRecipricant : public Recipricant
{
        virtual Args func (const Args& args);
};

class Console
{
        public:
                void output (const std::string& msg);

                void hookChannelOpen (GSMChannelOpen* co, Client* client);
                void hookChannelClose (GSMChannelClose* cc, Client* client);
                void hookSpeak (GSMSpeak* co, Client* client);

                void iload (uint32_t pluginId, Client* client);
                void iunload ();
                const std::string& iname ();

        private:
                GRMSpeak* consoleIn (const std::string& msg);
                GRMSpeak* consoleOut (const std::string& msg);

                uint32_t _pluginId;
                uint32_t _channelopen_hid;
                uint32_t _channelclose_hid;
                uint32_t _speak_hid;
                uint32_t _rid;

                Client*  _client;
                std::string _name;

                /* keep track of whether or not the console window is open */
                uint32_t _channelId;
                bool _open;
};

extern "C"
{
        void load (uint32_t id, Client* client);
        void unload ();
        const std::string& name ();
}

#endif

