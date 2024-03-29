/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

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

bool LSMessageFactory::isEnd ()
{
        //only the RSA message is part of LS
        return _msg->isRSAEOF ();
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

bool LRMessageFactory::isEnd ()
{
        //LR has no RSA messages
        return _msg->isXTEAEOF ();
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
        if (id == GSM_GAME_INIT_ID) {
                return (new GSMGameInit (_msg, _gs, _dat));
        } else if (id == GSM_LOGOUT_ID) {
                return (new GSMLogout (_msg, _gs, _dat));
        } else if (id == GSM_PING_ID) {
                return (new GSMPing (_msg, _gs, _dat));
        } else if (id == GSM_AUTO_WALK_ID) {
                return (new GSMAutoWalk (_msg, _gs, _dat));
        } else if (id == GSM_AUTO_WALK_CANCEL_ID) {
                return (new GSMAutoWalkCancel (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_NORTH_ID) {
                return (new GSMMoveNorth (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_EAST_ID) {
                return (new GSMMoveEast (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_SOUTH_ID) {
                return (new GSMMoveSouth (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_WEST_ID) {
                return (new GSMMoveWest (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_NE_ID) {
                return (new GSMMoveNE (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_SE_ID) {
                return (new GSMMoveSE (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_SW_ID) {
                return (new GSMMoveSW (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_NW_ID) {
                return (new GSMMoveNW (_msg, _gs, _dat));
        } else if (id == GSM_TURN_NORTH_ID) {
                return (new GSMTurnNorth (_msg, _gs, _dat));
        } else if (id == GSM_TURN_EAST_ID) {
                return (new GSMTurnEast (_msg, _gs, _dat));
        } else if (id == GSM_TURN_SOUTH_ID) {
                return (new GSMTurnSouth (_msg, _gs, _dat));
        } else if (id == GSM_TURN_WEST_ID) {
                return (new GSMTurnWest (_msg, _gs, _dat));
        } else if (id == GSM_MOVE_ITEM_ID) {
                return (new GSMMoveItem (_msg, _gs, _dat));
        } else if (id == GSM_TRADE_ITEM_ID) {
                return (new GSMTradeItem (_msg, _gs, _dat));
        } else if (id == GSM_TRADE_LOOK_ID) {
                return (new GSMTradeLook (_msg, _gs, _dat));
        } else if (id == GSM_TRADE_ACCEPT_ID) {
                return (new GSMTradeAccept (_msg, _gs, _dat));
        } else if (id == GSM_TRADE_REJECT_ID) {
                return (new GSMTradeReject (_msg, _gs, _dat));
        } else if (id == GSM_USE_ITEM_ID) {
                return (new GSMUseItem (_msg, _gs, _dat));
        } else if (id == GSM_USE_ITEM_WITH_ID) {
                return (new GSMUseItemWith (_msg, _gs, _dat));
        } else if (id == GSM_USE_BATTLE_WINDOW_ID) {
                return (new GSMUseBattleWindow (_msg, _gs, _dat));
        } else if (id == GSM_ROTATE_ITEM_ID) {
                return (new GSMRotateItem (_msg, _gs, _dat));
        } else if (id == GSM_CONTAINER_CLOSE_ID) {
                return (new GSMContainerClose (_msg, _gs, _dat));
        } else if (id == GSM_CONTAINER_UP_ID) {
                return (new GSMContainerUp (_msg, _gs, _dat));
        } else if (id == GSM_TEXT_WINDOW_ID) {
                return (new GSMTextWindow (_msg, _gs, _dat));
        } else if (id == GSM_TEXT_HOUSE_ID) {
                return (new GSMTextHouse (_msg, _gs, _dat));
        } else if (id == GSM_LOOK_ITEM_ID) {
                return (new GSMLookItem (_msg, _gs, _dat));
        } else if (id == GSM_SPEAK_ID) {
                return (new GSMSpeak (_msg, _gs, _dat));
        } else if (id == GSM_REQUEST_CHANNELS_ID) {
                return (new GSMRequestChannels (_msg, _gs, _dat));
        } else if (id == GSM_CHANNEL_OPEN_ID) {
                return (new GSMChannelOpen (_msg, _gs, _dat));
        } else if (id == GSM_CHANNEL_CLOSE_ID) {
                return (new GSMChannelClose (_msg, _gs, _dat));
        } else if (id == GSM_PRIVATE_CHAT_OPEN_ID) {
                return (new GSMPrivateChatOpen (_msg, _gs, _dat));
        } else if (id == GSM_RULE_VIOLATION_OPEN_ID) {
                return (new GSMRuleViolationOpen (_msg, _gs, _dat));
        } else if (id == GSM_RULE_VIOLATION_CLOSE_ID) {
                return (new GSMRuleViolationClose (_msg, _gs, _dat));
        } else if (id == GSM_RULE_VIOLATION_CANCEL_ID) {
                return (new GSMRuleViolationCancel (_msg, _gs, _dat));
        } else if (id == GSM_NPC_CHANNEL_CLOSE_ID) {
                return (new GSMNPCChannelClose (_msg, _gs, _dat));
        } else if (id == GSM_PRIVATE_CHANNEL_CREATE_ID) {
                return (new GSMPrivateChannelCreate (_msg, _gs, _dat));
        } else if (id == GSM_PRIVATE_CHANNEL_INVITE_ID) {
                return (new GSMPrivateChannelInvite (_msg, _gs, _dat));
        } else if (id == GSM_PRIVATE_CHANNEL_BAN_ID) {
                return (new GSMPrivateChannelBan (_msg, _gs, _dat));
        } else if (id == GSM_FIGHT_MODES_ID) {
                return (new GSMFightModes (_msg, _gs, _dat));
        } else if (id == GSM_CREATURE_ATTACK_ID) {
                return (new GSMCreatureAttack (_msg, _gs, _dat));
        } else if (id == GSM_CREATURE_FOLLOW_ID) {
                return (new GSMCreatureFollow (_msg, _gs, _dat));
        } else if (id == GSM_PARTY_INVITE_ID) {
                return (new GSMPartyInvite (_msg, _gs, _dat));
        } else if (id == GSM_PARTY_JOIN_ID) {
                return (new GSMPartyJoin (_msg, _gs, _dat));
        } else if (id == GSM_PARTY_CANCEL_ID) {
                return (new GSMPartyCancel (_msg, _gs, _dat));
        } else if (id == GSM_PARTY_PASS_LEADERSHIP_ID) {
                return (new GSMPartyPassLeadership (_msg, _gs, _dat));
        } else if (id == GSM_PARTY_LEAVE_ID) {
                return (new GSMPartyLeave (_msg, _gs, _dat));
        } else if (id == GSM_CANCEL_MOVE_ID) {
                return (new GSMCancelMove (_msg, _gs, _dat));
        } else if (id == GSM_OUTFIT_REQUEST_ID) {
                return (new GSMOutfitRequest (_msg, _gs, _dat));
        } else if (id == GSM_OUTFIT_SET_ID) {
                return (new GSMOutfitSet (_msg, _gs, _dat));
        } else if (id == GSM_VIP_ADD_ID) {
                return (new GSMVIPAdd (_msg, _gs, _dat));
        } else if (id == GSM_VIP_REMOVE_ID) {
                return (new GSMVIPRemove (_msg, _gs, _dat));
        } else if (id == GSM_BUG_REPORT_ID) {
                return (new GSMBugReport (_msg, _gs, _dat));
        } else if (id == GSM_QUEST_REQUEST_LOG_ID) {
                return (new GSMQuestRequestLog (_msg, _gs, _dat));
        } else if (id == GSM_QUEST_REQUEST_ID) {
                return (new GSMQuestRequest (_msg, _gs, _dat));
        } else if (id == GSM_SHOP_BUY_ID) {
                return (new GSMShopBuy (_msg, _gs, _dat));
        } else if (id == GSM_SHOP_SELL_ID) {
                return (new GSMShopSell (_msg, _gs, _dat));
        } else if (id == GSM_SHOP_CLOSE_ID) {
                return (new GSMShopClose (_msg, _gs, _dat));
        } else if (id == GSM_REQUEST_TILE_UPDATE_ID) {
                return (new GSMRequestTileUpdate (_msg, _gs, _dat));
        } else if (id == GSM_REQUEST_CONTAINER_UPDATE_ID) {
                return (new GSMRequestContainerUpdate (_msg, _gs, _dat));
        }
        printf ("Protocol error: unknown GS Message 0x%X\n", id);
        return NULL;
}

bool GSMessageFactory::isEnd ()
{
        //GS has RSA and xtea messages
        if (_msg->isRSA ()) {
                return _msg->isRSAEOF ();
        } else {
                return _msg->isXTEAEOF ();
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
        } else if (id == GRM_CONTAINER_OPEN_ID) {
                return (new GRMContainerOpen (_msg, _gs, _dat));
        } else if (id == GRM_CONTAINER_CLOSE_ID) {
                return (new GRMContainerClose (_msg, _gs, _dat));
        } else if (id == GRM_CONTAINER_ADD_ID) {
                return (new GRMContainerAdd (_msg, _gs, _dat));
        } else if (id == GRM_CONTAINER_UPDATE_ID) {
                return (new GRMContainerUpdate (_msg, _gs, _dat));
        } else if (id == GRM_CONTAINER_REMOVE_ID) {
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
        
bool GRMessageFactory::isEnd ()
{
        //GR has no RSA messages
        return _msg->isXTEAEOF ();
}

