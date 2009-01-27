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

#ifndef __CONNECTION_H
#define __CONNECTION_H

#ifdef WIN32
        #include <winsock2.h>
#else
        #include <sys/select.h>
#endif

#include <stdint.h>
#include <list>

#include "timer.h"

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

                uint32_t getSelectRequest ();

                //all these functions do is simply access the msgQueues
                void putMsg (NetworkMessage* msg);
                //if a network message is ready getMsg will return a
                //pointer to that message, else it returns null
                NetworkMessage* getMsg ();

                #ifdef WIN32
                SOCKET  query_fd (fd_set& readfds, fd_set& writefds,
                        fd_set& errfds);
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
                int32_t _put   (const void* buf, int32_t len);
                int32_t _close ();

                struct timeval _lastSend;

                void _putMsg ();
                NetworkMessage* _getMsg ();

                #ifdef WIN32
                SOCKET _connsock;
                #else
                int32_t _connsock;
                #endif
                uint32_t _connIP;
                uint16_t _connport;

                bool _isConnected;

                uint32_t _readPos;
                uint32_t _readLen;
                uint8_t* _readBuffer;

                NetworkMessage* _writeMsg;
                uint32_t _writePos;
                uint32_t _writeLen;
                uint8_t* _writeBuffer;

                MsgQueue _getQueue;
                MsgQueue _putQueue;

                uint32_t _selectRequest;
};

#endif

