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

#include <stdio.h>
#include "connectionmanager.h"

#define MAX(x,y) (x) > (y) ? (x) : (y)

ConnectionManager::ConnectionManager ()
{
        ids = new IdManager (100);
}

ConnectionManager::~ConnectionManager ()
{
        ConnectionList::iterator it;

        for (it = connections.begin (); it != connections.end (); ++ it)
        {
                delete (*it).second;
        }

        delete ids;
}

uint32_t ConnectionManager::addConnection (Connection* connection)
{
        if (!connection) {
                printf ("warning: attempt to assign NULL connectin\n");
                return -1;
        }

        uint32_t cid = ids->newId ();
        connections.insert (std::pair<uint32_t, Connection*> (cid, connection));
        return cid;
}

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
                new_fd = (*c).second->query_fd (readfds, writefds, errfds);
                max_fd = MAX (new_fd, max_fd);
        }

        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        select (max_fd + 1, &readfds, &writefds, &errfds, &tv);

        for (ConnectionList::iterator c = connections.begin();
                c != connections.end(); ++c) {
                (*c).second->tell_fd (readfds, writefds, errfds);
        }
}

void ConnectionManager::deleteConnection (uint32_t cid)
{
        ConnectionList::iterator i = connections.find (cid);
        if (i == connections.end ()) {
                printf ("connectionmanager error: delete: non-existant\
                         connection id\n");
                return;
        }
        delete (*i).second;
        connections.erase (cid);
        ids->recycleId (cid);
}

