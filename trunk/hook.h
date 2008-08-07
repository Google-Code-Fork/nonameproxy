#ifndef __HOOK_H
#define __HOOK_H

#include <stdlib.h>

class TibiaMessage;
class GameState; //note this doesnt actually exist yet

//These are stub classes. Hooks can either be defined in corehooks.h
//or a module can define a hook in its own code.
class ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, GameState* gs) {}
};

class WriteHook
{
        public:
                virtual TibiaMessage* func (TibiaMessage* tm, GameState* gs)
                        {return NULL;}
};

#endif

