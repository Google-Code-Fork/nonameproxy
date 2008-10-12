#ifndef __CORE_RECIPRICANT_H
#define __CORE_RECIPRICANT_H

#include <stdint.h>
#include "messenger.h"

class Client;

class CoreRecipricant : public Recipricant
{
        public:
                CoreRecipricant (Client* client);
                virtual void func (const Args& args);
        private:
                void loadPlugin (const Args& args, Args::const_iterator i,
                                        uint32_t argc);
                void unloadPlugin (const Args& args, Args::const_iterator i,
                                        uint32_t argc);

                Client* _client;
};

#endif

