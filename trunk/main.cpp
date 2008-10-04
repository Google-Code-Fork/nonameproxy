#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>

#include "server.h"
#include "connection.h"
#include "client.h"
#include "loginstate.h"

int main (uint32_t argc, char** argv)
{
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

