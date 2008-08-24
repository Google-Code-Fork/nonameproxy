#ifndef __HOOK_H
#define __HOOK_H

#include <stdlib.h>

class TibiaMessage;
class Client;

//These are stub classes. Hooks can either be defined in corehooks.h
//or a module can define a hook in its own code.
class ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client) {}
};

class WriteHook
{
        public:
                virtual TibiaMessage* func (TibiaMessage* tm, Client* client)
                        {return NULL;}
};

#endif

