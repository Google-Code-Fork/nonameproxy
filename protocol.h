#ifndef __PROTOCOL_H
#define __PROTOCOL_H

class NetworkMessage;
class GameState;

class Protocol
{
        public:
                virtual void OnRecv (NetworkMessage* msg, GameState* gstate);
                //all send functions will also be defined here, each
                //returning an unencrypted network message
};

#endif

