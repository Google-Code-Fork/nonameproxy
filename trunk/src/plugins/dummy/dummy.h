#ifndef __DUMMY_H
#define __DUMMY_H

#include <string>
#include <stdint.h>

#include "client.h"
#include "tibiamessage.h"
#include "messenger.h"
#include "hook.h"

class ChickenHook : public WriteHook
{
        virtual TibiaMessage* func (TibiaMessage* tm, Client* client);
};

class TestRecipricant : public Recipricant
{
        virtual void func (const Args& args);
};

extern "C"
{
        void load (uint32_t id, Client* client);
        void unload ();
        const std::string& name ();
}

#endif

