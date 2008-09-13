#include <stdio.h>
#include "networkmessage.h"
#include "messagefactory.h"
#include "tibiatypes.h"
#include "tibiamessage.h"
#include "messageids.h"

LSMessageFactory::LSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;
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
        uint8_t id;
        _msg->peekU8 (id);
        if (id == 0x01) {
                return (new LSMLoginMsg (_msg, _gs, _dat));
        }
        return NULL;
}

// LRMessage Factory
LRMessageFactory::LRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;
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

        uint8_t id;
        _msg->peekU8 (id);
        if (id == 0x0A) {
                return (new LRMError (_msg, _gs, _dat));
        } else if (id == 0x0B) {
                return (new LRMInfo (_msg, _gs, _dat));
        } else if (id == 0x14) {
                return (new LRMMOTD (_msg, _gs, _dat));
        } else if (id == 0x64) {
                return (new LRMCharacterList (_msg, _gs, _dat));
        }
        
        printf ("Protocol error: unknown LR Message 0x%X\n", id);
        return NULL;
}


// GSMessage Factory
GSMessageFactory::GSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;
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
                uint8_t id;
                _msg->peekU8 (id);
                if (id == 0x0A) {
                        return (new GSMGameInit (_msg, _gs, _dat));
                }
                return NULL;
        }

        if (_msg->isXTEAEOF ()) {
                return NULL;
        }

        uint8_t id;
        _msg->peekU8 (id);
        if (id == 0x0A) {
                return (new GSMGameInit (_msg, _gs, _dat));
        }
        printf ("Protocol error: unknown GS Message 0x%X\n", id);
        return NULL;
}

// GRMessage Factory
GRMessageFactory::GRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;
}

GRMessageFactory::~GRMessageFactory ()
{
        delete _msg;
}

TibiaMessage* GRMessageFactory::getMessage ()
{
        //GR has no RSA messages
        if (_msg->isXTEAEOF ()) {
                return NULL;
        }
        uint8_t id;
        _msg->peekU8 (id);
        if (id == GRM_SELF_INFO_ID) {
                return (new GRMSelfInfo (_msg, _gs, _dat));
        } else if (id == GRM_MAP_INIT_ID) {
                return (new GRMMapInit (_msg, _gs, _dat));
        } else if (id == GRM_MAGIC_EFFECT_ID) {
                return (new GRMMagicEffect (_msg, _gs, _dat));
        } else if (id == GRM_SLOT_ITEM_ID) {
                return (new GRMSlotItem (_msg, _gs, _dat));
        } else if (id == GRM_SLOT_CLEAR_ID) {
                return (new GRMSlotClear (_msg, _gs, _dat));
        } else if (id == GRM_GLOBAL_LIGHT_ID) {
                return (new GRMGlobalLight (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_LIGHT_ID) {
                return (new GRMCreatureLight (_msg, _gs, _dat));
        } else if (id == GRM_TEXT_MSG_ID) {
                return (new GRMTextMsg (_msg, _gs, _dat));
        } else if (id == GRM_PLAYER_STATS_ID) {
                return (new GRMPlayerStats (_msg, _gs, _dat));
        } else if (id == GRM_PLAYER_SKILLS_ID) {
                return (new GRMPlayerSkills (_msg, _gs, _dat));
        } else if (id == GRM_OPEN_CONTAINER_ID) {
                return (new GRMOpenContainer (_msg, _gs, _dat));
        } else if (id == GRM_CLOSE_CONTAINER_ID) {
                return (new GRMCloseContainer (_msg, _gs, _dat));
        } else if (id == GRM_CONT_ADD_ID) {
                return (new GRMContainerAdd (_msg, _gs, _dat));
        } else if (id == GRM_CONT_UPDATE_ID) {
                return (new GRMContainerUpdate (_msg, _gs, _dat));
        } else if (id == GRM_CONT_REMOVE_ID) {
                return (new GRMContainerRemove (_msg, _gs, _dat));
        }
        printf ("Protocol error: unknown GR Message 0x%X\n", id);
        return NULL;
}
        
