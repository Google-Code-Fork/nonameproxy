#ifndef __MESSAGE_LIST_H
#define __MESSAGE_LIST_H

//this class is basically a typed representation of tibia packet.

#include <list>

class NetworkMessage;
class TibiaMessage;

typedef std::list<TibiaMessage*> MsgList;

class MessageList
{
        public:
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
                //add a message to the end of the message list
                void add (TibiaMessage* tm);

                //returns an unencrypted networkmessage containing
                //the entire MessageList
                virtual NetworkMessage* put ();

        protected:
                MsgList _msglist;
                MsgList::iterator _it;
};

class LSMessageList : public MessageList
{
        public:
                LSMessageList (NetworkMessage* msg);
                LSMessageList ();
                virtual ~LSMessageList ();
                virtual NetworkMessage* put ();
};

class LRMessageList : public MessageList
{
        public:
                LRMessageList (NetworkMessage* msg);
                LRMessageList ();
                virtual ~LRMessageList ();
                virtual NetworkMessage* put ();
};
                
class GSMessageList : public MessageList
{
        public:
                GSMessageList (NetworkMessage* msg);
                GSMessageList ();
                virtual ~GSMessageList ();
                virtual NetworkMessage* put ();
};

#endif

