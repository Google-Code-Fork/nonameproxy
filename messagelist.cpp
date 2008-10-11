#include "messagelist.h"
#include "networkmessage.h"
#include "tibiamessage.h"
#include "messagefactory.h"
#include <stdio.h>

//ive never seen a tibia packet bigger than that so ill hope this ammount will
//be enough. Id like to eventually have the networkmessages to expand
//dynamically
#define MAX_MSG_SIZE 16384

MessageList::MessageList ()
{
        _isEnd = false;
        _it = _msglist.begin ();
}

MessageList::~MessageList ()
{
        delete _mf;
        for (_it = _msglist.begin (); _it != _msglist.end (); ++_it) {
                delete (*_it);
        }
}

void MessageList::begin ()
{
        _it = _msglist.begin ();
}

bool MessageList::isEnd ()
{
        return _isEnd;
}

void MessageList::next ()
{
        if (_it == _msglist.end ()) {
                printf ("next error: end of message list\n");
        }

        MsgList::const_iterator next = _it;
        next ++;
        /* if we are at the end we read a message, else wise we just ++ */
        if (next == _msglist.end () && _mf) {
                /* then well try to add a message
                 * but make sure we have the message factory first */
                TibiaMessage* tm = _mf->getMessage ();

                //assume weve reached the end
                _isEnd = true;
                if (tm) {
                        _msglist.push_back (tm);
                        //we lied
                        _isEnd = false;
                }
        }
        _it ++;
}

TibiaMessage* MessageList::read ()
{
        if (_it == _msglist.end ()) {
                TibiaMessage* tm = _mf->getMessage ();

                //assume weve reached the end
                _isEnd = true;
                if (tm) {
                        _msglist.push_back (tm);
                        //we lied
                        _isEnd = false;
                        //_it is currently at the end so we must move back
                        _it --;
                }
        }
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

void MessageList::remove ()
{
        if (_msglist.size () == 0) {
                printf ("msglist error: attemp to remove in empty list\n");
                return;
        }
        if (_it == _msglist.end ()) {
                printf ("msglist error: attemp to remove \"end\"\n");
                return;
        }
        _it = _msglist.erase (_it);
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
        /*LSMessageFactory lsmf (msg, gs, dat);
        TibiaMessage* tm;
        while ((tm = lsmf.getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
        _it = _msglist.begin ();*/
        _mf = new LSMessageFactory (msg, gs, dat);
        _it = _msglist.begin ();
}

LSMessageList::LSMessageList ()
{
        _mf = NULL;
        _it = _msglist.begin ();
}

NetworkMessage* LSMessageList::put ()
{
        /* add any pending messages */
        TibiaMessage* tm;
        while ((tm = _mf->getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
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
        /*LRMessageFactory lrmf (msg, gs, dat);
        TibiaMessage* tm;
        while ((tm = lrmf.getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
        _it = _msglist.begin ();*/
        _mf = new LRMessageFactory (msg, gs, dat);
        _it = _msglist.begin ();
}

LRMessageList::LRMessageList ()
{
        _mf = NULL;
        _it = _msglist.begin ();
}

NetworkMessage* LRMessageList::put ()
{
        /* add any pending messages */
        TibiaMessage* tm;
        while ((tm = _mf->getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
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
        /*GSMessageFactory lrmf (msg, gs, dat);
        TibiaMessage* tm;
        while ((tm = lrmf.getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
        _it = _msglist.begin ();*/
        _mf = new GSMessageFactory (msg, gs, dat);
        _it = _msglist.begin ();
}

GSMessageList::GSMessageList ()
{
        _mf = NULL;
        _it = _msglist.begin ();
}

NetworkMessage* GSMessageList::put ()
{
        /* add any pending messages */
        TibiaMessage* tm;
        while ((tm = _mf->getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
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
        _mf = new GRMessageFactory (msg, gs, dat);
        _it = _msglist.begin ();
}

GRMessageList::GRMessageList ()
{
        _mf = NULL;
        _it = _msglist.begin ();
}

NetworkMessage* GRMessageList::put ()
{
        /* add any pending messages */
        TibiaMessage* tm;
        while ((tm = _mf->getMessage ()) != NULL) {
                _msglist.push_back (tm);
        }
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

