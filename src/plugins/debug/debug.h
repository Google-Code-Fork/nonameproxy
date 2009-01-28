#ifndef __DEBUG
#define __DEBUG

#include <string>
#include <stdint.h>

#include "tibiamessage.h"
#include "networkmessage.h"
#include "hook.h"
#include "packethook.h"
#include "client.h"

/**************************************************************************
 * Debug is used for, well, debugging. More presicely debug can do two
 * things. It can display full packets in their serial form, or display
 * individual tibia messages in their typed form.
 * 
 * as a rule of thumb 
 *      show is for one off outputs
 *      disp is for recuring outputs
 *
 *
 * Usage:
 *      debug disp [send|recv] [pre|post|messageid]
 *      debug undisp [send|recv] [pre|post|messageid]
 *      debug show tile x y z                           relative to player
 *      debug show battlelist
 **************************************************************************/

class ShowMessage : public ReadHook
{
        public:
                virtual void func (TibiaMessage *tm, Client *client);
};

class ShowPacket : public PacketHook
{
        public:
                virtual void func (const NetworkMessage &msg);
};

class DebugRecipricant : public Recipricant
{
        public:
                virtual int32_t func (const Args &args, Args &out);
};

class Debug
{
        public:
                Debug ();

                int32_t usage (Args &out);

                std::string pre_send_on ();
                std::string post_send_on ();
                std::string send_on (const std::string &mid);

                std::string pre_recv_on ();
                std::string post_recv_on ();
                std::string recv_on (const std::string &mid);

                std::string pre_send_off ();
                std::string post_send_off ();
                std::string send_off (const std::string &mid);

                std::string pre_recv_off ();
                std::string post_recv_off ();
                std::string recv_off (const std::string &mid);

                std::string show_tile (uint32_t x, uint32_t y, uint32_t z);
                std::string show_battlelist ();
                
                std::string show_container (const std::string &cid);
                std::string show_inventory ();

                void iload (uint32_t pluginId, Client *client);
                void iunload ();
                const std::string &iname ();

        private:
                uint32_t _pluginId;
                Client  *_client;

                uint32_t _sendpre;
                uint32_t _sendpost;
                uint32_t _recvpre;
                uint32_t _recvpost;

                uint32_t _sendHooks[MAX_HOOKS];
                uint32_t _recvHooks[MAX_HOOKS];

                uint32_t _rid;

                std::string _name;
};

extern "C"
{
        void load (uint32_t id, Client *client);
        void unload ();
        const std::string &name ();
}

#endif

