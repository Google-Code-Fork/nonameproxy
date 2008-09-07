#include "messagelist.h"
#include "networkmessage.h"
#include "tibiamessage.h"
#include "messagefactory.h"
#include <stdio.h>

//ive never seen a tibia packet bigger than that so ill hope this ammount will
//be enough. Id like to eventually have the networkmessages to expand
//dynamically
#define MAX_MSG_SIZE 16384

void MessageList::begin ()
{
        _it = _msglist.begin ();
}

bool MessageList::isEnd ()
{
        if (_it == _msglist.end ()) {
                return true;
        } else {
                return false;
        }
}

void MessageList::next ()
{
        if (_it == _msglist.end ()) {
                printf ("error: attempt to seek past end of message list\n");
        } else {
                _it ++;
        }
}

TibiaMessage* MessageList::read ()
{
        return *_it;
}

void MessageList::insert (TibiaMessage* tm)
{
        _msglist.insert (_it, tm);
        //_it --;
}

void MessageList::replace (TibiaMessage* tm)
{
        if (_msglist.size () == 0) {
                printf ("msglist error: attemp to replace in empty list\n");
                return;
        }
        if (_it == _msglist.end ()) {
                printf ("msglist error: attemp to replace \"end\"\n");
                return;
        }
        _it = _msglist.erase (_it);
        _msglist.insert (_it, tm);
        _it --;
} 

void MessageList::add (TibiaMessage* tm)
{
        _msglist.push_back (tm);
}

NetworkMessage* MessageList::put ()
{
        return NULL;
}

//LSMessageList
LSMessageList::LSMessageList (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        LSMessageFactory lsmf (msg, gs, dat);
        TibiaMessage* tm;
        while ((tm = lsmf.getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
        _it = _msglist.begin ();
}

LSMessageList::LSMessageList ()
{
        _it = _msglist.begin ();
}

LSMessageList::~LSMessageList ()
{
        for (_it = _msglist.begin (); _it != _msglist.end (); ++_it) {
                delete (*_it);
        }
}

NetworkMessage* LSMessageList::put ()
{
        if (_msglist.size () == 0) {
                return NULL; //we dont want to be sending empty msgs
        }

        NetworkMessage* msg = new NetworkMessage (MAX_MSG_SIZE);
        
        //now we have to work out what type of header to use
        MsgList::iterator i = _msglist.begin ();
        if ((*i)->getId () == 0x01) { //rsa login message 
                msg->prepRSAHeader ();
                (*i)->put (msg);
                msg->writeRSAHeader ();
        } else {
                msg->prepHeader ();
                for (i = _msglist.begin (); i != _msglist.end (); ++i) {
                        (*i)->put (msg);
                }
                msg->writeHeader ();
        }
        return msg;
}

//LRMessageList
LRMessageList::LRMessageList (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        LRMessageFactory lrmf (msg, gs, dat);
        TibiaMessage* tm;
        while ((tm = lrmf.getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
        _it = _msglist.begin ();
}

LRMessageList::LRMessageList ()
{
        _it = _msglist.begin ();
}

LRMessageList::~LRMessageList ()
{
        for (_it = _msglist.begin (); _it != _msglist.end (); ++_it) {
                delete (*_it);
        }
}

NetworkMessage* LRMessageList::put ()
{
        //there are no rsa messages recvd from the login server
        if (_msglist.size () == 0) {
                return NULL; //we dont want to be sending empty msgs
        }

        NetworkMessage* msg = new NetworkMessage (MAX_MSG_SIZE);
        
        msg->prepHeader ();
        MsgList::iterator i = _msglist.begin ();
        for (i = _msglist.begin (); i != _msglist.end (); ++i) {
                (*i)->put (msg);
        }
        msg->writeHeader ();
        return msg;
}

//GSMessageList
GSMessageList::GSMessageList (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        GSMessageFactory lrmf (msg, gs, dat);
        TibiaMessage* tm;
        while ((tm = lrmf.getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
        _it = _msglist.begin ();
}

GSMessageList::GSMessageList ()
{
        _it = _msglist.begin ();
}

GSMessageList::~GSMessageList ()
{
        for (_it = _msglist.begin (); _it != _msglist.end (); ++_it) {
                delete (*_it);
        }
}

NetworkMessage* GSMessageList::put ()
{
        //there are no rsa messages recvd from the login server
        if (_msglist.size () == 0) {
                return NULL; //we dont want to be sending empty msgs
        }
        NetworkMessage* msg = new NetworkMessage (MAX_MSG_SIZE);
        
        msg->prepHeader ();
        MsgList::iterator i = _msglist.begin ();
        if ((*i)->getId () == 0x0A) { //rsa init message 
                msg->prepRSAHeader ();
                (*i)->put (msg);
                msg->writeRSAHeader ();
        } else {
                msg->prepHeader ();
                for (i = _msglist.begin (); i != _msglist.end (); ++i) {
                        (*i)->put (msg);
                }
                msg->writeHeader ();
        }
        return msg;
}

//GRMessageList
GRMessageList::GRMessageList (NetworkMessage* msg, GameState* gs, DatReader* dat)
{
        GRMessageFactory lrmf (msg, gs, dat);
        TibiaMessage* tm;
        while ((tm = lrmf.getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
        _it = _msglist.begin ();
}

GRMessageList::GRMessageList ()
{
        _it = _msglist.begin ();
}

GRMessageList::~GRMessageList ()
{
        for (_it = _msglist.begin (); _it != _msglist.end (); ++_it) {
                delete (*_it);
        }
}

NetworkMessage* GRMessageList::put ()
{
        //there are no rsa messages recvd from the game server
        if (_msglist.size () == 0) {
                return NULL; //we dont want to be sending empty msgs
        }

        NetworkMessage* msg = new NetworkMessage (MAX_MSG_SIZE);
        
        msg->prepHeader ();
        MsgList::iterator i = _msglist.begin ();
        for (i = _msglist.begin (); i != _msglist.end (); ++i) {
                (*i)->put (msg);
        }
        msg->writeHeader ();
        return msg;
}

