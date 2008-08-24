#include <stdio.h>
#include "networkmessage.h"
#include "messagefactory.h"
#include "tibiatypes.h"
#include "tibiamessage.h"

LSMessageFactory::LSMessageFactory (NetworkMessage* msg)
{
        _msg = msg;
}

LSMessageFactory::~LSMessageFactory ()
{
        delete _msg;
}
        
TibiaMessage* LSMessageFactory::getMessage ()
{
        //only the RSA message is part of LS
        if (_msg->isRSAEOF ()) {
                return NULL;
        }
        uint8_t id = TWord8 (_msg).getVal ();
        if (id == 0x01) {
                return (new LSMLoginMsg (_msg));
        }
        return NULL;
}

// LRMessage Factory
LRMessageFactory::LRMessageFactory (NetworkMessage* msg)
{
        _msg = msg;
}

LRMessageFactory::~LRMessageFactory ()
{
        delete _msg;
}
        
TibiaMessage* LRMessageFactory::getMessage ()
{
        //LR has no RSA messages
        if (_msg->isXTEAEOF ()) {
                return NULL;
        }

        uint8_t id = TWord8 (_msg).getVal ();
        if (id == 0x0A) {
                return (new LRMError (_msg));
        } else if (id == 0x0B) {
                return (new LRMInfo (_msg));
        } else if (id == 0x14) {
                return (new LRMMOTD (_msg));
        } else if (id == 0x64) {
                return (new LRMCharacterList (_msg));
        }
        
        printf ("Protocol error: unknown LR Message 0x%X\n", id);
        return NULL;
}


// GSMessage Factory
GSMessageFactory::GSMessageFactory (NetworkMessage* msg)
{
        _msg = msg;
}

GSMessageFactory::~GSMessageFactory ()
{
        delete _msg;
}
        
TibiaMessage* GSMessageFactory::getMessage ()
{
        //GS has RSA and xtea messages
        if (_msg->isRSA ()) {
                if (_msg->isRSAEOF ()) {
                        return NULL;
                }
                uint8_t id = TWord8 (_msg).getVal ();
                if (id == 0x0A) {
                        return (new GSMGameInit (_msg));
                }
                return NULL;
        }

        if (_msg->isXTEAEOF ()) {
                return NULL;
        }

        uint8_t id = TWord8 (_msg).getVal ();
        if (id == 0x0A) {
                return (new GSMGameInit (_msg));
        }
        printf ("Protocol error: unknown GS Message 0x%X\n", id);
        return NULL;
}

