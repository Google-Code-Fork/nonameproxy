#ifndef __CONNECTION_MANAGER_H
#define __CONNECTION_MANAGER_H

#include <stdint.h>
#include <list>

class Connection;

typedef std::list<Connection*> ConnectionList;

class ConnectionManager
{
        public:
                void addConnection (Connection* connection);
                void closeConnections ();
                //informs connections if their file descripters become
                //active within timeout milliseconds;
                void selectConnections (uint32_t timeout);
        private:
                ConnectionList connections;
};

#endif
        
