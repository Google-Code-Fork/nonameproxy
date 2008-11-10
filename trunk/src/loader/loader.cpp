#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "launchClient.h"

int main(int argc, char** argv) {

        uint16_t port = 7171;
        char* host = NULL;
        bool mc = false;
        bool ot = false;
        
        if (argc == 1) {
                printf ("usage: %s [-ot | -mc] [HostName] [Port]\n", argv[0]);
                return 0;
        }

        for (int32_t i = 1; i < argc; i ++) {
                if (argv[i][0] == '-') {
                        if (!strcmp ("-mc", argv[i])) {
                                mc = true;
                        } else if (!strcmp ("-ot", argv[i])) {
                                ot = true;
                        } else {
                                printf ("unreconized flag %s\n", argv[i]);
                                return 0;
                        }
                } else if (host == NULL) { //host comes before port
                        if (strlen (argv[i]) >= 100) {
                                printf ("error: host name must be less than \
                                                100 characters long\n");
                                return 0;
                        }
                        host = argv[i];
                } else {
                        port = atoi (argv[i]);
                        if (!(0 < port && port < 65500)) {
                                printf ("error: invalid port\n");
                                return 0;
                        }
                }
        }


	ClientLauncher launcher;
	launcher.launch(host, port, mc, ot);
	return 0;
}

