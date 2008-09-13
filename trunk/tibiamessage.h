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
                GRMMagicEffect (const TEffect& effect);
                GRMMagicEffect (const GRMMagicEffect& clone);
                virtual ~GRMMagicEffect ();

                virtual void put (NetworkMessage* msg);
                virtual void show ();
                virtual uint8_t getId ();

                const TEffect& getEffect () const;

                virtual void get (NetworkMessage* msg, GameState* gs,
                                  DatReader* dat);
        private:
                TWord8* _id;
                TEffect* _effect;
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
#endif

