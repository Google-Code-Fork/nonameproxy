#ifndef __CONNECTION_H
#define __CONNECTION_H

#ifdef WIN32
        #include <winsock2.h>
#endif

#include <stdint.h>
#include <list>

//#include "server.h"

class NetworkMessage;

typedef std::list<NetworkMessage*> MsgQueue;

class Connection
{
        public:
                Connection ();
                virtual ~Connection ();

                bool connectTo (const char* host, uint16_t port);
                bool connectTo (uint32_t ip, uint16_t port);

                bool closeConnection ();

                bool isConnected ();

                //all these functions do is simply access the msgQueues
                void putMsg (NetworkMessage* msg);
                //if a network message is ready getMsg will return a
                //pointer to that message, else it returns null
                NetworkMessage* getMsg ();

                #ifdef WIN32
                SOCKET  query_fd (fd_set& readfds, fd_set& writefds,
                        fd_set errfds);
                void    tell_fd  (fd_set& readfds, fd_set& writefds,
                        fd_set& errfds);
                #else
                int32_t query_fd (fd_set& readfds, fd_set& writefds,
                        fd_set& errfds);
                void    tell_fd  (fd_set& readfds, fd_set& writefds,
                        fd_set& errfds);
                #endif

        protected:
                #ifdef WIN32
                void setSocket (SOCKET socket);
                #else
                void setSocket (int32_t socket);
                #endif
                void setInfo (uint32_t ip, uint16_t port);

                friend class Server;
        private:
                bool _connectTo (struct sockaddr_in hostaddr);
                int32_t _get   (void* buf, int32_t len);
                int32_t _peek  (void* buf, int32_t len);
                int32_t _put   (void* buf, int32_t len);
                int32_t _close ();

                void _putMsg ();
                NetworkMessage* _getMsg ();

                #ifdef WIN32
                SOCKET socket;
                #else
                int32_t connsock;
                #endif
                uint32_t connIP;
                uint16_t connport;

                bool _isConnected;

                uint16_t readPos;
                uint16_t readLen;
                uint8_t* readBuffer;

                NetworkMessage* writeMsg;
                uint16_t writePos;
                uint16_t writeLen;
                uint8_t* writeBuffer;

                MsgQueue getQueue;
                MsgQueue putQueue;
};

#endif

