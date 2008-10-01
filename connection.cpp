#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>

#include "connection.h"
#include "networkmessage.h"

Connection::Connection ()
{
        _isConnected = false;
        readPos = 0;
        writePos = 0;
        readBuffer = NULL;
        writeBuffer = NULL;
        writeMsg = NULL;
}

Connection::~Connection ()
{
        //close connection and delete all pending messages
}

bool Connection::isConnected ()
{
        return _isConnected;
}

NetworkMessage* Connection::getMsg ()
{
        if (!getQueue.empty ()) {
                NetworkMessage* ret = getQueue.front ();
                getQueue.pop_front ();
                return ret;
        } else {
                return NULL;
        }
}

void Connection::putMsg (NetworkMessage* msg)
{
        putQueue.push_back (msg);
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
        int32_t n = recv (connsock, buf, len, 0);
        if (n == -1) {
                //this could be an error, or it could just be EAGAIN
                if (errno != EAGAIN) {
                        perror ("recv error get");
                }
        }
        return n;
}

int32_t Connection::_peek (void* buf, int32_t len)
{
        int32_t n = recv (connsock, buf, len, MSG_PEEK);
        if (n == -1) {
                //this could be an error, or it could just be EAGAIN
                if (errno != EAGAIN) {
                        perror ("recv error peek");
                }
        }
        return n;
}

int32_t Connection::_put (void* buf, int32_t len)
{
        int32_t n = send (connsock, buf, len, 0);
        if (n == -1) {
                //this could be an error, or it could just be EAGAIN
                if (errno != EAGAIN) {
                        perror ("send error");
                }
        }
        return n;
}

int32_t Connection::_close ()
{
        int32_t n = close (connsock);
        if (n != 0) {
                perror ("close error");
        }
        return n;
}

NetworkMessage* Connection::_getMsg ()
{
        //printf ("starting read\n");
        if (readPos == 0) {
                //printf ("reading header\n");
                uint16_t msgSize;
                int32_t n = _peek (&msgSize, 2);
                //printf ("peeked at %d\n", n);
                if (n == 0) {
                        closeConnection ();
                } else if (n == 2) {
                        //we can read the header and allocate the buffer
                        readLen = msgSize + 2;
                        readBuffer = new uint8_t[readLen];
                        if (_get (readBuffer, 2) == 2) {
                                readPos += 2;
                                return _getMsg ();
                        } else {
                                //in theory we should never get here
                                delete[] readBuffer;
                                readBuffer = NULL;
                                printf ("recv error: This shouldnt happen");
                                printf ("please inform us if it does\n");
                        }
                }
        } else {
                int16_t n = _get (&readBuffer[readPos], readLen - readPos);
                if (n == 0) {
                        closeConnection ();
                } else if (n != -1) {
                        readPos += n;
                }
                //printf ("partial read %d, readpos = %d, readlen = %d\n", n, readPos, readLen);
                if (readPos == readLen) {
                        NetworkMessage* ret;
                        ret = new NetworkMessage (readLen, readBuffer);
                        readPos = 0;
                        readBuffer = NULL;
                        return ret;
                }
        }
        return NULL;
}

void Connection::_putMsg ()
{
        if (writeMsg == NULL) {
                if (!putQueue.empty ()) {
                        writeMsg = putQueue.front ();
                        putQueue.pop_front ();

                        writeBuffer = writeMsg->getBuffer ();
                        writeLen = *(uint16_t*)writeBuffer + 2;
                        writePos = 0;
                        _putMsg ();
                }
        /*} else if (writeLen == writePos) {
                delete writeMsg;
                writeMsg = NULL;
                writePos = 0;
                writeBuffer = NULL;*/
        } else {
                int16_t n = _put (&writeBuffer[writePos], writeLen - writePos);
                if (n != -1) {
                        writePos += n;
                        if (writeLen == writePos) {
                                delete writeMsg;
                                writeMsg = NULL;
                                writePos = 0;
                                writeBuffer = NULL;
                        }
                }
        }
}

#ifdef WIN32
#else
//Ive got no idea how this is going to be implemented under windows
//but im going to leave that problem for another day

//this function takes a read write and error fd_set and modifies them
//if it needs to perform an action. it returns its own fd
int32_t Connection::query_fd (fd_set& readfds, fd_set& writefds,
        fd_set& errfds)
{
        if (_isConnected) {
                //we always try to recv
                FD_SET (connsock, &readfds);

                //we only want to write msgQueue is not empty
                if (!putQueue.empty ()) {
                        FD_SET (connsock, &writefds);
                }

                //we should probably know about errors as well
                FD_SET (connsock, &errfds);

                //printf ("quer %d %d %d %d\n", connsock,
                //        FD_ISSET (connsock, &readfds),
                //        FD_ISSET (connsock, &writefds),
                //        FD_ISSET (connsock, &errfds));
        }
        return connsock;
}

//this function takes the result of select and responds accordingly
void Connection::tell_fd (fd_set& readfds, fd_set& writefds,
        fd_set& errfds)
{
        //printf ("tell %d %d %d %d\n", connsock,
        //        FD_ISSET (connsock, &readfds),
        //        FD_ISSET (connsock, &writefds),
        //        FD_ISSET (connsock, &errfds));
        if (FD_ISSET (connsock, &readfds)) {
                NetworkMessage* msg = _getMsg ();
                if (msg != NULL) {
                        getQueue.push_back (msg);
                }
        }
        if (FD_ISSET (connsock, &writefds)) {
                if (!putQueue.empty ()) {
                        _putMsg ();
                }
        }
}
                
#endif

bool Connection::connectTo (const char* host, uint16_t port)
{
        struct hostent* he = gethostbyname (host);
        if (he == NULL) {
                herror ("gethostbyname error");
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
        if ((connsock = socket (AF_INET, SOCK_STREAM, NULL)) == -1) {
                perror ("socket error");
                return false;
        }
        uint32_t sin_size = sizeof (hostaddr);
        if (connect (connsock, (struct sockaddr*)&hostaddr, sin_size) == -1) {
                perror ("connect error");
                return false;
        }
        if (fcntl (connsock, F_SETFL, O_NONBLOCK) == -1) {
                perror ("fcntl error");
                return false;
        }
        _isConnected = true;
        return true;
}

#ifdef WIN32
#else
void Connection::setSocket (int32_t socket)
{
        //it is the responsibility of the calling function to ensure
        //socket is connected
        _isConnected = true;
        connsock = socket;
}
#endif

void Connection::setInfo (uint32_t ip, uint16_t port)
{
        connIP = ip;
        connport = port;
}

