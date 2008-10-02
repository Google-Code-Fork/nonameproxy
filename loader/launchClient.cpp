#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

#include "launchClient.h"
#include "memUtil.h"


//tibia 8.11
//#define RSA_ADDR 0x8483280
//#define LOGIN_BASE_ADDR 0x866ED40
//tibia 8.21
//#define RSA_ADDR 0x84B2C00
//#define LOGIN_BASE_ADDR 0x86A4540
//tibia 8.22
//#define MC_ADDR 0x804F427
//#define RSA_ADDR 0x84B5240
//#define LOGIN_BASE_ADDR 0x86A7D60
//#define PORT_BASE_ADDR (LOGIN_BASE_ADDR + 0x64)
//#define LOGIN_OFFSET 0x70
//tibia 8.30
//#define MC_ADDR 0x804F427
//#define RSA_ADDR 0x84BF3A0
//#define LOGIN_BASE_ADDR 0x86B2D80
//#define PORT_BASE_ADDR (LOGIN_BASE_ADDR + 0x64)
//#define LOGIN_OFFSET 0x70
//tibia 8.31
#define MC_ADDR 0x804F427
#define RSA_ADDR 0x84C0E40
#define LOGIN_BASE_ADDR 0x86B4D80
#define PORT_BASE_ADDR (LOGIN_BASE_ADDR + 0x64)
#define LOGIN_OFFSET 0x70

char rsakey[] = "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";

pid_t ClientLauncher::launch (const char* host, uint16_t loginPort, bool mc, bool ot)
{
        pid_t tibia_pid;
        tibia_pid = fork();

        if (tibia_pid < 0) {
                printf("Failed to fork the process!\n");
                return 0;
        }
        if (tibia_pid == 0) {
                ptrace(PTRACE_TRACEME, 0, NULL, NULL);
                execl("Tibia", NULL, NULL);
                printf("Couldn't find Tibia executable!\n");
        } else {
                wait(NULL);

                MemUtil injector(tibia_pid);

                if (mc) {
                        //there is something strange going on, if there are
                        //two calls to peek in a row i get ESRCH
                        printf ("Injecting MC\n");
                        uint8_t jge = 0x7D;
                        injector.writeMem (&jge, MC_ADDR, 1);
                }

                /* there is a slight annoyance here, if I try write the
                 * addresses now it will fail, but if i wait on 2 system
                 * calls as i previously did i will break the timer, the
                 * solution seems to be to detatch from the process,
                 * wait a second, and then reattatch */
                if (ot || host != NULL) {
                        printf ("Detatching from process\n");
                        ptrace(PTRACE_DETACH, tibia_pid, NULL, NULL);
                        uint32_t waitTime = 3;
                        for (uint32_t i = 0; i < waitTime; i ++) {
                                printf ("Reattatching in %d seconds...\n", waitTime - i); 
                                sleep (1);
                        }
                        printf ("Reattatching to process\n");
                        if (ptrace(PTRACE_ATTACH, tibia_pid, NULL, NULL) != 0) {
                                printf ("Reattatching failed, aborting\n");
                                return 0;
                        }
                        sleep (1);

                        if (host != NULL) {
                                printf ("Writing LoginServers\n");
                                for(uint32_t i = 0; i < 10; i ++) {
                                        injector.writeMem (host,
                                                LOGIN_BASE_ADDR + i * LOGIN_OFFSET, 10);
                                        injector.writeMem (&loginPort,
                                                PORT_BASE_ADDR + i * LOGIN_OFFSET, 2);
                                }
                        }

                        if (ot) {
                                printf ("Writing OT key\n");
                                injector.writeMem (rsakey, RSA_ADDR, 310);
                        }

                        printf ("Detatching from process\n");
                        ptrace(PTRACE_DETACH, tibia_pid, NULL, NULL);
                        printf ("Client successfully loaded\n");
                }
        }
        return 0;
}

