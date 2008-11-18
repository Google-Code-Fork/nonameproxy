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
#include "tibiamessage.h"
#include "gamestate.h"
#include "enums.h"

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
        const std::string& account, const std::string& password)
{
        _id = new TWord8 ((uint8_t)0x01);

        _OS = new TWord16 (OS);
        _version = new TWord16 (version);
        _datsig = new TWord32 (datsig);
        _sprsig = new TWord32 (sprsig);
        _picsig = new TWord32 (picsig);
        _u1 = new TWord8 ((uint8_t)0);
        _xtea = new TXTEAKey (xtea);
        _account = new TString (account);
        _password = new TString (password);
        //the TByteBuffer (uint32_t len) constructer automatically
        //generates a random byte buffer
        uint32_t rem = RSA_LEN - (21 + _password->getLen () + _account->getLen ());
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

const std::string& LSMLoginMsg::getAccount ()
{
        return _account->getString ();
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
        _account = new TString (msg);
        _password = new TString (msg);
        //this time we just take the random bytes from the packet
        uint32_t rem = RSA_LEN - (21 + _password->getLen () + _account->getLen ());
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

/********************************************************************
 * Game Send Messages
 ********************************************************************/

/********************************************************************
 * Logout
 ********************************************************************/

GSMLogout::GSMLogout (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMLogout::GSMLogout ()
{
        _id = new TWord8 ((uint8_t)GSM_LOGOUT_ID);
}

GSMLogout::~GSMLogout ()
{
        delete _id;
}

uint8_t GSMLogout::getId ()
{
        return _id->getVal ();
}

void GSMLogout::show ()
{
        printf ("GSMLogout {}\n");
}

void GSMLogout::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMLogout::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * Ping
 ********************************************************************/

GSMPing::GSMPing (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPing::GSMPing ()
{
        _id = new TWord8 ((uint8_t)GSM_PING_ID);
}

GSMPing::~GSMPing ()
{
        delete _id;
}

uint8_t GSMPing::getId ()
{
        return _id->getVal ();
}

void GSMPing::show ()
{
        printf ("GSMPing {}\n");
}

void GSMPing::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMPing::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
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
        uint32_t* xtea, uint8_t isGM, const std::string& account, 
        const std::string& name, const std::string& password)
{
        _id = new TWord8 ((uint8_t)GSM_GAME_INIT_ID);

        _OS = new TWord16 (OS);
        _version = new TWord16 (version);
        _u1 = new TWord8 ((uint8_t)0);
        _xtea = new TXTEAKey (xtea);
        _isGM = new TWord8 (isGM);
        _account = new TString (account);
        _name = new TString (name);
        _password = new TString (password);
        //the TByteBuffer (uint32_t len) constructer automatically
        //generates a random byte buffer
        uint32_t rem = RSA_LEN - (24 + _password->getLen () + _name->getLen ()
                                        + _account->getLen ());
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

const std::string& GSMGameInit::getAccount ()
{
        return _account->getString ();
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
        _account = new TString (msg);
        _name = new TString (msg);
        _password = new TString (msg);
        //this time we just take the random bytes from the packet
        uint32_t rem = RSA_LEN - (24 + _password->getLen () + _name->getLen ()
                                        + _account->getLen ());
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

/***************************************************************
 * AutoWalk
 ***************************************************************/

GSMAutoWalk::GSMAutoWalk (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMAutoWalk::GSMAutoWalk (TDirectionList* directions)
{
        _id = new TWord8 ((uint8_t)GSM_AUTO_WALK_ID);
        _directions = directions;
}

GSMAutoWalk::GSMAutoWalk (const GSMAutoWalk& clone)
{
        _id = new TWord8 (*clone._id);
        _directions = new TDirectionList (*clone._directions);
}
        
GSMAutoWalk::~GSMAutoWalk ()
{
        delete _id;
        delete _directions;
}

void GSMAutoWalk::put (NetworkMessage* msg)
{
        _id->put (msg);
        _directions->put (msg);
}

void GSMAutoWalk::show ()
{
        printf ("GSMAutoWalk {\n");
        _directions->show ();
        printf ("}\n");
}

uint8_t GSMAutoWalk::getId ()
{
        return _id->getVal ();
}

TDirectionList& GSMAutoWalk::getDirectionList ()
{
        return *_directions;
}

void GSMAutoWalk::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _directions = new TDirectionList (msg);
}

/********************************************************************
 * MoveNorth
 ********************************************************************/

GSMMoveNorth::GSMMoveNorth (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveNorth::GSMMoveNorth ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_NORTH_ID);
}

GSMMoveNorth::~GSMMoveNorth ()
{
        delete _id;
}

uint8_t GSMMoveNorth::getId ()
{
        return _id->getVal ();
}

void GSMMoveNorth::show ()
{
        printf ("GSMMoveNorth {}\n");
}

void GSMMoveNorth::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveNorth::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveEast
 ********************************************************************/

GSMMoveEast::GSMMoveEast (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveEast::GSMMoveEast ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_EAST_ID);
}

GSMMoveEast::~GSMMoveEast ()
{
        delete _id;
}

uint8_t GSMMoveEast::getId ()
{
        return _id->getVal ();
}

void GSMMoveEast::show ()
{
        printf ("GSMMoveEast {}\n");
}

void GSMMoveEast::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveEast::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveSouth
 ********************************************************************/

GSMMoveSouth::GSMMoveSouth (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveSouth::GSMMoveSouth ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_SOUTH_ID);
}

GSMMoveSouth::~GSMMoveSouth ()
{
        delete _id;
}

uint8_t GSMMoveSouth::getId ()
{
        return _id->getVal ();
}

void GSMMoveSouth::show ()
{
        printf ("GSMMoveSouth {}\n");
}

void GSMMoveSouth::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveSouth::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveWest
 ********************************************************************/

GSMMoveWest::GSMMoveWest (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveWest::GSMMoveWest ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_WEST_ID);
}

GSMMoveWest::~GSMMoveWest ()
{
        delete _id;
}

uint8_t GSMMoveWest::getId ()
{
        return _id->getVal ();
}

void GSMMoveWest::show ()
{
        printf ("GSMMoveWest {}\n");
}

void GSMMoveWest::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveWest::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveNE
 ********************************************************************/

GSMMoveNE::GSMMoveNE (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveNE::GSMMoveNE ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_NE_ID);
}

GSMMoveNE::~GSMMoveNE ()
{
        delete _id;
}

uint8_t GSMMoveNE::getId ()
{
        return _id->getVal ();
}

void GSMMoveNE::show ()
{
        printf ("GSMMoveNE {}\n");
}

void GSMMoveNE::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveNE::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveSE
 ********************************************************************/

GSMMoveSE::GSMMoveSE (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveSE::GSMMoveSE ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_SE_ID);
}

GSMMoveSE::~GSMMoveSE ()
{
        delete _id;
}

uint8_t GSMMoveSE::getId ()
{
        return _id->getVal ();
}

void GSMMoveSE::show ()
{
        printf ("GSMMoveSE {}\n");
}

void GSMMoveSE::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveSE::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveSW
 ********************************************************************/

GSMMoveSW::GSMMoveSW (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveSW::GSMMoveSW ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_SW_ID);
}

GSMMoveSW::~GSMMoveSW ()
{
        delete _id;
}

uint8_t GSMMoveSW::getId ()
{
        return _id->getVal ();
}

void GSMMoveSW::show ()
{
        printf ("GSMMoveSW {}\n");
}

void GSMMoveSW::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveSW::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveNW
 ********************************************************************/

GSMMoveNW::GSMMoveNW (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMMoveNW::GSMMoveNW ()
{
        _id = new TWord8 ((uint8_t)GSM_MOVE_NW_ID);
}

GSMMoveNW::~GSMMoveNW ()
{
        delete _id;
}

uint8_t GSMMoveNW::getId ()
{
        return _id->getVal ();
}

void GSMMoveNW::show ()
{
        printf ("GSMMoveNW {}\n");
}

void GSMMoveNW::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMMoveNW::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * TurnNorth
 ********************************************************************/

GSMTurnNorth::GSMTurnNorth (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTurnNorth::GSMTurnNorth ()
{
        _id = new TWord8 ((uint8_t)GSM_TURN_NORTH_ID);
}

GSMTurnNorth::~GSMTurnNorth ()
{
        delete _id;
}

uint8_t GSMTurnNorth::getId ()
{
        return _id->getVal ();
}

void GSMTurnNorth::show ()
{
        printf ("GSMTurnNorth {}\n");
}

void GSMTurnNorth::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMTurnNorth::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * TurnEast
 ********************************************************************/

GSMTurnEast::GSMTurnEast (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTurnEast::GSMTurnEast ()
{
        _id = new TWord8 ((uint8_t)GSM_TURN_EAST_ID);
}

GSMTurnEast::~GSMTurnEast ()
{
        delete _id;
}

uint8_t GSMTurnEast::getId ()
{
        return _id->getVal ();
}

void GSMTurnEast::show ()
{
        printf ("GSMTurnEast {}\n");
}

void GSMTurnEast::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMTurnEast::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * TurnSouth
 ********************************************************************/

GSMTurnSouth::GSMTurnSouth (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTurnSouth::GSMTurnSouth ()
{
        _id = new TWord8 ((uint8_t)GSM_TURN_SOUTH_ID);
}

GSMTurnSouth::~GSMTurnSouth ()
{
        delete _id;
}

uint8_t GSMTurnSouth::getId ()
{
        return _id->getVal ();
}

void GSMTurnSouth::show ()
{
        printf ("GSMTurnSouth {}\n");
}

void GSMTurnSouth::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMTurnSouth::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * TurnWest
 ********************************************************************/

GSMTurnWest::GSMTurnWest (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTurnWest::GSMTurnWest ()
{
        _id = new TWord8 ((uint8_t)GSM_TURN_WEST_ID);
}

GSMTurnWest::~GSMTurnWest ()
{
        delete _id;
}

uint8_t GSMTurnWest::getId ()
{
        return _id->getVal ();
}

void GSMTurnWest::show ()
{
        printf ("GSMTurnWest {}\n");
}

void GSMTurnWest::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMTurnWest::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * AutoWalkCancel
 ********************************************************************/

GSMAutoWalkCancel::GSMAutoWalkCancel (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMAutoWalkCancel::GSMAutoWalkCancel ()
{
        _id = new TWord8 ((uint8_t)GSM_AUTO_WALK_CANCEL_ID);
}

GSMAutoWalkCancel::~GSMAutoWalkCancel ()
{
        delete _id;
}

uint8_t GSMAutoWalkCancel::getId ()
{
        return _id->getVal ();
}

void GSMAutoWalkCancel::show ()
{
        printf ("GSMAutoWalkCancel {}\n");
}

void GSMAutoWalkCancel::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMAutoWalkCancel::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * MoveItem
 ********************************************************************/

GSMMoveItem::GSMMoveItem (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

/* Ground to Ground constructor */
GSMMoveItem::GSMMoveItem (const TPos& from, uint16_t itemid, uint8_t stackpos, 
                          const TPos& to, uint8_t count)
{
        _fromType = ground;
        _toType   = ground;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (from);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
        _to       = new TPos (to);
        _count    = new TWord8 (count);
}

/* Ground to Container constructor */
GSMMoveItem::GSMMoveItem (const TPos& from, uint16_t itemid, uint8_t stackpos, 
                          uint8_t toContainerId, uint8_t toContainerPos,
                          uint8_t count)
{
        _fromType = ground;
        _toType   = container;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (from);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
        _to       = new TPos (0xFFFF, 0x0040 | toContainerId, toContainerPos);
        _count    = new TWord8 (count);
}

/* Ground to Inventory constructor */
GSMMoveItem::GSMMoveItem (const TPos& from, uint16_t itemid, uint8_t stackpos,
                          uint8_t toSlot, uint8_t count)
{
        _fromType = ground;
        _toType   = inventory;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (from);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
        _to       = new TPos (0xFFFF, toSlot, 0);
        _count    = new TWord8 (count);
}

/* Container to Ground constructor */  
GSMMoveItem::GSMMoveItem (uint8_t fromContainerId, uint8_t fromContainerPos,
                             uint16_t itemid, const TPos& to, uint8_t count)
{
        _fromType = container;
        _toType   = ground;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (0xFFFF, 0x0040 | fromContainerId, 
                                                        fromContainerPos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (fromContainerPos);
        _to       = new TPos (to);
        _count    = new TWord8 (count);
}


/* Container to Container constructor */
GSMMoveItem::GSMMoveItem (uint8_t fromContainerId, uint8_t fromContainerPos,
                          uint16_t itemid, uint8_t toContainerId,
                          uint8_t toContainerPos, uint8_t count)
{
        _fromType = container;
        _toType   = container;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (0xFFFF, 0x0040 | fromContainerId, 
                                                        fromContainerPos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (fromContainerPos);
        _to       = new TPos (0xFFFF, 0x0040 | toContainerId, toContainerPos);
        _count    = new TWord8 (count);
}

/* Container to Inventory constructor */
GSMMoveItem::GSMMoveItem (uint8_t fromContainerId, uint8_t fromContainerPos,
                          uint16_t itemid, uint8_t toSlot, uint8_t count)
{
        _fromType = container;
        _toType   = inventory;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (0xFFFF, 0x0040 | fromContainerId, 
                                                        fromContainerPos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (fromContainerPos);
        _to       = new TPos (0xFFFF, toSlot, 0);
        _count    = new TWord8 (count);
}

/* Inventory to Ground constructor */
GSMMoveItem::GSMMoveItem (uint8_t fromSlot, uint16_t itemid, 
                          const TPos& to, uint8_t count)
{
        _fromType = inventory;
        _toType   = ground;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (0xFFFF, fromSlot, 0);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 ((uint8_t)0);
        _to       = new TPos (to);
        _count    = new TWord8 (count);
}

/* Inventory to Container constructor */
GSMMoveItem::GSMMoveItem (uint8_t fromSlot, uint16_t itemid, 
                          uint8_t toContainerId, uint8_t toContainerPos, 
                          uint8_t count)
{
        _fromType = inventory;
        _toType   = container;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (0xFFFF, fromSlot, 0);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 ((uint8_t)0);
        _to       = new TPos (0xFFFF, 0x0040 | toContainerId, toContainerPos);
        _count    = new TWord8 (count);
}

/* Inventory to Inventory constructor */
GSMMoveItem::GSMMoveItem (uint8_t fromSlot, uint16_t itemid,
                          uint8_t toSlot, uint8_t count)
{
        _fromType = inventory;
        _toType   = inventory;

        _id       = new TWord8 ((uint8_t)GSM_MOVE_ITEM_ID);
        _from     = new TPos (0xFFFF, fromSlot, 0);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 ((uint8_t)0);
        _to       = new TPos (0xFFFF, toSlot, 0);
        _count    = new TWord8 (count);
}


GSMMoveItem::GSMMoveItem (const GSMMoveItem& clone)
{
        _fromType = clone._fromType;
        _toType   = clone._toType;

        _id       = new TWord8 (*clone._id);
        _from     = new TPos (*clone._from);
        _itemid   = new TWord16 (*clone._itemid);
        _stackpos = new TWord8 (*clone._stackpos);
        _to       = new TPos (*clone._to);
        _count    = new TWord8 (*clone._count);
}

GSMMoveItem::~GSMMoveItem ()
{
        delete _id;
        delete _from;
        delete _itemid;
        delete _stackpos;
        delete _to;
        delete _count;
}

GSMMoveItem::MoveType GSMMoveItem::getFromType ()
{
        return _fromType;
}

GSMMoveItem::MoveType GSMMoveItem::getToType ()
{
        return _toType;
}

uint8_t GSMMoveItem::getId ()
{
        return _id->getVal ();
}


const TPos& GSMMoveItem::getFromPos ()
{
        return *_from;
}

uint8_t GSMMoveItem::getStackPos ()
{
        return _stackpos->getVal ();
}

uint8_t  GSMMoveItem::getFromContainerId ()
{
        return _from->y () & 0x3F;
}

uint8_t  GSMMoveItem::getFromContainerPos ()
{
        return _from->z ();
}

uint8_t GSMMoveItem::getFromSlot ()
{
        return _from->y ();
}

const TPos&GSMMoveItem::getToPos ()
{
        return *_to;
}

uint8_t GSMMoveItem::getToContainerId ()
{
        return _to->x () & 0x3F;
}

uint8_t GSMMoveItem::getToContainerPos ()
{
        return _to->z ();
}

uint8_t GSMMoveItem::getToSlot ()
{
        return _to->y ();
}

uint16_t GSMMoveItem::getItemId ()
{
        return _itemid->getVal ();
}

uint8_t GSMMoveItem::getCount ()
{
        return _count->getVal ();
}
 
void GSMMoveItem::show ()
{
        printf ("GSMMoveItem {\n");
        printf ("from: "); _from->show (); printf ("\n");
        printf ("itemid: "); _itemid->show (); printf ("\n");
        printf ("stackpos: "); _stackpos->show (); printf ("\n");
        printf ("to: "); _to->show (); printf ("\n");
        printf ("count: "); _count->show (); printf ("\n");
        printf ("}\n");
}

void GSMMoveItem::put (NetworkMessage* msg)
{
        _id->put (msg);
        _from->put (msg);
        _itemid->put (msg);
        _stackpos->put (msg);
        _to->put (msg);
        _count->put (msg);
}

void GSMMoveItem::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id       = new TWord8 (msg);
        _from     = new TPos (msg);
        _itemid   = new TWord16 (msg);
        _stackpos = new TWord8 (msg);
        _to       = new TPos (msg);
        _count    = new TWord8 (msg);

        if (_from->x () == 0xFFFF) {
                if (_from->y () & 0x40) {
                        _fromType = container;
                } else {
                        _fromType = inventory;
                }
        } else {
                _fromType = ground;
        }

        if (_to->x () == 0xFFFF) {
                if (_to->y () & 0x40) {
                        _toType = container;
                } else {
                        _toType = inventory;
                }
        } else {
                _toType = ground;
        }
}

/********************************************************************
 * TradeItem
 ********************************************************************/

GSMTradeItem::GSMTradeItem (NetworkMessage* msg, GameState* gs, DatReader* dat)
{

        get (msg, gs, dat);
}

/* Ground constructor */
GSMTradeItem::GSMTradeItem (const TPos& pos, uint16_t itemid, 
                            uint8_t stackpos, uint32_t playerid)
{
        _type     = ground;

        _id       = new TWord8 ((uint8_t)GSM_TRADE_ITEM_ID);
        _pos      = new TPos (pos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
        _playerid = new TWord32 (playerid);
}

/* Container constructor */
GSMTradeItem::GSMTradeItem (uint8_t containerid, uint8_t containerpos,
                            uint16_t itemid, uint32_t playerid)
{
        _type     = container;

        _id       = new TWord8 ((uint8_t)GSM_TRADE_ITEM_ID);
        _pos      = new TPos (0xFFFF, 0x40 | containerid, containerpos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (containerpos);
        _playerid = new TWord32 (playerid);
}

/* Container constructor */
GSMTradeItem::GSMTradeItem (uint8_t slot, uint16_t itemid, uint32_t playerid)
{
        _type     = inventory;

        _id       = new TWord8 ((uint8_t)GSM_TRADE_ITEM_ID);
        _pos      = new TPos (0xFFFF, slot, 0);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 ((uint8_t)0);
        _playerid = new TWord32 (playerid);
}

GSMTradeItem::GSMTradeItem (const GSMTradeItem& clone)
{
        _type   = clone._type;

        _id       = new TWord8 (*clone._id);
        _pos      = new TPos (*clone._pos);
        _itemid   = new TWord16 (*clone._itemid);
        _stackpos = new TWord8 (*clone._stackpos);
        _playerid = new TWord32 (*clone._playerid);
}

GSMTradeItem::~GSMTradeItem ()
{
        delete _id;
        delete _pos;
        delete _itemid;
        delete _stackpos;
        delete _playerid;
}

GSMTradeItem::TradeType GSMTradeItem::getType ()
{
        return _type;
}

uint8_t GSMTradeItem::getId ()
{
        return _id->getVal ();
}


const TPos& GSMTradeItem::getPos ()
{
        return *_pos;
}

uint8_t GSMTradeItem::getStackPos ()
{
        return _stackpos->getVal ();
}

uint8_t  GSMTradeItem::getContainerId ()
{
        return _pos->y () & 0x3F;
}

uint8_t  GSMTradeItem::getContainerPos ()
{
        return _pos->z ();
}

uint8_t GSMTradeItem::getSlot ()
{
        return _pos->y ();
}

uint16_t GSMTradeItem::getItemId ()
{
        return _itemid->getVal ();
}

uint32_t GSMTradeItem::getPlayerId ()
{
        return _playerid->getVal ();
}
 
void GSMTradeItem::show ()
{
        printf ("GSMTradeItem {\n");
        printf ("pos: "); _pos->show (); printf ("\n");
        printf ("itemid: "); _itemid->show (); printf ("\n");
        printf ("stackpos: "); _stackpos->show (); printf ("\n");
        printf ("playerid: "); _playerid->show (); printf ("\n");
        printf ("}\n");
}

void GSMTradeItem::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _itemid->put (msg);
        _stackpos->put (msg);
        _playerid->put (msg);
}

void GSMTradeItem::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id       = new TWord8 (msg);
        _pos      = new TPos (msg);
        _itemid   = new TWord16 (msg);
        _stackpos = new TWord8 (msg);
        _playerid = new TWord32 (msg);

        if (_pos->x () == 0xFFFF) {
                if (_pos->y () & 0x40) {
                        _type = container;
                } else {
                        _type = inventory;
                }
        } else {
                _type = ground;
        }
}

/***************************************************************
 * TradeLook
 ***************************************************************/

GSMTradeLook::GSMTradeLook (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTradeLook::GSMTradeLook (uint8_t myTrade, uint8_t index)
{
        _id = new TWord8 ((uint8_t)GSM_TRADE_LOOK_ID);
        _myTrade = new TWord8 (myTrade);
        _index = new TWord8 (index);
}

GSMTradeLook::GSMTradeLook (const GSMTradeLook& clone)
{
        _id = new TWord8 (*clone._id);
        _myTrade = new TWord8 (*clone._myTrade);
        _index = new TWord8 (*clone._index);
}
        
GSMTradeLook::~GSMTradeLook ()
{
        delete _id;
        delete _myTrade;
        delete _index;
}

void GSMTradeLook::put (NetworkMessage* msg)
{
        _id->put (msg);
        _myTrade->put (msg);
        _index->put (msg);
}

void GSMTradeLook::show ()
{
        printf ("GSMTradeLook {\n"); 
        printf ("myTrade: "); _myTrade->show (); printf ("\n");
        printf ("index: "); _index->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMTradeLook::getId ()
{
        return _id->getVal ();
}

uint8_t GSMTradeLook::getMyTrade ()
{
        return _myTrade->getVal ();
}

uint8_t GSMTradeLook::getIndex ()
{
        return _index->getVal ();
}

void GSMTradeLook::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _myTrade = new TWord8 (msg);
        _index = new TWord8 (msg);
}

/********************************************************************
 * TradeAccept
 ********************************************************************/

GSMTradeAccept::GSMTradeAccept (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTradeAccept::GSMTradeAccept ()
{
        _id = new TWord8 ((uint8_t)GSM_TRADE_ACCEPT_ID);
}

GSMTradeAccept::~GSMTradeAccept ()
{
        delete _id;
}

uint8_t GSMTradeAccept::getId ()
{
        return _id->getVal ();
}

void GSMTradeAccept::show ()
{
        printf ("GSMTradeAccept {}\n");
}

void GSMTradeAccept::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMTradeAccept::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * TradeReject
 ********************************************************************/

GSMTradeReject::GSMTradeReject (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTradeReject::GSMTradeReject ()
{
        _id = new TWord8 ((uint8_t)GSM_TRADE_REJECT_ID);
}

GSMTradeReject::~GSMTradeReject ()
{
        delete _id;
}

uint8_t GSMTradeReject::getId ()
{
        return _id->getVal ();
}

void GSMTradeReject::show ()
{
        printf ("GSMTradeReject {}\n");
}

void GSMTradeReject::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMTradeReject::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/********************************************************************
 * UseItem
 ********************************************************************/

GSMUseItem::GSMUseItem (NetworkMessage* msg, GameState* gs, DatReader* dat)
{

        get (msg, gs, dat);
}

/* Ground constructor */
GSMUseItem::GSMUseItem (const TPos& pos, uint16_t itemid, uint8_t stackpos)
{
        _type     = ground;

        _id       = new TWord8 ((uint8_t)GSM_USE_ITEM_ID);
        _pos      = new TPos (pos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
        _index    = new TWord8 ((uint8_t)1);
}

/* Container constructor */
GSMUseItem::GSMUseItem (uint8_t containerid, uint8_t containerpos,
                            uint16_t itemid)
{
        _type     = container;

        _id       = new TWord8 ((uint8_t)GSM_USE_ITEM_ID);
        _pos      = new TPos (0xFFFF, 0x40 | containerid, containerpos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (containerpos);
        _index    = new TWord8 (containerid);
}

/* Inventory constructor */
GSMUseItem::GSMUseItem (uint8_t slot, uint16_t itemid)
{
        _type     = inventory;

        _id       = new TWord8 ((uint8_t)GSM_USE_ITEM_ID);
        _pos      = new TPos (0xFFFF, slot, 0);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 ((uint8_t)0);
        _index    = new TWord8 ((uint8_t)1);
}

GSMUseItem::GSMUseItem (const GSMUseItem& clone)
{
        _type   = clone._type;

        _id       = new TWord8 (*clone._id);
        _pos      = new TPos (*clone._pos);
        _itemid   = new TWord16 (*clone._itemid);
        _stackpos = new TWord8 (*clone._stackpos);
        _index    = new TWord8 (*clone._index);
}

GSMUseItem::~GSMUseItem ()
{
        delete _id;
        delete _pos;
        delete _itemid;
        delete _stackpos;
        delete _index;
}

GSMUseItem::UseType GSMUseItem::getType ()
{
        return _type;
}

uint8_t GSMUseItem::getId ()
{
        return _id->getVal ();
}


const TPos& GSMUseItem::getPos ()
{
        return *_pos;
}

uint8_t GSMUseItem::getStackPos ()
{
        return _stackpos->getVal ();
}

uint8_t  GSMUseItem::getContainerId ()
{
        return _pos->y () & 0x3F;
}

uint8_t  GSMUseItem::getContainerPos ()
{
        return _pos->z ();
}

uint8_t GSMUseItem::getSlot ()
{
        return _pos->y ();
}

uint16_t GSMUseItem::getItemId ()
{
        return _itemid->getVal ();
}

void GSMUseItem::show ()
{
        printf ("GSMUseItem {\n");
        printf ("pos: "); _pos->show (); printf ("\n");
        printf ("itemid: "); _itemid->show (); printf ("\n");
        printf ("stackpos: "); _stackpos->show (); printf ("\n");
        printf ("index: "); _index->show (); printf ("\n");
        printf ("}\n");
}

void GSMUseItem::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _itemid->put (msg);
        _stackpos->put (msg);
        _index->put (msg);
}

void GSMUseItem::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id       = new TWord8 (msg);
        _pos      = new TPos (msg);
        _itemid   = new TWord16 (msg);
        _stackpos = new TWord8 (msg);
        _index    = new TWord8 (msg);

        if (_pos->x () == 0xFFFF) {
                if (_pos->y () & 0x40) {
                        /* i think i know how the protocol works
                         * this is just a test to be sure */
                        if (_index->getVal () != _pos->y () - 0x40) {
                                printf ("useitem: index %d != container id %d\n",
                                        _index->getVal (), _pos->y ());
                        }
                        _type = container;
                } else {
                        _type = inventory;
                }
        } else {
                _type = ground;
        }
}

/********************************************************************
 * UseItemWith
 ********************************************************************/

GSMUseItemWith::GSMUseItemWith (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

/* Ground to Ground constructor */
GSMUseItemWith::GSMUseItemWith (const TPos& from, uint16_t fromItemid, 
                                uint8_t fromStackpos, const TPos& to, 
                                uint16_t toItemid, uint8_t toStackpos)
{
        _fromType       = ground;
        _toType         = ground;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (from);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 (fromStackpos);
        _to             = new TPos (to);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 (toStackpos);
}

/* Ground to Container constructor */
GSMUseItemWith::GSMUseItemWith (const TPos& from, uint16_t fromItemid, 
                                uint8_t fromStackpos, uint8_t toContainerId, 
                                uint8_t toContainerPos, uint16_t toItemid)
{
        _fromType       = ground;
        _toType         = container;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (from);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 (fromStackpos);
        _to             = new TPos (0xFFFF, 0x0040 | toContainerId, 
                                        toContainerPos);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 (toContainerPos);
}

/* Ground to Inventory constructor */
GSMUseItemWith::GSMUseItemWith (const TPos& from, uint16_t fromItemid, 
                                uint8_t fromStackpos, uint8_t toSlot, 
                                uint16_t toItemid)
{
        _fromType       = ground;
        _toType         = inventory;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (from);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 (fromStackpos);
        _to             = new TPos (0xFFFF, toSlot, 0);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 ((uint8_t)0);
}

/* Container to Ground constructor */  
GSMUseItemWith::GSMUseItemWith (uint8_t fromContainerId, uint8_t fromContainerPos,
                                uint16_t fromItemid, const TPos& to, 
                                uint16_t toItemid, uint8_t toStackpos)
{
        _fromType       = container;
        _toType         = ground;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (0xFFFF, 0x0040 | fromContainerId, 
                                                        fromContainerPos);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 (fromContainerPos);
        _to             = new TPos (to);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 (toStackpos);
}


/* Container to Container constructor */
GSMUseItemWith::GSMUseItemWith (uint8_t fromContainerId, uint8_t fromContainerPos,
                                uint16_t fromItemid, uint8_t toContainerId,
                                uint8_t toContainerPos, uint16_t toItemid)
{
        _fromType       = container;
        _toType         = container;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (0xFFFF, 0x0040 | fromContainerId, 
                                                        fromContainerPos);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 (fromContainerPos);
        _to             = new TPos (0xFFFF, 0x0040 | toContainerId, 
                                                        toContainerPos);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 (toContainerPos);
}

/* Container to Inventory constructor */
GSMUseItemWith::GSMUseItemWith (uint8_t fromContainerId, uint8_t fromContainerPos,
                                uint16_t fromItemid, uint8_t toSlot, 
                                uint16_t toItemid)
{
        _fromType       = container;
        _toType         = inventory;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (0xFFFF, 0x0040 | fromContainerId, 
                                                        fromContainerPos);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 (fromContainerPos);
        _to             = new TPos (0xFFFF, toSlot, 0);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 ((uint8_t)0);
}

/* Inventory to Ground constructor */
GSMUseItemWith::GSMUseItemWith (uint8_t fromSlot, uint16_t fromItemid, 
                                const TPos& to, uint16_t toItemid, 
                                uint8_t toStackpos)
{
        _fromType       = inventory;
        _toType         = ground;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (0xFFFF, fromSlot, 0);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 ((uint8_t)0);
        _to             = new TPos (to);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 (toStackpos);
}

/* Inventory to Container constructor */
GSMUseItemWith::GSMUseItemWith (uint8_t fromSlot, uint16_t fromItemid, 
                                uint8_t toContainerId, uint8_t toContainerPos, 
                                uint8_t toItemid)
{
        _fromType       = inventory;
        _toType         = container;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (0xFFFF, fromSlot, 0);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 ((uint8_t)0);
        _to             = new TPos (0xFFFF, 0x0040 | toContainerId, 
                                                        toContainerPos);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 (toContainerPos);
}

/* Inventory to Inventory constructor */
GSMUseItemWith::GSMUseItemWith (uint8_t fromSlot, uint16_t fromItemid,
                                uint8_t toSlot, uint16_t toItemid)
{
        _fromType       = inventory;
        _toType         = inventory;

        _id             = new TWord8 ((uint8_t)GSM_USE_ITEM_WITH_ID);
        _from           = new TPos (0xFFFF, fromSlot, 0);
        _fromitemid     = new TWord16 (fromItemid);
        _fromstackpos   = new TWord8 ((uint8_t)0);
        _to             = new TPos (0xFFFF, toSlot, 0);
        _toitemid       = new TWord16 (toItemid);
        _tostackpos     = new TWord8 ((uint8_t)0);
}


GSMUseItemWith::GSMUseItemWith (const GSMUseItemWith& clone)
{
        _fromType = clone._fromType;
        _toType   = clone._toType;

        _id             = new TWord8 (*clone._id);
        _from           = new TPos (*clone._from);
        _fromitemid     = new TWord16 (*clone._fromitemid);
        _fromstackpos   = new TWord8 (*clone._fromstackpos);
        _to             = new TPos (*clone._to);
        _toitemid       = new TWord16 (*clone._toitemid);
        _tostackpos     = new TWord8 (*clone._tostackpos);
}

GSMUseItemWith::~GSMUseItemWith ()
{
        delete _id;
        delete _from;
        delete _fromitemid;
        delete _fromstackpos;
        delete _to;
        delete _toitemid;
        delete _tostackpos;
}

GSMUseItemWith::UseType GSMUseItemWith::getFromType ()
{
        return _fromType;
}

GSMUseItemWith::UseType GSMUseItemWith::getToType ()
{
        return _toType;
}

uint8_t GSMUseItemWith::getId ()
{
        return _id->getVal ();
}

const TPos& GSMUseItemWith::getFromPos ()
{
        return *_from;
}

uint16_t GSMUseItemWith::getFromItemId ()
{
        return _fromitemid->getVal ();
}

uint8_t GSMUseItemWith::getFromStackPos ()
{
        return _fromstackpos->getVal ();
}

uint8_t  GSMUseItemWith::getFromContainerId ()
{
        return _from->y () & 0x3F;
}

uint8_t  GSMUseItemWith::getFromContainerPos ()
{
        return _from->z ();
}

uint8_t GSMUseItemWith::getFromSlot ()
{
        return _from->y ();
}

const TPos& GSMUseItemWith::getToPos ()
{
        return *_to;
}

uint16_t GSMUseItemWith::getToItemId ()
{
        return _toitemid->getVal ();
}

uint8_t GSMUseItemWith::getToStackPos ()
{
        return _tostackpos->getVal ();
}

uint8_t  GSMUseItemWith::getToContainerId ()
{
        return _to->y () & 0x3F;
}

uint8_t  GSMUseItemWith::getToContainerPos ()
{
        return _to->z ();
}

uint8_t GSMUseItemWith::getToSlot ()
{
        return _to->y ();
}

void GSMUseItemWith::show ()
{
        printf ("GSMUseItemWith {\n");
        printf ("from: "); _from->show (); printf ("\n");
        printf ("fromitemid: "); _fromitemid->show (); printf ("\n");
        printf ("fromstackpos: "); _fromstackpos->show (); printf ("\n");
        printf ("to: "); _to->show (); printf ("\n");
        printf ("toitemid: "); _toitemid->show (); printf ("\n");
        printf ("tostackpos: "); _tostackpos->show (); printf ("\n");
        printf ("}\n");
}

void GSMUseItemWith::put (NetworkMessage* msg)
{
        _id->put (msg);
        _from->put (msg);
        _fromitemid->put (msg);
        _fromstackpos->put (msg);
        _to->put (msg);
        _toitemid->put (msg);
        _tostackpos->put (msg);
}

void GSMUseItemWith::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id             = new TWord8 (msg);
        _from           = new TPos (msg);
        _fromitemid     = new TWord16 (msg);
        _fromstackpos   = new TWord8 (msg);
        _to             = new TPos (msg);
        _toitemid       = new TWord16 (msg);
        _tostackpos     = new TWord8 (msg);

        if (_from->x () == 0xFFFF) {
                if (_from->y () & 0x40) {
                        _fromType = container;
                } else {
                        _fromType = inventory;
                }
        } else {
                _fromType = ground;
        }

        if (_to->x () == 0xFFFF) {
                if (_to->y () & 0x40) {
                        _toType = container;
                } else {
                        _toType = inventory;
                }
        } else {
                _toType = ground;
        }
}

/***************************************************************
 * UseBattleWindow
 ***************************************************************/

GSMUseBattleWindow::GSMUseBattleWindow (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

/* Ground constructor */
GSMUseBattleWindow::GSMUseBattleWindow (const TPos& pos, uint16_t itemid, 
                                        uint8_t stackpos, uint32_t creatureid)
{
        _type     = ground;

        _id = new TWord8 ((uint8_t)GSM_USE_BATTLE_WINDOW_ID);
        _pos = new TPos (pos);
        _itemid = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
        _creatureid = new TWord32 (creatureid);
}

/* Container constructor */
GSMUseBattleWindow::GSMUseBattleWindow (uint8_t containerid, 
                                        uint8_t containerpos,
                                        uint16_t itemid,
                                        uint32_t creatureid)
{
        _type           = container;

        _id             = new TWord8 ((uint8_t)GSM_USE_BATTLE_WINDOW_ID);
        _pos            = new TPos (0xFFFF, 0x40 | containerid, containerpos);
        _itemid         = new TWord16 (itemid);
        _stackpos       = new TWord8 (containerpos);
        _creatureid     = new TWord32 (creatureid);
}

/* Inventory constructor */
GSMUseBattleWindow::GSMUseBattleWindow (uint8_t slot, uint16_t itemid,
                                        uint32_t creatureid)
{
        _type           = inventory;

        _id             = new TWord8 ((uint8_t)GSM_USE_BATTLE_WINDOW_ID);
        _pos            = new TPos (0xFFFF, slot, 0);
        _itemid         = new TWord16 (itemid);
        _stackpos       = new TWord8 ((uint8_t)0);
        _creatureid     = new TWord32 (creatureid);
}

GSMUseBattleWindow::GSMUseBattleWindow (const GSMUseBattleWindow& clone)
{
        _type = clone._type;

        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
        _itemid = new TWord16 (*clone._itemid);
        _stackpos = new TWord8 (*clone._stackpos);
        _creatureid = new TWord32 (*clone._creatureid);
}
        
GSMUseBattleWindow::~GSMUseBattleWindow ()
{
        delete _id;
        delete _pos;
        delete _itemid;
        delete _stackpos;
        delete _creatureid;
}

void GSMUseBattleWindow::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _itemid->put (msg);
        _stackpos->put (msg);
        _creatureid->put (msg);
}

void GSMUseBattleWindow::show ()
{
        printf ("GSMUseBattleWindow {\n"); 
        printf ("pos: "); _pos->show (); printf ("\n");
        printf ("itemid: "); _itemid->show (); printf ("\n");
        printf ("stackpos: "); _stackpos->show (); printf ("\n");
        printf ("creatureid: "); _creatureid->show (); printf ("\n");
        printf ("}\n");
}

GSMUseBattleWindow::UseType GSMUseBattleWindow::getType ()
{
        return _type;
}

uint8_t GSMUseBattleWindow::getId ()
{
        return _id->getVal ();
}

const TPos& GSMUseBattleWindow::getPos ()
{
        return *_pos;
}

uint8_t GSMUseBattleWindow::getStackPos ()
{
        return _stackpos->getVal ();
}

uint8_t  GSMUseBattleWindow::getContainerId ()
{
        return _pos->y () & 0x3F;
}

uint8_t  GSMUseBattleWindow::getContainerPos ()
{
        return _pos->z ();
}

uint8_t GSMUseBattleWindow::getSlot ()
{
        return _pos->y ();
}

uint16_t GSMUseBattleWindow::getItemId ()
{
        return _itemid->getVal ();
}

uint32_t GSMUseBattleWindow::getCreatureId()
{
        return _creatureid->getVal ();
}

void GSMUseBattleWindow::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
        _itemid = new TWord16 (msg);
        _stackpos = new TWord8 (msg);
        _creatureid = new TWord32 (msg);

        if (_pos->x () == 0xFFFF) {
                if (_pos->y () & 0x40) {
                        _type = container;
                } else {
                        _type = inventory;
                }
        } else {
                _type = ground;
        }
}

/********************************************************************
 * RotateItem
 ********************************************************************/

GSMRotateItem::GSMRotateItem (NetworkMessage* msg, GameState* gs, DatReader* dat)
{

        get (msg, gs, dat);
}

/* Ground constructor */
GSMRotateItem::GSMRotateItem (const TPos& pos, uint16_t itemid, uint8_t stackpos)
{
        _type     = ground;

        _id       = new TWord8 ((uint8_t)GSM_ROTATE_ITEM_ID);
        _pos      = new TPos (pos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
}

/* Container constructor */
GSMRotateItem::GSMRotateItem (uint8_t containerid, uint8_t containerpos,
                            uint16_t itemid)
{
        _type     = container;

        _id       = new TWord8 ((uint8_t)GSM_ROTATE_ITEM_ID);
        _pos      = new TPos (0xFFFF, 0x40 | containerid, containerpos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (containerpos);
}

/* Inventory constructor */
GSMRotateItem::GSMRotateItem (uint8_t slot, uint16_t itemid)
{
        _type     = inventory;

        _id       = new TWord8 ((uint8_t)GSM_ROTATE_ITEM_ID);
        _pos      = new TPos (0xFFFF, slot, 0);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 ((uint8_t)0);
}

GSMRotateItem::GSMRotateItem (const GSMRotateItem& clone)
{
        _type   = clone._type;

        _id       = new TWord8 (*clone._id);
        _pos      = new TPos (*clone._pos);
        _itemid   = new TWord16 (*clone._itemid);
        _stackpos = new TWord8 (*clone._stackpos);
}

GSMRotateItem::~GSMRotateItem ()
{
        delete _id;
        delete _pos;
        delete _itemid;
        delete _stackpos;
}

GSMRotateItem::UseType GSMRotateItem::getType ()
{
        return _type;
}

uint8_t GSMRotateItem::getId ()
{
        return _id->getVal ();
}


const TPos& GSMRotateItem::getPos ()
{
        return *_pos;
}

uint8_t GSMRotateItem::getStackPos ()
{
        return _stackpos->getVal ();
}

uint8_t  GSMRotateItem::getContainerId ()
{
        return _pos->y () & 0x3F;
}

uint8_t  GSMRotateItem::getContainerPos ()
{
        return _pos->z ();
}

uint8_t GSMRotateItem::getSlot ()
{
        return _pos->y ();
}

uint16_t GSMRotateItem::getItemId ()
{
        return _itemid->getVal ();
}

void GSMRotateItem::show ()
{
        printf ("GSMRotateItem {\n");
        printf ("pos: "); _pos->show (); printf ("\n");
        printf ("itemid: "); _itemid->show (); printf ("\n");
        printf ("stackpos: "); _stackpos->show (); printf ("\n");
        printf ("}\n");
}

void GSMRotateItem::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _itemid->put (msg);
        _stackpos->put (msg);
}

void GSMRotateItem::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id       = new TWord8 (msg);
        _pos      = new TPos (msg);
        _itemid   = new TWord16 (msg);
        _stackpos = new TWord8 (msg);

        if (_pos->x () == 0xFFFF) {
                if (_pos->y () & 0x40) {
                        /* i think i know how the protocol works
                         * this is just a test to be sure */
                        _type = container;
                } else {
                        _type = inventory;
                }
        } else {
                _type = ground;
        }
}

/***************************************************************
 * ContainerClose
 ***************************************************************/

GSMContainerClose::GSMContainerClose (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMContainerClose::GSMContainerClose (uint8_t containerid)
{
        _id = new TWord8 ((uint8_t)GSM_CONTAINER_CLOSE_ID);
        _containerid = new TWord8 (containerid);
}

GSMContainerClose::GSMContainerClose (const GSMContainerClose& clone)
{
        _id = new TWord8 (*clone._id);
        _containerid = new TWord8 (*clone._containerid);
}
        
GSMContainerClose::~GSMContainerClose ()
{
        delete _id;
        delete _containerid;
}

void GSMContainerClose::put (NetworkMessage* msg)
{
        _id->put (msg);
        _containerid->put (msg);
}

void GSMContainerClose::show ()
{
        printf ("GSMContainerClose {\n"); 
        printf ("containerid: "); _containerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMContainerClose::getId ()
{
        return _id->getVal ();
}

uint8_t GSMContainerClose::getContainerId ()
{
        return _containerid->getVal ();
}

void GSMContainerClose::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _containerid = new TWord8 (msg);
}

/***************************************************************
 * ContainerUp
 ***************************************************************/

GSMContainerUp::GSMContainerUp (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMContainerUp::GSMContainerUp (uint8_t containerid)
{
        _id = new TWord8 ((uint8_t)GSM_CONTAINER_UP_ID);
        _containerid = new TWord8 (containerid);
}

GSMContainerUp::GSMContainerUp (const GSMContainerUp& clone)
{
        _id = new TWord8 (*clone._id);
        _containerid = new TWord8 (*clone._containerid);
}
        
GSMContainerUp::~GSMContainerUp ()
{
        delete _id;
        delete _containerid;
}

void GSMContainerUp::put (NetworkMessage* msg)
{
        _id->put (msg);
        _containerid->put (msg);
}

void GSMContainerUp::show ()
{
        printf ("GSMContainerUp {\n"); 
        printf ("containerid: "); _containerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMContainerUp::getId ()
{
        return _id->getVal ();
}

uint8_t GSMContainerUp::getContainerId ()
{
        return _containerid->getVal ();
}

void GSMContainerUp::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _containerid = new TWord8 (msg);
}

/***************************************************************
 * TextWindow
 ***************************************************************/

GSMTextWindow::GSMTextWindow (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTextWindow::GSMTextWindow (uint32_t windowid, const std::string& text)
{
        _id = new TWord8 ((uint8_t)GSM_TEXT_WINDOW_ID);
        _windowid = new TWord32 (windowid);
        _text = new TString (text);
}

GSMTextWindow::GSMTextWindow (const GSMTextWindow& clone)
{
        _id = new TWord8 (*clone._id);
        _windowid = new TWord32 (*clone._windowid);
        _text = new TString (*clone._text);
}
        
GSMTextWindow::~GSMTextWindow ()
{
        delete _id;
        delete _windowid;
        delete _text;
}

void GSMTextWindow::put (NetworkMessage* msg)
{
        _id->put (msg);
        _windowid->put (msg);
        _text->put (msg);
}

void GSMTextWindow::show ()
{
        printf ("GSMTextWindow {\n"); 
        printf ("windowid: "); _windowid->show (); printf ("\n");
        printf ("text: "); _text->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMTextWindow::getId ()
{
        return _id->getVal ();
}

uint32_t GSMTextWindow::getWindowId ()
{
        return _windowid->getVal ();
}

const std::string& GSMTextWindow::getText ()
{
        return _text->getString ();
}

void GSMTextWindow::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _windowid = new TWord32 (msg);
        _text = new TString (msg);
}

/***************************************************************
 * TextHouse
 ***************************************************************/

GSMTextHouse::GSMTextHouse (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMTextHouse::GSMTextHouse (uint32_t windowid, uint8_t u1,
                                const std::string& text)
{
        _id = new TWord8 ((uint8_t)GSM_TEXT_WINDOW_ID);
        _windowid = new TWord32 (windowid);
        _u1 = new TWord8 (u1);
        _text = new TString (text);
}

GSMTextHouse::GSMTextHouse (const GSMTextHouse& clone)
{
        _id = new TWord8 (*clone._id);
        _windowid = new TWord32 (*clone._windowid);
        _u1 = new TWord8 (*clone._u1);
        _text = new TString (*clone._text);
}
        
GSMTextHouse::~GSMTextHouse ()
{
        delete _id;
        delete _windowid;
        delete _u1;
        delete _text;
}

void GSMTextHouse::put (NetworkMessage* msg)
{
        _id->put (msg);
        _windowid->put (msg);
        _u1->put (msg);
        _text->put (msg);
}

void GSMTextHouse::show ()
{
        printf ("GSMTextHouse {\n"); 
        printf ("windowid: "); _windowid->show (); printf ("\n");
        printf ("u1: "); _u1->show (); printf ("\n");
        printf ("text: "); _text->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMTextHouse::getId ()
{
        return _id->getVal ();
}

uint32_t GSMTextHouse::getHouseId ()
{
        return _windowid->getVal ();
}

uint8_t GSMTextHouse::getU1 ()
{
        return _u1->getVal ();
}

const std::string& GSMTextHouse::getText ()
{
        return _text->getString ();
}

void GSMTextHouse::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _windowid = new TWord32 (msg);
        _text = new TString (msg);
}

/********************************************************************
 * LookItem
 ********************************************************************/

GSMLookItem::GSMLookItem (NetworkMessage* msg, GameState* gs, DatReader* dat)
{

        get (msg, gs, dat);
}

/* Ground constructor */
GSMLookItem::GSMLookItem (const TPos& pos, uint16_t itemid, uint8_t stackpos)
{
        _type     = ground;

        _id       = new TWord8 ((uint8_t)GSM_LOOK_ITEM_ID);
        _pos      = new TPos (pos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (stackpos);
}

/* Container constructor */
GSMLookItem::GSMLookItem (uint8_t containerid, uint8_t containerpos,
                            uint16_t itemid)
{
        _type     = container;

        _id       = new TWord8 ((uint8_t)GSM_LOOK_ITEM_ID);
        _pos      = new TPos (0xFFFF, 0x40 | containerid, containerpos);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 (containerpos);
}

/* Inventory constructor */
GSMLookItem::GSMLookItem (uint8_t slot, uint16_t itemid)
{
        _type     = inventory;

        _id       = new TWord8 ((uint8_t)GSM_LOOK_ITEM_ID);
        _pos      = new TPos (0xFFFF, slot, 0);
        _itemid   = new TWord16 (itemid);
        _stackpos = new TWord8 ((uint8_t)0);
}

GSMLookItem::GSMLookItem (const GSMLookItem& clone)
{
        _type   = clone._type;

        _id       = new TWord8 (*clone._id);
        _pos      = new TPos (*clone._pos);
        _itemid   = new TWord16 (*clone._itemid);
        _stackpos = new TWord8 (*clone._stackpos);
}

GSMLookItem::~GSMLookItem ()
{
        delete _id;
        delete _pos;
        delete _itemid;
        delete _stackpos;
}

GSMLookItem::UseType GSMLookItem::getType ()
{
        return _type;
}

uint8_t GSMLookItem::getId ()
{
        return _id->getVal ();
}


const TPos& GSMLookItem::getPos ()
{
        return *_pos;
}

uint8_t GSMLookItem::getStackPos ()
{
        return _stackpos->getVal ();
}

uint8_t  GSMLookItem::getContainerId ()
{
        return _pos->y () & 0x3F;
}

uint8_t  GSMLookItem::getContainerPos ()
{
        return _pos->z ();
}

uint8_t GSMLookItem::getSlot ()
{
        return _pos->y ();
}

uint16_t GSMLookItem::getItemId ()
{
        return _itemid->getVal ();
}

void GSMLookItem::show ()
{
        printf ("GSMLookItem {\n");
        printf ("pos: "); _pos->show (); printf ("\n");
        printf ("itemid: "); _itemid->show (); printf ("\n");
        printf ("stackpos: "); _stackpos->show (); printf ("\n");
        printf ("}\n");
}

void GSMLookItem::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
        _itemid->put (msg);
        _stackpos->put (msg);
}

void GSMLookItem::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id       = new TWord8 (msg);
        _pos      = new TPos (msg);
        _itemid   = new TWord16 (msg);
        _stackpos = new TWord8 (msg);

        if (_pos->x () == 0xFFFF) {
                if (_pos->y () & 0x40) {
                        /* i think i know how the protocol works
                         * this is just a test to be sure */
                        _type = container;
                } else {
                        _type = inventory;
                }
        } else {
                _type = ground;
        }
}

/***************************************************************
 * Speak
 ***************************************************************/

GSMSpeak::GSMSpeak (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

/* public speak constructor */
GSMSpeak::GSMSpeak (uint8_t type, const std::string& msg)
{
        _speaktype = GSMSpeak::pub;

        _id             = new TWord8 ((uint8_t)GSM_SPEAK_ID);
        _type           = new TWord8 (type);
        _msg            = new TString (msg);

        _channelid      = NULL;
        _name           = NULL;
}

/* channel speak constructor */
GSMSpeak::GSMSpeak (uint8_t type, uint16_t channelid, const std::string& msg)
{
        _speaktype = GSMSpeak::channel;

        _id             = new TWord8 ((uint8_t)GSM_SPEAK_ID);
        _type           = new TWord8 (type);
        _channelid      = new TWord16 (channel);
        _msg            = new TString (msg);

        _name           = NULL;
}

/* channel private constructor */
GSMSpeak::GSMSpeak (uint8_t type, const std::string& name,  
                        const std::string& msg)
{
        _speaktype = GSMSpeak::priv;

        _id             = new TWord8 ((uint8_t)GSM_SPEAK_ID);
        _type           = new TWord8 (type);
        _name           = new TString (name);
        _msg            = new TString (msg);

        _channelid      = NULL;
}

GSMSpeak::GSMSpeak (const GSMSpeak& clone)
{
        _speaktype = clone._speaktype;;

        _id             = new TWord8 (*clone._id);
        _type           = new TWord8 (*clone._type);
        _msg            = new TString (*clone._msg);

        if (_speaktype == GSMSpeak::pub) {
                _name           = NULL;
                _channelid      = NULL;
        } else if (_speaktype == GSMSpeak::channel) {
                _name           = NULL;
                _channelid      = new TWord16 (*clone._channelid);
        } else if (_speaktype == GSMSpeak::priv) {
                _name           = new TString (*clone._name);
                _channelid      = NULL;
        }
}
        
GSMSpeak::~GSMSpeak ()
{
        delete _id;
        delete _type;
        delete _msg;
        delete _channelid;
        delete _name;
}

void GSMSpeak::put (NetworkMessage* msg)
{
        _id->put (msg);
        _type->put (msg);
        if (_speaktype == GSMSpeak::channel) {
                _channelid->put (msg);
        } else if (_speaktype == GSMSpeak::priv) {
                _name->put (msg);
        }
        _msg->put (msg);
}

void GSMSpeak::show ()
{
        printf ("GSMSpeak {\n"); 
        if (_speaktype == GSMSpeak::pub) {
                printf ("\tpublic speak\n");
        } else if (_speaktype == GSMSpeak::channel) {
                printf ("\tchannel speak\n");
        } else if (_speaktype == GSMSpeak::priv) {
                printf ("\tprivate speak\n");
        }
        printf ("\ttype: "); _type->show (); printf ("\n");
        if (_speaktype == GSMSpeak::channel) {
                printf ("\tchannelid: "); _channelid->show (); printf ("\n");
        } else if (_speaktype == GSMSpeak::priv) {
                printf ("\tname "); _name->show (); printf ("\n");
        }
        printf ("msg: "); _msg->show (); printf ("\n");
        printf ("}\n");
}

GSMSpeak::SpeakType GSMSpeak::getSpeakType ()
{
        return _speaktype;
}

uint8_t GSMSpeak::getId ()
{
        return _id->getVal ();
}

uint8_t GSMSpeak::getType ()
{
        return _type->getVal ();
}

uint16_t GSMSpeak::getChannelId ()
{
        if (_speaktype != GSMSpeak::channel) {
                printf ("GSMSpeak error: request for channelid for ");
                printf ("non-channel speak\n");
        }
        return _channelid->getVal ();
}

const std::string& GSMSpeak::getMsg ()
{
        return _msg->getString ();
}

const std::string& GSMSpeak::getName ()
{
        if (_speaktype != GSMSpeak::priv) {
                printf ("GSMSpeak error: request for name for ");
                printf ("non-private speak\n");
        }
        return _name->getString ();
}

void GSMSpeak::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _type = new TWord8 (msg);

        uint32_t t = _type->getVal ();

        if (t == SPEAK_SAY
         || t == SPEAK_WHISPER
         || t == SPEAK_YELL
         || t == SPEAK_PRIVATE_PN)
        {
                _speaktype = GSMSpeak::pub;
                _channelid = NULL;
                _name = NULL;
        } else if (t == SPEAK_CHANNEL_Y
                || t == SPEAK_CHANNEL_R1
                || t == SPEAK_CHANNEL_R2
                || t == SPEAK_CHANNEL_O)
        {
                _speaktype = GSMSpeak::channel;
                _channelid = new TWord16 (msg); 
                _name = NULL;
        } else if (t == SPEAK_PRIVATE
                || t == SPEAK_PRIVATE_RED)
        {
                _speaktype = GSMSpeak::priv;
                _channelid = NULL;
                _name = new TString (msg);
        } else {
                printf ("GSMSpeak error: unknown speak_id %d\n", t);
        }
        _msg = new TString (msg);
}

/***************************************************************
 * RequestChannels
 ***************************************************************/

GSMRequestChannels::GSMRequestChannels (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMRequestChannels::GSMRequestChannels ()
{
        _id = new TWord8 ((uint8_t)GSM_REQUEST_CHANNELS_ID);
}

GSMRequestChannels::GSMRequestChannels (const GSMRequestChannels& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMRequestChannels::~GSMRequestChannels ()
{
        delete _id;
}

void GSMRequestChannels::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMRequestChannels::show ()
{
        printf ("GSMRequestChannels {}\n");
}

uint8_t GSMRequestChannels::getId ()
{
        return _id->getVal ();
}

void GSMRequestChannels::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * ChannelOpen
 ***************************************************************/

GSMChannelOpen::GSMChannelOpen (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMChannelOpen::GSMChannelOpen (uint16_t channelid)
{
        _id = new TWord8 ((uint8_t)GSM_CHANNEL_OPEN_ID);
        _channelid = new TWord16 (channelid);
}

GSMChannelOpen::GSMChannelOpen (const GSMChannelOpen& clone)
{
        _id = new TWord8 (*clone._id);
        _channelid = new TWord16 (*clone._channelid);
}
        
GSMChannelOpen::~GSMChannelOpen ()
{
        delete _id;
        delete _channelid;
}

void GSMChannelOpen::put (NetworkMessage* msg)
{
        _id->put (msg);
        _channelid->put (msg);
}

void GSMChannelOpen::show ()
{
        printf ("GSMChannelOpen {\n"); 
        printf ("channelid: "); _channelid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMChannelOpen::getId ()
{
        return _id->getVal ();
}

uint16_t GSMChannelOpen::getChannelId ()
{
        return _channelid->getVal ();
}

void GSMChannelOpen::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _channelid = new TWord16 (msg);
}

/***************************************************************
 * ChannelClose
 ***************************************************************/

GSMChannelClose::GSMChannelClose (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMChannelClose::GSMChannelClose (uint16_t channelid)
{
        _id = new TWord8 ((uint8_t)GSM_CHANNEL_CLOSE_ID);
        _channelid = new TWord16 (channelid);
}

GSMChannelClose::GSMChannelClose (const GSMChannelClose& clone)
{
        _id = new TWord8 (*clone._id);
        _channelid = new TWord16 (*clone._channelid);
}
        
GSMChannelClose::~GSMChannelClose ()
{
        delete _id;
        delete _channelid;
}

void GSMChannelClose::put (NetworkMessage* msg)
{
        _id->put (msg);
        _channelid->put (msg);
}

void GSMChannelClose::show ()
{
        printf ("GSMChannelClose {\n"); 
        printf ("channelid: "); _channelid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMChannelClose::getId ()
{
        return _id->getVal ();
}

uint16_t GSMChannelClose::getChannelId ()
{
        return _channelid->getVal ();
}

void GSMChannelClose::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _channelid = new TWord16 (msg);
}

/***************************************************************
 * PrivateChatOpen
 ***************************************************************/

GSMPrivateChatOpen::GSMPrivateChatOpen (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPrivateChatOpen::GSMPrivateChatOpen (const std::string& name)
{
        _id = new TWord8 ((uint8_t)GSM_PRIVATE_CHAT_OPEN_ID);
        _name = new TString (name);
}

GSMPrivateChatOpen::GSMPrivateChatOpen (const GSMPrivateChatOpen& clone)
{
        _id = new TWord8 (*clone._id);
        _name = new TString (*clone._name);
}
        
GSMPrivateChatOpen::~GSMPrivateChatOpen ()
{
        delete _id;
        delete _name;
}

void GSMPrivateChatOpen::put (NetworkMessage* msg)
{
        _id->put (msg);
        _name->put (msg);
}

void GSMPrivateChatOpen::show ()
{
        printf ("GSMPrivateChatOpen {\n"); 
        printf ("name: "); _name->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMPrivateChatOpen::getId ()
{
        return _id->getVal ();
}

const std::string& GSMPrivateChatOpen::getName ()
{
        return _name->getString ();
}

void GSMPrivateChatOpen::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _name = new TString (msg);
}

/***************************************************************
 * RuleViolationOpen
 ***************************************************************/

GSMRuleViolationOpen::GSMRuleViolationOpen (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMRuleViolationOpen::GSMRuleViolationOpen (const std::string& msg)
{
        _id = new TWord8 ((uint8_t)GSM_RULE_VIOLATION_OPEN_ID);
        _msg = new TString (msg);
}

GSMRuleViolationOpen::GSMRuleViolationOpen (const GSMRuleViolationOpen& clone)
{
        _id = new TWord8 (*clone._id);
        _msg = new TString (*clone._msg);
}
        
GSMRuleViolationOpen::~GSMRuleViolationOpen ()
{
        delete _id;
        delete _msg;
}

void GSMRuleViolationOpen::put (NetworkMessage* msg)
{
        _id->put (msg);
        _msg->put (msg);
}

void GSMRuleViolationOpen::show ()
{
        printf ("GSMRuleViolationOpen {\n"); 
        printf ("msg: "); _msg->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMRuleViolationOpen::getId ()
{
        return _id->getVal ();
}

const std::string& GSMRuleViolationOpen::getMsg ()
{
        return _msg->getString ();
}

void GSMRuleViolationOpen::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _msg = new TString (msg);
}

/***************************************************************
 * RuleViolationClose
 ***************************************************************/

GSMRuleViolationClose::GSMRuleViolationClose (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMRuleViolationClose::GSMRuleViolationClose (const std::string& msg)
{
        _id = new TWord8 ((uint8_t)GSM_RULE_VIOLATION_CLOSE_ID);
        _msg = new TString (msg);
}

GSMRuleViolationClose::GSMRuleViolationClose (const GSMRuleViolationClose& clone)
{
        _id = new TWord8 (*clone._id);
        _msg = new TString (*clone._msg);
}
        
GSMRuleViolationClose::~GSMRuleViolationClose ()
{
        delete _id;
        delete _msg;
}

void GSMRuleViolationClose::put (NetworkMessage* msg)
{
        _id->put (msg);
        _msg->put (msg);
}

void GSMRuleViolationClose::show ()
{
        printf ("GSMRuleViolationClose {\n"); 
        printf ("msg: "); _msg->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMRuleViolationClose::getId ()
{
        return _id->getVal ();
}

const std::string& GSMRuleViolationClose::getMsg ()
{
        return _msg->getString ();
}

void GSMRuleViolationClose::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _msg = new TString (msg);
}

/***************************************************************
 * RuleViolationCancel
 ***************************************************************/

GSMRuleViolationCancel::GSMRuleViolationCancel (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMRuleViolationCancel::GSMRuleViolationCancel ()
{
        _id = new TWord8 ((uint8_t)GSM_RULE_VIOLATION_CANCEL_ID);
}

GSMRuleViolationCancel::GSMRuleViolationCancel (const GSMRuleViolationCancel& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMRuleViolationCancel::~GSMRuleViolationCancel ()
{
        delete _id;
}

void GSMRuleViolationCancel::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMRuleViolationCancel::show ()
{
        printf ("GSMRuleViolationCancel {}\n");
}

uint8_t GSMRuleViolationCancel::getId ()
{
        return _id->getVal ();
}

void GSMRuleViolationCancel::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * FightModes
 ***************************************************************/

GSMFightModes::GSMFightModes (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMFightModes::GSMFightModes (uint8_t attack, uint8_t chase, uint8_t secure)
{
        _id = new TWord8 ((uint8_t)GSM_FIGHT_MODES_ID);
        _attack = new TWord8 (attack);
        _chase = new TWord8 (chase);
        _secure = new TWord8 (secure);
}

GSMFightModes::GSMFightModes (const GSMFightModes& clone)
{
        _id = new TWord8 (*clone._id);
        _attack = new TWord8 (*clone._attack);
        _chase = new TWord8 (*clone._chase);
        _secure = new TWord8 (*clone._secure);
}
        
GSMFightModes::~GSMFightModes ()
{
        delete _id;
        delete _attack;
        delete _chase;
        delete _secure;
}

void GSMFightModes::put (NetworkMessage* msg)
{
        _id->put (msg);
        _attack->put (msg);
        _chase->put (msg);
        _secure->put (msg);
}

void GSMFightModes::show ()
{
        printf ("GSMFightModes {\n"); 
        printf ("\tattack: "); _attack->show (); printf ("\n");
        printf ("\tchase: "); _chase->show (); printf ("\n");
        printf ("\tsecure: "); _secure->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMFightModes::getId ()
{
        return _id->getVal ();
}

uint8_t GSMFightModes::getAttack ()
{
        return _attack->getVal ();
}

uint8_t GSMFightModes::getChase ()
{
        return _chase->getVal ();
}

uint8_t GSMFightModes::getSecure ()
{
        return _secure->getVal ();
}

void GSMFightModes::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _attack = new TWord8 (msg);
        _chase = new TWord8 (msg);
        _secure = new TWord8 (msg);
}

/***************************************************************
 * CreatureAttack
 ***************************************************************/

GSMCreatureAttack::GSMCreatureAttack (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMCreatureAttack::GSMCreatureAttack (uint32_t creatureid)
{
        _id = new TWord8 ((uint8_t)GSM_CREATURE_ATTACK_ID);
        _creatureid = new TWord32 (creatureid);
}

GSMCreatureAttack::GSMCreatureAttack (const GSMCreatureAttack& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
}
        
GSMCreatureAttack::~GSMCreatureAttack ()
{
        delete _id;
        delete _creatureid;
}

void GSMCreatureAttack::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
}

void GSMCreatureAttack::show ()
{
        printf ("GSMCreatureAttack {\n"); 
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMCreatureAttack::getId ()
{
        return _id->getVal ();
}

uint32_t GSMCreatureAttack::getCreatureId ()
{
        return _creatureid->getVal ();
}

void GSMCreatureAttack::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
}

/***************************************************************
 * CreatureFollow
 ***************************************************************/

GSMCreatureFollow::GSMCreatureFollow (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMCreatureFollow::GSMCreatureFollow (uint32_t creatureid)
{
        _id = new TWord8 ((uint8_t)GSM_CREATURE_FOLLOW_ID);
        _creatureid = new TWord32 (creatureid);
}

GSMCreatureFollow::GSMCreatureFollow (const GSMCreatureFollow& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
}
        
GSMCreatureFollow::~GSMCreatureFollow ()
{
        delete _id;
        delete _creatureid;
}

void GSMCreatureFollow::put (NetworkMessage* msg)
{
        _id->put (msg);
        _creatureid->put (msg);
}

void GSMCreatureFollow::show ()
{
        printf ("GSMCreatureFollow {\n"); 
        printf ("\tcreatureid: "); _creatureid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMCreatureFollow::getId ()
{
        return _id->getVal ();
}

uint32_t GSMCreatureFollow::getCreatureId ()
{
        return _creatureid->getVal ();
}

void GSMCreatureFollow::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
}

/***************************************************************
 * PrivateChannelCreate
 ***************************************************************/

GSMPrivateChannelCreate::GSMPrivateChannelCreate (NetworkMessage* msg, 
                                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPrivateChannelCreate::GSMPrivateChannelCreate ()
{
        _id = new TWord8 ((uint8_t)GSM_PRIVATE_CHANNEL_CREATE_ID);
}

GSMPrivateChannelCreate::GSMPrivateChannelCreate (
                        const GSMPrivateChannelCreate& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMPrivateChannelCreate::~GSMPrivateChannelCreate ()
{
        delete _id;
}

void GSMPrivateChannelCreate::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMPrivateChannelCreate::show ()
{
        printf ("GSMPrivateChannelCreate {}\n");
}

uint8_t GSMPrivateChannelCreate::getId ()
{
        return _id->getVal ();
}

void GSMPrivateChannelCreate::get 
                (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * PrivateChannelInvite
 ***************************************************************/

GSMPrivateChannelInvite::GSMPrivateChannelInvite (NetworkMessage* msg, 
                                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPrivateChannelInvite::GSMPrivateChannelInvite (const std::string& name)
{
        _id = new TWord8 ((uint8_t)GSM_PRIVATE_CHANNEL_INVITE_ID);
        _name = new TString (name);
}

GSMPrivateChannelInvite::GSMPrivateChannelInvite (const GSMPrivateChannelInvite& clone)
{
        _id = new TWord8 (*clone._id);
        _name = new TString (*clone._name);
}
        
GSMPrivateChannelInvite::~GSMPrivateChannelInvite ()
{
        delete _id;
        delete _name;
}

void GSMPrivateChannelInvite::put (NetworkMessage* msg)
{
        _id->put (msg);
        _name->put (msg);
}

void GSMPrivateChannelInvite::show ()
{
        printf ("GSMPrivateChannelInvite {\n"); 
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMPrivateChannelInvite::getId ()
{
        return _id->getVal ();
}

const std::string& GSMPrivateChannelInvite::getName ()
{
        return _name->getString ();
}

void GSMPrivateChannelInvite::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _name = new TString (msg);
}

/***************************************************************
 * PrivateChannelBan
 ***************************************************************/

GSMPrivateChannelBan::GSMPrivateChannelBan (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPrivateChannelBan::GSMPrivateChannelBan (const std::string& name)
{
        _id = new TWord8 ((uint8_t)GSM_PRIVATE_CHANNEL_BAN_ID);
        _name = new TString (name);
}

GSMPrivateChannelBan::GSMPrivateChannelBan (const GSMPrivateChannelBan& clone)
{
        _id = new TWord8 (*clone._id);
        _name = new TString (*clone._name);
}
        
GSMPrivateChannelBan::~GSMPrivateChannelBan ()
{
        delete _id;
        delete _name;
}

void GSMPrivateChannelBan::put (NetworkMessage* msg)
{
        _id->put (msg);
        _name->put (msg);
}

void GSMPrivateChannelBan::show ()
{
        printf ("GSMPrivateChannelBan {\n"); 
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMPrivateChannelBan::getId ()
{
        return _id->getVal ();
}

const std::string& GSMPrivateChannelBan::getName ()
{
        return _name->getString ();
}

void GSMPrivateChannelBan::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _name = new TString (msg);
}

/***************************************************************
 * PartyInvite
 ***************************************************************/

GSMPartyInvite::GSMPartyInvite (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPartyInvite::GSMPartyInvite (uint32_t playerid)
{
        _id = new TWord8 ((uint8_t)GSM_PARTY_INVITE_ID);
        _playerid = new TWord32 (playerid);
}

GSMPartyInvite::GSMPartyInvite (const GSMPartyInvite& clone)
{
        _id = new TWord8 (*clone._id);
        _playerid = new TWord32 (*clone._playerid);
}
        
GSMPartyInvite::~GSMPartyInvite ()
{
        delete _id;
        delete _playerid;
}

void GSMPartyInvite::put (NetworkMessage* msg)
{
        _id->put (msg);
        _playerid->put (msg);
}

void GSMPartyInvite::show ()
{
        printf ("GSMPartyInvite {\n"); 
        printf ("\tplayerid: "); _playerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMPartyInvite::getId ()
{
        return _id->getVal ();
}

uint32_t GSMPartyInvite::getPlayerId ()
{
        return _playerid->getVal ();
}

void GSMPartyInvite::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _playerid = new TWord32 (msg);
}

/***************************************************************
 * PartyJoin
 ***************************************************************/

GSMPartyJoin::GSMPartyJoin (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPartyJoin::GSMPartyJoin (uint32_t playerid)
{
        _id = new TWord8 ((uint8_t)GSM_PARTY_JOIN_ID);
        _playerid = new TWord32 (playerid);
}

GSMPartyJoin::GSMPartyJoin (const GSMPartyJoin& clone)
{
        _id = new TWord8 (*clone._id);
        _playerid = new TWord32 (*clone._playerid);
}
        
GSMPartyJoin::~GSMPartyJoin ()
{
        delete _id;
        delete _playerid;
}

void GSMPartyJoin::put (NetworkMessage* msg)
{
        _id->put (msg);
        _playerid->put (msg);
}

void GSMPartyJoin::show ()
{
        printf ("GSMPartyJoin {\n"); 
        printf ("\tplayerid: "); _playerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMPartyJoin::getId ()
{
        return _id->getVal ();
}

uint32_t GSMPartyJoin::getPlayerId ()
{
        return _playerid->getVal ();
}

void GSMPartyJoin::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _playerid = new TWord32 (msg);
}

/***************************************************************
 * PartyCancel
 ***************************************************************/

GSMPartyCancel::GSMPartyCancel (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPartyCancel::GSMPartyCancel (uint32_t playerid)
{
        _id = new TWord8 ((uint8_t)GSM_PARTY_CANCEL_ID);
        _playerid = new TWord32 (playerid);
}

GSMPartyCancel::GSMPartyCancel (const GSMPartyCancel& clone)
{
        _id = new TWord8 (*clone._id);
        _playerid = new TWord32 (*clone._playerid);
}
        
GSMPartyCancel::~GSMPartyCancel ()
{
        delete _id;
        delete _playerid;
}

void GSMPartyCancel::put (NetworkMessage* msg)
{
        _id->put (msg);
        _playerid->put (msg);
}

void GSMPartyCancel::show ()
{
        printf ("GSMPartyCancel {\n"); 
        printf ("\tplayerid: "); _playerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMPartyCancel::getId ()
{
        return _id->getVal ();
}

uint32_t GSMPartyCancel::getPlayerId ()
{
        return _playerid->getVal ();
}

void GSMPartyCancel::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _playerid = new TWord32 (msg);
}

/***************************************************************
 * PartyPassLeadership
 ***************************************************************/

GSMPartyPassLeadership::GSMPartyPassLeadership (NetworkMessage* msg, 
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPartyPassLeadership::GSMPartyPassLeadership (uint32_t playerid)
{
        _id = new TWord8 ((uint8_t)GSM_PARTY_PASS_LEADERSHIP_ID);
        _playerid = new TWord32 (playerid);
}

GSMPartyPassLeadership::GSMPartyPassLeadership (
                        const GSMPartyPassLeadership& clone)
{
        _id = new TWord8 (*clone._id);
        _playerid = new TWord32 (*clone._playerid);
}
        
GSMPartyPassLeadership::~GSMPartyPassLeadership ()
{
        delete _id;
        delete _playerid;
}

void GSMPartyPassLeadership::put (NetworkMessage* msg)
{
        _id->put (msg);
        _playerid->put (msg);
}

void GSMPartyPassLeadership::show ()
{
        printf ("GSMPartyPassLeadership {\n"); 
        printf ("\tplayerid: "); _playerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMPartyPassLeadership::getId ()
{
        return _id->getVal ();
}

uint32_t GSMPartyPassLeadership::getPlayerId ()
{
        return _playerid->getVal ();
}

void GSMPartyPassLeadership::get (NetworkMessage* msg, 
                        GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _playerid = new TWord32 (msg);
}

/***************************************************************
 * PartyLeave
 ***************************************************************/

GSMPartyLeave::GSMPartyLeave (NetworkMessage* msg, 
                                GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMPartyLeave::GSMPartyLeave ()
{
        _id = new TWord8 ((uint8_t)GSM_PARTY_LEAVE_ID);
}

GSMPartyLeave::GSMPartyLeave (const GSMPartyLeave& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMPartyLeave::~GSMPartyLeave ()
{
        delete _id;
}

void GSMPartyLeave::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMPartyLeave::show ()
{
        printf ("GSMPartyLeave {}\n");
}

uint8_t GSMPartyLeave::getId ()
{
        return _id->getVal ();
}

void GSMPartyLeave::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * CancelMove
 ***************************************************************/

GSMCancelMove::GSMCancelMove (NetworkMessage* msg, 
                                GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMCancelMove::GSMCancelMove ()
{
        _id = new TWord8 ((uint8_t)GSM_CANCEL_MOVE_ID);
}

GSMCancelMove::GSMCancelMove (const GSMCancelMove& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMCancelMove::~GSMCancelMove ()
{
        delete _id;
}

void GSMCancelMove::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMCancelMove::show ()
{
        printf ("GSMCancelMove {}\n");
}

uint8_t GSMCancelMove::getId ()
{
        return _id->getVal ();
}

void GSMCancelMove::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * OutfitRequest
 ***************************************************************/

GSMOutfitRequest::GSMOutfitRequest (NetworkMessage* msg, 
                                GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMOutfitRequest::GSMOutfitRequest ()
{
        _id = new TWord8 ((uint8_t)GSM_OUTFIT_REQUEST_ID);
}

GSMOutfitRequest::GSMOutfitRequest (const GSMOutfitRequest& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMOutfitRequest::~GSMOutfitRequest ()
{
        delete _id;
}

void GSMOutfitRequest::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMOutfitRequest::show ()
{
        printf ("GSMOutfitRequest {}\n");
}

uint8_t GSMOutfitRequest::getId ()
{
        return _id->getVal ();
}

void GSMOutfitRequest::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * OutfitSet
 ***************************************************************/

GSMOutfitSet::GSMOutfitSet (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMOutfitSet::GSMOutfitSet (uint16_t looktype, uint8_t head, uint8_t body, 
                                uint8_t legs, uint8_t feet, uint8_t addons)
{
        _id = new TWord8 ((uint8_t)GSM_OUTFIT_SET_ID);
        _looktype = new TWord16 (looktype);
        _head = new TWord8 (head);
        _body = new TWord8 (body);
        _legs = new TWord8 (legs);
        _feet = new TWord8 (feet);
        _addons = new TWord8 (addons);
}

GSMOutfitSet::GSMOutfitSet (const GSMOutfitSet& clone)
{
        _id = new TWord8 (*clone._id);
        _looktype = new TWord16 (*clone._looktype);
        _head = new TWord8 (*clone._head);
        _body = new TWord8 (*clone._body);
        _legs = new TWord8 (*clone._legs);
        _feet = new TWord8 (*clone._feet);
        _addons = new TWord8 (*clone._addons);
}
        
GSMOutfitSet::~GSMOutfitSet ()
{
        delete _id;
        delete _looktype;
        delete _head;
        delete _body;
        delete _legs;
        delete _feet;
        delete _addons;
}

void GSMOutfitSet::put (NetworkMessage* msg)
{
        _id->put (msg);
        _looktype->put (msg);
        _head->put (msg);
        _body->put (msg);
        _legs->put (msg);
        _feet->put (msg);
        _addons->put (msg);
}

void GSMOutfitSet::show ()
{
        printf ("GSMOutfitSet {\n"); 
        printf ("\tlooktype: "); _looktype->show (); printf ("\n");
        printf ("\thead: "); _head->show (); printf ("\n");
        printf ("\tbody: "); _body->show (); printf ("\n");
        printf ("\tlegs: "); _legs->show (); printf ("\n");
        printf ("\tfeet: "); _feet->show (); printf ("\n");
        printf ("\taddons: "); _addons->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMOutfitSet::getId ()
{
        return _id->getVal ();
}

uint16_t GSMOutfitSet::getLookType ()
{
        return _looktype->getVal ();
}

uint8_t GSMOutfitSet::getHead ()
{
        return _head->getVal ();
}

uint8_t GSMOutfitSet::getBody ()
{
        return _body->getVal ();
}

uint8_t GSMOutfitSet::getLegs ()
{
        return _legs->getVal ();
}

uint8_t GSMOutfitSet::getFeet ()
{
        return _feet->getVal ();
}

uint8_t GSMOutfitSet::getAddons ()
{
        return _addons->getVal ();
}

void GSMOutfitSet::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _looktype = new TWord16 (msg);
        _head = new TWord8 (msg);
        _body = new TWord8 (msg);
        _legs = new TWord8 (msg);
        _feet = new TWord8 (msg);
        _addons = new TWord8 (msg);
}

/***************************************************************
 * VIPAdd
 ***************************************************************/

GSMVIPAdd::GSMVIPAdd (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMVIPAdd::GSMVIPAdd (const std::string& name)
{
        _id = new TWord8 ((uint8_t)GSM_VIP_ADD_ID);
        _name = new TString (name);
}

GSMVIPAdd::GSMVIPAdd (const GSMVIPAdd& clone)
{
        _id = new TWord8 (*clone._id);
        _name = new TString (*clone._name);
}
        
GSMVIPAdd::~GSMVIPAdd ()
{
        delete _id;
        delete _name;
}

void GSMVIPAdd::put (NetworkMessage* msg)
{
        _id->put (msg);
        _name->put (msg);
}

void GSMVIPAdd::show ()
{
        printf ("GSMVIPAdd {\n"); 
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMVIPAdd::getId ()
{
        return _id->getVal ();
}

const std::string& GSMVIPAdd::getName ()
{
        return _name->getString ();
}

void GSMVIPAdd::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _name = new TString (msg);
}

/***************************************************************
 * VIPRemove
 ***************************************************************/

GSMVIPRemove::GSMVIPRemove (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMVIPRemove::GSMVIPRemove (uint32_t playerid)
{
        _id = new TWord8 ((uint8_t)GSM_VIP_REMOVE_ID);
        _playerid = new TWord32 (playerid);
}

GSMVIPRemove::GSMVIPRemove (const GSMVIPRemove& clone)
{
        _id = new TWord8 (*clone._id);
        _playerid = new TWord32 (*clone._playerid);
}
        
GSMVIPRemove::~GSMVIPRemove ()
{
        delete _id;
        delete _playerid;
}

void GSMVIPRemove::put (NetworkMessage* msg)
{
        _id->put (msg);
        _playerid->put (msg);
}

void GSMVIPRemove::show ()
{
        printf ("GSMVIPRemove {\n"); 
        printf ("\tplayerid: "); _playerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMVIPRemove::getId ()
{
        return _id->getVal ();
}

uint32_t GSMVIPRemove::getPlayerId ()
{
        return _playerid->getVal ();
}

void GSMVIPRemove::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _playerid = new TWord32 (msg);
}

/***************************************************************
 * BugReport
 ***************************************************************/

GSMBugReport::GSMBugReport (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMBugReport::GSMBugReport (const std::string& text)
{
        _id = new TWord8 ((uint8_t)GSM_BUG_REPORT_ID);
        _text = new TString (text);
}

GSMBugReport::GSMBugReport (const GSMBugReport& clone)
{
        _id = new TWord8 (*clone._id);
        _text = new TString (*clone._text);
}
        
GSMBugReport::~GSMBugReport ()
{
        delete _id;
        delete _text;
}

void GSMBugReport::put (NetworkMessage* msg)
{
        _id->put (msg);
        _text->put (msg);
}

void GSMBugReport::show ()
{
        printf ("GSMBugReport {\n"); 
        printf ("text: "); _text->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMBugReport::getId ()
{
        return _id->getVal ();
}

const std::string& GSMBugReport::getText ()
{
        return _text->getString ();
}

void GSMBugReport::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _text = new TString (msg);
}

/***************************************************************
 * QuestRequestLog
 ***************************************************************/

GSMQuestRequestLog::GSMQuestRequestLog (NetworkMessage* msg, 
                                GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMQuestRequestLog::GSMQuestRequestLog ()
{
        _id = new TWord8 ((uint8_t)GSM_QUEST_REQUEST_LOG_ID);
}

GSMQuestRequestLog::GSMQuestRequestLog (const GSMQuestRequestLog& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMQuestRequestLog::~GSMQuestRequestLog ()
{
        delete _id;
}

void GSMQuestRequestLog::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMQuestRequestLog::show ()
{
        printf ("GSMQuestRequestLog {}\n");
}

uint8_t GSMQuestRequestLog::getId ()
{
        return _id->getVal ();
}

void GSMQuestRequestLog::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * QuestRequest
 ***************************************************************/

GSMQuestRequest::GSMQuestRequest (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMQuestRequest::GSMQuestRequest (uint32_t questid)
{
        _id = new TWord8 ((uint8_t)GSM_QUEST_REQUEST_ID);
        _questid = new TWord32 (questid);
}

GSMQuestRequest::GSMQuestRequest (const GSMQuestRequest& clone)
{
        _id = new TWord8 (*clone._id);
        _questid = new TWord32 (*clone._questid);
}
        
GSMQuestRequest::~GSMQuestRequest ()
{
        delete _id;
        delete _questid;
}

void GSMQuestRequest::put (NetworkMessage* msg)
{
        _id->put (msg);
        _questid->put (msg);
}

void GSMQuestRequest::show ()
{
        printf ("GSMQuestRequest {\n"); 
        printf ("\tquestid: "); _questid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMQuestRequest::getId ()
{
        return _id->getVal ();
}

uint32_t GSMQuestRequest::getQuestId ()
{
        return _questid->getVal ();
}

void GSMQuestRequest::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _questid = new TWord32 (msg);
}

/***************************************************************
 * ShopBuy
 ***************************************************************/

GSMShopBuy::GSMShopBuy (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMShopBuy::GSMShopBuy (uint16_t itemid, uint16_t count, uint8_t u1, uint8_t u2)
{
        _id = new TWord8 ((uint8_t)GSM_SHOP_BUY_ID);
        _itemid = new TWord16 (itemid);
        _count = new TWord16 (count);
        _u1 = new TWord8 (u1);
        _u2 = new TWord8 (u2);
}

GSMShopBuy::GSMShopBuy (const GSMShopBuy& clone)
{
        _id = new TWord8 (*clone._id);
        _itemid = new TWord16 (*clone._itemid);
        _count = new TWord16 (*clone._count);
        _u1 = new TWord8 (*clone._u1);
        _u2 = new TWord8 (*clone._u2);
}
        
GSMShopBuy::~GSMShopBuy ()
{
        delete _id;
        delete _itemid;
        delete _count;
        delete _u1;
        delete _u2;
}

void GSMShopBuy::put (NetworkMessage* msg)
{
        _id->put (msg);
        _itemid->put (msg);
        _count->put (msg);
        _u1->put (msg);
        _u2->put (msg);
}

void GSMShopBuy::show ()
{
        printf ("GSMShopBuy {\n"); 
        printf ("\titemid: "); _itemid->show (); printf ("\n");
        printf ("\tcount: "); _count->show (); printf ("\n");
        printf ("\tu1: "); _u1->show (); printf ("\n");
        printf ("\tu2: "); _u2->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMShopBuy::getId ()
{
        return _id->getVal ();
}

uint16_t GSMShopBuy::getItemId ()
{
        return _itemid->getVal ();
}

uint16_t GSMShopBuy::getCount ()
{
        return _count->getVal ();
}

uint8_t GSMShopBuy::getU1 ()
{
        return _u1->getVal ();
}

uint8_t GSMShopBuy::getU2 ()
{
        return _u1->getVal ();
}

void GSMShopBuy::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _itemid = new TWord16 (msg);
        _count = new TWord16 (msg);
        _u1 = new TWord8 (msg);
        _u2 = new TWord8 (msg);
}

/***************************************************************
 * ShopSell
 ***************************************************************/

GSMShopSell::GSMShopSell (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMShopSell::GSMShopSell (uint16_t itemid, uint16_t count, uint8_t u1)
{
        _id = new TWord8 ((uint8_t)GSM_SHOP_SELL_ID);
        _itemid = new TWord16 (itemid);
        _count = new TWord16 (count);
        _u1 = new TWord8 (u1);
}

GSMShopSell::GSMShopSell (const GSMShopSell& clone)
{
        _id = new TWord8 (*clone._id);
        _itemid = new TWord16 (*clone._itemid);
        _count = new TWord16 (*clone._count);
        _u1 = new TWord8 (*clone._u1);
}
        
GSMShopSell::~GSMShopSell ()
{
        delete _id;
        delete _itemid;
        delete _count;
        delete _u1;
}

void GSMShopSell::put (NetworkMessage* msg)
{
        _id->put (msg);
        _itemid->put (msg);
        _count->put (msg);
        _u1->put (msg);
}

void GSMShopSell::show ()
{
        printf ("GSMShopSell {\n"); 
        printf ("\titemid: "); _itemid->show (); printf ("\n");
        printf ("\tcount: "); _count->show (); printf ("\n");
        printf ("\tu1: "); _u1->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMShopSell::getId ()
{
        return _id->getVal ();
}

uint16_t GSMShopSell::getItemId ()
{
        return _itemid->getVal ();
}

uint16_t GSMShopSell::getCount ()
{
        return _count->getVal ();
}

uint8_t GSMShopSell::getU1 ()
{
        return _u1->getVal ();
}

void GSMShopSell::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _itemid = new TWord16 (msg);
        _count = new TWord16 (msg);
        _u1 = new TWord8 (msg);
}

/***************************************************************
 * ShopClose
 ***************************************************************/

GSMShopClose::GSMShopClose (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMShopClose::GSMShopClose ()
{
        _id = new TWord8 ((uint8_t)GSM_SHOP_CLOSE_ID);
}

GSMShopClose::GSMShopClose (const GSMShopClose& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMShopClose::~GSMShopClose ()
{
        delete _id;
}

void GSMShopClose::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMShopClose::show ()
{
        printf ("GSMShopClose {}\n");
}

uint8_t GSMShopClose::getId ()
{
        return _id->getVal ();
}

void GSMShopClose::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * NPCChannelClose
 ***************************************************************/

GSMNPCChannelClose::GSMNPCChannelClose (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMNPCChannelClose::GSMNPCChannelClose ()
{
        _id = new TWord8 ((uint8_t)GSM_NPC_CHANNEL_CLOSE_ID);
}

GSMNPCChannelClose::GSMNPCChannelClose (const GSMNPCChannelClose& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GSMNPCChannelClose::~GSMNPCChannelClose ()
{
        delete _id;
}

void GSMNPCChannelClose::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GSMNPCChannelClose::show ()
{
        printf ("GSMNPCChannelClose {}\n");
}

uint8_t GSMNPCChannelClose::getId ()
{
        return _id->getVal ();
}

void GSMNPCChannelClose::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * RequestTileUpdate
 ***************************************************************/

GSMRequestTileUpdate::GSMRequestTileUpdate (NetworkMessage* msg, 
        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMRequestTileUpdate::GSMRequestTileUpdate (const TPos& pos)
{
        _id = new TWord8 ((uint8_t)GSM_REQUEST_CONTAINER_UPDATE_ID);
        _pos = new TPos (pos);
}

GSMRequestTileUpdate::GSMRequestTileUpdate (
                const GSMRequestTileUpdate& clone)
{
        _id = new TWord8 (*clone._id);
        _pos = new TPos (*clone._pos);
}
        
GSMRequestTileUpdate::~GSMRequestTileUpdate ()
{
        delete _id;
        delete _pos;
}

void GSMRequestTileUpdate::put (NetworkMessage* msg)
{
        _id->put (msg);
        _pos->put (msg);
}

void GSMRequestTileUpdate::show ()
{
        printf ("GSMRequestTileUpdate {\n"); 
        printf ("\tpos: "); _pos->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMRequestTileUpdate::getId ()
{
        return _id->getVal ();
}

const TPos& GSMRequestTileUpdate::getPos ()
{
        return *_pos;
}

void GSMRequestTileUpdate::get (NetworkMessage* msg, 
        GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _pos = new TPos (msg);
}

/***************************************************************
 * RequestTileUpdate
 ***************************************************************/

GSMRequestContainerUpdate::GSMRequestContainerUpdate (NetworkMessage* msg, 
        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GSMRequestContainerUpdate::GSMRequestContainerUpdate (uint8_t containerid)
{
        _id = new TWord8 ((uint8_t)GSM_REQUEST_CONTAINER_UPDATE_ID);
        _containerid = new TWord8 (containerid);
}

GSMRequestContainerUpdate::GSMRequestContainerUpdate (
                const GSMRequestContainerUpdate& clone)
{
        _id = new TWord8 (*clone._id);
        _containerid = new TWord8 (*clone._containerid);
}
        
GSMRequestContainerUpdate::~GSMRequestContainerUpdate ()
{
        delete _id;
        delete _containerid;
}

void GSMRequestContainerUpdate::put (NetworkMessage* msg)
{
        _id->put (msg);
        _containerid->put (msg);
}

void GSMRequestContainerUpdate::show ()
{
        printf ("GSMRequestContainerUpdate {\n"); 
        printf ("containerid: "); _containerid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GSMRequestContainerUpdate::getId ()
{
        return _id->getVal ();
}

uint8_t GSMRequestContainerUpdate::getContainerId ()
{
        return _containerid->getVal ();
}

void GSMRequestContainerUpdate::get (NetworkMessage* msg, 
        GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _containerid = new TWord8 (msg);
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
        return _id->getVal ();
}

uint32_t GRMSelfInfo::getTibiaId ()
{
        return _tibiaId->getVal ();
}

uint8_t GRMSelfInfo::getReportErrors ()
{
        return _reportErrors->getVal ();
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
        return _id->getVal ();
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
                
        TPos start (_pos->x () - 8, _pos->y () - 6, minz);
        TPos end (_pos->x () + 9, _pos->y () + 7, maxz);

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
                
        TPos start (pos.x - 8, pos.y - 7, minz);
        TPos end (pos.x + 9, pos.y - 7, maxz);

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
                
        TPos start (pos.x + 10, pos.y - 6, minz);
        TPos end (pos.x + 10, pos.y + 7, maxz);

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

        TPos start (pos.x - 8, pos.y + 8, minz);
        TPos end (pos.x + 9, pos.y + 8, maxz);

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
                
        TPos start (pos.x - 9, pos.y - 6, minz);
        TPos end (pos.x - 9, pos.y + 7, maxz);

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

bool GRMMapUp::hasMap ()
{
        return _hasmap;
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
                _hasmap = true;
        } else if (pos.z > 8){
                //we are under ground and need the floor above us
                minz = pos.z - 3;
                maxz = minz;

                TPos start (pos.x - 8, pos.y - 6, minz);
                TPos end (pos.x + 9, pos.y + 7, maxz);
                _map = new TMapDescription (start, end, msg, dat);
                _hasmap = true;
        } else {
                //we are above ground
                _hasmap = false;
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

bool GRMMapDown::hasMap ()
{
        return _hasmap;
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
        const Pos& pos = gs->map->getCurPos ();
        uint32_t minz;
        uint32_t maxz;
        if (pos.z == 7) {
                //we are about to go underground and have 6 & 7
                minz = 8;
                maxz = 10;
                
                TPos start (pos.x - 8, pos.y - 6, minz);
                TPos end (pos.x + 9, pos.y + 7, maxz);
                _map = new TMapDescription (start, end, msg, dat);
                _hasmap = true;
        } else if (pos.z > 7){
                //we are under ground and need the below above us
                minz = MIN (pos.z + 3, MAX_FLOOR);
                maxz = minz;

                TPos start (pos.x - 8, pos.y - 6, minz);
                TPos end (pos.x + 9, pos.y + 7, maxz);
                _map = new TMapDescription (start, end, msg, dat);
                _hasmap = true;
        } else {
                //we are above ground
                _hasmap = false;
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
        return _id->getVal ();
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
        /* this is a bit annoying, if we get an item 0xFF01 we clear the tile
         * else we read a whole tile description */
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
 * ContainerOpen
 ***************************************************************/
GRMContainerOpen::GRMContainerOpen (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMContainerOpen::GRMContainerOpen (uint8_t cid, const TContainer& container)
{
        _id = new TWord8 ((uint8_t)GRM_CONTAINER_OPEN_ID);
        _cid = new TWord8 (cid);
        _container = new TContainer (container);
}

GRMContainerOpen::GRMContainerOpen (const GRMContainerOpen& clone)
{
        _id = new TWord8 (*clone._id);
        _cid = new TWord8 (*clone._cid);
        _container = new TContainer (*clone._container);
}

GRMContainerOpen::~GRMContainerOpen ()
{
        delete _id;
        delete _cid;
        delete _container;
}

void GRMContainerOpen::put (NetworkMessage* msg)
{
        _id->put (msg);
        _cid->put (msg);
        _container->put (msg);
}

void GRMContainerOpen::show ()
{
        printf ("GRMContainerOpen {\n");
        printf ("\tcid: "); _cid->show (); printf ("\n");
        _container->show ();
        printf ("}\n");
}

uint8_t GRMContainerOpen::getId ()
{
        return _id->getVal ();
}

uint8_t GRMContainerOpen::getCid ()
{
        return _cid->getVal ();
}

TContainer& GRMContainerOpen::getContainer ()
{
        return *_container;
}

void GRMContainerOpen::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _cid = new TWord8 (msg);
        _container = new TContainer (msg, dat);
}

/***************************************************************
 * ContainerClose
 ***************************************************************/
GRMContainerClose::GRMContainerClose (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat)
{
        get (msg, gs, dat);
}

GRMContainerClose::GRMContainerClose (uint8_t cid)
{
        _id = new TWord8 ((uint8_t)GRM_CONTAINER_CLOSE_ID);
        _cid = new TWord8 (cid);
}

GRMContainerClose::GRMContainerClose (const GRMContainerClose& clone)
{
        _id = new TWord8 (*clone._id);
        _cid = new TWord8 (*clone._cid);
}

GRMContainerClose::~GRMContainerClose ()
{
        delete _id;
        delete _cid;
}

void GRMContainerClose::put (NetworkMessage* msg)
{
        _id->put (msg);
        _cid->put (msg);
}

void GRMContainerClose::show ()
{
        printf ("GRMContainerClose {\n");
        printf ("\tcid: "); _cid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMContainerClose::getId ()
{
        return _id->getVal ();
}

uint8_t GRMContainerClose::getCid ()
{
        return _cid->getVal ();
}

void GRMContainerClose::get (NetworkMessage* msg,
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
        _id = new TWord8 ((uint8_t)GRM_CONTAINER_ADD_ID);
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
        _id = new TWord8 ((uint8_t)GRM_CONTAINER_UPDATE_ID);
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
        _id = new TWord8 ((uint8_t)GRM_CONTAINER_REMOVE_ID);
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

GRMCreatureSpeed::GRMCreatureSpeed (uint32_t creatureid, uint16_t speed)
{
        _id = new TWord8 ((uint8_t)GRM_CREATURE_SPEED_ID);
        _creatureid = new TWord32 (creatureid);
        _speed = new TWord16 (speed);
}

GRMCreatureSpeed::GRMCreatureSpeed (const GRMCreatureSpeed& clone)
{
        _id = new TWord8 (*clone._id);
        _creatureid = new TWord32 (*clone._creatureid);
        _speed = new TWord16 (*clone._speed);
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

uint16_t GRMCreatureSpeed::getSpeed ()
{
        return _speed->getVal ();
}

void GRMCreatureSpeed::get (NetworkMessage* msg,
                                GameState* gs, 
                                DatReader* dat)
{
        _id = new TWord8 (msg);
        _creatureid = new TWord32 (msg);
        _speed = new TWord16 (msg);
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

GRMShopGold::GRMShopGold (uint32_t ngold, TShopSellList* selllist)
{
        _id = new TWord8 ((uint8_t)GRM_SHOP_GOLD_ID);
        _ngold = new TWord32 (ngold);
        _selllist = selllist;
}

GRMShopGold::GRMShopGold (const GRMShopGold& clone)
{
        _id = new TWord8 (*clone._id);
        _ngold = new TWord32 (*clone._ngold);
        _selllist = new TShopSellList (*clone._selllist);
}
        
GRMShopGold::~GRMShopGold ()
{
        delete _id;
        delete _ngold;
        delete _selllist;
}

void GRMShopGold::put (NetworkMessage* msg)
{
        _id->put (msg);
        _ngold->put (msg);
        _selllist->put (msg);
}

void GRMShopGold::show ()
{
        printf ("GRMShopGold {\n");
        printf ("\tgold: "); _ngold->show (); printf ("\n");
        printf ("\tsell list: "); _selllist->show (); printf ("\n");
}

uint8_t GRMShopGold::getId ()
{
        return _id->getVal ();
}

uint32_t GRMShopGold::getNGold ()
{
        return _ngold->getVal ();
}

const TShopSellList& GRMShopGold::getSellList ()
{
        return *_selllist;
}

void GRMShopGold::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _ngold = new TWord32 (msg);
        _selllist = new TShopSellList (msg, dat);
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

/***************************************************************
 * GRMChannelOpen
 ***************************************************************/

GRMChannelOpen::GRMChannelOpen (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelOpen::GRMChannelOpen (uint16_t channelid, const std::string& name)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_OPEN_ID);
        _channelid = new TWord16 (channelid);
        _name = new TString (name);
}
        
GRMChannelOpen::GRMChannelOpen (const GRMChannelOpen& clone)
{
        _id = new TWord8 (*clone._id);
        _channelid = new TWord16 (*clone._channelid);
        _name = new TString (*clone._name);
}
        
GRMChannelOpen::~GRMChannelOpen ()
{
        delete _id;
        delete _channelid;
        delete _name;
}

void GRMChannelOpen::put (NetworkMessage* msg)
{
        _id->put (msg);
        _channelid->put (msg);
        _name->put (msg);
}

void GRMChannelOpen::show ()
{
        printf ("GRMChannelOpen {"); 
        printf ("\tchannelid: "); _channelid->show (); printf ("\n");
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMChannelOpen::getId ()
{
        return _id->getVal ();
}

uint16_t GRMChannelOpen::getChannelId ()
{
        return _channelid->getVal ();
}

const std::string& GRMChannelOpen::getName ()
{
        return _name->getString ();
}

void GRMChannelOpen::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _channelid = new TWord16 (msg);
        _name= new TString (msg);
}

/***************************************************************
 * GRMChannelPlayer
 ***************************************************************/

GRMChannelPlayer::GRMChannelPlayer (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelPlayer::GRMChannelPlayer (const std::string& name)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_PLAYER_ID);
        _name = new TString (name);
}
        
GRMChannelPlayer::GRMChannelPlayer (const GRMChannelPlayer& clone)
{
        _id = new TWord8 (*clone._id);
        _name = new TString (*clone._name);
}
        
GRMChannelPlayer::~GRMChannelPlayer ()
{
        delete _id;
        delete _name;
}

void GRMChannelPlayer::put (NetworkMessage* msg)
{
        _id->put (msg);
        _name->put (msg);
}

void GRMChannelPlayer::show ()
{
        printf ("GRMChannelPlayer {"); 
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMChannelPlayer::getId ()
{
        return _id->getVal ();
}

const std::string& GRMChannelPlayer::getName ()
{
        return _name->getString ();
}

void GRMChannelPlayer::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _name= new TString (msg);
}

/***************************************************************
 * GRMChannelRuleOpen
 ***************************************************************/

GRMChannelRuleOpen::GRMChannelRuleOpen (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelRuleOpen::GRMChannelRuleOpen (uint16_t channelid)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_RULE_OPEN_ID);
        _channelid = new TWord16 (channelid);
}
        
GRMChannelRuleOpen::GRMChannelRuleOpen (const GRMChannelRuleOpen& clone)
{
        _id = new TWord8 (*clone._id);
        _channelid = new TWord16 (*clone._channelid);
}
        
GRMChannelRuleOpen::~GRMChannelRuleOpen ()
{
        delete _id;
        delete _channelid;
}

void GRMChannelRuleOpen::put (NetworkMessage* msg)
{
        _id->put (msg);
        _channelid->put (msg);
}

void GRMChannelRuleOpen::show ()
{
        printf ("GRMChannelRuleOpen {"); 
        printf ("\tchannelid: "); _channelid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMChannelRuleOpen::getId ()
{
        return _id->getVal ();
}

uint16_t GRMChannelRuleOpen::getChannelId ()
{
        return _channelid->getVal ();
}

void GRMChannelRuleOpen::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _channelid = new TWord16 (msg);
}

/***************************************************************
 * GRMChannelRuleU1
 ***************************************************************/

GRMChannelRuleU1::GRMChannelRuleU1 (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelRuleU1::GRMChannelRuleU1 (const std::string& string)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_RULE_U1_ID);
        _string = new TString (string);
}
        
GRMChannelRuleU1::GRMChannelRuleU1 (const GRMChannelRuleU1& clone)
{
        _id = new TWord8 (*clone._id);
        _string = new TString (*clone._string);
}
        
GRMChannelRuleU1::~GRMChannelRuleU1 ()
{
        delete _id;
        delete _string;
}

void GRMChannelRuleU1::put (NetworkMessage* msg)
{
        _id->put (msg);
        _string->put (msg);
}

void GRMChannelRuleU1::show ()
{
        printf ("GRMChannelRuleU1 {"); 
        printf ("\tstring: "); _string->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMChannelRuleU1::getId ()
{
        return _id->getVal ();
}

const std::string& GRMChannelRuleU1::getString ()
{
        return _string->getString ();
}

void GRMChannelRuleU1::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _string= new TString (msg);
}

/***************************************************************
 * GRMChannelRuleU2
 ***************************************************************/

GRMChannelRuleU2::GRMChannelRuleU2 (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelRuleU2::GRMChannelRuleU2 (const std::string& string)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_RULE_U2_ID);
        _string = new TString (string);
}
        
GRMChannelRuleU2::GRMChannelRuleU2 (const GRMChannelRuleU2& clone)
{
        _id = new TWord8 (*clone._id);
        _string = new TString (*clone._string);
}
        
GRMChannelRuleU2::~GRMChannelRuleU2 ()
{
        delete _id;
        delete _string;
}

void GRMChannelRuleU2::put (NetworkMessage* msg)
{
        _id->put (msg);
        _string->put (msg);
}

void GRMChannelRuleU2::show ()
{
        printf ("GRMChannelRuleU2 {"); 
        printf ("\tstring: "); _string->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMChannelRuleU2::getId ()
{
        return _id->getVal ();
}

const std::string& GRMChannelRuleU2::getString ()
{
        return _string->getString ();
}

void GRMChannelRuleU2::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _string= new TString (msg);
}

/***************************************************************
 * GRMChannelRuleU3
 ***************************************************************/

GRMChannelRuleU3::GRMChannelRuleU3 (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelRuleU3::GRMChannelRuleU3 ()
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_RULE_U3_ID);
}
        
GRMChannelRuleU3::GRMChannelRuleU3 (const GRMChannelRuleU3& clone)
{
        _id = new TWord8 (*clone._id);
}
        
GRMChannelRuleU3::~GRMChannelRuleU3 ()
{
        delete _id;
}

void GRMChannelRuleU3::put (NetworkMessage* msg)
{
        _id->put (msg);
}

void GRMChannelRuleU3::show ()
{
        printf ("GRMChannelRuleU3 {"); 
        printf ("}\n");
}

uint8_t GRMChannelRuleU3::getId ()
{
        return _id->getVal ();
}

void GRMChannelRuleU3::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
}

/***************************************************************
 * GRMChannelChat
 ***************************************************************/

GRMChannelChat::GRMChannelChat (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelChat::GRMChannelChat (uint16_t channelid, const std::string& name)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_CHAT_ID);
        _channelid = new TWord16 (channelid);
        _name = new TString (name);
}
        
GRMChannelChat::GRMChannelChat (const GRMChannelChat& clone)
{
        _id = new TWord8 (*clone._id);
        _channelid = new TWord16 (*clone._channelid);
        _name = new TString (*clone._name);
}
        
GRMChannelChat::~GRMChannelChat ()
{
        delete _id;
        delete _channelid;
        delete _name;
}

void GRMChannelChat::put (NetworkMessage* msg)
{
        _id->put (msg);
        _channelid->put (msg);
        _name->put (msg);
}

void GRMChannelChat::show ()
{
        printf ("GRMChannelChat {"); 
        printf ("\tchannelid: "); _channelid->show (); printf ("\n");
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMChannelChat::getId ()
{
        return _id->getVal ();
}

uint16_t GRMChannelChat::getChannelId ()
{
        return _channelid->getVal ();
}

const std::string& GRMChannelChat::getName ()
{
        return _name->getString ();
}

void GRMChannelChat::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _channelid = new TWord16 (msg);
        _name= new TString (msg);
}

/***************************************************************
 * GRMChannelClose
 ***************************************************************/

GRMChannelClose::GRMChannelClose (NetworkMessage* msg, GameState* gs,
              DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelClose::GRMChannelClose (uint16_t channelid)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_CLOSE_ID);
        _channelid = new TWord16 (channelid);
}
        
GRMChannelClose::GRMChannelClose (const GRMChannelClose& clone)
{
        _id = new TWord8 (*clone._id);
        _channelid = new TWord16 (*clone._channelid);
}
        
GRMChannelClose::~GRMChannelClose ()
{
        delete _id;
        delete _channelid;
}

void GRMChannelClose::put (NetworkMessage* msg)
{
        _id->put (msg);
        _channelid->put (msg);
}

void GRMChannelClose::show ()
{
        printf ("GRMChannelClose {"); 
        printf ("\tchannelid: "); _channelid->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMChannelClose::getId ()
{
        return _id->getVal ();
}

uint16_t GRMChannelClose::getChannelId ()
{
        return _channelid->getVal ();
}

void GRMChannelClose::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _channelid = new TWord16 (msg);
}

/***************************************************************
 * PlayerCancelWalk
 ***************************************************************/

GRMPlayerCancelWalk::GRMPlayerCancelWalk (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMPlayerCancelWalk::GRMPlayerCancelWalk (uint8_t direction)
{
        _id = new TWord8 ((uint8_t)GRM_PLAYER_CANCEL_WALK_ID);
        _direction = new TWord8 (direction);
}

GRMPlayerCancelWalk::GRMPlayerCancelWalk (const GRMPlayerCancelWalk& clone)
{
        _id = new TWord8 (*clone._id);
        _direction = new TWord8 (*clone._direction);
}
        
GRMPlayerCancelWalk::~GRMPlayerCancelWalk ()
{
        delete _id;
        delete _direction;
}

void GRMPlayerCancelWalk::put (NetworkMessage* msg)
{
        _id->put (msg);
        _direction->put (msg);
}

void GRMPlayerCancelWalk::show ()
{
        printf ("GRMPlayerCancelWalk {\n"); 
        printf ("\tdirection: "); _direction->show (); printf ("\n");
        printf ("}\n");
}

uint8_t GRMPlayerCancelWalk::getId ()
{
        return _id->getVal ();
}

uint8_t GRMPlayerCancelWalk::getDirection ()
{
        return _direction->getVal ();
}

void GRMPlayerCancelWalk::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _direction = new TWord8 (msg);
}

/***************************************************************
 * GRMTextWindow
 ***************************************************************/

GRMTextWindow::GRMTextWindow (NetworkMessage* msg, GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

GRMTextWindow::GRMTextWindow ( uint32_t           windowid,
                               const TThing&      item,
                               uint16_t           maxlen,
                               const std::string& text,
                               const std::string& writer,
                               const std::string& date)
{
        _id =           new TWord8 ((uint8_t)GRM_TEXT_WINDOW_ID);
        _windowid =     new TWord32 (windowid);

        TThingFactory tf;
        _item = tf.cloneThing (item);

        _maxlen =       new TWord16 (maxlen);
        _text =         new TString (text);
        _writer =       new TString (writer);
        _date =         new TString (date);
}
        
GRMTextWindow::GRMTextWindow (const GRMTextWindow& clone)
{
        _id =           new TWord8 (*clone._id);
        _windowid =     new TWord32 (*clone._windowid);

        TThingFactory tf;
        _item = tf.cloneThing (*clone._item);

        _maxlen =       new TWord16 (*clone._maxlen);
        _text =         new TString (*clone._text);
        _writer =       new TString (*clone._writer);
        _date =         new TString (*clone._date);
}

GRMTextWindow::~GRMTextWindow ()
{
        delete _id;
        delete _windowid;
        delete _item;
        delete _maxlen;
        delete _text;
        delete _writer;
        delete _date;
}

void GRMTextWindow::put (NetworkMessage* msg)
{
        _id->put (msg);
        _windowid->put (msg);
        _item->put (msg);
        _maxlen->put (msg);
        _text->put (msg);
        _writer->put (msg);
        _date->put (msg);
}

void GRMTextWindow::show ()
{
        printf ("TextWindw {\n");
        printf ("\tid: "); _id->show (); printf ("\n");
        printf ("\twindowid: "); _windowid->show (); printf ("\n");
        printf ("\titem: "); _item->show (); printf ("\n");
        printf ("\tmaxlen: "); _maxlen->show (); printf ("\n");
        printf ("\ttext: "); _text->show (); printf ("\n");
        printf ("\twriter: "); _writer->show (); printf ("\n");
        printf ("\tdate: "); _date->show (); printf ("\n");
}

uint8_t GRMTextWindow::getId ()
{
        return _id->getVal ();
}

uint32_t GRMTextWindow::getWindowId ()
{
        return _windowid->getVal ();
}

const TThing& GRMTextWindow::getItem ()
{
        return *_item;
}

uint16_t GRMTextWindow::getMaxLen ()
{
        return _maxlen->getVal ();
}

const std::string& GRMTextWindow::getText ()
{
        return _text->getString ();
}

const std::string& GRMTextWindow::getWriter ()
{
        return _writer->getString ();
}

const std::string& GRMTextWindow::getDate ()
{
        return _date->getString ();
}

void GRMTextWindow::get (NetworkMessage* msg, GameState* gs,
                        DatReader* dat)
{
        _id =           new TWord8 (msg);
        _windowid =     new TWord32 (msg);

        TThingFactory tf (msg, dat);
        _item = tf.getThing ();

        _maxlen =       new TWord16 (msg);
        _text =         new TString (msg);
        _writer =       new TString (msg);
        _date =         new TString (msg);
}


/***************************************************************
 * GRMTextHouse
 ***************************************************************/

GRMTextHouse::GRMTextHouse (NetworkMessage* msg, GameState* gs,
                                DatReader* dat)
{
        get (msg, gs, dat);
}

GRMTextHouse::GRMTextHouse ( uint8_t            u1,
                             uint32_t           windowid,
                             const std::string& text)
{
        _id =           new TWord8 ((uint8_t)GRM_TEXT_HOUSE_ID);
        _u1 =           new TWord8 (u1);
        _windowid =     new TWord32 (windowid);
        _text =         new TString (text);
}
        
GRMTextHouse::GRMTextHouse (const GRMTextHouse& clone)
{
        _id =           new TWord8 (*clone._id);
        _u1 =           new TWord8 (*clone._u1);
        _windowid =     new TWord32 (*clone._windowid);
        _text =         new TString (*clone._text);
}

GRMTextHouse::~GRMTextHouse ()
{
        delete _id;
        delete _u1;
        delete _windowid;
        delete _text;
}

void GRMTextHouse::put (NetworkMessage* msg)
{
        _id->put (msg);
        _u1->put (msg);
        _windowid->put (msg);
        _text->put (msg);
}

void GRMTextHouse::show ()
{
        printf ("TextWindw {\n");
        printf ("\tid: "); _id->show (); printf ("\n");
        printf ("\tu1: "); _u1->show (); printf ("\n");
        printf ("\twindowid: "); _windowid->show (); printf ("\n");
        printf ("\ttext: "); _text->show (); printf ("\n");
}

uint8_t GRMTextHouse::getId ()
{
        return _id->getVal ();
}

uint8_t GRMTextHouse::getU1 ()
{
        return _u1->getVal ();
}

uint32_t GRMTextHouse::getWindowId ()
{
        return _windowid->getVal ();
}

const std::string& GRMTextHouse::getText ()
{
        return _text->getString ();
}
void GRMTextHouse::get (NetworkMessage* msg, GameState* gs,
                        DatReader* dat)
{
        _id =           new TWord8 (msg);
        _u1 =           new TWord8 (msg);
        _windowid =     new TWord32 (msg);
        _text =         new TString (msg);
}

/***************************************************************
 * ChannelList
 ***************************************************************/

GRMChannelList::GRMChannelList (NetworkMessage* msg,
                        GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

GRMChannelList::GRMChannelList (TChannelList* channellist)
{
        _id = new TWord8 ((uint8_t)GRM_CHANNEL_LIST_ID);
        _channellist = channellist;
}

GRMChannelList::GRMChannelList (const GRMChannelList& clone)
{
        _id = new TWord8 (*clone._id);
        _channellist = new TChannelList (*clone._channellist);
}
        
GRMChannelList::~GRMChannelList ()
{
        delete _id;
        delete _channellist;
}

void GRMChannelList::put (NetworkMessage* msg)
{
        _id->put (msg);
        _channellist->put (msg);
}

void GRMChannelList::show ()
{
        printf ("GRMChannelList {\n");
        _channellist->show ();
        printf ("}\n");
}

uint8_t GRMChannelList::getId ()
{
        return _id->getVal ();
}

TChannelList& GRMChannelList::getChannelList ()
{
        return *_channellist;
}

void GRMChannelList::get (NetworkMessage* msg, GameState* gs,
                                        DatReader* dat)
{
        _id = new TWord8 (msg);
        _channellist = new TChannelList (msg);
}

/***************************************************************
 * Speak
 ***************************************************************/

GRMSpeak::GRMSpeak (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        get (msg, gs, dat);
}

/* public speak constructor */
GRMSpeak::GRMSpeak (uint32_t u1, const std::string& name, uint16_t level,
                    uint8_t type, const TPos& pos, const std::string& msg)
{
        _speaktype = GRMSpeak::pub;

        _id             = new TWord8 ((uint8_t)GRM_SPEAK_ID);
        _u1             = new TWord32 (u1);
        _name           = new TString (name);
        _level          = new TWord16 (level);
        _type           = new TWord8 (type);
        _pos            = new TPos (pos);
        _msg            = new TString (msg);

        _channelid      = NULL;
}

/* channel speak constructor */
GRMSpeak::GRMSpeak (uint32_t u1, const std::string& name, uint16_t level,
                    uint8_t type, uint16_t channelid, const std::string& msg)
{
        _speaktype = GRMSpeak::channel;

        _id             = new TWord8 ((uint8_t)GRM_SPEAK_ID);
        _u1             = new TWord32 (u1);
        _name           = new TString (name);
        _level          = new TWord16 (level);
        _type           = new TWord8 (type);
        _channelid      = new TWord16 (channelid);
        _msg            = new TString (msg);

        _pos            = NULL;
}

/* channel private constructor */
GRMSpeak::GRMSpeak (uint32_t u1, const std::string& name, uint16_t level,
                    uint8_t type, const std::string& msg)
{
        _speaktype = GRMSpeak::priv;

        _id             = new TWord8 ((uint8_t)GRM_SPEAK_ID);
        _u1             = new TWord32 (u1);
        _name           = new TString (name);
        _level          = new TWord16 (level);
        _type           = new TWord8 (type);
        _msg            = new TString (msg);

        _pos            = NULL;
        _channelid      = NULL;
}

GRMSpeak::GRMSpeak (const GRMSpeak& clone)
{
        _speaktype = clone._speaktype;;

        _id             = new TWord8 (*clone._id);
        _u1             = new TWord32 (*clone._u1);
        _name           = new TString (*clone._name);
        _level          = new TWord16 (*clone._level);
        _type           = new TWord8 (*clone._type);
        _msg            = new TString (*clone._msg);

        if (_speaktype == GRMSpeak::pub) {
                _pos            = new TPos (*clone._pos);
                _channelid      = NULL;
        } else if (_speaktype == GRMSpeak::channel) {
                _pos            = NULL;
                _channelid      = new TWord16 (*clone._channelid);
        } else if (_speaktype == GRMSpeak::priv) {
                _pos            = NULL;
                _channelid      = NULL;
        }
}
        
GRMSpeak::~GRMSpeak ()
{
        delete _id;
        delete _u1;
        delete _name;
        delete _level;
        delete _type;
        delete _msg;

        delete _pos;
        delete _channelid;
}

void GRMSpeak::put (NetworkMessage* msg)
{
        _id->put (msg);
        _u1->put (msg);
        _name->put (msg);
        _level->put (msg);
        _type->put (msg);
        if (_speaktype == GRMSpeak::pub) {
                _pos->put (msg);
        } else if (_speaktype == GRMSpeak::channel) {
                _channelid->put (msg);
        }
        _msg->put (msg);
}

void GRMSpeak::show ()
{
        printf ("GRMSpeak {\n"); 
        if (_speaktype == GRMSpeak::pub) {
                printf ("\tpublic speak\n");
        } else if (_speaktype == GRMSpeak::channel) {
                printf ("\tchannel speak\n");
        } else if (_speaktype == GRMSpeak::priv) {
                printf ("\tprivate speak\n");
        }
        printf ("\tu1: "); _u1->show (); printf ("\n");
        printf ("\tname: "); _name->show (); printf ("\n");
        printf ("\tlevel: "); _level->show (); printf ("\n");
        printf ("\ttype: "); _type->show (); printf ("\n");
        if (_speaktype == GRMSpeak::pub) {
                printf ("\tpos: "); _pos->show (); printf ("\n");
        } else if (_speaktype == GRMSpeak::channel) {
                printf ("\tchannelid: "); _channelid->show (); printf ("\n");
        }
        printf ("msg: "); _msg->show (); printf ("\n");
        printf ("}\n");
}

GRMSpeak::SpeakType GRMSpeak::getSpeakType ()
{
        return _speaktype;
}

uint8_t GRMSpeak::getId ()
{
        return _id->getVal ();
}

uint32_t GRMSpeak::getU1 ()
{
        return _u1->getVal ();
}

const std::string& GRMSpeak::getName ()
{
        return _name->getString ();
}

uint16_t GRMSpeak::getLevel ()
{
        return _level->getVal ();
}

uint8_t GRMSpeak::getType ()
{
        return _type->getVal ();
}

const TPos& GRMSpeak::getPos ()
{
        if (_speaktype != GRMSpeak::pub) {
                printf ("GRMSpeak error: request for pos for ");
                printf ("non-public speak\n");
        }
        return *_pos;
}

uint16_t GRMSpeak::getChannelId ()
{
        if (_speaktype != GRMSpeak::channel) {
                printf ("GRMSpeak error: request for channelid for ");
                printf ("non-channel speak\n");
        }
        return _channelid->getVal ();
}

const std::string& GRMSpeak::getMsg ()
{
        return _msg->getString ();
}

void GRMSpeak::get (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        _id = new TWord8 (msg);
        _u1 = new TWord32 (msg);
        _name = new TString (msg);
        _level = new TWord16 (msg);
        _type = new TWord8 (msg);

        uint32_t t = _type->getVal ();

        if (t == SPEAK_SAY
         || t == SPEAK_WHISPER
         || t == SPEAK_YELL
         || t == SPEAK_PRIVATE_PN
         || t == SPEAK_PRIVATE_NP
         || t == SPEAK_MONSTER_SAY
         || t == SPEAK_MONSTER_YELL)
        {
                _speaktype = GRMSpeak::pub;
                _pos = new TPos (msg);
                _channelid = NULL;
        } else if (t == SPEAK_CHANNEL_Y
                || t == SPEAK_CHANNEL_R1
                || t == SPEAK_CHANNEL_R2
                || t == SPEAK_CHANNEL_O)
        {
                _speaktype = GRMSpeak::channel;
                _pos = NULL;
                _channelid = new TWord16 (msg); 
        } else if (t == SPEAK_PRIVATE
                || t == SPEAK_BROADCAST
                || t == SPEAK_PRIVATE_RED)
        {
                _speaktype = GRMSpeak::priv;
                _pos = NULL;
                _channelid = NULL;
        } else {
                printf ("GRMSpeak error: unknown speak_id %d\n", t);
        }
        _msg = new TString (msg);
}

