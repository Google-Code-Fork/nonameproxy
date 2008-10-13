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
        
