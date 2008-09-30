#include <stdio.h>
#include "networkmessage.h"
#include "messagefactory.h"
#include "tibiatypes.h"
#include "tibiamessage.h"
#include "enums.h"

LSMessageFactory::LSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;

        _readable = true;
}

LSMessageFactory::LSMessageFactory ()
{
        _readable = false;
}

LSMessageFactory::~LSMessageFactory ()
{
        if (_readable) {
                delete _msg;
        }
}
        
TibiaMessage* LSMessageFactory::getMessage ()
{
        if (!_readable) {
                printf ("lsmessagefactory error: not set for reading\n");
                return NULL;
        }
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

void LSMessageFactory::deleteMessage (TibiaMessage* tm)
{
        if (!tm) {
                return;
        }
        uint8_t id = tm->getId ();
        if (id == 0x01) {
                delete ((LSMLoginMsg*)tm);
        } else {
                printf ("Protocol error: delete unknown LS Message 0x%X\n", id);
        }
}

// LRMessage Factory
LRMessageFactory::LRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;

        _readable = true;
}

LRMessageFactory::LRMessageFactory ()
{
        _readable = false;
}

LRMessageFactory::~LRMessageFactory ()
{
        if (_readable) {
                delete _msg;
        }
}
        
TibiaMessage* LRMessageFactory::getMessage ()
{
        if (!_readable) {
                printf ("lrmessagefactory error: not set for reading\n");
                return NULL;
        }
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

void LRMessageFactory::deleteMessage (TibiaMessage* tm)
{
        if (!tm) {
                return;
        }
        uint8_t id = tm->getId ();
        if (id == 0x0A) {
                delete ((LRMError*)tm);
        } else if (id == 0x0B) {
                delete ((LRMInfo*)tm);
        } else if (id == 0x14) {
                delete ((LRMMOTD*)tm);
        } else if (id == 0x64) {
                delete ((LRMCharacterList*)tm);
        } else {
                printf ("Protocol error: delete unknown LR Message 0x%X\n", id);
        }
}


// GSMessage Factory
GSMessageFactory::GSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;

        _readable = true;
}

GSMessageFactory::GSMessageFactory ()
{
        _readable = false;
}

GSMessageFactory::~GSMessageFactory ()
{
        if (_readable) {
                delete _msg;
        }
}
        
TibiaMessage* GSMessageFactory::getMessage ()
{
        //GS has RSA and xtea messages
        if (!_readable) {
                printf ("gsmessagefactory error: not set for reading\n");
                return NULL;
        }
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

void GSMessageFactory::deleteMessage (TibiaMessage* tm)
{
        if (!tm) {
                return;
        }
        uint8_t id = tm->getId ();
        if (id == 0x0A) {
                delete ((GSMGameInit*)tm);
        } else {
                printf ("Protocol error: delete unknown GS Message 0x%X\n", id);
        }
}

// GRMessage Factory
GRMessageFactory::GRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        _msg = msg;
        _gs = gs;
        _dat = dat;

        _readable = true;
}

GRMessageFactory::GRMessageFactory ()
{
        _readable = false;
}

GRMessageFactory::~GRMessageFactory ()
{
        if (_readable) {
                delete _msg;
        }
}

TibiaMessage* GRMessageFactory::getMessage ()
{
        
        if (!_readable) {
                printf ("grmessagefactory error: not set for reading\n");
                return NULL;
        }
        //GR has no RSA messages
        if (_msg->isXTEAEOF ()) {
                return NULL;
        }
        uint8_t id;

        _msg->peekU8 (id);
        //uint32_t tmp = id;
        //printf ("GRM 0x%d\n", _msg->getPos ());
        
        if (id == GRM_SELF_INFO_ID) {
                return (new GRMSelfInfo (_msg, _gs, _dat));
        } else if (id == GRM_GM_ACTION_ID) {
                return (new GRMGMAction (_msg, _gs, _dat));
        } else if (id == GRM_ERROR_ID) {
                return (new GRMError (_msg, _gs, _dat));
        } else if (id == GRM_FYI_ID) {
                return (new GRMFYI (_msg, _gs, _dat));
        } else if (id == GRM_QUEUE_ID) {
                return (new GRMQueue (_msg, _gs, _dat));
        } else if (id == GRM_PING_ID) {
                return (new GRMPing (_msg, _gs, _dat));
        } else if (id == GRM_LOGIN_WINDOW_ID) {
                return (new GRMLoginWindow (_msg, _gs, _dat));
        } else if (id == GRM_MAP_INIT_ID) {
                return (new GRMMapInit (_msg, _gs, _dat));
        } else if (id == GRM_MAP_NORTH_ID) {
                return (new GRMMapNorth (_msg, _gs, _dat));
        } else if (id == GRM_MAP_EAST_ID) {
                return (new GRMMapEast (_msg, _gs, _dat));
        } else if (id == GRM_MAP_SOUTH_ID) {
                return (new GRMMapSouth (_msg, _gs, _dat));
        } else if (id == GRM_MAP_WEST_ID) {
                return (new GRMMapWest (_msg, _gs, _dat));
        } else if (id == GRM_MAP_UP_ID) {
                return (new GRMMapUp (_msg, _gs, _dat));
        } else if (id == GRM_MAP_DOWN_ID) {
                return (new GRMMapDown (_msg, _gs, _dat));
        } else if (id == GRM_TILE_SET_ID) {
                return (new GRMTileSet (_msg, _gs, _dat));
        } else if (id == GRM_TILE_ADD_ID) {
                return (new GRMTileAdd (_msg, _gs, _dat));
        } else if (id == GRM_TILE_UPDATE_ID) {
                return (new GRMTileUpdate (_msg, _gs, _dat));
        } else if (id == GRM_TILE_REMOVE_ID) {
                return (new GRMTileRemove (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_MOVE_ID) {
                return (new GRMCreatureMove (_msg, _gs, _dat));
        } else if (id == GRM_MAGIC_EFFECT_ID) {
                return (new GRMMagicEffect (_msg, _gs, _dat));
        } else if (id == GRM_SHOOT_EFFECT_ID) {
                return (new GRMShootEffect (_msg, _gs, _dat));
        } else if (id == GRM_SLOT_ITEM_ID) {
                return (new GRMSlotItem (_msg, _gs, _dat));
        } else if (id == GRM_SLOT_CLEAR_ID) {
                return (new GRMSlotClear (_msg, _gs, _dat));
        } else if (id == GRM_GLOBAL_LIGHT_ID) {
                return (new GRMGlobalLight (_msg, _gs, _dat));
        } else if (id == GRM_ANIMATED_TEXT_ID) {
                return (new GRMAnimatedText (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_SQUARE_ID) {
                return (new GRMCreatureSquare (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_HEALTH_ID) {
                return (new GRMCreatureHealth (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_LIGHT_ID) {
                return (new GRMCreatureLight (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_OUTFIT_ID) {
                return (new GRMCreatureOutfit (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_SPEED_ID) {
                return (new GRMCreatureSpeed (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_SKULL_ID) {
                return (new GRMCreatureSkull (_msg, _gs, _dat));
        } else if (id == GRM_CREATURE_SHIELD_ID) {
                return (new GRMCreatureShield (_msg, _gs, _dat));
        } else if (id == GRM_TEXT_MSG_ID) {
                return (new GRMTextMsg (_msg, _gs, _dat));
        } else if (id == GRM_PLAYER_STATS_ID) {
                return (new GRMPlayerStats (_msg, _gs, _dat));
        } else if (id == GRM_PLAYER_SKILLS_ID) {
                return (new GRMPlayerSkills (_msg, _gs, _dat));
        } else if (id == GRM_PLAYER_ICONS_ID) {
                return (new GRMPlayerIcons (_msg, _gs, _dat));
        } else if (id == GRM_PLAYER_CANCEL_ATTACK_ID) {
                return (new GRMPlayerCancelAttack (_msg, _gs, _dat));
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
        } else if (id == GRM_OPEN_SELF_TRADE_ID) {
                return (new GRMOpenSelfTrade (_msg, _gs, _dat));
        } else if (id == GRM_OPEN_PLAYER_TRADE_ID) {
                return (new GRMOpenPlayerTrade (_msg, _gs, _dat));
        } else if (id == GRM_CLOSE_TRADE_ID) {
                return (new GRMCloseTrade (_msg, _gs, _dat));
        } else if (id == GRM_SHOP_TRADE_ID) {
                return (new GRMShopTrade (_msg, _gs, _dat));
        } else if (id == GRM_SHOP_GOLD_ID) {
                return (new GRMShopGold (_msg, _gs, _dat));
        } else if (id == GRM_SHOP_CLOSE_ID) {
                return (new GRMShopClose (_msg, _gs, _dat));
        } else if (id == GRM_OUTFIT_WINDOW_ID) {
                return (new GRMOutfitWindow (_msg, _gs, _dat));
        } else if (id == GRM_VIP_INIT_ID) {
                return (new GRMVipInit (_msg, _gs, _dat));
        } else if (id == GRM_VIP_LOGIN_ID) {
                return (new GRMVipLogin (_msg, _gs, _dat));
        } else if (id == GRM_VIP_LOGOUT_ID) {
                return (new GRMVipLogout (_msg, _gs, _dat));
        } else if (id == GRM_SHOW_TUTORIAL_ID) {
                return (new GRMShowTutorial (_msg, _gs, _dat));
        } else if (id == GRM_MINI_MAP_MARK_ID) {
                return (new GRMMiniMapMark (_msg, _gs, _dat));
        } else if (id == GRM_QUEST_LIST_ID) {
                return (new GRMQuestList (_msg, _gs, _dat));
        } else if (id == GRM_SUB_QUEST_LIST_ID) {
                return (new GRMSubQuestList (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_LIST_ID) {
                return (new GRMChannelList (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_OPEN_ID) {
                return (new GRMChannelOpen (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_PLAYER_ID) {
                return (new GRMChannelPlayer (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_RULE_OPEN_ID) {
                return (new GRMChannelRuleOpen (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_RULE_U1_ID) {
                return (new GRMChannelRuleU1 (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_RULE_U2_ID) {
                return (new GRMChannelRuleU2 (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_RULE_U3_ID) {
                return (new GRMChannelRuleU3 (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_CHAT_ID) {
                return (new GRMChannelChat (_msg, _gs, _dat));
        } else if (id == GRM_CHANNEL_CLOSE_ID) {
                return (new GRMChannelClose (_msg, _gs, _dat));
        } else if (id == GRM_PLAYER_CANCEL_WALK_ID) {
                return (new GRMPlayerCancelWalk (_msg, _gs, _dat));
        } else if (id == GRM_TEXT_WINDOW_ID) {
                return (new GRMTextWindow (_msg, _gs, _dat));
        } else if (id == GRM_TEXT_HOUSE_ID) {
                return (new GRMTextHouse (_msg, _gs, _dat));
        } else if (id == GRM_SPEAK_ID) {
                return (new GRMSpeak (_msg, _gs, _dat));
        }
        printf ("Protocol error: unknown GR Message 0x%X\n", id);
        return NULL;
}
        
void GRMessageFactory::deleteMessage (TibiaMessage* tm)
{
        if (!tm) {
                return;
        }
        uint8_t id = tm->getId ();
        if (id == GRM_SELF_INFO_ID) {
                delete ((GRMSelfInfo*)tm);
        } else if (id == GRM_GM_ACTION_ID) {
                delete ((GRMGMAction*)tm);
        } else if (id == GRM_ERROR_ID) {
                delete ((GRMError*)tm);
        } else if (id == GRM_FYI_ID) {
                delete ((GRMFYI*)tm);
        } else if (id == GRM_QUEUE_ID) {
                delete ((GRMQueue*)tm);
        } else if (id == GRM_PING_ID) {
                delete ((GRMPing*)tm);
        } else if (id == GRM_LOGIN_WINDOW_ID) {
                delete ((GRMLoginWindow*)tm);
        } else if (id == GRM_MAP_INIT_ID) {
                delete ((GRMMapInit*)tm);
        } else if (id == GRM_MAP_NORTH_ID) {
                delete ((GRMMapNorth*)tm);
        } else if (id == GRM_MAP_EAST_ID) {
                delete ((GRMMapEast*)tm);
        } else if (id == GRM_MAP_SOUTH_ID) {
                delete ((GRMMapSouth*)tm);
        } else if (id == GRM_MAP_WEST_ID) {
                delete ((GRMMapWest*)tm);
        } else if (id == GRM_MAP_UP_ID) {
                delete ((GRMMapUp*)tm);
        } else if (id == GRM_MAP_DOWN_ID) {
                delete ((GRMMapDown*)tm);
        } else if (id == GRM_TILE_SET_ID) {
                delete ((GRMTileSet*)tm);
        } else if (id == GRM_TILE_ADD_ID) {
                delete ((GRMTileAdd*)tm);
        } else if (id == GRM_TILE_UPDATE_ID) {
                delete ((GRMTileUpdate*)tm);
        } else if (id == GRM_TILE_REMOVE_ID) {
                delete ((GRMTileRemove*)tm);
        } else if (id == GRM_CREATURE_MOVE_ID) {
                delete ((GRMCreatureMove*)tm);
        } else if (id == GRM_MAGIC_EFFECT_ID) {
                delete ((GRMMagicEffect*)tm);
        } else if (id == GRM_SHOOT_EFFECT_ID) {
                delete ((GRMShootEffect*)tm);
        } else if (id == GRM_SLOT_ITEM_ID) {
                delete ((GRMSlotItem*)tm);
        } else if (id == GRM_SLOT_CLEAR_ID) {
                delete ((GRMSlotClear*)tm);
        } else if (id == GRM_GLOBAL_LIGHT_ID) {
                delete ((GRMGlobalLight*)tm);
        } else if (id == GRM_ANIMATED_TEXT_ID) {
                delete ((GRMAnimatedText*)tm);
        } else if (id == GRM_CREATURE_SQUARE_ID) {
                delete ((GRMCreatureSquare*)tm);
        } else if (id == GRM_CREATURE_HEALTH_ID) {
                delete ((GRMCreatureHealth*)tm);
        } else if (id == GRM_CREATURE_LIGHT_ID) {
                delete ((GRMCreatureLight*)tm);
        } else if (id == GRM_CREATURE_OUTFIT_ID) {
                delete ((GRMCreatureOutfit*)tm);
        } else if (id == GRM_CREATURE_SPEED_ID) {
                delete ((GRMCreatureSpeed*)tm);
        } else if (id == GRM_CREATURE_SKULL_ID) {
                delete ((GRMCreatureSkull*)tm);
        } else if (id == GRM_CREATURE_SHIELD_ID) {
                delete ((GRMCreatureShield*)tm);
        } else if (id == GRM_TEXT_MSG_ID) {
                delete ((GRMTextMsg*)tm);
        } else if (id == GRM_PLAYER_STATS_ID) {
                delete ((GRMPlayerStats*)tm);
        } else if (id == GRM_PLAYER_SKILLS_ID) {
                delete ((GRMPlayerSkills*)tm);
        } else if (id == GRM_PLAYER_ICONS_ID) {
                delete ((GRMPlayerIcons*)tm);
        } else if (id == GRM_PLAYER_CANCEL_ATTACK_ID) {
                delete ((GRMPlayerCancelAttack*)tm);
        } else if (id == GRM_OPEN_CONTAINER_ID) {
                delete ((GRMOpenContainer*)tm);
        } else if (id == GRM_CLOSE_CONTAINER_ID) {
                delete ((GRMCloseContainer*)tm);
        } else if (id == GRM_CONT_ADD_ID) {
                delete ((GRMContainerAdd*)tm);
        } else if (id == GRM_CONT_UPDATE_ID) {
                delete ((GRMContainerUpdate*)tm);
        } else if (id == GRM_CONT_REMOVE_ID) {
                delete ((GRMContainerRemove*)tm);
        } else if (id == GRM_OPEN_SELF_TRADE_ID) {
                delete ((GRMOpenSelfTrade*)tm);
        } else if (id == GRM_OPEN_PLAYER_TRADE_ID) {
                delete ((GRMOpenPlayerTrade*)tm);
        } else if (id == GRM_CLOSE_TRADE_ID) {
                delete ((GRMCloseTrade*)tm);
        } else if (id == GRM_SHOP_TRADE_ID) {
                delete ((GRMShopTrade*)tm);
        } else if (id == GRM_SHOP_GOLD_ID) {
                delete ((GRMShopGold*)tm);
        } else if (id == GRM_SHOP_CLOSE_ID) {
                delete ((GRMShopClose*)tm);
        } else if (id == GRM_OUTFIT_WINDOW_ID) {
                delete ((GRMOutfitWindow*)tm);
        } else if (id == GRM_VIP_INIT_ID) {
                delete ((GRMVipInit*)tm);
        } else if (id == GRM_VIP_LOGIN_ID) {
                delete ((GRMVipLogin*)tm);
        } else if (id == GRM_VIP_LOGOUT_ID) {
                delete ((GRMVipLogout*)tm);
        } else if (id == GRM_SHOW_TUTORIAL_ID) {
                delete ((GRMShowTutorial*)tm);
        } else if (id == GRM_MINI_MAP_MARK_ID) {
                delete ((GRMMiniMapMark*)tm);
        } else if (id == GRM_QUEST_LIST_ID) {
                delete ((GRMQuestList*)tm);
        } else if (id == GRM_SUB_QUEST_LIST_ID) {
                delete ((GRMSubQuestList*)tm);
        } else if (id == GRM_CHANNEL_LIST_ID) {
                delete ((GRMChannelList*)tm);
        } else if (id == GRM_CHANNEL_OPEN_ID) {
                delete ((GRMChannelOpen*)tm);
        } else if (id == GRM_CHANNEL_PLAYER_ID) {
                delete ((GRMChannelPlayer*)tm);
        } else if (id == GRM_CHANNEL_RULE_OPEN_ID) {
                delete ((GRMChannelRuleOpen*)tm);
        } else if (id == GRM_CHANNEL_RULE_U1_ID) {
                delete ((GRMChannelRuleU1*)tm);
        } else if (id == GRM_CHANNEL_RULE_U2_ID) {
                delete ((GRMChannelRuleU2*)tm);
        } else if (id == GRM_CHANNEL_RULE_U3_ID) {
                delete ((GRMChannelRuleU3*)tm);
        } else if (id == GRM_CHANNEL_CHAT_ID) {
                delete ((GRMChannelChat*)tm);
        } else if (id == GRM_CHANNEL_CLOSE_ID) {
                delete ((GRMChannelClose*)tm);
        } else if (id == GRM_PLAYER_CANCEL_WALK_ID) {
                delete ((GRMPlayerCancelWalk*)tm);
        } else if (id == GRM_TEXT_WINDOW_ID) {
                delete ((GRMTextWindow*)tm);
        } else if (id == GRM_TEXT_HOUSE_ID) {
                delete ((GRMTextHouse*)tm);
        } else if (id == GRM_SPEAK_ID) {
                delete ((GRMSpeak*)tm);
        } else {
                printf ("Protocol error: delete unknown GR Message 0x%X\n", id);
        }
}

