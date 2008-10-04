#ifndef __CONNECTION_MANAGER_H
#define __CONNECTION_MANAGER_H

#include <stdint.h>
#include <map>

#include "idmanager.h"
#include "connection.h"

typedef std::map<uint32_t, Connection*> ConnectionList;

class ConnectionManager
{
        public:
                ConnectionManager ();
                virtual ~ConnectionManager ();

                uint32_t addConnection (Connection* connection);
                void     deleteConnection (uint32_t cid);

                /* informs connections if their file descripters become
                 * active within timeout milliseconds; */
                void selectConnections (uint32_t timeout);
        private:
                ConnectionList  connections;
                IdManager*      ids;
};

#endif
        
