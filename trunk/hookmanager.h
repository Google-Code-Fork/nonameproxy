#ifndef __HOOK_MANAGER_H
#define __HOOK_MANAGER_H

#define MAX_HOOKS 256

#include <stdint.h>
#include <map>

#include "idmanager.h"

class Client;

class ReadHook;
class WriteHook;
class TibiaMessage;

typedef std::map<uint32_t, ReadHook*> RHookList;

/* used for storing the message id into which the hook is inserted */
typedef std::map<uint32_t, uint32_t> HookMap;

/* note that we only need to define a single hook manager class
 * however each protocol will require a hook manager */

class HookManager
{
        public:
                HookManager ();
                virtual ~HookManager ();
                /* these functions return a hook id, so it can be 
                 * deleted later. 0 means failure to insert hook */
                uint32_t addReadHook (uint8_t id, ReadHook* hook); 
                uint32_t addWriteHook (uint8_t id, WriteHook* hook);
                
                void deleteReadHook (uint32_t hid);
                void deleteWriteHook (uint32_t hid);

                void            hookReadMessage (TibiaMessage* tm, Client* client);
                TibiaMessage*   hookWriteMessage (TibiaMessage* tm, Client* client);
        private:
                /* the easiest way to store the hooks is in an array,
                 * and considering there are only 256 message possibilites
                 * its not worth the effort only storing valid messages */
                WriteHook*      wHooks[MAX_HOOKS];

                /* unlike a write hook, an read hooks id is not reflective
                 * of its position in the array. Therefore the hook id and
                 * message id are inserted into a lookup table so we can
                 * find it later */
                IdManager*      ids;
                HookMap         rLookup;
                RHookList       rHooks[MAX_HOOKS];
};
#endif

