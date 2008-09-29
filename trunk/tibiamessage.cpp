#include <stdio.h>
#include "tibiamessage.h"
#include "gamestate.h"
#include "messageids.h"

//only for testing
#include "networkmessage.h"
//

#define RSA_LEN 128
//TODO check this really is the max floor
#define MAX_FLOOR 15

#define MIN(a,b) ((a)<(b)?a:b)

//LSMLoginMsg
LSMLoginMsg::LSMLoginMsg (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

LSMLoginMsg::LSMLoginMsg (uint16_t OS, uint16_t version, uint32_t datsig,
        uint32_t sprsig, uint32_t picsig, uint8_t u1, uint32_t* xtea,
        uint32_t account, std::string password)
{
        _id = new TWord8 ((uint8_t)0x01);

        _OS = new TWord16 (OS);
        _version = new TWord16 (version);
        _datsig = new TWord32 (datsig);
        _sprsig = new TWord32 (sprsig);
        _picsig = new TWord32 (picsig);
        _u1 = new TWord8 ((uint8_t)0);
        _xtea = new TXTEAKey (xtea);
        _account = new TWord32 (account);
        _password = new TString (password);
        //the TByteBuffer (uint32_t len) constructer automatically
        //generates a random byte buffer
        uint32_t rem = RSA_LEN - (23 + _password->getLen ());
        _bytes = new TByteBuffer (rem);
}

LSMLoginMsg::~LSMLoginMsg ()
{
        delete _id;
        delete _OS; 
        delete _version; 
        delete _datsig;
        delete _sprsig;
        delete _picsig;
        delete _u1;
        delete _xtea;
        delete _account;
        delete _password;
        delete _bytes;
}

uint8_t LSMLoginMsg::getId ()
{
        return _id->getVal ();
}

uint32_t LSMLoginMsg::getAccount ()
{
        return _account->getVal ();
}

const std::string& LSMLoginMsg::getPassword ()
{
        return _password->getString ();
}

void LSMLoginMsg::put (NetworkMessage* msg)
{
        _id->put (msg);
        _OS->put (msg);
        _version->put (msg);
        _datsig->put (msg);
        _sprsig->put (msg);
        _picsig->put (msg);
        _u1->put (msg);
        _xtea->put (msg);
        _account->put (msg);
        _password->put (msg);
        _bytes->put (msg);
}

void LSMLoginMsg::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _OS = new TWord16 (msg);
        _version = new TWord16 (msg);
        _datsig = new TWord32 (msg);
        _sprsig = new TWord32 (msg);
        _picsig = new TWord32 (msg);
        _u1 = new TWord8 (msg);
        _xtea = new TXTEAKey (msg);
        _account = new TWord32 (msg);
        _password = new TString (msg);
        //this time we just take the random bytes from the packet
        uint32_t rem = RSA_LEN - (23 + _password->getLen ());
        _bytes = new TByteBuffer (rem, msg);
}

const uint32_t* LSMLoginMsg::getXTEA ()
{
        return _xtea->getKey ();
}
        
void LSMLoginMsg::show ()
{
        printf ("LSMLoginMsg {\n");
        printf ("\tOS: "); _OS->show (); printf ("\n");
        printf ("\tversion: "); _version->show (); printf ("\n");
        printf ("\tdatsig: "); _datsig->show (); printf ("\n");
        printf ("\tsprsig: "); _sprsig->show (); printf ("\n");
        printf ("\tpicsig: "); _picsig->show (); printf ("\n");
        printf ("\tu1: "); _u1->show (); printf ("\n");
        printf ("\txtea: "); _xtea->show (); printf ("\n");
        printf ("\taccount: "); _account->show (); printf ("\n");
        printf ("\tpassword: "); _password->show (); printf ("\n");
        printf ("\tbytes: "); _bytes->show (); printf ("\n");
        printf ("}\n");
}

//LRMError
LRMError::LRMError (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

LRMError::LRMError (std::string msg)
{
        _id = new TWord8 ((uint8_t)0x0A);
        _errormsg = new TString (msg);
}

LRMError::~LRMError ()
{
        delete _id;
        delete _errormsg;
}

uint8_t LRMError::getId ()
{
        return _id->getVal ();
}

void LRMError::put (NetworkMessage* msg)
{
        _id->put (msg);
        _errormsg->put (msg);
}

void LRMError::get (NetworkMessage* msg,
                        GameState* gs,
                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _errormsg = new TString (msg);
}

void LRMError::show ()
{
        printf ("LRMError {\n");
        printf ("\tmessage: "); _errormsg->show (); printf ("\n");
        printf ("}\n");
}

//LRMInfo
LRMInfo::LRMInfo (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

LRMInfo::LRMInfo (std::string msg)
{
        _id = new TWord8 ((uint8_t)0x0B);
        _infomsg = new TString (msg);
}

LRMInfo::~LRMInfo ()
{
        delete _id;
        delete _infomsg;
}

uint8_t LRMInfo::getId ()
{
        return _id->getVal ();
}

void LRMInfo::put (NetworkMessage* msg)
{
        _id->put (msg);
        _infomsg->put (msg);
}

void LRMInfo::get (NetworkMessage* msg,
                        GameState* gs,
                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _infomsg = new TString (msg);
}

void LRMInfo::show ()
{
        printf ("LRMInfo {\n");
        printf ("\tmessage: "); _infomsg->show (); printf ("\n");
        printf ("}\n");
}

//LRMMOTD
LRMMOTD::LRMMOTD (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

LRMMOTD::LRMMOTD (std::string msg)
{
        _id = new TWord8 ((uint8_t)0x14);
        _motd = new TString (msg);
}

LRMMOTD::~LRMMOTD ()
{
        delete _id;
        delete _motd;
}

uint8_t LRMMOTD::getId ()
{
        return _id->getVal ();
}

void LRMMOTD::put (NetworkMessage* msg)
{
        _id->put (msg);
        _motd->put (msg);
}

void LRMMOTD::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _motd = new TString (msg);
}

void LRMMOTD::show ()
{
        printf ("LRMMOTD {\n");
        printf ("\tmessage: "); _motd->show (); printf ("\n");
        printf ("}\n");
}

//LRMCharacterList
LRMCharacterList::LRMCharacterList (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

LRMCharacterList::LRMCharacterList (TCharacterList* charlist, uint16_t daysprem)
{
        _id = new TWord8 (0x64);
        _charlist = charlist;
        _daysprem = new TWord16 (daysprem);
}

LRMCharacterList::~LRMCharacterList ()
{
        delete _id;
        delete _charlist;
        delete _daysprem;
}

uint8_t LRMCharacterList::getId ()
{
        return _id->getVal ();
}

void LRMCharacterList::put (NetworkMessage* msg)
{
        _id->put (msg);
        _charlist->put (msg);
        _daysprem->put (msg);
}

TCharacterList* LRMCharacterList::getCharList ()
{
        return _charlist;
}

uint16_t LRMCharacterList::getDaysPrem ()
{
        return _daysprem->getVal ();
}

void LRMCharacterList::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _charlist = new TCharacterList (msg);
        _daysprem = new TWord16 (msg);
}

void LRMCharacterList::show ()
{
        printf ("LRMCharacterList {\n");
        printf ("\tchars:\n"); _charlist->show (); printf ("\n");
        printf ("\tdaysprem: "); _daysprem->show (); printf ("\n");
        printf ("}\n");
}

//Patch Exe
LRMPatchExe::LRMPatchExe (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

LRMPatchExe::LRMPatchExe ()
{
        _id = new TWord8 ((uint8_t)0x1E);
}

LRMPatchExe::~LRMPatchExe ()
{
        delete _id;
}

uint8_t LRMPatchExe::getId ()
{
        return _id->getVal ();
}

void LRMPatchExe::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void LRMPatchExe::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
}

void LRMPatchExe::show ()
{
        printf ("LRMPatchExe {}\n");
}

//Patch Dat
LRMPatchDat::LRMPatchDat (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

LRMPatchDat::LRMPatchDat ()
{
        _id = new TWord8 ((uint8_t)0x1F);
}

LRMPatchDat::~LRMPatchDat ()
{
        delete _id;
}

uint8_t LRMPatchDat::getId ()
{
        return _id->getVal ();
}

void LRMPatchDat::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void LRMPatchDat::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
}

void LRMPatchDat::show ()
{
        printf ("LRMPatchDat {}\n");
}

//Patch Spr
LRMPatchSpr::LRMPatchSpr (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

LRMPatchSpr::LRMPatchSpr ()
{
        _id = new TWord8 ((uint8_t)0x20);
}

LRMPatchSpr::~LRMPatchSpr ()
{
        delete _id;
}

uint8_t LRMPatchSpr::getId ()
{
        return _id->getVal ();
}

void LRMPatchSpr::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void LRMPatchSpr::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
}

void LRMPatchSpr::show ()
{
        printf ("LRMPatchSpr {}\n");
}

//Change Login Server
LRMNewLoginServer::LRMNewLoginServer (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

LRMNewLoginServer::LRMNewLoginServer ()
{
        _id = new TWord8 ((uint8_t)0x28);
}

LRMNewLoginServer::~LRMNewLoginServer ()
{
        delete _id;
}

uint8_t LRMNewLoginServer::getId ()
{
        return _id->getVal ();
}

void LRMNewLoginServer::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void LRMNewLoginServer::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
}

void LRMNewLoginServer::show ()
{
        printf ("LRMNewLoginServer {}\n");
}

/***************************************************************
 * Game Recv Messages
 ***************************************************************/
/***************************************************************
 * GMActions
 ***************************************************************/

GRMGMAction::GRMGMAction (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMGMAction::GRMGMAction (uint8_t* actions)
{
        _id = new TWord8 ((uint8_t)GRM_GM_ACTION_ID);
        for (uint32_t i = 0; i < 32; i ++) {
                _dump[i] = new TWord8 (actions[i]);
        }
}

GRMGMAction::GRMGMAction (const GRMGMAction& clone)
{
        _id = new TWord8 (*clone._id);
        for (uint32_t i = 0; i < 32; i ++) {
                _dump[i] = new TWord8 (*clone._dump[i]);
        }
}
        
GRMGMAction::~GRMGMAction ()
{
        delete _id;
        for (uint32_t i = 0; i < 32; i ++) {
                delete _dump[i];
        }
}

void GRMGMAction::put (NetworkMessage* msg)
{
        _id->put (msg);
        for (uint32_t i = 0; i < 32; i ++) {
                _dump[i]->put (msg);
        }
}

void GRMGMAction::show ()
{
        printf ("GRMGMAction {");
        for (uint32_t i = 0; i < 32; i ++) {
                _dump[i]->show ();
                printf (" ");
        }
        printf ("}\n");
}

uint8_t GRMGMAction::getId ()
{
        return _id->getVal ();
}

void GRMGMAction::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        for (uint32_t i = 0; i < 32; i ++) {
                _dump[i] = new TWord8 (msg);
        }
}

/***************************************************************
 * Error
 ***************************************************************/

GRMError::GRMError (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMError::GRMError (const std::string& msg)
{
        _id = new TWord8 ((uint8_t)GRM_ERROR_ID);
        _msg = new TString (msg);
}

GRMError::GRMError (const GRMError& clone)
{
        _id = new TWord8 (*clone._id);
        _msg = new TString (*clone._msg);
}
        
GRMError::~GRMError ()
{
        delete _id;
        delete _msg;
}

void GRMError::put (NetworkMessage* msg)
{
        _id->put (msg);
        _msg->put (msg);
}

void GRMError::show ()
{
        printf ("GRMError {"); _msg->show (); printf ("}\n");
}

uint8_t GRMError::getId ()
{
        return _id->getVal ();
}

const std::string& GRMError::getMsg ()
{
        return _msg->getString ();
}

void GRMError::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _msg = new TString (msg);
}

/***************************************************************
 * FYI
 ***************************************************************/

GRMFYI::GRMFYI (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMFYI::GRMFYI (const std::string& msg)
{
        _id = new TWord8 ((uint8_t)GRM_FYI_ID);
        _msg = new TString (msg);
}

GRMFYI::GRMFYI (const GRMFYI& clone)
{
        _id = new TWord8 (*clone._id);
        _msg = new TString (*clone._msg);
}
        
GRMFYI::~GRMFYI ()
{
        delete _id;
        delete _msg;
}

void GRMFYI::put (NetworkMessage* msg)
{
        _id->put (msg);
        _msg->put (msg);
}

void GRMFYI::show ()
{
        printf ("GRMFYI {"); _msg->show (); printf ("}\n");
}

uint8_t GRMFYI::getId ()
{
        return _id->getVal ();
}

const std::string& GRMFYI::getMsg ()
{
        return _msg->getString ();
}

void GRMFYI::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _msg = new TString (msg);
}

/***************************************************************
 * Queue
 ***************************************************************/

GRMQueue::GRMQueue (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMQueue::GRMQueue (const std::string& msg, uint8_t time)
{
        _id = new TWord8 ((uint8_t)GRM_QUEUE_ID);
        _msg = new TString (msg);
        _time = new TWord8 (time);
}

GRMQueue::GRMQueue (const GRMQueue& clone)
{
        _id = new TWord8 (*clone._id);
        _msg = new TString (*clone._msg);
        _time = new TWord8 (*clone._time);
}
        
GRMQueue::~GRMQueue ()
{
        delete _id;
        delete _msg;
        delete _time;
}

void GRMQueue::put (NetworkMessage* msg)
{
        _id->put (msg);
        _msg->put (msg);
        _time->put (msg);
}

void GRMQueue::show ()
{
        printf ("GRMQueue {\n"); 
        printf ("msg: "); _msg->show (); printf ("\n");
        printf ("time: "); _time->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMQueue::getId ()
{
        return _id->getVal ();
}

const std::string& GRMQueue::getMsg ()
{
        return _msg->getString ();
}

uint8_t GRMQueue::getTime ()
{
        return _time->getVal ();
}

void GRMQueue::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _msg = new TString (msg);
        _time = new TWord8 (msg);
}

/***************************************************************
 * Ping
 ***************************************************************/

GRMPing::GRMPing (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMPing::GRMPing ()
{
        _id = new TWord8 ((uint8_t)GRM_PING_ID);
}

GRMPing::GRMPing (const GRMPing& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GRMPing::~GRMPing ()
{
        delete _id;
}

void GRMPing::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GRMPing::show ()
{
        printf ("GRMPing {}\n");
}

uint8_t GRMPing::getId ()
{
        return _id->getVal ();
}

void GRMPing::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * LoginWindow
 ***************************************************************/

GRMLoginWindow::GRMLoginWindow (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMLoginWindow::GRMLoginWindow ()
{
        _id = new TWord8 ((uint8_t)GRM_LOGIN_WINDOW_ID);
}

GRMLoginWindow::GRMLoginWindow (const GRMLoginWindow& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GRMLoginWindow::~GRMLoginWindow ()
{
        delete _id;
}

void GRMLoginWindow::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GRMLoginWindow::show ()
{
        printf ("GRMLoginWindow {}\n");
}

uint8_t GRMLoginWindow::getId ()
{
        return _id->getVal ();
}

void GRMLoginWindow::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * GSMGameInit
 ********************************************************************/

GSMGameInit::GSMGameInit (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMGameInit::GSMGameInit (uint16_t OS, uint16_t version, uint8_t u1,
        uint32_t* xtea, uint8_t isGM,  uint32_t account, std::string name,
        std::string password)
{
        _id = new TWord8 ((uint8_t)0x0A);

        _OS = new TWord16 (OS);
        _version = new TWord16 (version);
        _u1 = new TWord8 ((uint8_t)0);
        _xtea = new TXTEAKey (xtea);
        _isGM = new TWord8 (isGM);
        _account = new TWord32 (account);
        _name = new TString (name);
        _password = new TString (password);
        //the TByteBuffer (uint32_t len) constructer automatically
        //generates a random byte buffer
        uint32_t rem = RSA_LEN - (23 + _password->getLen ());
        _bytes = new TByteBuffer (rem);
}

GSMGameInit::~GSMGameInit ()
{
        delete _id;
        delete _OS; 
        delete _version; 
        delete _u1;
        delete _xtea;
        delete _isGM;
        delete _password;
        delete _account;
        delete _name;
        delete _bytes;
}

uint8_t GSMGameInit::getId ()
{
        return _id->getVal ();
}

uint32_t GSMGameInit::getAccount ()
{
        return _account->getVal ();
}

const std::string& GSMGameInit::getPassword ()
{
        return _password->getString ();
}

const std::string& GSMGameInit::getName ()
{
        return _name->getString ();
}

void GSMGameInit::put (NetworkMessage* msg)
{
        _id->put (msg);
        _OS->put (msg);
        _version->put (msg);
        _u1->put (msg);
        _xtea->put (msg);
        _isGM->put (msg);
        _account->put (msg);
        _name->put (msg);
        _password->put (msg);
        _bytes->put (msg);
}

void GSMGameInit::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _OS = new TWord16 (msg);
        _version = new TWord16 (msg);
        _u1 = new TWord8 (msg);
        _xtea = new TXTEAKey (msg);
        _isGM = new TWord8 (msg);
        _account = new TWord32 (msg);
        _name = new TString (msg);
        _password = new TString (msg);
        //this time we just take the random bytes from the packet
        uint32_t rem = RSA_LEN - (26 + _password->getLen () + _name->getLen ());
        _bytes = new TByteBuffer (rem, msg);
}

const uint32_t* GSMGameInit::getXTEA ()
{
        return _xtea->getKey ();
}
        
void GSMGameInit::show ()
{
        printf ("GSMGameInit {\n");
        printf ("\tOS: "); _OS->show (); printf ("\n");
        printf ("\tversion: "); _version->show (); printf ("\n");
        printf ("\tu1: "); _u1->show (); printf ("\n");
        printf ("\txtea: "); _xtea->show (); printf ("\n");
        printf ("\tisGM: "); _isGM->show (); printf ("\n");
        printf ("\taccount: "); _account->show (); printf ("\n");
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("\tpassword: "); _password->show (); printf ("\n");
        printf ("\tbytes: "); _bytes->show (); printf ("\n");
        printf ("}\n");
}

/****************************************************************
 * GRMSelfInfo
 ****************************************************************/

GRMSelfInfo::GRMSelfInfo (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

GRMSelfInfo::GRMSelfInfo (uint32_t tibiaId, uint8_t u1, 
                uint8_t u2, uint8_t reportErrors)
{
        _id = new TWord8 ((uint8_t)0x0A);
        _tibiaId = new TWord32 (tibiaId);
        _u1 = new TWord8 (u1);
        _u2 = new TWord8 (u2);
        _reportErrors = new TWord8 (reportErrors);
}

GRMSelfInfo::GRMSelfInfo (const GRMSelfInfo& clone)
{
        _id = new TWord8 (*clone._id);
        _tibiaId = new TWord32 (*clone._tibiaId);
        _u1 = new TWord8 (*clone._u1);
        _u2 = new TWord8 (*clone._u2);
        _reportErrors = new TWord8 (*clone._reportErrors);
}

GRMSelfInfo::~GRMSelfInfo ()
{
        delete _id;
        delete _tibiaId;
        delete _u1;
        delete _u2;
        delete _reportErrors;
}

uint8_t GRMSelfInfo::getId ()
{
        return GRM_SELF_INFO_ID;
}

void GRMSelfInfo::put (NetworkMessage* msg)
{
        _id->put (msg);
        _tibiaId->put (msg);
        _u1->put (msg);
        _u2->put (msg);
        _reportErrors->put (msg);
}

void GRMSelfInfo::show ()
{
        printf ("GSMSelfInfo {\n");
        printf ("\ttibiaId: "); _tibiaId->show (); printf ("\n");
        printf ("\tu1: "); _u1->show (); printf ("\n");
        printf ("\tu2: "); _u2->show (); printf ("\n");
        printf ("\treportErrors: "); _reportErrors->show (); printf ("\n");
        printf ("}\n");
}

void GRMSelfInfo::get (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _tibiaId = new TWord32 (msg);
        _u1 = new TWord8 (msg);
        _u2 = new TWord8 (msg);
        _reportErrors = new TWord8 (msg);
}

/***************************************************************
 * GRMMapInit
 ***************************************************************/
GRMMapInit::GRMMapInit (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMapInit::GRMMapInit (const TPos& pos, TMapDescription* map)
{
        _id = new TWord8 (GRM_MAP_INIT_ID);
        _pos = new TPos (pos);
        _map = map;
}

GRMMapInit::GRMMapInit (const GRMMapInit& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _map = new TMapDescription (*clone._map);
}

GRMMapInit::~GRMMapInit ()
{
        delete _id;
        delete _pos;
        delete _map;
}

void GRMMapInit::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _map->put (msg);
}

void GRMMapInit::show ()
{
        printf ("GRMMapInit {\n");
        _pos->show ();
        _map->show ();
        printf ("}\n");
}

uint8_t GRMMapInit::getId ()
{
        return GRM_MAP_INIT_ID;
}

const TPos& GRMMapInit::getPos ()
{
        return *_pos;
}

TMapDescription& GRMMapInit::getMap ()
{
        return *_map;
}

void GRMMapInit::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        uint32_t minz;
        uint32_t maxz;
        if (_pos->z () <= 7) {
                //we are above ground
                minz = 0;
                maxz = 7;
        } else {
                //under ground
                minz = _pos->z () - 2;
                maxz = MIN (_pos->z () + 2, MAX_FLOOR);
        }
                
        TPos start (_pos->x () - 9, _pos->y () - 7, minz);
        TPos end (_pos->x () + 8, _pos->y () + 6, maxz);

        _map = new TMapDescription (start, end, msg, dat);
}

/***************************************************************
 * GRMMapNorth
 ***************************************************************/
GRMMapNorth::GRMMapNorth (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMapNorth::GRMMapNorth (TMapDescription* map)
{
        _id = new TWord8 (GRM_MAP_NORTH_ID);
        _map = map;
}

GRMMapNorth::GRMMapNorth (const GRMMapNorth& clone)
{
        _id = new TWord8 (*clone._id);
        _map = new TMapDescription (*clone._map);
}

GRMMapNorth::~GRMMapNorth ()
{
        delete _id;
        delete _map;
}

void GRMMapNorth::put (NetworkMessage* msg)
{
        _id->put (msg);
        _map->put (msg);
}

void GRMMapNorth::show ()
{
        printf ("GRMMapNorth {\n");
        _map->show ();
        printf ("}\n");
}

uint8_t GRMMapNorth::getId ()
{
        return _id->getVal ();
}

TMapDescription& GRMMapNorth::getMap ()
{
        return *_map;
}

void GRMMapNorth::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        Pos pos = gs->map->getCurPos ();
        uint32_t minz;
        uint32_t maxz;
        if (pos.z <= 7) {
                //we are above ground
                minz = 0;
                maxz = 7;
        } else {
                //under ground
                minz = pos.z - 2;
                maxz = MIN (pos.z + 2, MAX_FLOOR);
        }
                
        TPos start (pos.x - 8, pos.y - 6, minz);
        TPos end (pos.x + 9, pos.y - 6, maxz);

        _map = new TMapDescription (start, end, msg, dat);
}

/***************************************************************
 * GRMMapEast
 ***************************************************************/
GRMMapEast::GRMMapEast (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMapEast::GRMMapEast (TMapDescription* map)
{
        _id = new TWord8 (GRM_MAP_EAST_ID);
        _map = map;
}

GRMMapEast::GRMMapEast (const GRMMapEast& clone)
{
        _id = new TWord8 (*clone._id);
        _map = new TMapDescription (*clone._map);
}

GRMMapEast::~GRMMapEast ()
{
        delete _id;
        delete _map;
}

void GRMMapEast::put (NetworkMessage* msg)
{
        _id->put (msg);
        _map->put (msg);
}

void GRMMapEast::show ()
{
        printf ("GRMMapEast {\n");
        _map->show ();
        printf ("}\n");
}

uint8_t GRMMapEast::getId ()
{
        return _id->getVal ();
}

TMapDescription& GRMMapEast::getMap ()
{
        return *_map;
}

void GRMMapEast::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        Pos pos = gs->map->getCurPos ();
        uint32_t minz;
        uint32_t maxz;
        if (pos.z <= 7) {
                //we are above ground
                minz = 0;
                maxz = 7;
        } else {
                //under ground
                minz = pos.z - 2;
                maxz = MIN (pos.z + 2, 15);
        }
                
        TPos start (pos.x + 9, pos.y - 6, minz);
        TPos end (pos.x + 9, pos.y + 7, maxz);

        _map = new TMapDescription (start, end, msg, dat);
}

/***************************************************************
 * GRMMapSouth
 ***************************************************************/
GRMMapSouth::GRMMapSouth (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMapSouth::GRMMapSouth (TMapDescription* map)
{
        _id = new TWord8 (GRM_MAP_SOUTH_ID);
        _map = map;
}

GRMMapSouth::GRMMapSouth (const GRMMapSouth& clone)
{
        _id = new TWord8 (*clone._id);
        _map = new TMapDescription (*clone._map);
}

GRMMapSouth::~GRMMapSouth ()
{
        delete _id;
        delete _map;
}

void GRMMapSouth::put (NetworkMessage* msg)
{
        _id->put (msg);
        _map->put (msg);
}

void GRMMapSouth::show ()
{
        printf ("GRMMapSouth {\n");
        _map->show ();
        printf ("}\n");
}

uint8_t GRMMapSouth::getId ()
{
        return _id->getVal ();
}

TMapDescription& GRMMapSouth::getMap ()
{
        return *_map;
}

void GRMMapSouth::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        Pos pos = gs->map->getCurPos ();
        uint32_t minz;
        uint32_t maxz;
        if (pos.z <= 7) {
                //we are above ground
                minz = 0;
                maxz = 7;
        } else {
                //under ground
                minz = pos.z - 2;
                maxz = MIN (pos.z + 2, 15);
        }
                
        TPos start (pos.x - 8, pos.y + 7, minz);
        TPos end (pos.x + 9, pos.y + 7, maxz);

        _map = new TMapDescription (start, end, msg, dat);
}

/***************************************************************
 * GRMMapWest
 ***************************************************************/
GRMMapWest::GRMMapWest (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMapWest::GRMMapWest (TMapDescription* map)
{
        _id = new TWord8 (GRM_MAP_WEST_ID);
        _map = map;
}

GRMMapWest::GRMMapWest (const GRMMapWest& clone)
{
        _id = new TWord8 (*clone._id);
        _map = new TMapDescription (*clone._map);
}

GRMMapWest::~GRMMapWest ()
{
        delete _id;
        delete _map;
}

void GRMMapWest::put (NetworkMessage* msg)
{
        _id->put (msg);
        _map->put (msg);
}

void GRMMapWest::show ()
{
        printf ("GRMMapWest {\n");
        _map->show ();
        printf ("}\n");
}

uint8_t GRMMapWest::getId ()
{
        return _id->getVal ();
}

TMapDescription& GRMMapWest::getMap ()
{
        return *_map;
}

void GRMMapWest::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        Pos pos = gs->map->getCurPos ();
        uint32_t minz;
        uint32_t maxz;
        if (pos.z <= 7) {
                //we are above ground
                minz = 0;
                maxz = 7;
        } else {
                //under ground
                minz = pos.z - 2;
                maxz = MIN (pos.z + 2, 15);
        }
                
        TPos start (pos.x - 8, pos.y - 6, minz);
        TPos end (pos.x - 8, pos.y + 7, maxz);

        _map = new TMapDescription (start, end, msg, dat);
}

/***************************************************************
 * GRMMapUp
 ***************************************************************/
GRMMapUp::GRMMapUp (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMapUp::GRMMapUp ()
{
        _id = new TWord8 (GRM_MAP_UP_ID);
        _map = NULL;
        _hasmap = false;
}

GRMMapUp::GRMMapUp (TMapDescription* map)
{
        _id = new TWord8 (GRM_MAP_DOWN_ID);
        _map = map;
        _hasmap = true;
}

GRMMapUp::GRMMapUp (const GRMMapUp& clone)
{
        _id = new TWord8 (*clone._id);

        _hasmap = clone._hasmap;
        if (_hasmap) {
                _map = new TMapDescription (*clone._map);
        } else {
                _map = NULL;
        }
}

GRMMapUp::~GRMMapUp ()
{
        delete _id;
        //not actually necessary, as delete checks for NULL
        if (_hasmap) {
                delete _map;
        }
}

void GRMMapUp::put (NetworkMessage* msg)
{
        _id->put (msg);
        if (_hasmap) {
                _map->put (msg);
        }
}

void GRMMapUp::show ()
{
        printf ("GRMMapUp {\n");
        if (_hasmap) {
                _map->show ();
        }
        printf ("}\n");
}

uint8_t GRMMapUp::getId ()
{
        return _id->getVal ();
}

TMapDescription& GRMMapUp::getMap ()
{
        if (!_hasmap) {
                printf ("GRMMapUp: i told you to ask if i have a map first, now\
                         you segfault\n");
        }
        return *_map;
}

void GRMMapUp::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        Pos pos = gs->map->getCurPos ();
        uint32_t minz;
        uint32_t maxz;
        if (pos.z == 8) {
                //we have come from underground and have 6 & 7
                minz = 0;
                maxz = 5;
                
                TPos start (pos.x - 8, pos.y - 6, minz);
                TPos end (pos.x + 9, pos.y + 7, maxz);
                _map = new TMapDescription (start, end, msg, dat);
        } else if (pos.z > 8){
                //we are under ground and need the floor above us
                minz = pos.z - 2;
                maxz = minz;

                TPos start (pos.x - 8, pos.y - 6, minz);
                TPos end (pos.x + 9, pos.y + 7, maxz);
                _map = new TMapDescription (start, end, msg, dat);
        }
}

/***************************************************************
 * GRMMapDown
 ***************************************************************/
GRMMapDown::GRMMapDown (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMapDown::GRMMapDown ()
{
        _id = new TWord8 (GRM_MAP_DOWN_ID);
        _map = NULL;
        _hasmap = false;
}

GRMMapDown::GRMMapDown (TMapDescription* map)
{
        _id = new TWord8 (GRM_MAP_DOWN_ID);
        _map = map;
        _hasmap = true;
}

GRMMapDown::GRMMapDown (const GRMMapDown& clone)
{
        _id = new TWord8 (*clone._id);

        _hasmap = clone._hasmap;
        if (_hasmap) {
                _map = new TMapDescription (*clone._map);
        } else {
                _map = NULL;
        }
}

GRMMapDown::~GRMMapDown ()
{
        delete _id;
        //not actually necessary, as delete checks for NULL
        if (_hasmap) {
                delete _map;
        }
}

void GRMMapDown::put (NetworkMessage* msg)
{
        _id->put (msg);
        if (_hasmap) {
                _map->put (msg);
        }
}

void GRMMapDown::show ()
{
        printf ("GRMMapDown {\n");
        if (_hasmap) {
                _map->show ();
        }
        printf ("}\n");
}

uint8_t GRMMapDown::getId ()
{
        return _id->getVal ();
}

TMapDescription& GRMMapDown::getMap ()
{
        if (!_hasmap) {
                printf ("GRMMapDown: i told you to ask if i have a map first, now\
                         you segfault\n");
        }
        return *_map;
}

void GRMMapDown::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        Pos pos = gs->map->getCurPos ();
        uint32_t minz;
        uint32_t maxz;
        if (pos.z == 7) {
                //we are about to go underground and have 6 & 7
                minz = 8;
                maxz = 10;
                
                TPos start (pos.x - 8, pos.y - 6, minz);
                TPos end (pos.x + 9, pos.y + 7, maxz);
                _map = new TMapDescription (start, end, msg, dat);
        } else if (pos.z > 8){
                //we are under ground and need the below above us
                minz = MIN (pos.z + 2, MAX_FLOOR);
                maxz = minz;

                TPos start (pos.x - 8, pos.y - 6, minz);
                TPos end (pos.x + 9, pos.y + 7, maxz);
                _map = new TMapDescription (start, end, msg, dat);
        }
}

/***************************************************************
 * GRMTileSet
 ***************************************************************/
GRMTileSet::GRMTileSet (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMTileSet::GRMTileSet (const TPos& pos, TMapDescription* map)
{
        _id = new TWord8 (GRM_TILE_SET_ID);
        _pos = new TPos (pos);
        _map = map;
}

GRMTileSet::GRMTileSet (const GRMTileSet& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _map = new TMapDescription (*clone._map);
}

GRMTileSet::~GRMTileSet ()
{
        delete _id;
        delete _pos;
        delete _map;
}

void GRMTileSet::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _map->put (msg);
}

void GRMTileSet::show ()
{
        printf ("GRMTileSet {\n");
        _pos->show ();
        _map->show ();
        printf ("}\n");
}

uint8_t GRMTileSet::getId ()
{
        return GRM_MAP_INIT_ID;
}

const TPos& GRMTileSet::getPos ()
{
        return *_pos;
}

TMapDescription& GRMTileSet::getMap ()
{
        return *_map;
}

void GRMTileSet::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        //this is a bit annoying, if we get an item 0xFF01 we clear the tile
        //else we read a whole tile description
        _map = new TMapDescription (*_pos, *_pos);

        TThingFactory tf (msg, dat);
        TThing* thing = tf.getThing ();
        while (thing->getType () != TThing::skip) {
                _map->add (thing);
                thing = tf.getThing ();
        }
        _map->add (thing);
}

/***************************************************************
 * GRMTileAdd
 ***************************************************************/

GRMTileAdd::GRMTileAdd (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMTileAdd::GRMTileAdd (const TPos& pos, const TThing& thing)
{
        _id = new TWord8 ((uint8_t)GRM_TILE_ADD_ID);
        _pos = new TPos (pos);

        TThingFactory tf;
        _thing = tf.cloneThing (thing);
}

GRMTileAdd::GRMTileAdd (const GRMTileAdd& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);

        TThingFactory tf;
        _thing = tf.cloneThing (*clone._thing);
}
        
GRMTileAdd::~GRMTileAdd ()
{
        delete _id;
        delete _pos;
        delete _thing;
}

void GRMTileAdd::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _thing->put (msg);
}

void GRMTileAdd::show ()
{
        printf ("GRMTileAdd {\n");
        printf ("\tpos: "); _pos->show (); printf ("\n");
        printf ("\tthing: "); _thing->show (); printf ("\n}\n");
}

uint8_t GRMTileAdd::getId ()
{
        return _id->getVal ();
}

const TPos& GRMTileAdd::getPos ()
{
        return *_pos;
}

const TThing& GRMTileAdd::getThing ()
{
        return *_thing;
}

void GRMTileAdd::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);

        TThingFactory tf (msg, dat);
        _thing = tf.getThing ();
}

/***************************************************************
 * GRMTileUpdate
 ***************************************************************/

GRMTileUpdate::GRMTileUpdate (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMTileUpdate::GRMTileUpdate (const TPos& pos, uint8_t stackpos,
                                const TThing& thing)
{
        _id = new TWord8 ((uint8_t)GRM_TILE_UPDATE_ID);
        _pos = new TPos (pos);
        _stackpos = new TWord8 (stackpos);

        TThingFactory tf;
        _thing = tf.cloneThing (thing);
}

GRMTileUpdate::GRMTileUpdate (const GRMTileUpdate& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _stackpos = new TWord8 (*clone._stackpos);

        TThingFactory tf;
        _thing = tf.cloneThing (*clone._thing);
}
        
GRMTileUpdate::~GRMTileUpdate ()
{
        delete _id;
        delete _pos;
        delete _stackpos;
        delete _thing;
}

void GRMTileUpdate::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _stackpos->put (msg);
        _thing->put (msg);
}

void GRMTileUpdate::show ()
{
        printf ("GRMTileUpdate {\n");
        printf ("\tpos: "); _pos->show (); printf ("\n");
        printf ("\tstackpos: "); _stackpos->show (); printf ("\n");
        printf ("\tthing: "); _thing->show (); printf ("\n}\n");
}

uint8_t GRMTileUpdate::getId ()
{
        return _id->getVal ();
}

const TPos& GRMTileUpdate::getPos ()
{
        return *_pos;
}

uint8_t GRMTileUpdate::getStackPos ()
{
        return _stackpos->getVal ();
}

const TThing& GRMTileUpdate::getThing ()
{
        return *_thing;
}

void GRMTileUpdate::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        _stackpos = new TWord8 (msg);

        //in yact new/old creature are handled as creature turns. I dont thing
        //that tibia would actually use non-creature turn items for turns, this
        //is to catch the segfault if they do
        uint16_t test;
        msg->peekU16 (test);
        if (test == 0x0061 || test == 0x0062) {
                printf ("TileUpdate: new/old creature used as creature turn\n");
        }
        //

        TThingFactory tf (msg, dat);
        _thing = tf.getThing ();
}

/***************************************************************
 * GRMTileRemove
 ***************************************************************/

GRMTileRemove::GRMTileRemove (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMTileRemove::GRMTileRemove (const TPos& pos, uint8_t stackpos)
{
        _id = new TWord8 ((uint8_t)GRM_TILE_REMOVE_ID);
        _pos = new TPos (pos);
        _stackpos = new TWord8 (stackpos);
}

GRMTileRemove::GRMTileRemove (const GRMTileRemove& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _stackpos = new TWord8 (*clone._stackpos);
}
        
GRMTileRemove::~GRMTileRemove ()
{
        delete _id;
        delete _pos;
        delete _stackpos;
}

void GRMTileRemove::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _stackpos->put (msg);
}

void GRMTileRemove::show ()
{
        printf ("GRMTileRemove {\n");
        printf ("\tpos: "); _pos->show (); printf ("\n");
        printf ("\tstack: "); _stackpos->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMTileRemove::getId ()
{
        return _id->getVal ();
}

const TPos& GRMTileRemove::getPos ()
{
        return *_pos;
}

uint8_t GRMTileRemove::getStackPos ()
{
        return _stackpos->getVal ();
}

void GRMTileRemove::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        _stackpos = new TWord8 (msg);
}

/***************************************************************
 * GRMCreatureMove
 ***************************************************************/

GRMCreatureMove::GRMCreatureMove (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureMove::GRMCreatureMove (const TPos& from, uint8_t stackpos,
                                        const TPos& to)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_MOVE_ID);
        _from = new TPos (from);
        _stackpos =  new TWord8 (stackpos);
        _to = new TPos (to);
}

GRMCreatureMove::GRMCreatureMove (const GRMCreatureMove& clone)
{
        _id = new TWord8 (*clone._id);
        _from = new TPos (*clone._from);
        _stackpos =  new TWord8 (*clone._stackpos);
        _to = new TPos (*clone._to);
}
        
GRMCreatureMove::~GRMCreatureMove ()
{
        delete _id;
        delete _from;
        delete _stackpos;
        delete _to;
}

void GRMCreatureMove::put (NetworkMessage* msg)
{
        _id->put (msg);
        _from->put (msg);
        _stackpos->put (msg);
        _to->put (msg);
}

void GRMCreatureMove::show ()
{
        printf ("GRMCreatureMove {\n");
        printf ("\tfrom: "); _from->show (); printf ("\n");
        printf ("\tstackpos: "); _stackpos->show (); printf ("\n");
        printf ("\tto: "); _to->show (); printf ("\n");
}

uint8_t GRMCreatureMove::getId ()
{
        return _id->getVal ();
}

const TPos& GRMCreatureMove::getFrom ()
{
        return *_from;
}

const TPos& GRMCreatureMove::getTo ()
{
        return *_to;
}

uint8_t GRMCreatureMove::getStackPos ()
{
        return _stackpos->getVal ();
}

void GRMCreatureMove::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _from = new TPos (msg);
        _stackpos =  new TWord8 (msg);
        _to = new TPos (msg);
}

/***************************************************************
 * GRMSlotItem
 ***************************************************************/

GRMSlotItem::GRMSlotItem (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMSlotItem::GRMSlotItem (uint8_t slot, const TThing& thing)
{
        _id = new TWord8 ((uint8_t)GRM_SLOT_ITEM_ID);
        _slot = new TWord8 (slot);
        
        TThingFactory tf;
        _thing = tf.cloneThing (thing);
}
        
GRMSlotItem::GRMSlotItem (const GRMSlotItem& clone)
{
        _id = new TWord8 (*clone._id);
        _slot = new TWord8 (*clone._slot);
        
        TThingFactory tf;
        _thing = tf.cloneThing (*clone._thing);
}
        
GRMSlotItem::~GRMSlotItem ()
{
        delete _id;
        delete _slot;
        delete _thing;
}

void GRMSlotItem::put (NetworkMessage* msg)
{
        _id->put (msg);
        _slot->put (msg);
        _thing->put (msg);
}

void GRMSlotItem::show ()
{
        printf ("GRMSlotItem {Slot: "); _slot->show ();
        printf (" "); _thing->show ();
        printf ("}\n");
}

uint8_t GRMSlotItem::getId ()
{
        return _id->getVal ();
}

uint8_t GRMSlotItem::getSlot ()
{
        return _slot->getVal ();
}

const TThing& GRMSlotItem::getItem ()
{
        return *_thing;
}

void GRMSlotItem::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _slot = new TWord8 (msg);

        TThingFactory tf (msg, dat);
        _thing = tf.getThing ();
}

/***************************************************************
 * GRMSlotClear
 ***************************************************************/

GRMSlotClear::GRMSlotClear (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMSlotClear::GRMSlotClear (uint8_t slot)
{
        _id = new TWord8 ((uint8_t)GRM_SLOT_CLEAR_ID);
        _slot = new TWord8 (slot);
}
        
GRMSlotClear::GRMSlotClear (const GRMSlotClear& clone)
{
        _id = new TWord8 (*clone._id);
        _slot = new TWord8 (*clone._slot);
}
        
GRMSlotClear::~GRMSlotClear ()
{
        delete _id;
        delete _slot;
}

void GRMSlotClear::put (NetworkMessage* msg)
{
        _id->put (msg);
        _slot->put (msg);
}

void GRMSlotClear::show ()
{
        printf ("GRMSlotClear {Slot: "); _slot->show ();
        printf ("}\n");
}

uint8_t GRMSlotClear::getId ()
{
        return _id->getVal ();
}

uint8_t GRMSlotClear::getSlot ()
{
        return _slot->getVal ();
}

void GRMSlotClear::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _slot = new TWord8 (msg);
}

/***************************************************************
 * TMagicEffect
 ***************************************************************/

GRMMagicEffect::GRMMagicEffect (NetworkMessage* msg, GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMagicEffect::GRMMagicEffect (const TPos& pos, uint8_t effect)
{
        _id = new TWord8 ((uint8_t)GRM_MAGIC_EFFECT_ID);
        _pos = new TPos (pos);
        _effect = new TWord8 (effect);
}

GRMMagicEffect::GRMMagicEffect (const GRMMagicEffect& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _effect = new TWord8 (*clone._effect);
}
        
GRMMagicEffect::~GRMMagicEffect ()
{
        delete _id;
        delete _pos;
        delete _effect;
}

void GRMMagicEffect::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _effect->put (msg);
}

void GRMMagicEffect::show ()
{
        printf ("TEffect {\n");
        printf ("\tpos: "); _pos->show (); printf ("\n");
        printf ("\teffect: "); _effect->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMMagicEffect::getId ()
{
        return _id->getVal ();
}

uint8_t GRMMagicEffect::getEffect () const
{
        return _effect->getVal ();
}

const TPos& GRMMagicEffect::getPos () const
{
        return *_pos;
}

void GRMMagicEffect::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        _effect = new TWord8 (msg);
}
        
/***************************************************************
 * GRMGlobalLight
 ***************************************************************/

GRMGlobalLight::GRMGlobalLight (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMGlobalLight::GRMGlobalLight (const TCreatureLight& light)
{
        _id = new TWord8 ((uint8_t)GRM_GLOBAL_LIGHT_ID);
        _light = new TCreatureLight (light);
}

GRMGlobalLight::GRMGlobalLight (const GRMGlobalLight& clone)
{
        _id = new TWord8 (*clone._id);
        _light = new TCreatureLight (*clone._light);
}

GRMGlobalLight::~GRMGlobalLight ()
{
        delete _id;
        delete _light;
}

void GRMGlobalLight::put (NetworkMessage* msg)
{
        _id->put (msg);
        _light->put (msg);
}

void GRMGlobalLight::show ()
{
        printf ("GRMGlobalLight {"); _light->show (); printf ("}\n");
}

uint8_t GRMGlobalLight::getId ()
{
        return _id->getVal ();
}

const TCreatureLight& GRMGlobalLight::getLight ()
{
        return *_light;
}

void GRMGlobalLight::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _light = new TCreatureLight (msg);
}

/***************************************************************
 * GRMCreatureLight
 ***************************************************************/

GRMCreatureLight::GRMCreatureLight (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureLight::GRMCreatureLight (uint32_t tibiaId, const TCreatureLight& light)
{
        _id = new TWord8 ((uint8_t)GRM_GLOBAL_LIGHT_ID);
        _tibiaId = new TWord32 (tibiaId);
        _light = new TCreatureLight (light);
}

GRMCreatureLight::GRMCreatureLight (const GRMCreatureLight& clone)
{
        _id = new TWord8 (*clone._id);
        _tibiaId = new TWord32 (*clone._tibiaId);
        _light = new TCreatureLight (*clone._light);
}

GRMCreatureLight::~GRMCreatureLight ()
{
        delete _id;
        delete _tibiaId;
        delete _light;
}

void GRMCreatureLight::put (NetworkMessage* msg)
{
        _id->put (msg);
        _tibiaId->put (msg);
        _light->put (msg);
}

void GRMCreatureLight::show ()
{
        printf ("GRMCreatureLight {id: "); _tibiaId->show ();
        printf (", "); _light->show (); printf ("}\n");
}

uint8_t GRMCreatureLight::getId ()
{
        return _id->getVal ();
}

uint32_t GRMCreatureLight::getTibiaId ()
{
        return _tibiaId->getVal ();
}

const TCreatureLight& GRMCreatureLight::getLight ()
{
        return *_light;
}

void GRMCreatureLight::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _tibiaId = new TWord32 (msg);
        _light = new TCreatureLight (msg);
}

/***************************************************************
 * GRMTextMsg
 ***************************************************************/

GRMTextMsg::GRMTextMsg (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMTextMsg::GRMTextMsg (const TTextMsg& msg)
{
        _id = new TWord8 ((uint8_t)GRM_TEXT_MSG_ID);
        _msg = new TTextMsg (msg);
}

GRMTextMsg::GRMTextMsg (const GRMTextMsg& clone)
{
        _id = new TWord8 (*clone._id);
        _msg = new TTextMsg (*clone._msg);
}

GRMTextMsg::~GRMTextMsg ()
{
        delete _id;
        delete _msg;
}

void GRMTextMsg::put (NetworkMessage* msg)
{
        _id->put (msg);
        _msg->put (msg);
}

void GRMTextMsg::show ()
{
        printf ("GRMTextMsg {"); _msg->show (); printf ("}\n");
}

uint8_t GRMTextMsg::getId ()
{
        return _id->getVal ();
}

const TTextMsg& GRMTextMsg::getLight ()
{
        return *_msg;
}

void GRMTextMsg::get (NetworkMessage* msg, GameState* gs,
                  DatReader* dat)
{
        _id = new TWord8 (msg);
        _msg = new TTextMsg (msg);
}

/***************************************************************
 * GRMPlayerStats
 ***************************************************************/

GRMPlayerStats::GRMPlayerStats (NetworkMessage* msg, GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

GRMPlayerStats::GRMPlayerStats (const TPlayerStats& stats)
{
        _id = new TWord8 (GRM_PLAYER_STATS_ID);
        _stats = new TPlayerStats (stats);
}

GRMPlayerStats::GRMPlayerStats (const GRMPlayerStats& clone)
{
        _id = new TWord8 (*clone._id);
        _stats = new TPlayerStats (*clone._stats);
}

GRMPlayerStats::~GRMPlayerStats ()
{
        delete _id;
        delete _stats;
}

void GRMPlayerStats::put (NetworkMessage* msg)
{
        _id->put (msg);
        _stats->put (msg);
}

void GRMPlayerStats::show ()
{
        printf ("GRMSPlayerStats {"); _stats->show (); printf ("}\n");
}

uint8_t GRMPlayerStats::getId ()
{
        return _id->getVal ();
}

const TPlayerStats& GRMPlayerStats::getStats ()
{
        return * _stats;
}

void GRMPlayerStats::get (NetworkMessage* msg, GameState* gs,
                          DatReader* dat)
{
        _id = new TWord8 (msg);
        _stats = new TPlayerStats (msg);
}

/***************************************************************
 * GRMPlayerSkills
 ***************************************************************/

GRMPlayerSkills::GRMPlayerSkills (NetworkMessage* msg, GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

GRMPlayerSkills::GRMPlayerSkills (const TPlayerSkills& skills)
{
        _id = new TWord8 (GRM_PLAYER_SKILLS_ID);
        _skills = new TPlayerSkills (skills);
}

GRMPlayerSkills::GRMPlayerSkills (const GRMPlayerSkills& clone)
{
        _id = new TWord8 (*clone._id);
        _skills = new TPlayerSkills (*clone._skills);
}

GRMPlayerSkills::~GRMPlayerSkills ()
{
        delete _id;
        delete _skills;
}

void GRMPlayerSkills::put (NetworkMessage* msg)
{
        _id->put (msg);
        _skills->put (msg);
}

void GRMPlayerSkills::show ()
{
        printf ("GRMSPlayerSkills {"); _skills->show (); printf ("}\n");
}

uint8_t GRMPlayerSkills::getId ()
{
        return _id->getVal ();
}

const TPlayerSkills& GRMPlayerSkills::getSkills ()
{
        return * _skills;
}

void GRMPlayerSkills::get (NetworkMessage* msg, GameState* gs,
                          DatReader* dat)
{
        _id = new TWord8 (msg);
        _skills = new TPlayerSkills (msg);
}

/***************************************************************
 * OpenContainer
 ***************************************************************/
GRMOpenContainer::GRMOpenContainer (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMOpenContainer::GRMOpenContainer (uint8_t cid, const TContainer& container)
{
        _id = new TWord8 ((uint8_t)GRM_OPEN_CONTAINER_ID);
        _cid = new TWord8 (cid);
        _container = new TContainer (container);
}

GRMOpenContainer::GRMOpenContainer (const GRMOpenContainer& clone)
{
        _id = new TWord8 (*clone._id);
        _cid = new TWord8 (*clone._cid);
        _container = new TContainer (*clone._container);
}

GRMOpenContainer::~GRMOpenContainer ()
{
        delete _id;
        delete _cid;
        delete _container;
}

void GRMOpenContainer::put (NetworkMessage* msg)
{
        _id->put (msg);
        _cid->put (msg);
        _container->put (msg);
}

void GRMOpenContainer::show ()
{
        printf ("GRMOpenContainer {\n");
        printf ("\tcid: "); _cid->show (); printf ("\n");
        _container->show ();
        printf ("}\n");
}

uint8_t GRMOpenContainer::getId ()
{
        return _id->getVal ();
}

uint8_t GRMOpenContainer::getCid ()
{
        return _cid->getVal ();
}

TContainer& GRMOpenContainer::getContainer ()
{
        return *_container;
}

void GRMOpenContainer::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _cid = new TWord8 (msg);
        _container = new TContainer (msg, dat);
}

/***************************************************************
 * CloseContainer
 ***************************************************************/
GRMCloseContainer::GRMCloseContainer (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCloseContainer::GRMCloseContainer (uint8_t cid)
{
        _id = new TWord8 ((uint8_t)GRM_CLOSE_CONTAINER_ID);
        _cid = new TWord8 (cid);
}

GRMCloseContainer::GRMCloseContainer (const GRMCloseContainer& clone)
{
        _id = new TWord8 (*clone._id);
        _cid = new TWord8 (*clone._cid);
}

GRMCloseContainer::~GRMCloseContainer ()
{
        delete _id;
        delete _cid;
}

void GRMCloseContainer::put (NetworkMessage* msg)
{
        _id->put (msg);
        _cid->put (msg);
}

void GRMCloseContainer::show ()
{
        printf ("GRMCloseContainer {\n");
        printf ("\tcid: "); _cid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMCloseContainer::getId ()
{
        return _id->getVal ();
}

uint8_t GRMCloseContainer::getCid ()
{
        return _cid->getVal ();
}

void GRMCloseContainer::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _cid = new TWord8 (msg);
}

/***************************************************************
 * ContainerAdd
 ***************************************************************/
GRMContainerAdd::GRMContainerAdd (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMContainerAdd::GRMContainerAdd (uint8_t cid, const TThing& item)
{
        _id = new TWord8 ((uint8_t)GRM_CONT_ADD_ID);
        _cid = new TWord8 (cid);

        TThingFactory tf;
        _item = tf.cloneThing (item);
}

GRMContainerAdd::GRMContainerAdd (const GRMContainerAdd& clone)
{
        _id = new TWord8 (*clone._id);
        _cid = new TWord8 (*clone._cid);

        TThingFactory tf;
        _item = tf.cloneThing (*clone._item);
}

GRMContainerAdd::~GRMContainerAdd ()
{
        delete _id;
        delete _cid;
        delete _item;
}

void GRMContainerAdd::put (NetworkMessage* msg)
{
        _id->put (msg);
        _cid->put (msg);
        _item->put (msg);
}

void GRMContainerAdd::show ()
{
        printf ("GRMContainerAdd {\n");
        printf ("\tcid: "); _cid->show (); printf ("\n");
        _item->show ();
        printf ("\n}\n");
}

uint8_t GRMContainerAdd::getId ()
{
        return _id->getVal ();
}

uint8_t GRMContainerAdd::getCid ()
{
        return _cid->getVal ();
}

const TThing& GRMContainerAdd::getItem ()
{
        return *_item;
}

void GRMContainerAdd::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _cid = new TWord8 (msg);

        TThingFactory tf (msg, dat);
        _item = tf.getThing ();
}

/***************************************************************
 * ContainerUpdate
 ***************************************************************/
GRMContainerUpdate::GRMContainerUpdate (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMContainerUpdate::GRMContainerUpdate (uint8_t cid, uint8_t slot,
                                        const TThing& item)
{
        _id = new TWord8 ((uint8_t)GRM_CONT_UPDATE_ID);
        _cid = new TWord8 (cid);
        _slot = new TWord8 (slot);

        TThingFactory tf;
        _item = tf.cloneThing (item);
}

GRMContainerUpdate::GRMContainerUpdate (const GRMContainerUpdate& clone)
{
        _id = new TWord8 (*clone._id);
        _cid = new TWord8 (*clone._cid);
        _slot = new TWord8 (*clone._slot);

        TThingFactory tf;
        _item = tf.cloneThing (*clone._item);
}

GRMContainerUpdate::~GRMContainerUpdate ()
{
        delete _id;
        delete _cid;
        delete _slot;
        delete _item;
}

void GRMContainerUpdate::put (NetworkMessage* msg)
{
        _id->put (msg);
        _cid->put (msg);
        _slot->put (msg);
        _item->put (msg);
}

void GRMContainerUpdate::show ()
{
        printf ("GRMContainerUpdate {\n");
        printf ("\tcid: "); _cid->show (); printf ("\n");
        printf ("\tslot: "); _slot->show (); printf ("\n");
        _item->show ();
        printf ("\n}\n");
}

uint8_t GRMContainerUpdate::getId ()
{
        return _id->getVal ();
}

uint8_t GRMContainerUpdate::getCid ()
{
        return _cid->getVal ();
}

uint8_t GRMContainerUpdate::getSlot ()
{
        return _slot->getVal ();
}

const TThing& GRMContainerUpdate::getItem ()
{
        return *_item;
}

void GRMContainerUpdate::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _cid = new TWord8 (msg);
        _slot = new TWord8 (msg);

        TThingFactory tf (msg, dat);
        _item = tf.getThing ();
}


/***************************************************************
 * ContainerRemove
 ***************************************************************/
GRMContainerRemove::GRMContainerRemove (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMContainerRemove::GRMContainerRemove (uint8_t cid, uint8_t slot)
{
        _id = new TWord8 ((uint8_t)GRM_CONT_REMOVE_ID);
        _cid = new TWord8 (cid);
        _slot = new TWord8 (slot);
}

GRMContainerRemove::GRMContainerRemove (const GRMContainerRemove& clone)
{
        _id = new TWord8 (*clone._id);
        _cid = new TWord8 (*clone._cid);
        _slot = new TWord8 (*clone._slot);
}

GRMContainerRemove::~GRMContainerRemove ()
{
        delete _id;
        delete _cid;
        delete _slot;
}

void GRMContainerRemove::put (NetworkMessage* msg)
{
        _id->put (msg);
        _cid->put (msg);
        _slot->put (msg);
}

void GRMContainerRemove::show ()
{
        printf ("GRMContainerRemove {\n");
        printf ("\tcid: "); _cid->show (); printf ("\n");
        printf ("\tslot: "); _slot->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMContainerRemove::getId ()
{
        return _id->getVal ();
}

uint8_t GRMContainerRemove::getCid ()
{
        return _cid->getVal ();
}

uint8_t GRMContainerRemove::getSlot ()
{
        return _slot->getVal ();
}

void GRMContainerRemove::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _cid = new TWord8 (msg);
        _slot = new TWord8 (msg);
}

/***************************************************************
 * OpenSelfTrade
 ***************************************************************/
GRMOpenSelfTrade::GRMOpenSelfTrade (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMOpenSelfTrade::GRMOpenSelfTrade (const TTradeContainer& container)
{
        _id = new TWord8 ((uint8_t)GRM_OPEN_SELF_TRADE_ID);
        _container = new TTradeContainer (container);
}

GRMOpenSelfTrade::GRMOpenSelfTrade (const GRMOpenSelfTrade& clone)
{
        _id = new TWord8 (*clone._id);
        _container = new TTradeContainer (*clone._container);
}

GRMOpenSelfTrade::~GRMOpenSelfTrade ()
{
        delete _id;
        delete _container;
}

void GRMOpenSelfTrade::put (NetworkMessage* msg)
{
        _id->put (msg);
        _container->put (msg);
}

void GRMOpenSelfTrade::show ()
{
        printf ("GRMOpenSelfTrade {\n");
        _container->show ();
        printf ("}\n");
}

uint8_t GRMOpenSelfTrade::getId ()
{
        return _id->getVal ();
}

TTradeContainer& GRMOpenSelfTrade::getTradeContainer ()
{
        return *_container;
}

void GRMOpenSelfTrade::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _container = new TTradeContainer (msg, dat);
}

/***************************************************************
 * OpenPlayerTrade
 ***************************************************************/
GRMOpenPlayerTrade::GRMOpenPlayerTrade (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMOpenPlayerTrade::GRMOpenPlayerTrade (const TTradeContainer& container)
{
        _id = new TWord8 ((uint8_t)GRM_OPEN_PLAYER_TRADE_ID);
        _container = new TTradeContainer (container);
}

GRMOpenPlayerTrade::GRMOpenPlayerTrade (const GRMOpenPlayerTrade& clone)
{
        _id = new TWord8 (*clone._id);
        _container = new TTradeContainer (*clone._container);
}

GRMOpenPlayerTrade::~GRMOpenPlayerTrade ()
{
        delete _id;
        delete _container;
}

void GRMOpenPlayerTrade::put (NetworkMessage* msg)
{
        _id->put (msg);
        _container->put (msg);
}

void GRMOpenPlayerTrade::show ()
{
        printf ("GRMOpenPlayerTrade {\n");
        _container->show ();
        printf ("}\n");
}

uint8_t GRMOpenPlayerTrade::getId ()
{
        return _id->getVal ();
}

TTradeContainer& GRMOpenPlayerTrade::getTradeContainer ()
{
        return *_container;
}

void GRMOpenPlayerTrade::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _container = new TTradeContainer (msg, dat);
}

/***************************************************************
 * CloseTrade
 ***************************************************************/
GRMCloseTrade::GRMCloseTrade (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCloseTrade::GRMCloseTrade ()
{
        _id = new TWord8 ((uint8_t)GRM_CLOSE_TRADE_ID);
}

GRMCloseTrade::GRMCloseTrade (const GRMCloseTrade& clone)
{
        _id = new TWord8 (*clone._id);
}

GRMCloseTrade::~GRMCloseTrade ()
{
        delete _id;
}

void GRMCloseTrade::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GRMCloseTrade::show ()
{
        printf ("GRMCloseTrade {}\n");
}

uint8_t GRMCloseTrade::getId ()
{
        return _id->getVal ();
}

void GRMCloseTrade::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
}


/***************************************************************
 * CreatureMessages
 ***************************************************************/
/***************************************************************
 * CreatureSquare
 ***************************************************************/
GRMCreatureSquare::GRMCreatureSquare (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureSquare::GRMCreatureSquare (uint32_t creatureid, uint8_t color)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_SQUARE_ID);
        _creatureid = new TWord32 (creatureid);
        _color = new TWord8 (color);
}

GRMCreatureSquare::GRMCreatureSquare (const GRMCreatureSquare& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        _color = new TWord8 (*clone._color);
}

GRMCreatureSquare::~GRMCreatureSquare ()
{
        delete _id;
        delete _creatureid;
        delete _color;
}

void GRMCreatureSquare::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
        _color->put (msg);
}

void GRMCreatureSquare::show ()
{
        printf ("GRMCreatureSquare {\n");
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("\tcolor: "); _color->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMCreatureSquare::getId ()
{
        return _id->getVal ();
}

uint32_t GRMCreatureSquare::getCreatureId ()
{
        return _creatureid->getVal ();
}

uint8_t GRMCreatureSquare::getColor ()
{
        return _color->getVal ();
}

void GRMCreatureSquare::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
        _color = new TWord8 (msg);
}

/***************************************************************
 * CreatureOutfit
 ***************************************************************/
GRMCreatureOutfit::GRMCreatureOutfit (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureOutfit::GRMCreatureOutfit (uint32_t creatureid, const TOutfit& outfit)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_OUTFIT_ID);
        _creatureid = new TWord32 (creatureid);

        TOutfitFactory of;
        _outfit = of.cloneOutfit (outfit);
}

GRMCreatureOutfit::GRMCreatureOutfit (const GRMCreatureOutfit& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        
        TOutfitFactory of;
        _outfit = of.cloneOutfit (*clone._outfit);
}

GRMCreatureOutfit::~GRMCreatureOutfit ()
{
        delete _id;
        delete _creatureid;
        delete _outfit;
}

void GRMCreatureOutfit::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
        _outfit->put (msg);
}

void GRMCreatureOutfit::show ()
{
        printf ("GRMCreatureOutfit {\n");
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("\toutfit: "); _outfit->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMCreatureOutfit::getId ()
{
        return _id->getVal ();
}

uint32_t GRMCreatureOutfit::getCreatureId ()
{
        return _creatureid->getVal ();
}

const TOutfit& GRMCreatureOutfit::getOutfit ()
{
        return *_outfit;
}

void GRMCreatureOutfit::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);

        TOutfitFactory of (msg, dat);
        _outfit = of.getOutfit ();
}

/***************************************************************
 * CreatureHealth
 ***************************************************************/
GRMCreatureHealth::GRMCreatureHealth (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureHealth::GRMCreatureHealth (uint32_t creatureid, uint8_t hp)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_HEALTH_ID);
        _creatureid = new TWord32 (creatureid);
        _hp = new TWord8 (hp);
}

GRMCreatureHealth::GRMCreatureHealth (const GRMCreatureHealth& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        _hp = new TWord8 (*clone._hp);
}

GRMCreatureHealth::~GRMCreatureHealth ()
{
        delete _id;
        delete _creatureid;
        delete _hp;
}

void GRMCreatureHealth::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
        _hp->put (msg);
}

void GRMCreatureHealth::show ()
{
        printf ("GRMCreatureHealth {\n");
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("\thp: "); _hp->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMCreatureHealth::getId ()
{
        return _id->getVal ();
}

uint32_t GRMCreatureHealth::getCreatureId ()
{
        return _creatureid->getVal ();
}

uint8_t GRMCreatureHealth::getHp ()
{
        return _hp->getVal ();
}

void GRMCreatureHealth::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
        _hp = new TWord8 (msg);
}

/***************************************************************
 * CreatureSpeed
 ***************************************************************/
GRMCreatureSpeed::GRMCreatureSpeed (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureSpeed::GRMCreatureSpeed (uint32_t creatureid, uint8_t speed)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_SPEED_ID);
        _creatureid = new TWord32 (creatureid);
        _speed = new TWord8 (speed);
}

GRMCreatureSpeed::GRMCreatureSpeed (const GRMCreatureSpeed& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        _speed = new TWord8 (*clone._speed);
}

GRMCreatureSpeed::~GRMCreatureSpeed ()
{
        delete _id;
        delete _creatureid;
        delete _speed;
}

void GRMCreatureSpeed::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
        _speed->put (msg);
}

void GRMCreatureSpeed::show ()
{
        printf ("GRMCreatureSpeed {\n");
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("\tspeed: "); _speed->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMCreatureSpeed::getId ()
{
        return _id->getVal ();
}

uint32_t GRMCreatureSpeed::getCreatureId ()
{
        return _creatureid->getVal ();
}

uint8_t GRMCreatureSpeed::getSpeed ()
{
        return _speed->getVal ();
}

void GRMCreatureSpeed::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
        _speed = new TWord8 (msg);
}

/***************************************************************
 * CreatureSkull
 ***************************************************************/
GRMCreatureSkull::GRMCreatureSkull (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureSkull::GRMCreatureSkull (uint32_t creatureid, uint8_t skull)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_SKULL_ID);
        _creatureid = new TWord32 (creatureid);
        _skull = new TWord8 (skull);
}

GRMCreatureSkull::GRMCreatureSkull (const GRMCreatureSkull& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        _skull = new TWord8 (*clone._skull);
}

GRMCreatureSkull::~GRMCreatureSkull ()
{
        delete _id;
        delete _creatureid;
        delete _skull;
}

void GRMCreatureSkull::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
        _skull->put (msg);
}

void GRMCreatureSkull::show ()
{
        printf ("GRMCreatureSkull {\n");
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("\tskull: "); _skull->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMCreatureSkull::getId ()
{
        return _id->getVal ();
}

uint32_t GRMCreatureSkull::getCreatureId ()
{
        return _creatureid->getVal ();
}

uint8_t GRMCreatureSkull::getSkull ()
{
        return _skull->getVal ();
}

void GRMCreatureSkull::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
        _skull = new TWord8 (msg);
}

/***************************************************************
 * CreatureShield
 ***************************************************************/
GRMCreatureShield::GRMCreatureShield (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMCreatureShield::GRMCreatureShield (uint32_t creatureid, uint8_t shield)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_SHIELD_ID);
        _creatureid = new TWord32 (creatureid);
        _shield = new TWord8 (shield);
}

GRMCreatureShield::GRMCreatureShield (const GRMCreatureShield& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        _shield = new TWord8 (*clone._shield);
}

GRMCreatureShield::~GRMCreatureShield ()
{
        delete _id;
        delete _creatureid;
        delete _shield;
}

void GRMCreatureShield::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
        _shield->put (msg);
}

void GRMCreatureShield::show ()
{
        printf ("GRMCreatureShield {\n");
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("\tshield: "); _shield->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMCreatureShield::getId ()
{
        return _id->getVal ();
}

uint32_t GRMCreatureShield::getCreatureId ()
{
        return _creatureid->getVal ();
}

uint8_t GRMCreatureShield::getShield ()
{
        return _shield->getVal ();
}

void GRMCreatureShield::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
        _shield = new TWord8 (msg);
}

/***************************************************************
 * AnimatedText
 ***************************************************************/
GRMAnimatedText::GRMAnimatedText (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMAnimatedText::GRMAnimatedText (const TPos& pos, uint8_t color,
                                  const std::string& text)
{
        _id = new TWord8 ((uint8_t)GRM_ANIMATED_TEXT_ID);
        _pos = new TPos (pos);
        _color = new TWord8 (color);
        _text = new TString (text);
}

GRMAnimatedText::GRMAnimatedText (const GRMAnimatedText& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _color = new TWord8 (*clone._color);
        _text = new TString (*clone._text);
}

GRMAnimatedText::~GRMAnimatedText ()
{
        delete _id;
        delete _pos;
        delete _color;
        delete _text;
}

void GRMAnimatedText::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _color->put (msg);
        _text->put (msg);
}

void GRMAnimatedText::show ()
{
        printf ("GRMAnimatedText {\n");
        printf ("\tpos: "); _pos->show (); printf ("\n");
        printf ("\tcolor: "); _color->show (); printf ("\n");
        printf ("\ttext: "); _text->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMAnimatedText::getId ()
{
        return _id->getVal ();
}


uint8_t GRMAnimatedText::getColor ()
{
        return _color->getVal ();
}

const TPos& GRMAnimatedText::getPos ()
{
        return *_pos;
}

const std::string& GRMAnimatedText::getText ()
{
        return _text->getString ();
}

void GRMAnimatedText::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        _color = new TWord8 (msg);
        _text = new TString (msg);
}

/***************************************************************
 * PlayerCancelAttack
 ***************************************************************/

GRMPlayerCancelAttack::GRMPlayerCancelAttack (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMPlayerCancelAttack::GRMPlayerCancelAttack ()
{
        _id = new TWord8 ((uint8_t)GRM_PLAYER_CANCEL_ATTACK_ID);
}

GRMPlayerCancelAttack::GRMPlayerCancelAttack (const GRMPlayerCancelAttack& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GRMPlayerCancelAttack::~GRMPlayerCancelAttack ()
{
        delete _id;
}

void GRMPlayerCancelAttack::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GRMPlayerCancelAttack::show ()
{
        printf ("GRMPlayerCancelAttack {}\n");
}

uint8_t GRMPlayerCancelAttack::getId ()
{
        return _id->getVal ();
}

void GRMPlayerCancelAttack::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * ShopTrade
 ***************************************************************/

GRMShopTrade::GRMShopTrade (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMShopTrade::GRMShopTrade (TShopList* shoplist)
{
        _id = new TWord8 ((uint8_t)GRM_SHOP_TRADE_ID);
        _shoplist = shoplist;
}

GRMShopTrade::GRMShopTrade (const GRMShopTrade& clone)
{
        _id = new TWord8 (*clone._id);
        _shoplist = new TShopList (*clone._shoplist);
}
        
GRMShopTrade::~GRMShopTrade ()
{
        delete _id;
        delete _shoplist;
}

void GRMShopTrade::put (NetworkMessage* msg)
{
        _id->put (msg);
        _shoplist->put (msg);
}

void GRMShopTrade::show ()
{
        printf ("GRMShopTrade {\n");
        _shoplist->show ();
        printf ("}\n");
}

uint8_t GRMShopTrade::getId ()
{
        return _id->getVal ();
}

TShopList& GRMShopTrade::getShopList ()
{
        return *_shoplist;
}

void GRMShopTrade::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _shoplist = new TShopList (msg, dat);
}

/***************************************************************
 * ShopGold
 ***************************************************************/

GRMShopGold::GRMShopGold (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMShopGold::GRMShopGold (uint32_t ngold)
{
        _id = new TWord8 ((uint8_t)GRM_SHOP_GOLD_ID);
        _ngold = new TWord32 (ngold);
}

GRMShopGold::GRMShopGold (const GRMShopGold& clone)
{
        _id = new TWord8 (*clone._id);
        _ngold = new TWord32 (*clone._ngold);
}
        
GRMShopGold::~GRMShopGold ()
{
        delete _id;
        delete _ngold;
}

void GRMShopGold::put (NetworkMessage* msg)
{
        _id->put (msg);
        _ngold->put (msg);
}

void GRMShopGold::show ()
{
        printf ("GRMShopGold {gold: "); _ngold->show (); printf ("}\n");
}

uint8_t GRMShopGold::getId ()
{
        return _id->getVal ();
}

uint32_t GRMShopGold::getNGold ()
{
        return _ngold->getVal ();
}

void GRMShopGold::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _ngold = new TWord32 (msg);
}

/***************************************************************
 * ShopClose
 ***************************************************************/

GRMShopClose::GRMShopClose (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMShopClose::GRMShopClose ()
{
        _id = new TWord8 ((uint8_t)GRM_SHOP_CLOSE_ID);
}

GRMShopClose::GRMShopClose (const GRMShopClose& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GRMShopClose::~GRMShopClose ()
{
        delete _id;
}

void GRMShopClose::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GRMShopClose::show ()
{
        printf ("GRMShopClose {}\n");
}

uint8_t GRMShopClose::getId ()
{
        return _id->getVal ();
}

void GRMShopClose::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * TShootEffect
 ***************************************************************/

GRMShootEffect::GRMShootEffect (NetworkMessage* msg, GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

GRMShootEffect::GRMShootEffect (const TPos& start, const TPos& end,
                                uint8_t effect)
{
        _id = new TWord8 ((uint8_t)GRM_SHOOT_EFFECT_ID);
        _start = new TPos (start);
        _end = new TPos (end);
        _effect = new TWord8 (effect);
}

GRMShootEffect::GRMShootEffect (const GRMShootEffect& clone)
{
        _id = new TWord8 (*clone._id);
        _start = new TPos (*clone._start);
        _end = new TPos (*clone._end);
        _effect = new TWord8 (*clone._effect);
}
        
GRMShootEffect::~GRMShootEffect ()
{
        delete _id;
        delete _start;
        delete _end;
        delete _effect;
}

void GRMShootEffect::put (NetworkMessage* msg)
{
        _id->put (msg);
        _start->put (msg);
        _end->put (msg);
        _effect->put (msg);
}

void GRMShootEffect::show ()
{
        printf ("TShootEffect {\n");
        printf ("\tstart: "); _start->show (); printf ("\n");
        printf ("\tend: "); _end->show (); printf ("\n");
        printf ("\teffect: "); _effect->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMShootEffect::getId ()
{
        return _id->getVal ();
}

uint8_t GRMShootEffect::getEffect () const
{
        return _effect->getVal ();
}

const TPos& GRMShootEffect::getStart () const
{
        return *_start;
}

const TPos& GRMShootEffect::getEnd () const
{
        return *_end;
}

void GRMShootEffect::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _start = new TPos (msg);
        _end = new TPos (msg);
        _effect = new TWord8 (msg);
}

/***************************************************************
 * PlayerIcons
 ***************************************************************/

GRMPlayerIcons::GRMPlayerIcons (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMPlayerIcons::GRMPlayerIcons (uint16_t icons)
{
        _id = new TWord8 ((uint8_t)GRM_SHOP_GOLD_ID);
        _icons = new TWord16 (icons);
}

GRMPlayerIcons::GRMPlayerIcons (const GRMPlayerIcons& clone)
{
        _id = new TWord8 (*clone._id);
        _icons = new TWord16 (*clone._icons);
}
        
GRMPlayerIcons::~GRMPlayerIcons ()
{
        delete _id;
        delete _icons;
}

void GRMPlayerIcons::put (NetworkMessage* msg)
{
        _id->put (msg);
        _icons->put (msg);
}

void GRMPlayerIcons::show ()
{
        printf ("GRMPlayerIcons {icons: "); _icons->show (); printf ("}\n");
}

uint8_t GRMPlayerIcons::getId ()
{
        return _id->getVal ();
}

uint16_t GRMPlayerIcons::getIcons ()
{
        return _icons->getVal ();
}

void GRMPlayerIcons::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _icons = new TWord16 (msg);
}

/***************************************************************
 * OutfitWindow
 ***************************************************************/

GRMOutfitWindow::GRMOutfitWindow (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMOutfitWindow::GRMOutfitWindow (const TOutfit& self, TOutfitList* outfits)
{
        _id = new TWord8 ((uint8_t)GRM_OUTFIT_WINDOW_ID);

        TOutfitFactory of;
        _self = of.cloneOutfit (self);
        _outfits = outfits;
}

GRMOutfitWindow::GRMOutfitWindow (const GRMOutfitWindow& clone)
{
        _id = new TWord8 (*clone._id);

        TOutfitFactory of;
        _self = of.cloneOutfit (*clone._self);
        _outfits = new TOutfitList (*clone._outfits);
}
        
GRMOutfitWindow::~GRMOutfitWindow ()
{
        delete _id;
        delete _self;
        delete _outfits;
}

void GRMOutfitWindow::put (NetworkMessage* msg)
{
        _id->put (msg);
        _self->put (msg);
        _outfits->put (msg);
}

void GRMOutfitWindow::show ()
{
        printf ("GRMOutfitWindow {\n");
        printf ("\tself outfit: "); _self->show (); printf ("}\n");
        printf ("\toutfit list: "); _outfits->show (); printf ("}\n");
}

uint8_t GRMOutfitWindow::getId ()
{
        return _id->getVal ();
}

const TOutfit& GRMOutfitWindow::getSelf ()
{
        return *_self;
}

TOutfitList& GRMOutfitWindow::getOutfits ()
{
        return *_outfits;
}

void GRMOutfitWindow::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        
        TOutfitFactory of (msg, dat);
        _self = of.getOutfit ();
        _outfits = new TOutfitList (msg);
}

/***************************************************************
 * VipInit
 ***************************************************************/

GRMVipInit::GRMVipInit (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMVipInit::GRMVipInit (uint32_t creatureid, const std::string& name,
                                uint8_t online)
{
        _id = new TWord8 ((uint8_t)GRM_VIP_INIT_ID);
        _creatureid = new TWord32 (creatureid);
        _name = new TString (name);
        _online = new TWord8 (online);
}

GRMVipInit::GRMVipInit (const GRMVipInit& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        _name = new TString (*clone._name);
        _online = new TWord8 (*clone._online);
}
        
GRMVipInit::~GRMVipInit ()
{
        delete _id;
        delete _creatureid;
        delete _name;
        delete _online;
}

void GRMVipInit::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
        _name->put (msg);
        _online->put (msg);
}

void GRMVipInit::show ()
{
        printf ("GRMVipInit {\n"); 
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("\tonline: "); _online->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMVipInit::getId ()
{
        return _id->getVal ();
}

uint32_t GRMVipInit::getCreatureId ()
{
        return _creatureid->getVal ();
}

const std::string& GRMVipInit::getName ()
{
        return _name->getString ();
}

uint8_t GRMVipInit::getOnline ()
{
        return _online->getVal ();
}

void GRMVipInit::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
        _name = new TString (msg);
        _online = new TWord8 (msg);
}

/***************************************************************
 * VipLogin
 ***************************************************************/

GRMVipLogin::GRMVipLogin (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMVipLogin::GRMVipLogin (uint32_t creatureid)
{
        _id = new TWord8 ((uint8_t)GRM_VIP_LOGIN_ID);
        _creatureid = new TWord32 (creatureid);
}

GRMVipLogin::GRMVipLogin (const GRMVipLogin& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
}
        
GRMVipLogin::~GRMVipLogin ()
{
        delete _id;
        delete _creatureid;
}

void GRMVipLogin::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
}

void GRMVipLogin::show ()
{
        printf ("GRMVipLogin {\n"); 
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMVipLogin::getId ()
{
        return _id->getVal ();
}

uint32_t GRMVipLogin::getCreatureId ()
{
        return _creatureid->getVal ();
}

void GRMVipLogin::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
}

/***************************************************************
 * VipLogout
 ***************************************************************/

GRMVipLogout::GRMVipLogout (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMVipLogout::GRMVipLogout (uint32_t creatureid)
{
        _id = new TWord8 ((uint8_t)GRM_VIP_LOGOUT_ID);
        _creatureid = new TWord32 (creatureid);
}

GRMVipLogout::GRMVipLogout (const GRMVipLogout& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
}
        
GRMVipLogout::~GRMVipLogout ()
{
        delete _id;
        delete _creatureid;
}

void GRMVipLogout::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
}

void GRMVipLogout::show ()
{
        printf ("GRMVipLogout {\n"); 
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMVipLogout::getId ()
{
        return _id->getVal ();
}

uint32_t GRMVipLogout::getCreatureId ()
{
        return _creatureid->getVal ();
}

void GRMVipLogout::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
}

/***************************************************************
 * ShowTutorial
 ***************************************************************/

GRMShowTutorial::GRMShowTutorial (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMShowTutorial::GRMShowTutorial (uint8_t tutorialid)
{
        _id = new TWord8 ((uint8_t)GRM_SHOW_TUTORIAL_ID);
        _tutorialid = new TWord8 (tutorialid);
}

GRMShowTutorial::GRMShowTutorial (const GRMShowTutorial& clone)
{
        _id = new TWord8 (*clone._id);
        _tutorialid = new TWord8 (*clone._tutorialid);
}
        
GRMShowTutorial::~GRMShowTutorial ()
{
        delete _id;
        delete _tutorialid;
}

void GRMShowTutorial::put (NetworkMessage* msg)
{
        _id->put (msg);
        _tutorialid->put (msg);
}

void GRMShowTutorial::show ()
{
        printf ("GRMShowTutorial {\n"); 
        printf ("\ttutorialid: "); _tutorialid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMShowTutorial::getId ()
{
        return _id->getVal ();
}

uint8_t GRMShowTutorial::getTutorialId ()
{
        return _tutorialid->getVal ();
}

void GRMShowTutorial::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _tutorialid = new TWord8 (msg);
}

/***************************************************************
 * MiniMapMark
 ***************************************************************/

GRMMiniMapMark::GRMMiniMapMark (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMMiniMapMark::GRMMiniMapMark (const TPos& pos, uint8_t icon,
                                const std::string& description)
{
        _id = new TWord8 ((uint8_t)GRM_MINI_MAP_MARK_ID);
        _pos = new TPos (pos);
        _icon = new TWord8 (icon);
        _description = new TString (description);
}

GRMMiniMapMark::GRMMiniMapMark (const GRMMiniMapMark& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _icon = new TWord8 (*clone._icon);
        _description = new TString (*clone._description);
}
        
GRMMiniMapMark::~GRMMiniMapMark ()
{
        delete _id;
        delete _pos;
        delete _icon;
        delete _description;
}

void GRMMiniMapMark::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _icon->put (msg);
        _description->put (msg);
}

void GRMMiniMapMark::show ()
{
        printf ("GRMMiniMapMark {\n"); 
        printf ("\tpos: "); _pos->show (); printf ("\n");
        printf ("\ticon: "); _icon->show (); printf ("\n");
        printf ("\tdescription: "); _description->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMMiniMapMark::getId ()
{
        return _id->getVal ();
}

const TPos& GRMMiniMapMark::getPos ()
{
        return *_pos;
}

uint8_t GRMMiniMapMark::getIcon ()
{
        return _icon->getVal ();
}

const std::string& GRMMiniMapMark::getDescription ()
{
        return _description->getString ();
}

void GRMMiniMapMark::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        _icon = new TWord8 (msg);
        _description = new TString (msg);
}

/***************************************************************
 * QuestList
 ***************************************************************/

GRMQuestList::GRMQuestList (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMQuestList::GRMQuestList (TQuestList* questlist)
{
        _id = new TWord8 ((uint8_t)GRM_QUEST_LIST_ID);
        _questlist = questlist;
}

GRMQuestList::GRMQuestList (const GRMQuestList& clone)
{
        _id = new TWord8 (*clone._id);
        _questlist = new TQuestList (*clone._questlist);
}
        
GRMQuestList::~GRMQuestList ()
{
        delete _id;
        delete _questlist;
}

void GRMQuestList::put (NetworkMessage* msg)
{
        _id->put (msg);
        _questlist->put (msg);
}

void GRMQuestList::show ()
{
        printf ("GRMQuestList {\n");
        _questlist->show ();
        printf ("}\n");
}

uint8_t GRMQuestList::getId ()
{
        return _id->getVal ();
}

TQuestList& GRMQuestList::getQuestList ()
{
        return *_questlist;
}

void GRMQuestList::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _questlist = new TQuestList (msg);
}

/***************************************************************
 * SubQuestList
 ***************************************************************/

GRMSubQuestList::GRMSubQuestList (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMSubQuestList::GRMSubQuestList (uint16_t questid, TSubQuestList* subquestlist)
{
        _id = new TWord8 ((uint8_t)GRM_SUB_QUEST_LIST_ID);
        _questid = new TWord16 (questid);
        _subquestlist = subquestlist;
}

GRMSubQuestList::GRMSubQuestList (const GRMSubQuestList& clone)
{
        _id = new TWord8 (*clone._id);
        _questid = new TWord16 (*clone._questid);
        _subquestlist = new TSubQuestList (*clone._subquestlist);
}
        
GRMSubQuestList::~GRMSubQuestList ()
{
        delete _id;
        delete _questid;
        delete _subquestlist;
}

void GRMSubQuestList::put (NetworkMessage* msg)
{
        _id->put (msg);
        _questid->put (msg);
        _subquestlist->put (msg);
}

void GRMSubQuestList::show ()
{
        printf ("GRMSubQuestList {\n");
        printf ("\tquestid: "); _questid->show (); printf ("\n");
        _subquestlist->show ();
        printf ("}\n");
}

uint8_t GRMSubQuestList::getId ()
{
        return _id->getVal ();
}

uint16_t GRMSubQuestList::getQuestId ()
{
        return _questid->getVal ();
}

TSubQuestList& GRMSubQuestList::getSubQuestList ()
{
        return *_subquestlist;
}

void GRMSubQuestList::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _questid = new TWord16 (msg);
        _subquestlist = new TSubQuestList (msg);
}

