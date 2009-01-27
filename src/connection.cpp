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

#ifdef WIN32
#else
        #include <netdb.h>
        #include <errno.h>
        #include <sys/select.h>
        #include <fcntl.h>
#endif

#include <string.h>
#include <stdio.h>

#include "connection.h"
#include "networkmessage.h"
#include "timer.h"

#define SEND_RATE 125

#ifdef WIN32
#define NETWORK_ERROR(str)      if (WSAGetLastError () != EAGAIN) { \
                                        printf (str); \
                                        printf (": error no %d\n", \
                                                        WSAGetLastError ()); \
                                }
#else
#define NETWORK_ERROR(str)      if (errno != EAGAIN) { \
                                        perror (str); \
                                }
#endif

Connection::Connection ()
{
        _isConnected = false;
        _readPos = 0;
        _writePos = 0;
        _readBuffer = NULL;
        _writeBuffer = NULL;
        _writeMsg = NULL;

        _selectRequest = 0;

        Timer timer;
        if (timer.gettimeofday (&_lastSend, NULL)) {
                printf ("connection error: failed to init gettimeofday\n");
        }
}

Connection::~Connection ()
{
        //close connection and delete all pending messages
}

bool Connection::isConnected ()
{
        return _isConnected;
}

uint32_t Connection::getSelectRequest ()
{
        return _selectRequest;
}

NetworkMessage* Connection::getMsg ()
{
        if (!_getQueue.empty ()) {
                NetworkMessage* ret = _getQueue.front ();
                _getQueue.pop_front ();
                return ret;
        } else {
                return NULL;
        }
}

void Connection::putMsg (NetworkMessage* msg)
{
        _putQueue.push_back (msg);
}

bool Connection::closeConnection ()
{
        if (_close () == 0) {
                _isConnected = false;
                return true;
        } else {
                return false;
        }
}

int32_t Connection::_get (void* buf, int32_t len)
{
#ifdef WIN32
        int32_t n = recv (_connsock, (char*)buf, len, 0);
#else
        int32_t n = recv (_connsock, buf, len, 0);
#endif
        if (n == -1) {
                //this could be an error, or it could just be EAGAIN
                NETWORK_ERROR ("recv error get");
        }
        return n;
}

int32_t Connection::_peek (void* buf, int32_t len)
{
#ifdef WIN32
        int32_t n = recv (_connsock, (char*)buf, len, MSG_PEEK);
#else
        int32_t n = recv (_connsock, buf, len, MSG_PEEK);
#endif
        if (n == -1) {
                //this could be an error, or it could just be EAGAIN
                NETWORK_ERROR ("recv error peek");
        }
        return n;
}

int32_t Connection::_put (const void* buf, int32_t len)
{
#ifdef WIN32
        int32_t n = send (_connsock, (const char*)buf, len, 0);
#else
        int32_t n = send (_connsock, buf, len, 0);
#endif
        if (n == -1) {
                //this could be an error, or it could just be EAGAIN
                NETWORK_ERROR ("send error");
        }
        return n;
}

int32_t Connection::_close ()
{
#ifdef WIN32
        int32_t n = closesocket (_connsock);
#else
        int32_t n = close (_connsock);
#endif
        if (n != 0) {
                NETWORK_ERROR ("close error");
        }
        return n;
}

NetworkMessage* Connection::_getMsg ()
{
        //printf ("starting read\n");
        if (_readPos == 0) {
                //printf ("reading header\n");
                uint16_t msgSize;
                int32_t n = _peek (&msgSize, 2);
                //printf ("peeked at %d\n", n);
                if (n == 0) {
                        closeConnection ();
                } else if (n == 2) {
                        //we can read the header and allocate the buffer
                        _readLen = msgSize + 2;
                        _readBuffer = new uint8_t[_readLen];
                        if (_get (_readBuffer, 2) == 2) {
                                _readPos += 2;
                                return _getMsg ();
                        } else {
                                //in theory we should never get here
                                delete[] _readBuffer;
                                _readBuffer = NULL;
                                printf ("recv error: This shouldnt happen");
                                printf ("please inform us if it does\n");
                        }
                }
        } else {
                int32_t n = _get (&_readBuffer[_readPos], _readLen - _readPos);
                if (n == 0) {
                        closeConnection ();
                } else if (n != -1) {
                        _readPos += n;
                }
                if (_readPos == _readLen) {
                        NetworkMessage* ret;
                        ret = new NetworkMessage (_readLen, _readBuffer);
                        _readPos = 0;
                        _readBuffer = NULL;
                        return ret;
                }
        }
        return NULL;
}

void Connection::_putMsg ()
{
        if (_writeMsg == NULL) {
                if (!_putQueue.empty ()) {
                        _writeMsg = _putQueue.front ();
                        _putQueue.pop_front ();

                        _writeBuffer = _writeMsg->getBuffer ();
                        _writeLen = *(uint16_t*)_writeBuffer + 2;
                        _writePos = 0;
                        _putMsg ();
                }
        /*} else if (_writeLen == _writePos) {
                delete _writeMsg;
                _writeMsg = NULL;
                _writePos = 0;
                _writeBuffer = NULL;*/
        } else {
                int32_t n = _put (&_writeBuffer[_writePos], _writeLen - _writePos);
                if (n != -1) {
                        _writePos += n;
                        if (_writeLen == _writePos) {
                                delete _writeMsg;
                                _writeMsg = NULL;
                                _writePos = 0;
                                _writeBuffer = NULL;

                                /**
                                 * Update the last send time so we know
                                 * not to send another packet until it
                                 * is safe to doso without flooding the
                                 * server
                                 */
                                Timer timer;
                                if (timer.gettimeofday (&_lastSend, NULL)) {
                                        printf ("connection error: ");
                                        printf ("gettimeofday\n");
                                }
                        }
                }
        }
}

#ifdef WIN32
SOCKET Connection::query_fd (fd_set& readfds, fd_set& writefds,
        fd_set& errfds)
{
        _selectRequest == 0;
        if (_isConnected) {
                //we always try to recv
                FD_SET (_connsock, &readfds);

                if (!_putQueue.empty () || _writeBuffer) {
                        Timer timer;
                        struct timeval tv;
                        if (timer.gettimeofday (&tv, NULL)) {
                                printf ("connection error: gettimeofday\n");
                        }

                        int32_t sdiff = tv.tv_sec - _lastSend.tv_sec;
                        int32_t udiff = tv.tv_usec - _lastSend.tv_usec;

                        int64_t mdiff = sdiff * 1000 + udiff / 1000;
                        if (mdiff >= SEND_RATE) {
                                FD_SET (_connsock, &writefds);
                        } else {
                                _selectRequest = SEND_RATE - mdiff;
                        }
                }

                FD_SET (_connsock, &errfds);
        }
        return _connsock;
}

//this function takes the result of select and responds accordingly
void Connection::tell_fd (fd_set& readfds, fd_set& writefds,
        fd_set& errfds)
{
        if (FD_ISSET (_connsock, &readfds)) {
                NetworkMessage* msg = _getMsg ();
                if (msg != NULL) {
                        _getQueue.push_back (msg);
                }
        }
        if (FD_ISSET (_connsock, &writefds)) {
                if (!_putQueue.empty ()) {
                        _putMsg ();
                }
        }
}
                
#else
//Ive got no idea how this is going to be implemented under windows
//but im going to leave that problem for another day

//this function takes a read write and error fd_set and modifies them
//if it needs to perform an action. it returns its own fd
int32_t Connection::query_fd (fd_set& readfds, fd_set& writefds,
        fd_set& errfds)
{
        _selectRequest = 0;
        if (_isConnected) {
                //we always try to recv
                FD_SET (_connsock, &readfds);

                //we only want to write msgQueue is not empty
                if (!_putQueue.empty () || _writeBuffer) {
                        Timer timer;
                        struct timeval tv;
                        if (timer.gettimeofday (&tv, NULL)) {
                                printf ("connection error: gettimeofday\n");
                        }

                        int32_t sdiff = tv.tv_sec - _lastSend.tv_sec;
                        int32_t udiff = tv.tv_usec - _lastSend.tv_usec;

                        int64_t mdiff = sdiff * 1000 + udiff / 1000;
                        /* TODO look into mdiff == 0 */
                        if (mdiff >= SEND_RATE) {
                                FD_SET (_connsock, &writefds);
                        } else {
                                _selectRequest = SEND_RATE - mdiff;
                        }
                }

                //we should probably know about errors as well
                FD_SET (_connsock, &errfds);

                //printf ("quer %d %d %d %d\n", _connsock,
                //        FD_ISSET (_connsock, &readfds),
                //        FD_ISSET (_connsock, &writefds),
                //        FD_ISSET (_connsock, &errfds));
        }
        return _connsock;
}

//this function takes the result of select and responds accordingly
void Connection::tell_fd (fd_set& readfds, fd_set& writefds,
        fd_set& errfds)
{
        //printf ("tell %d %d %d %d\n", _connsock,
        //        FD_ISSET (_connsock, &readfds),
        //        FD_ISSET (_connsock, &writefds),
        //        FD_ISSET (_connsock, &errfds));
        if (FD_ISSET (_connsock, &readfds)) {
                NetworkMessage* msg = _getMsg ();
                if (msg != NULL) {
                        _getQueue.push_back (msg);
                }
        }
        if (FD_ISSET (_connsock, &writefds)) {
                if (!_putQueue.empty () || _writeBuffer) {
                        _putMsg ();
                }
        }
}
                
#endif

bool Connection::connectTo (const char* host, uint16_t port)
{
        struct hostent* he = gethostbyname (host);
        if (he == NULL) {
#ifdef WIN32
                printf ("gethostbyname error: %d\n", WSAGetLastError ());
#else
                herror ("gethostbyname error");
#endif
                return false;
        } else {
                struct sockaddr_in hostaddr;
                memcpy (&hostaddr.sin_addr, he->h_addr_list[0], he->h_length);
                hostaddr.sin_port = htons (port);
                hostaddr.sin_family = AF_INET;
                return _connectTo (hostaddr);
        }
}

bool Connection::connectTo (uint32_t ip, uint16_t port)
{
        struct sockaddr_in hostaddr;
        hostaddr.sin_addr.s_addr = ip;
        hostaddr.sin_port = htons (port);
        hostaddr.sin_family = AF_INET;
        return _connectTo (hostaddr);
}

bool Connection::_connectTo (struct sockaddr_in hostaddr)
{
        if ((_connsock = socket (AF_INET, SOCK_STREAM, NULL)) == -1) {
                NETWORK_ERROR ("socket error");
                return false;
        }
        uint32_t sin_size = sizeof (hostaddr);
        if (connect (_connsock, (struct sockaddr*)&hostaddr, sin_size) == -1) {
                NETWORK_ERROR ("connect error");
                return false;
        }
#ifdef WIN32
        u_long notzero = 1;
        if (ioctlsocket (_connsock, FIONBIO, &notzero) == SOCKET_ERROR) {
                NETWORK_ERROR ("iocntlsocket error");
                return false;
        }
#else
        if (fcntl (_connsock, F_SETFL, O_NONBLOCK) == -1) {
                NETWORK_ERROR ("fcntl error");
                return false;
        }
#endif
        _isConnected = true;
        return true;
}

#ifdef WIN32
void Connection::setSocket (SOCKET socket)
{
        //it is the responsibility of the calling function to ensure
        //socket is connected
        _isConnected = true;
        _connsock = socket;
}
#else
void Connection::setSocket (int32_t socket)
{
        //it is the responsibility of the calling function to ensure
        //socket is connected
        _isConnected = true;
        _connsock = socket;
}
#endif

void Connection::setInfo (uint32_t ip, uint16_t port)
{
        _connIP = ip;
        _connport = port;
}

