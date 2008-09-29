#ifndef __TIBIA_MESSAGE_H
#define __TIBIA_MESSAGE_H

#include <stdint.h>
#include <string>
#include "tibiatypes.h"

class NetworkMessage;
class GameState;
class DatReader;

class TibiaMessage
{
        public:
                virtual void put (NetworkMessage* msg) = 0;
                virtual uint8_t getId () = 0;
                virtual void show () = 0;
                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat
                                 ) = 0;
};

//TODO add clone constructors
//for message naming, L = login, G = game, S = send, R = recv
class LSMLoginMsg : public TibiaMessage
{
        public:
                //note that this message reads 145 bytes from the buffer, ie it
                //reads the entire rsa buffer even if they are only random bytes
                LSMLoginMsg (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LSMLoginMsg (uint16_t OS, uint16_t version, uint32_t datsig,
                        uint32_t sprsig, uint32_t picsig, uint8_t u1,
                        uint32_t* xtea, uint32_t account, std::string password);

                virtual ~LSMLoginMsg ();
                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                uint16_t                getOS ();
                uint16_t                getVersion ();
                uint32_t                getDatsig ();
                uint32_t                getSprsig ();
                uint32_t                getPicSig ();
                uint8_t                 getU1 ();
                const uint32_t*         getXTEA ();
                uint32_t                getAccount ();
                const std::string&      getPassword (); 
                
                //im not exactly sure how deriving classes works, if this
                //can go into private it should
                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*   _id;
                TWord16*  _OS;
                TWord16*  _version;
                TWord32*  _datsig;
                TWord32*  _sprsig;
                TWord32*  _picsig;
                TWord8*   _u1;
                TXTEAKey* _xtea;
                TWord32*  _account;
                TString*  _password;
                
                //and somewhere to store the remaining bytes
                TByteBuffer* _bytes;
};

class LRMError : public TibiaMessage
{
        public:
                LRMError (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LRMError (std::string msg);

                virtual ~LRMError ();
                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:

                TWord8*  _id;
                TString* _errormsg;
};

class LRMInfo : public TibiaMessage
{
        public:
                LRMInfo (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LRMInfo (std::string msg);

                virtual ~LRMInfo ();
                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:

                TWord8*  _id;
                TString* _infomsg;
};

//message of the day
class LRMMOTD : public TibiaMessage
{
        public:
                LRMMOTD (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LRMMOTD (std::string msg);

                virtual ~LRMMOTD ();
                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:

                TWord8*  _id;
                TString* _motd;
};

class LRMCharacterList : public TibiaMessage
{
        public:
                LRMCharacterList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //this function takes control of the TCharacterList passed to it
                LRMCharacterList (TCharacterList* charlist, uint16_t daysprem);

                virtual ~LRMCharacterList ();

                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                TCharacterList* getCharList ();
                uint16_t getDaysPrem ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TCharacterList* _charlist;
                TWord16*        _daysprem;
};

class LRMPatchExe : public TibiaMessage
{
        public:
                LRMPatchExe (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LRMPatchExe ();

                virtual ~LRMPatchExe ();

                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
};

class LRMPatchDat : public TibiaMessage
{
        public:
                LRMPatchDat (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LRMPatchDat ();

                virtual ~LRMPatchDat ();

                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
};

class LRMPatchSpr : public TibiaMessage
{
        public:
                LRMPatchSpr (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LRMPatchSpr ();

                virtual ~LRMPatchSpr ();

                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
};

class LRMNewLoginServer : public TibiaMessage
{
        public:
                LRMNewLoginServer (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                LRMNewLoginServer ();

                virtual ~LRMNewLoginServer ();

                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
};

/******************************************************************
 * Game Send Messages
 ******************************************************************/

class GSMGameInit : public TibiaMessage
{
        public:
                //note that this message reads 137 bytes from the buffer, ie it
                //reads the entire rsa buffer even if they are only random bytes
                GSMGameInit (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GSMGameInit (uint16_t OS, uint16_t version, uint8_t u1,
                        uint32_t* xtea, uint8_t isGM, uint32_t account,
                        std::string password, std::string name);

                virtual ~GSMGameInit ();
                virtual uint8_t getId ();
                virtual void put (NetworkMessage* msg);
                virtual void show ();

                uint16_t                getOS ();
                uint16_t                getVersion ();
                uint8_t                 getU1 ();
                const uint32_t*         getXTEA ();
                uint8_t                 getIsGM ();
                uint32_t                getAccount ();
                const std::string&      getName (); 
                const std::string&      getPassword (); 
                
                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*   _id;
                TWord16*  _OS;
                TWord16*  _version;
                TWord8*   _u1;
                TXTEAKey* _xtea;
                TWord8*   _isGM;
                TWord32*  _account;
                TString*  _name;
                TString*  _password;
                
                //and somewhere to store the remaining bytes
                TByteBuffer* _bytes;
};


/******************************************************************
 * Game Recv Messages
 ******************************************************************/

/***************************************************************
 * GMAction
 ***************************************************************/

class GRMGMAction : public TibiaMessage
{
        public:
                GRMGMAction (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMGMAction (uint8_t* actions);
                GRMGMAction (const GRMGMAction& clone);
                virtual ~GRMGMAction ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getAction (uint32_t n);

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TWord8* _dump[32];
};

/***************************************************************
 * Error
 ***************************************************************/

class GRMError : public TibiaMessage
{
        public:
                GRMError (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMError (const std::string& msg);
                GRMError (const GRMError& clone);
                virtual ~GRMError ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TString* _msg;
};

/***************************************************************
 * FYI
 ***************************************************************/

class GRMFYI : public TibiaMessage
{
        public:
                GRMFYI (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMFYI (const std::string& msg);
                GRMFYI (const GRMFYI& clone);
                virtual ~GRMFYI ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const std::string& getMsg ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TString* _msg;
};

/***************************************************************
 * Queue
 ***************************************************************/

class GRMQueue : public TibiaMessage
{
        public:
                GRMQueue (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMQueue (const std::string& msg, uint8_t time);
                GRMQueue (const GRMQueue& clone);
                virtual ~GRMQueue ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const std::string& getMsg ();
                uint8_t getTime ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TString* _msg;
                TWord8*  _time;
};

/***************************************************************
 * Ping
 ***************************************************************/

class GRMPing : public TibiaMessage
{
        public:
                GRMPing (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMPing ();
                GRMPing (const GRMPing& clone);
                virtual ~GRMPing ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
};

/***************************************************************
 * LoginWindow
 ***************************************************************/

class GRMLoginWindow : public TibiaMessage
{
        public:
                GRMLoginWindow (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMLoginWindow ();
                GRMLoginWindow (const GRMLoginWindow& clone);
                virtual ~GRMLoginWindow ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
};

/***************************************************************
 * GRMSelfInfo
 ***************************************************************/

class GRMSelfInfo : public TibiaMessage
{
        public:
                GRMSelfInfo (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMSelfInfo (uint32_t tibiaId, uint8_t u1, 
                                uint8_t u2, uint8_t reportErrors);
                GRMSelfInfo (const GRMSelfInfo& clone);
                virtual ~GRMSelfInfo ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getTibiaId ();
                uint8_t  getU1 ();
                uint8_t  getU2 ();
                uint8_t  reportErrors ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord32* _tibiaId;
                TWord8*  _u1;
                TWord8*  _u2;
                TWord8*  _reportErrors;
};

/***************************************************************
 * GRMSlotItem
 ***************************************************************/

class GRMSlotItem : public TibiaMessage
{
        public:
                GRMSlotItem (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMSlotItem (uint8_t slot, const TThing& thing);
                GRMSlotItem (const GRMSlotItem& clone);
                virtual ~GRMSlotItem ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getSlot ();
                const TThing& getItem ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TWord8* _slot;
                TThing* _thing;
};

/***************************************************************
 * GRMSlotClear
 ***************************************************************/

class GRMSlotClear : public TibiaMessage
{
        public:
                GRMSlotClear (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMSlotClear (uint8_t slot);
                GRMSlotClear (const GRMSlotClear& clone);
                virtual ~GRMSlotClear ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getSlot ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TWord8* _slot;
};

/***************************************************************
 * GRMMagicEffect
 ***************************************************************/

class GRMMagicEffect : public TibiaMessage
{
        public:
                GRMMagicEffect (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMMagicEffect (const TPos& pos, uint8_t effect);
                GRMMagicEffect (const GRMMagicEffect& clone);
                virtual ~GRMMagicEffect ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getEffect () const;
                const TPos& getPos () const;

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TPos* _pos;
                TWord8* _effect;
};
        
/***************************************************************
 * GRMTextMsg
 ***************************************************************/

class GRMTextMsg : public TibiaMessage
{
        public:
                GRMTextMsg (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMTextMsg (const TTextMsg& msg);
                GRMTextMsg (const GRMTextMsg& clone);
                virtual ~GRMTextMsg ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TTextMsg& getLight ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TTextMsg*       _msg;
};

/***************************************************************
 * GRMGlobalLight
 ***************************************************************/

class GRMGlobalLight : public TibiaMessage
{
        public:
                GRMGlobalLight (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMGlobalLight (const TCreatureLight& light);
                GRMGlobalLight (const GRMGlobalLight& clone);
                virtual ~GRMGlobalLight ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TCreatureLight& getLight ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TCreatureLight* _light;

};

/***************************************************************
 * GRMTextWindow
 ***************************************************************/

class GRMTextWindow : public TibiaMessage
{
        public:
                GRMTextWindow (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMTextWindow (uint32_t           windowid,
                               const TThing&      item,
                               uint16_t           maxlen,
                               const std::string& text,
                               const std::string& writer,
                               const std::string& date);

                GRMTextWindow (const GRMTextWindow& clone);
                virtual ~GRMTextWindow ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t                getWindowId ();
                const TThing&           getItem ();
                uint16_t                getMaxLen ();
                const std::string&      getText ();
                const std::string&      getWriter ();
                const std::string&      getDate ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TWord32*        _windowid;
                TThing*         _item;
                TWord16*        _maxlen;
                TString*        _text;
                TString*        _writer;
                TString*        _date;
};

/***************************************************************
 * GRMTextHouse
 ***************************************************************/

class GRMTextHouse : public TibiaMessage
{
        public:
                GRMTextHouse (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMTextHouse ( uint8_t            u1,
                               uint32_t           windowId,
                               const std::string& text);

                GRMTextHouse (const GRMTextHouse& clone);
                virtual ~GRMTextHouse ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t                 getU1 ();
                uint32_t                getWindowId ();
                const std::string&      getText ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TWord8*         _u1;
                TWord32*        _windowid;
                TString*        _text;
};

/***************************************************************
 * GRMCreatureLight
 ***************************************************************/

class GRMCreatureLight : public TibiaMessage
{
        public:
                GRMCreatureLight (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMCreatureLight (uint32_t tibiaId, const TCreatureLight& light);
                GRMCreatureLight (const GRMCreatureLight& clone);
                virtual ~GRMCreatureLight ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getTibiaId ();
                const TCreatureLight& getLight ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TWord32*        _tibiaId;
                TCreatureLight* _light;

};

/***************************************************************
 * GRMPlayerStats
 ***************************************************************/

class GRMPlayerStats : public TibiaMessage
{
        public:
                GRMPlayerStats (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMPlayerStats (const TPlayerStats& stats);
                GRMPlayerStats (const GRMPlayerStats& clone);
                virtual ~GRMPlayerStats ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPlayerStats& getStats ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TPlayerStats*   _stats;
};

/*************************************************************************
 * TPlayerSkills
 *************************************************************************/

class GRMPlayerSkills : public TibiaMessage
{
        public:
                GRMPlayerSkills (NetworkMessage* msg, GameState* gs,
                              DatReader* dat);
                GRMPlayerSkills (const TPlayerSkills& skills);
                GRMPlayerSkills (const GRMPlayerSkills& clone);
                virtual ~GRMPlayerSkills ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPlayerSkills& getSkills ();

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*         _id;
                TPlayerSkills*  _skills;
};

/***************************************************************
 * MapMessages
 ***************************************************************/
/***************************************************************
 * GRMMapInit
 ***************************************************************/
class GRMMapInit : public TibiaMessage
{
        public:
                GRMMapInit (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //note this function takes control of map
                GRMMapInit (const TPos& initPos, TMapDescription* map);
                GRMMapInit (const GRMMapInit& clone);
                virtual ~GRMMapInit ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPos& getPos ();
                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TPos* _pos;
                TMapDescription* _map;
};

/***************************************************************
 * GRMMapNorth
 ***************************************************************/
class GRMMapNorth : public TibiaMessage
{
        public:
                GRMMapNorth (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //note this function takes control of map
                GRMMapNorth (TMapDescription* map);
                GRMMapNorth (const GRMMapNorth& clone);
                virtual ~GRMMapNorth ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TMapDescription* _map;
};

/***************************************************************
 * GRMMapEast
 ***************************************************************/
class GRMMapEast : public TibiaMessage
{
        public:
                GRMMapEast (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //note this function takes control of map
                GRMMapEast (TMapDescription* map);
                GRMMapEast (const GRMMapEast& clone);
                virtual ~GRMMapEast ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TMapDescription* _map;
};

/***************************************************************
 * GRMMapSouth
 ***************************************************************/
class GRMMapSouth : public TibiaMessage
{
        public:
                GRMMapSouth (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //note this function takes control of map
                GRMMapSouth (TMapDescription* map);
                GRMMapSouth (const GRMMapSouth& clone);
                virtual ~GRMMapSouth ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TMapDescription* _map;
};

/***************************************************************
 * GRMMapWest
 ***************************************************************/
class GRMMapWest : public TibiaMessage
{
        public:
                GRMMapWest (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //note this function takes control of map
                GRMMapWest (TMapDescription* map);
                GRMMapWest (const GRMMapWest& clone);
                virtual ~GRMMapWest ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TMapDescription* _map;
};

/***************************************************************
 * GRMMapUp
 ***************************************************************/
class GRMMapUp : public TibiaMessage
{
        public:
                GRMMapUp (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //use this constructor for no map
                GRMMapUp ();
                //note this function takes control of map
                GRMMapUp (TMapDescription* map);
                GRMMapUp (const GRMMapUp& clone);
                virtual ~GRMMapUp ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                //warning not all floor change messages have maps, ask
                //first or be segfaulted
                bool hasMap ();
                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                bool _hasmap;

                TWord8* _id;
                TMapDescription* _map;
};

/***************************************************************
 * GRMMapDown
 ***************************************************************/
class GRMMapDown : public TibiaMessage
{
        public:
                GRMMapDown (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //use this constructor for no map
                GRMMapDown ();
                //note this function takes control of map
                GRMMapDown (TMapDescription* map);
                GRMMapDown (const GRMMapDown& clone);
                virtual ~GRMMapDown ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                //warning not all floor change messages have maps, ask
                //first or be segfaulted
                bool hasMap ();
                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                bool _hasmap;

                TWord8* _id;
                TMapDescription* _map;
};

/***************************************************************
 * GRMTileSet
 ***************************************************************/
class GRMTileSet : public TibiaMessage
{
        public:
                GRMTileSet (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //note this function takes control of map
                GRMTileSet (const TPos& pos, TMapDescription* map);
                GRMTileSet (const GRMTileSet& clone);
                virtual ~GRMTileSet ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPos& getPos ();
                TMapDescription& getMap ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TPos* _pos;
                TMapDescription* _map;
};

/***************************************************************
 * GRMTileAdd
 ***************************************************************/

class GRMTileAdd : public TibiaMessage
{
        public:
                GRMTileAdd (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMTileAdd (const TPos& pos, const TThing& thing);
                GRMTileAdd (const GRMTileAdd& clone);
                virtual ~GRMTileAdd ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPos& getPos ();
                const TThing& getThing ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
        private:
                TWord8* _id;
                TPos*   _pos;
                TThing* _thing;
};

/***************************************************************
 * GRMTileUpdate
 ***************************************************************/

class GRMTileUpdate : public TibiaMessage
{
        public:
                GRMTileUpdate (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMTileUpdate (const TPos& pos, uint8_t stackpos, 
                                const TThing& thing);
                GRMTileUpdate (const GRMTileUpdate& clone);
                virtual ~GRMTileUpdate ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPos& getPos ();
                uint8_t getStackPos ();
                const TThing& getThing ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
        private:
                TWord8* _id;
                TPos*   _pos;
                TWord8* _stackpos;
                TThing* _thing;
};

/***************************************************************
 * GRMTileRemove
 ***************************************************************/

class GRMTileRemove : public TibiaMessage
{
        public:
                GRMTileRemove (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMTileRemove (const TPos& pos, uint8_t stackpos);
                GRMTileRemove (const GRMTileRemove& clone);
                virtual ~GRMTileRemove ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPos& getPos ();
                uint8_t getStackPos ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
        private:
                TWord8* _id;
                TPos*   _pos;
                TWord8* _stackpos;
};

/***************************************************************
 * GRMCreatureMove
 ***************************************************************/

class GRMCreatureMove : public TibiaMessage
{
        public:
                GRMCreatureMove (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCreatureMove (const TPos& from, uint8_t stackpos,
                                        const TPos& to);
                GRMCreatureMove (const GRMCreatureMove& clone);
                virtual ~GRMCreatureMove ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPos& getFrom ();
                const TPos& getTo ();
                uint8_t getStackPos ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
        private:
                TWord8* _id;
                TPos*   _from;
                TWord8* _stackpos;
                TPos*   _to;
};

/***************************************************************
 * ContainerMessages
 ***************************************************************/
/***************************************************************
 * OpenContainer
 ***************************************************************/
class GRMOpenContainer : public TibiaMessage
{
        public:
                GRMOpenContainer (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMOpenContainer (uint8_t cid, const TContainer& container);
                GRMOpenContainer (const GRMOpenContainer& clone);

                virtual ~GRMOpenContainer ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getCid ();
                TContainer& getContainer ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord8* _cid;
                TContainer* _container;
};

/***************************************************************
 * CloseContainer
 ***************************************************************/
class GRMCloseContainer : public TibiaMessage
{
        public:
                GRMCloseContainer (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCloseContainer (uint8_t cid);
                GRMCloseContainer (const GRMCloseContainer& clone);

                virtual ~GRMCloseContainer ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getCid ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord8* _cid;
};

/***************************************************************
 * ContainerAdd
 ***************************************************************/
class GRMContainerAdd : public TibiaMessage
{
        public:
                GRMContainerAdd (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMContainerAdd (uint8_t cid, const TThing& item);
                GRMContainerAdd (const GRMContainerAdd& clone);

                virtual ~GRMContainerAdd ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getCid ();
                const TThing& getItem ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord8* _cid;
                TThing* _item;
};

/***************************************************************
 * ContainerUpdate
 ***************************************************************/
class GRMContainerUpdate : public TibiaMessage
{
        public:
                GRMContainerUpdate (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMContainerUpdate (uint8_t cid, uint8_t slot,
                                        const TThing& item);
                GRMContainerUpdate (const GRMContainerUpdate& clone);

                virtual ~GRMContainerUpdate ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getCid ();
                uint8_t getSlot ();
                const TThing& getItem ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord8* _cid;
                TWord8* _slot;
                TThing* _item;
};

/***************************************************************
 * ContainerRemove
 ***************************************************************/
class GRMContainerRemove : public TibiaMessage
{
        public:
                GRMContainerRemove (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMContainerRemove (uint8_t cid, uint8_t slot);
                GRMContainerRemove (const GRMContainerRemove& clone);

                virtual ~GRMContainerRemove ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getCid ();
                uint8_t getSlot ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord8* _cid;
                TWord8* _slot;
};

/***************************************************************
 * OpenSelfTrade - your trade window
 ***************************************************************/
class GRMOpenSelfTrade : public TibiaMessage
{
        public:
                GRMOpenSelfTrade (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMOpenSelfTrade (const TTradeContainer& container);
                GRMOpenSelfTrade (const GRMOpenSelfTrade& clone);

                virtual ~GRMOpenSelfTrade ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TTradeContainer& getTradeContainer ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TTradeContainer* _container;
};

/***************************************************************
 * OpenPlayerTrade - other players trade window
 ***************************************************************/
class GRMOpenPlayerTrade : public TibiaMessage
{
        public:
                GRMOpenPlayerTrade (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMOpenPlayerTrade (const TTradeContainer& container);
                GRMOpenPlayerTrade (const GRMOpenPlayerTrade& clone);

                virtual ~GRMOpenPlayerTrade ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TTradeContainer& getTradeContainer ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TTradeContainer* _container;
};

/***************************************************************
 * CloseTrade - other players trade window
 ***************************************************************/
class GRMCloseTrade : public TibiaMessage
{
        public:
                GRMCloseTrade (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCloseTrade ();
                GRMCloseTrade (const GRMCloseTrade& clone);

                virtual ~GRMCloseTrade ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
};

/***************************************************************
 * CreatureMessages
 ***************************************************************/
/***************************************************************
 * CreatureSquare
 ***************************************************************/

class GRMCreatureSquare : public TibiaMessage
{
        public:
                GRMCreatureSquare (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCreatureSquare (uint32_t creatureid, uint8_t color);
                GRMCreatureSquare (const GRMCreatureSquare& clone);

                virtual ~GRMCreatureSquare ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();
                uint8_t  getColor ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord32* _creatureid;
                TWord8* _color;
};

/***************************************************************
 * CreatureHealth
 ***************************************************************/

class GRMCreatureHealth : public TibiaMessage
{
        public:
                GRMCreatureHealth (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCreatureHealth (uint32_t creatureid, uint8_t hp);
                GRMCreatureHealth (const GRMCreatureHealth& clone);

                virtual ~GRMCreatureHealth ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();
                uint8_t  getHp ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord32* _creatureid;
                TWord8* _hp;
};

/***************************************************************
 * CreatureOutfit
 ***************************************************************/

class GRMCreatureOutfit : public TibiaMessage
{
        public:
                GRMCreatureOutfit (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCreatureOutfit (uint32_t creatureid, const TOutfit& outfit);
                GRMCreatureOutfit (const GRMCreatureOutfit& clone);

                virtual ~GRMCreatureOutfit ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();
                const TOutfit& getOutfit ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord32* _creatureid;
                TOutfit* _outfit;
};

/***************************************************************
 * CreatureSpeed
 ***************************************************************/

class GRMCreatureSpeed : public TibiaMessage
{
        public:
                GRMCreatureSpeed (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCreatureSpeed (uint32_t creatureid, uint8_t speed);
                GRMCreatureSpeed (const GRMCreatureSpeed& clone);

                virtual ~GRMCreatureSpeed ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();
                uint8_t  getSpeed ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord32* _creatureid;
                TWord8* _speed;
};

/***************************************************************
 * CreatureSkull
 ***************************************************************/

class GRMCreatureSkull : public TibiaMessage
{
        public:
                GRMCreatureSkull (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCreatureSkull (uint32_t creatureid, uint8_t skull);
                GRMCreatureSkull (const GRMCreatureSkull& clone);

                virtual ~GRMCreatureSkull ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();
                uint8_t  getSkull ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord32* _creatureid;
                TWord8* _skull;
};

/***************************************************************
 * CreatureShield
 ***************************************************************/

class GRMCreatureShield : public TibiaMessage
{
        public:
                GRMCreatureShield (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMCreatureShield (uint32_t creatureid, uint8_t shield);
                GRMCreatureShield (const GRMCreatureShield& clone);

                virtual ~GRMCreatureShield ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();
                uint8_t  getShield ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TWord32* _creatureid;
                TWord8* _shield;
};

/***************************************************************
 * AnimatedText
 ***************************************************************/

class GRMAnimatedText : public TibiaMessage
{
        public:
                GRMAnimatedText (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMAnimatedText (const TPos& pos, uint8_t color,
                                 const std::string& msg);
                GRMAnimatedText (const GRMAnimatedText& text);

                virtual ~GRMAnimatedText ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getColor ();
                const TPos& getPos ();
                const std::string& getText ();

                virtual void get (NetworkMessage* msg,
                                        GameState* gs, 
                                        DatReader* dat);

        private:
                TWord8* _id;
                TPos* _pos;
                TWord8* _color;
                TString* _text;
};

/***************************************************************
 * PlayerCancelAttack
 ***************************************************************/

class GRMPlayerCancelAttack : public TibiaMessage
{
        public:
                GRMPlayerCancelAttack (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMPlayerCancelAttack ();
                GRMPlayerCancelAttack (const GRMPlayerCancelAttack& clone);
                virtual ~GRMPlayerCancelAttack ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
};

/***************************************************************
 * ShopTrade
 ***************************************************************/

class GRMShopTrade : public TibiaMessage
{
        public:
                GRMShopTrade (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //This constructor takes control of shoplist
                GRMShopTrade (TShopList* shoplist);
                GRMShopTrade (const GRMShopTrade& clone);
                virtual ~GRMShopTrade ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TShopList& getShopList ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TShopList* _shoplist;
};

/***************************************************************
 * ShopGold
 ***************************************************************/

class GRMShopGold : public TibiaMessage
{
        public:
                GRMShopGold (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMShopGold (uint32_t ngold);
                GRMShopGold (const GRMShopGold& clone);
                virtual ~GRMShopGold ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getNGold ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TWord32* _ngold;
};

/***************************************************************
 * ShopClose
 ***************************************************************/

class GRMShopClose : public TibiaMessage
{
        public:
                GRMShopClose (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMShopClose ();
                GRMShopClose (const GRMShopClose& clone);
                virtual ~GRMShopClose ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
};

/***************************************************************
 * GRMShootEffect
 ***************************************************************/

class GRMShootEffect : public TibiaMessage
{
        public:
                GRMShootEffect (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat);
                GRMShootEffect (const TPos& start, const TPos& end,
                                        uint8_t effect);
                GRMShootEffect (const GRMShootEffect& clone);
                virtual ~GRMShootEffect ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getEffect () const;
                const TPos& getStart () const;
                const TPos& getEnd () const;

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TPos* _start;
                TPos* _end;
                TWord8* _effect;
};
        
/***************************************************************
 * PlayerIcons
 ***************************************************************/

class GRMPlayerIcons : public TibiaMessage
{
        public:
                GRMPlayerIcons (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMPlayerIcons (uint16_t icons);
                GRMPlayerIcons (const GRMPlayerIcons& clone);
                virtual ~GRMPlayerIcons ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint16_t getIcons ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TWord16* _icons;
};

/***************************************************************
 * OutfitWindow
 ***************************************************************/

class GRMOutfitWindow : public TibiaMessage
{
        public:
                GRMOutfitWindow (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMOutfitWindow (const TOutfit& self, TOutfitList* outfits);
                GRMOutfitWindow (const GRMOutfitWindow& clone);
                virtual ~GRMOutfitWindow ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TOutfit& getSelf ();
                TOutfitList& getOutfits ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TOutfit* _self;
                TOutfitList* _outfits;
};

/***************************************************************
 * VipInit
 ***************************************************************/

class GRMVipInit : public TibiaMessage
{
        public:
                GRMVipInit (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMVipInit (uint32_t creatureid, const std::string& name,
                                uint8_t online);
                GRMVipInit (const GRMVipInit& clone);
                virtual ~GRMVipInit ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();
                const std::string& getName ();
                uint8_t getOnline ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord32* _creatureid;
                TString* _name;
                TWord8*  _online;
};

/***************************************************************
 * VipLogin
 ***************************************************************/

class GRMVipLogin : public TibiaMessage
{
        public:
                GRMVipLogin (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMVipLogin (uint32_t creatureid);
                GRMVipLogin (const GRMVipLogin& clone);
                virtual ~GRMVipLogin ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord32* _creatureid;
};

/***************************************************************
 * VipLogout
 ***************************************************************/

class GRMVipLogout : public TibiaMessage
{
        public:
                GRMVipLogout (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMVipLogout (uint32_t creatureid);
                GRMVipLogout (const GRMVipLogout& clone);
                virtual ~GRMVipLogout ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint32_t getCreatureId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord32* _creatureid;
};

/***************************************************************
 * ShowTutorial
 ***************************************************************/

class GRMShowTutorial : public TibiaMessage
{
        public:
                GRMShowTutorial (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMShowTutorial (uint8_t tutorialid);
                GRMShowTutorial (const GRMShowTutorial& clone);
                virtual ~GRMShowTutorial ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getTutorialId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TWord8* _tutorialid;
};

/***************************************************************
 * MiniMapMark
 ***************************************************************/

class GRMMiniMapMark : public TibiaMessage
{
        public:
                GRMMiniMapMark (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMMiniMapMark (const TPos& pos, uint8_t icon, 
                                        const std::string& description);
                GRMMiniMapMark (const GRMMiniMapMark& clone);
                virtual ~GRMMiniMapMark ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TPos& getPos ();
                uint8_t getIcon ();
                const std::string& getDescription ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TPos*    _pos;
                TWord8*  _icon;
                TString* _description;
};

/***************************************************************
 * QuestList
 ***************************************************************/

class GRMQuestList : public TibiaMessage
{
        public:
                GRMQuestList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //This constructor takes control of questlist
                GRMQuestList (TQuestList* questlist);
                GRMQuestList (const GRMQuestList& clone);
                virtual ~GRMQuestList ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                TQuestList& getQuestList ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TQuestList* _questlist;
};

/***************************************************************
 * SubQuestList
 ***************************************************************/

class GRMSubQuestList : public TibiaMessage
{
        public:
                GRMSubQuestList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //This constructor takes control of questlist
                GRMSubQuestList (uint16_t questid, TSubQuestList* questlist);
                GRMSubQuestList (const GRMSubQuestList& clone);
                virtual ~GRMSubQuestList ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint16_t getQuestId ();
                TSubQuestList& getSubQuestList ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;

                TWord16* _questid;
                TSubQuestList* _subquestlist;
};

/***************************************************************
 * ChannelList
 ***************************************************************/

class GRMChannelList : public TibiaMessage
{
        public:
                GRMChannelList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                //This constructor takes control of questlist
                GRMChannelList (TChannelList* questlist);
                GRMChannelList (const GRMChannelList& clone);
                virtual ~GRMChannelList ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint16_t getQuestId ();
                TChannelList& getChannelList();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;

                TChannelList* _channellist;
};

/***************************************************************
 * ChannelOpen
 ***************************************************************/

class GRMChannelOpen : public TibiaMessage
{
        public:
                GRMChannelOpen (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelOpen (uint16_t channelid, const std::string& name);
                GRMChannelOpen (const GRMChannelOpen& clone);
                virtual ~GRMChannelOpen ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint16_t getChannelId ();
                const std::string& getName ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord16* _channelid;
                TString* _name;
};

/***************************************************************
 * ChannelPlayer
 ***************************************************************/

class GRMChannelPlayer : public TibiaMessage
{
        public:
                GRMChannelPlayer (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelPlayer (const std::string& name);
                GRMChannelPlayer (const GRMChannelPlayer& clone);
                virtual ~GRMChannelPlayer ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const std::string& getName ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TString* _name;
};

/***************************************************************
 * ChannelRuleOpen
 ***************************************************************/

class GRMChannelRuleOpen : public TibiaMessage
{
        public:
                GRMChannelRuleOpen (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelRuleOpen (uint16_t channelid);
                GRMChannelRuleOpen (const GRMChannelRuleOpen& clone);
                virtual ~GRMChannelRuleOpen ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint16_t getChannelId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord16* _channelid;
};

/***************************************************************
 * ChannelRuleU1
 ***************************************************************/

class GRMChannelRuleU1 : public TibiaMessage
{
        public:
                GRMChannelRuleU1 (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelRuleU1 (const std::string& string);
                GRMChannelRuleU1 (const GRMChannelRuleU1& clone);
                virtual ~GRMChannelRuleU1 ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const std::string& getString ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TString* _string;
};

/***************************************************************
 * ChannelRuleU2
 ***************************************************************/

class GRMChannelRuleU2 : public TibiaMessage
{
        public:
                GRMChannelRuleU2 (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelRuleU2 (const std::string& string);
                GRMChannelRuleU2 (const GRMChannelRuleU2& clone);
                virtual ~GRMChannelRuleU2 ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const std::string& getString ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TString* _string;
};

/***************************************************************
 * ChannelRuleU3
 ***************************************************************/

class GRMChannelRuleU3 : public TibiaMessage
{
        public:
                GRMChannelRuleU3 (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelRuleU3 ();
                GRMChannelRuleU3 (const GRMChannelRuleU3& clone);
                virtual ~GRMChannelRuleU3 ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
};

/***************************************************************
 * ChannelChat
 ***************************************************************/

class GRMChannelChat : public TibiaMessage
{
        public:
                GRMChannelChat (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelChat (uint16_t channelid, const std::string& name);
                GRMChannelChat (const GRMChannelChat& clone);
                virtual ~GRMChannelChat ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint16_t getChannelId ();
                const std::string& getName ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord16* _channelid;
                TString* _name;
};

/***************************************************************
 * ChannelClose
 ***************************************************************/

class GRMChannelClose : public TibiaMessage
{
        public:
                GRMChannelClose (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMChannelClose (uint16_t channelid);
                GRMChannelClose (const GRMChannelClose& clone);
                virtual ~GRMChannelClose ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint16_t getChannelId ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8*  _id;
                TWord16* _channelid;
};

/***************************************************************
 * PlayerCancelWalk
 ***************************************************************/

class GRMPlayerCancelWalk : public TibiaMessage
{
        public:
                GRMPlayerCancelWalk (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMPlayerCancelWalk (uint8_t direction);
                GRMPlayerCancelWalk (const GRMPlayerCancelWalk& clone);
                virtual ~GRMPlayerCancelWalk ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                uint8_t getDirection ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TWord8* _direction;
};

/***************************************************************
 * Speak
 ***************************************************************/

class GRMSpeak : public TibiaMessage
{
        public:
                GRMSpeak (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMSpeak (const TSpeak& speak);
                GRMSpeak (const GRMSpeak& clone);
                virtual ~GRMSpeak ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TSpeak& getSpeak ();

                virtual void get (NetworkMessage* msg,
                                  GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TSpeak* _speak;
};

#endif

