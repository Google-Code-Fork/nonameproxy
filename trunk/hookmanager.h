#ifndef __HOOK_MANAGER_H
#define __HOOK_MANAGER_H

#define MAX_HOOKS 256

#include <stdint.h>
#include <list>

class Client;

class ReadHook;
class WriteHook;

class RHookInfo
{
        public:
                RHookInfo (ReadHook* hook, uint64_t id);
                virtual ~RHookInfo ();
                ReadHook* getHook ();
                uint64_t getId ();
        private:
                ReadHook* _hook;
                uint64_t _id;
};

typedef std::list<RHookInfo*> rHookList;

//note that we only need to define a single hook manager class
//however each protocol will require a hook manager

class HookManager
{
        public:
                HookManager ();
                virtual ~HookManager ();
                //these functions return a hook id, so that they can be removed
                //later. -1 means failure to insert hook
                uint64_t addReadHook (uint8_t id, ReadHook* hook); 
                uint64_t addWriteHook (uint8_t id, WriteHook* hook);
                
                void deleteReadHook (uint64_t hid);
                void deleteWriteHook (uint64_t hid);

                void            hookReadMessage (TibiaMessage* tm, Client* client);
                TibiaMessage*   hookWriteMessage (TibiaMessage* tm, Client* client);
        private:
                //the easiest way to store the hooks is in an array,
                //and considering there are only 256 message possibilites
                //its not worth the effort only storing valid messages
                WriteHook*      wHooks[MAX_HOOKS];
                rHookList       rHooks[MAX_HOOKS];
};
#endif

