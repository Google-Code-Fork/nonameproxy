#ifndef __MESSAGE_FACTORY_H
#define __MESSAGE_FACTORY_H

class TibiaMessage;
class NetworkMessage;
class GameState;
class DatReader;

//A Message factory takes an unecrypted network message and processes it
//getMessage will return a ptr to the next TibiaMessage or return NULL
//if packet has been completely read. When its deconstructor is called
//it will also call delete the NetworkMessage, so after assigning a
//networkmessage to a factory do not use it again

class MessageFactory
{
        public:
                virtual ~MessageFactory () {}

                virtual TibiaMessage* getMessage () = 0;
                virtual bool isEnd () = 0;
};

class LSMessageFactory : public MessageFactory
{
        public:
                LSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~LSMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class LRMessageFactory : public MessageFactory
{
        public:
                LRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~LRMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class GSMessageFactory : public MessageFactory
{
        public:
                GSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~GSMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class GRMessageFactory : public MessageFactory
{
        public:
                GRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~GRMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};
#endif

