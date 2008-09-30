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
                LSMessageFactory ();
                virtual ~LSMessageFactory ();

                TibiaMessage* getMessage ();

                void deleteMessage (TibiaMessage* tm);
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;

                bool _readable;
};

class LRMessageFactory
{
        public:
                LRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                LRMessageFactory ();
                virtual ~LRMessageFactory ();

                TibiaMessage* getMessage ();

                void deleteMessage (TibiaMessage* tm);
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;

                bool _readable;
};

class GSMessageFactory
{
        public:
                GSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GSMessageFactory ();
                virtual ~GSMessageFactory ();

                TibiaMessage* getMessage ();

                void deleteMessage (TibiaMessage* tm);
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;

                bool _readable;
};

class GRMessageFactory
{
        public:
                GRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                GRMessageFactory ();
                virtual ~GRMessageFactory ();

                TibiaMessage* getMessage ();

                void deleteMessage (TibiaMessage* tm);
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;

                bool _readable;
};
#endif

