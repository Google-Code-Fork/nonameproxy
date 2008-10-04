#ifndef __HOOK_H
#define __HOOK_H

#include <stdlib.h>
#include <list>

class TibiaMessage;
class Client;

//These are stub classes. Hooks can either be defined in corehooks.h
//or a module can define a hook in its own code.
class ReadHook
{
        public:
                virtual ~ReadHook () {};
                virtual void func (TibiaMessage* tm, Client* client) {}
};

class WriteHook
{
        public:
                virtual ~WriteHook () {};
                virtual TibiaMessage* func (TibiaMessage* tm, Client* client)
                        {return NULL;}
};
#endif

