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

#include <stdint.h>

#include "server.h"
#include "connection.h"
#include "client.h"
#include "loginstate.h"

#include "wsastartup.h"

int main (int32_t argc, char** argv)
{
        /* does nothing under non-windows */
        iWSAStartup ();

        Client* test;

        Server* loginServer = new Server ();
        loginServer->listenOn (1337);
        Connection* clientConn = loginServer->acceptConnection ();

        LoginState* ls = new LoginState ();

        test = new Client (ls);
        test->runLogin (clientConn);
        delete test;
        
        Server* gameServer = new Server ();
        gameServer->listenOn (7331);
        clientConn = gameServer->acceptConnection ();

        test = new Client (ls);
        test->runGame (clientConn);
        delete test;

        return 0;
}

