#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>

#include "server.h"
#include "connection.h"
#include "client.h"
#include "loginstate.h"

int main (uint32_t argc, char** argv)
{
        Server* loginServer = new Server ();
        loginServer->listenOn (1337);
        Connection* clientConn = loginServer->acceptConnection ();

        LoginState* ls = new LoginState ();

        Client test (ls);
        test.runLogin (clientConn);

        return 0;
}

