#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "server.h"
#include "connection.h"

Server::Server ()
{
        mastersock = NULL;
}

bool Server::listenOn (uint16_t port)
{
        struct sockaddr_in addr;
        
        if ((mastersock = socket (AF_INET, SOCK_STREAM, NULL)) == -1) {
                perror ("socket error");
                return false;
        }

        uint32_t opt = 1;
        if (setsockopt (mastersock, SOL_SOCKET, SO_REUSEADDR,
                (char*)&opt, sizeof (opt)) == -1) {
                perror ("setsockopt error");
                return false;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons (port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind (mastersock, (struct sockaddr*)&addr, sizeof (addr)) == -1) {
                perror ("bind error");
                return false;
        }

        if (listen (mastersock, 10) == -1) {
                perror ("listen error");
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
        int32_t connsock = accept (mastersock, (struct sockaddr*)&connaddr, &sin_size);
        if (connsock == -1) {
                perror ("accept error");
                return NULL;
        }
                
        if (fcntl (connsock, F_SETFL, O_NONBLOCK) == -1) {
                perror ("fcntl error");
                return NULL;
        }

        conn->setSocket (connsock);
        conn->setInfo (connaddr.sin_addr.s_addr, ntohs (connaddr.sin_port));
        return conn;
}

bool Server::shutdown ()
{
        return false;
}

