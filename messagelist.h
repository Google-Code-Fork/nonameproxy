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

#ifndef __MESSAGE_LIST_H
#define __MESSAGE_LIST_H

//this class is basically a typed representation of tibia packet.

#include <list>

class NetworkMessage;
class TibiaMessage;
class GameState;
class DatReader;
class MessageFactory;

typedef std::list<TibiaMessage*> MsgList;

class MessageList
{
        public:
                MessageList ();
                virtual ~MessageList ();
                //set iterator to beginning of list
                void begin ();
                //returns true if iterator is at the end of the list
                bool isEnd ();
                //increment the iterator forward
                void next ();
                //get the current message
                TibiaMessage* read ();
                //put a message BEFORE the current message
                void insert (TibiaMessage* tm);
                //replace the current message
                void replace (TibiaMessage* tm);
                //removes the current, the iterator points to the next message
                void remove ();
                //add a message to the end of the message list
                void add (TibiaMessage* tm);

                //returns an unencrypted networkmessage containing
                //the entire MessageList
                virtual NetworkMessage* put ();

        protected:
                MsgList _msglist;
                MsgList::iterator _it;
                MessageFactory* _mf;
                bool _isEnd;
};

class LSMessageList : public MessageList
{
        public:
                LSMessageList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LSMessageList ();
                virtual NetworkMessage* put ();
};

class LRMessageList : public MessageList
{
        public:
                LRMessageList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                LRMessageList ();
                virtual NetworkMessage* put ();
};
                
class GSMessageList : public MessageList
{
        public:
                GSMessageList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GSMessageList ();
                virtual NetworkMessage* put ();
};

class GRMessageList : public MessageList
{
        public:
                GRMessageList (NetworkMessage* msg,
                                GameState* gs,
                                DatReader* dat);
                GRMessageList ();
                virtual NetworkMessage* put ();
};

#endif

