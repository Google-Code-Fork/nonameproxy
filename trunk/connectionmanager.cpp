#include "connectionmanager.h"
#include "connection.h"

#define MAX(x,y) (x) > (y) ? (x) : (y)

void ConnectionManager::addConnection (Connection* connection)
{
        connections.push_back (connection);
}

void ConnectionManager::closeConnections ()
{
}

#ifdef WIN32
#else
void ConnectionManager::selectConnections (uint32_t timeout)
{
        timeval tv;

        fd_set readfds, writefds, errfds; 
        FD_ZERO (&readfds);
        FD_ZERO (&writefds);
        FD_ZERO (&errfds);

        int32_t max_fd = 0;
        int32_t new_fd;
        

        for (ConnectionList::iterator c = connections.begin();
                c != connections.end(); ++c) {
                new_fd = (*c)->query_fd (readfds, writefds, errfds);
                max_fd = MAX (new_fd, max_fd);
        }

        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        select (max_fd + 1, &readfds, &writefds, &errfds, &tv);

        for (ConnectionList::iterator c = connections.begin();
                c != connections.end(); ++c) {
                (*c)->tell_fd (readfds, writefds, errfds);
        }
}
#endif

