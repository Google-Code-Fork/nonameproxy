#ifndef __DUMMY_H
#define __DUMMY_H

#include <string>
#include <stdint.h>

#include "client.h"
#include "tibiamessage.h"
#include "messenger.h"
#include "hook.h"

/* the hook that will replace the speak messages */
class ChickenHook : public WriteHook
{
        public:
                virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

/* the recipricant for communicating with core */
class ChickenRecipricant : public Recipricant
{
        public:
                virtual int32_t func (const Args &args, Args &out);
};

extern "C"
{
        void load (uint32_t id, Client *client);
        void unload ();
        const std::string &name ();
}

#endif

