#ifndef __NONAME_PROXY_ENUMS_H
#define __NONAME_PROXY_ENUMS_H

/******************************************************************
 * Enum of tibias game send 
 ******************************************************************/

enum gs_message_id_t
{
        GSM_GAME_INIT_ID                = 0x0A,
        GSM_LOGOUT_ID                   = 0x14,
        GSM_PING_ID                     = 0x1E,

        GSM_AUTO_WALK_ID                = 0x64,
        GSM_AUTO_WALK_CANCEL_ID         = 0x69,

        GSM_MOVE_NORTH_ID               = 0x65,
        GSM_MOVE_EAST_ID                = 0x66,
        GSM_MOVE_SOUTH_ID               = 0x67,
        GSM_MOVE_WEST_ID                = 0x68,
        GSM_MOVE_NE_ID                  = 0x6A,
        GSM_MOVE_SE_ID                  = 0x6B,
        GSM_MOVE_SW_ID                  = 0x6C,
        GSM_MOVE_NW_ID                  = 0x6D,

        GSM_TURN_NORTH_ID               = 0x6F,
        GSM_TURN_EAST_ID                = 0x70,
        GSM_TURN_SOUTH_ID               = 0x71,
        GSM_TURN_WEST_ID                = 0x72,

        GSM_MOVE_ITEM_ID                = 0x78,

        GSM_TRADE_ITEM_ID               = 0x7D,
        GSM_TRADE_LOOK_ID               = 0x7E,
        GSM_TRADE_ACCEPT_ID             = 0x7F,
        GSM_TRADE_REJECT_ID             = 0x80,

        GSM_USE_ITEM_ID                 = 0x82,
        GSM_USE_ITEM_WITH_ID            = 0x83,
        GSM_USE_BATTLE_WINDOW_ID        = 0x84,
        GSM_ROTATE_ITEM_ID              = 0x85,

        GSM_CONTAINER_CLOSE_ID          = 0x87,
        GSM_CONTAINER_UP_ID             = 0x88,

        GSM_TEXT_WINDOW_ID              = 0x89,
        GSM_TEXT_HOUSE_ID               = 0x8A,

        GSM_LOOK_ITEM_ID                = 0x8C,

        GSM_SPEAK_ID                    = 0x96,
        GSM_REQUEST_CHANNELS_ID         = 0x97,
        GSM_CHANNEL_OPEN_ID             = 0x98,
        GSM_CHANNEL_CLOSE_ID            = 0x99,
        GSM_PRIVATE_CHAT_OPEN_ID        = 0x9A,
        GSM_RULE_VIOLATION_OPEN_ID      = 0x9B,
        GSM_RULE_VIOLATION_CLOSE_ID     = 0x9C,
        GSM_RULE_VIOLATION_CANCEL_ID    = 0x9D,

        GSM_NPC_CHANNEL_CLOSE_ID        = 0x9E,

        GSM_PRIVATE_CHANNEL_CREATE_ID   = 0xAA,
        GSM_PRIVATE_CHANNEL_INVITE_ID   = 0xAB,
        GSM_PRIVATE_CHANNEL_BAN_ID      = 0xAC,

        GSM_FIGHT_MODES_ID              = 0xA0,

        GSM_CREATURE_ATTACK_ID          = 0xA1,
        GSM_CREATURE_FOLLOW_ID          = 0xA2,

        GSM_PARTY_INVITE_ID             = 0xA3,
        GSM_PARTY_JOIN_ID               = 0xA4,
        GSM_PARTY_CANCEL_ID             = 0xA5,
        GSM_PARTY_PASS_LEADERSHIP_ID    = 0xA6,
        GSM_PARTY_LEAVE_ID              = 0xA7,

        /* cancel follow / attack */
        GSM_CANCEL_MOVE_ID              = 0xBE,

        GSM_OUTFIT_REQUEST_ID           = 0xD2,
        GSM_OUTFIT_SET_ID               = 0xD3,

        GSM_VIP_ADD_ID                  = 0xDC,
        GSM_VIP_REMOVE_ID               = 0xDD,

        GSM_BUG_REPORT_ID               = 0xE6,

        GSM_QUEST_REQUEST_LOG_ID        = 0xF0,
        GSM_QUEST_REQUEST_ID            = 0xF1,

        GSM_SHOP_BUY_ID                 = 0x7A,
        GSM_SHOP_SELL_ID                = 0x7B,
        GSM_SHOP_CLOSE_ID               = 0x7C,

        GSM_REQUEST_TILE_UPDATE_ID      = 0xC9,
        GSM_REQUEST_CONTAINER_UPDATE_ID = 0xCA,
};

/******************************************************************
 * Enum of tibias game recieve
 ******************************************************************/

enum gr_message_id_t
{
        GRM_SELF_INFO_ID                = 0x0A,

        GRM_GM_ACTION_ID                = 0x0B,
        GRM_ERROR_ID                    = 0x14,
        GRM_FYI_ID                      = 0x15,
        GRM_QUEUE_ID                    = 0x16,
        GRM_PING_ID                     = 0x1E,
        GRM_LOGIN_WINDOW_ID             = 0x28,

        GRM_MAP_INIT_ID                 = 0x64,
        GRM_MAP_NORTH_ID                = 0x65,
        GRM_MAP_EAST_ID                 = 0x66,
        GRM_MAP_SOUTH_ID                = 0x67,
        GRM_MAP_WEST_ID                 = 0x68,

        GRM_MAP_UP_ID                   = 0xBE,
        GRM_MAP_DOWN_ID                 = 0xBF,

        GRM_TILE_SET_ID                 = 0x69,
        GRM_TILE_ADD_ID                 = 0x6A,
        GRM_TILE_UPDATE_ID              = 0x6B,
        GRM_TILE_REMOVE_ID              = 0x6C,

        GRM_CREATURE_MOVE_ID            = 0x6D,

        GRM_CONTAINER_OPEN_ID           = 0x6E,
        GRM_CONTAINER_CLOSE_ID          = 0x6F,
        GRM_CONT_ADD_ID                 = 0x70,
        GRM_CONT_UPDATE_ID              = 0x71,
        GRM_CONT_REMOVE_ID              = 0x72,

        GRM_SLOT_ITEM_ID                = 0x78,
        GRM_SLOT_CLEAR_ID               = 0x79,

        GRM_SHOP_TRADE_ID               = 0x7A,
        GRM_SHOP_GOLD_ID                = 0x7B,
        GRM_SHOP_CLOSE_ID               = 0x7C,

        GRM_OPEN_SELF_TRADE_ID          = 0x7D,
        GRM_OPEN_PLAYER_TRADE_ID        = 0x7E,
        GRM_CLOSE_TRADE_ID              = 0x7F,

        GRM_GLOBAL_LIGHT_ID             = 0x82,

        GRM_MAGIC_EFFECT_ID             = 0x83,
        GRM_ANIMATED_TEXT_ID            = 0x84,
        GRM_SHOOT_EFFECT_ID             = 0x85,

        GRM_CREATURE_SQUARE_ID          = 0x86,
        GRM_CREATURE_HEALTH_ID          = 0x8C,
        GRM_CREATURE_LIGHT_ID           = 0x8D,
        GRM_CREATURE_OUTFIT_ID          = 0x8E,
        GRM_CREATURE_SPEED_ID           = 0x8F,
        GRM_CREATURE_SKULL_ID           = 0x90,
        GRM_CREATURE_SHIELD_ID          = 0x91,

        GRM_TEXT_WINDOW_ID              = 0x96,
        GRM_TEXT_HOUSE_ID               = 0x97,

        GRM_PLAYER_STATS_ID             = 0xA0,
        GRM_PLAYER_SKILLS_ID            = 0xA1,
        GRM_PLAYER_ICONS_ID             = 0xA2,
        GRM_PLAYER_CANCEL_ATTACK_ID     = 0xA3,
        GRM_PLAYER_CANCEL_WALK_ID       = 0xB5,

        GRM_SPEAK_ID                    = 0xAA,
        GRM_CHANNEL_LIST_ID             = 0xAB,
        GRM_CHANNEL_OPEN_ID             = 0xAC,
        GRM_CHANNEL_PLAYER_ID           = 0xAD,
        GRM_CHANNEL_RULE_OPEN_ID        = 0xAE,
        GRM_CHANNEL_RULE_U1_ID          = 0xAF,
        GRM_CHANNEL_RULE_U2_ID          = 0xB0,
        GRM_CHANNEL_RULE_U3_ID          = 0xB1,
        GRM_CHANNEL_CHAT_ID             = 0xB2,
        GRM_CHANNEL_CLOSE_ID            = 0xB3,

        GRM_TEXT_MSG_ID                 = 0xB4,

        GRM_OUTFIT_WINDOW_ID            = 0xC8,

        GRM_VIP_INIT_ID                 = 0xD2,
        GRM_VIP_LOGIN_ID                = 0xD3,
        GRM_VIP_LOGOUT_ID               = 0xD4,

        GRM_SHOW_TUTORIAL_ID            = 0xDC,

        GRM_MINI_MAP_MARK_ID            = 0xDD,

        GRM_QUEST_LIST_ID               = 0xF0,
        GRM_SUB_QUEST_LIST_ID           = 0xF1,
};

/******************************************************************
 * Enum of tibias speak types
 * The following is taken from YACT sources
 ******************************************************************/
enum speak_id_t
{
        SPEAK_SAY               = 0x01, //normal talk
        SPEAK_WHISPER           = 0x02, //whispering - #w text
        SPEAK_YELL              = 0x03, //yelling - #y text
        SPEAK_PRIVATE_PN        = 0x04, //Player-to-NPC speaking(NPCs channel)
        SPEAK_PRIVATE_NP        = 0x05, //NPC-to-Player speaking
        SPEAK_PRIVATE           = 0x06, //Players speaking privately to players
        SPEAK_CHANNEL_Y         = 0x07, //Yellow message on chat
        //SPEAK_RVR_CHANNEL     = 0x08, //Reporting rule violation - Ctrl+R
        //SPEAK_RVR_ANSWER      = 0x09, //Answering report
        //SPEAK_RVR_CONTINUE= 0x0A  //Answering the answer of the report
        SPEAK_BROADCAST         = 0x0B, //Broadcast a message - #b
        SPEAK_CHANNEL_R1        = 0x0C, //Talk red on chat - #c
        SPEAK_PRIVATE_RED       = 0x0D, //Red private - @name@ text
        SPEAK_CHANNEL_O         = 0x0E, //Talk orange on text
        SPEAK_CHANNEL_R2        = 0x10, //Talk red anonymously on chat - #d
        //0x11
        SPEAK_MONSTER_SAY       = 0x12, //Talk orange
        SPEAK_MONSTER_YELL      = 0x13, //Yell orange
};

/******************************************************************
 * Enum of player icons
 * Thank you otserv, credit where its due
 ******************************************************************/
enum icons_t
{
	ICON_POISON     = 0x0001,
	ICON_BURN       = 0x0002,
	ICON_ENERGY     = 0x0004,
	ICON_DRUNK      = 0x0008,
	ICON_MANASHIELD = 0x0010,
	ICON_PARALYZE   = 0x0020,
	ICON_HASTE      = 0x0040,
	ICON_SWORDS     = 0x0080,
	ICON_DROWNING   = 0x0100,
	ICON_FREEZING   = 0x0200,
	ICON_DAZZLED    = 0x0400,
	ICON_CURSED     = 0x0800,
};

/******************************************************************
 * Directions
 ******************************************************************/

enum direction_t 
{
        DIRECTION_EAST  = 0x01,
        DIRECTION_NE    = 0x02,
        DIRECTION_NORTH = 0x03,
        DIRECTION_NW    = 0x04,
        DIRECTION_WEST  = 0x05,
        DIRECTION_SW    = 0x06,
        DIRECTION_SOUTH = 0x07, 
        DIRECTION_SE    = 0x08,
};

#endif
