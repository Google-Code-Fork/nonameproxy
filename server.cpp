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
        #include <winsock2.h>
#else
        #include <sys/socket.h>
        #include <sys/types.h>
        #include <netinet/in.h>
        #include <errno.h>
        #include <fcntl.h>
#endif

#include <string.h>
#include <stdio.h>
#include "server.h"
#include "connection.h"

#ifdef WIN32
#define NETWORK_ERROR(str) printf (": error no %d\n", WSAGetLastError ());
#else
#define NETWORK_ERROR(str) perror (str);
#endif

Server::Server ()
{
        mastersock = NULL;
}

bool Server::listenOn (uint16_t port)
{
        struct sockaddr_in addr;
        
        if ((mastersock = socket (AF_INET, SOCK_STREAM, NULL)) == -1) {
                NETWORK_ERROR ("socket error");
                return false;
        }

        uint32_t opt = 1;
        if (setsockopt (mastersock, SOL_SOCKET, SO_REUSEADDR,
                (char*)&opt, sizeof (opt)) == -1) {
                NETWORK_ERROR ("setsockopt error");
                return false;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons (port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind (mastersock, (struct sockaddr*)&addr, sizeof (addr)) == -1) {
                NETWORK_ERROR ("bind error");
                return false;
        }

        if (listen (mastersock, 10) == -1) {
                NETWORK_ERROR ("listen error");
                return false;
        }

        return true;
}

Connection* Server::acceptConnection ()
{
        if (mastersock == -1) {
                return NULL;
        }
        Connection* conn = new Connection ();
        struct sockaddr_in connaddr;
        uint32_t sin_size = sizeof (connaddr);
        #ifdef WIN32
        int32_t connsock = accept (mastersock, (struct sockaddr*)&connaddr,
                (int32_t*)&sin_size);
        #else
        int32_t connsock = accept (mastersock, (struct sockaddr*)&connaddr, 
                &sin_size);
        #endif
        if (connsock == -1) {
                NETWORK_ERROR ("accept error");
                return NULL;
        }
        #ifdef WIN32 
        uint32_t notzero = 1;
        if (iocntlsocket (connsock, FIONBIO, &notzero) == SOCKET_ERROR) {
                NETWORK_ERROR ("iocntlsocket error");
                return false;
        }
        #else
        if (fcntl (connsock, F_SETFL, O_NONBLOCK) == -1) {
                NETWORK_ERROR ("fcntl error");
                return NULL;
        }
        #endif

        conn->setSocket (connsock);
        conn->setInfo (connaddr.sin_addr.s_addr, ntohs (connaddr.sin_port));
        return conn;
}

bool Server::shutdown ()
{
        /* TODO */
        return false;
}

