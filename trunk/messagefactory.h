#ifndef __MESSAGE_FACTORY_H
#define __MESSAGE_FACTORY_H

class TibiaMessage;
class NetworkMessage;

//A Message factory takes an unecrypted network message and processes it
//getMessage will return a ptr to the next TibiaMessage or return NULL
//if packet has been completely read. When its deconstructor is called
//it will also call delete the NetworkMessage, so after assigning a
//networkmessage to a factory do not use it again

class LSMessageFactory
{
        public:
                LSMessageFactory (NetworkMessage* msg);
                virtual ~LSMessageFactory ();

                TibiaMessage* getMessage ();
        private:
                NetworkMessage* _msg;
};

class LRMessageFactory
{
        public:
                LRMessageFactory (NetworkMessage* msg);
                virtual ~LRMessageFactory ();

                TibiaMessage* getMessage ();
        private:
                NetworkMessage* _msg;
};

class GSMessageFactory
{
        public:
                GSMessageFactory (NetworkMessage* msg);
                virtual ~GSMessageFactory ();

                TibiaMessage* getMessage ();
        private:
                NetworkMessage* _msg;
};
/*
class GRMessageFactory
{
        public:
                GRMessageFactory (NetworkMessage* msg);
                virtual ~GRMessageFactory ();

                TibiaMessage* getMessage ();
};
*/
#endif

