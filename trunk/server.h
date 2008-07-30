#ifndef __SERVER_H
#define __SERVER_H

#include <stdint.h>

class Connection;

class Server
{
        public:
                Server ();

                bool listenOn (uint16_t port);
                Connection* acceptConnection ();
                bool shutdown ();
        private:
                #ifdef WIN32
                #else
                int32_t mastersock;
                #endif
};

#endif

