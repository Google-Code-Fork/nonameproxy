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

class LSMessageFactory
{
        public:
                LSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~LSMessageFactory ();

                TibiaMessage* getMessage ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class LRMessageFactory
{
        public:
                LRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~LRMessageFactory ();

                TibiaMessage* getMessage ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class GSMessageFactory
{
        public:
                GSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~GSMessageFactory ();

                TibiaMessage* getMessage ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class GRMessageFactory
{
        public:
                GRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~GRMessageFactory ();

                TibiaMessage* getMessage ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};
#endif

