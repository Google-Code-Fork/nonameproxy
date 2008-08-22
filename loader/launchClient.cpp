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
#define RSA_ADDR 0x84B5240
#define LOGIN_BASE_ADDR 0x86A7D60
#define PORT_BASE_ADDR (LOGIN_BASE_ADDR + 0x64)
#define LOGIN_OFFSET 0x70

pid_t ClientLauncher::launch(const char* workingDirectory, const char* target) {
	pid_t child;
	child = fork();

	if(child == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		chdir(workingDirectory);
		execl(target, NULL);
	} else {
		//after 2 syscalls client is ready for modification
		for(int sysCalls = 0; sysCalls < 2; sysCalls ++) {
			wait(NULL);
			ptrace(PTRACE_CONT, child, NULL, NULL);
		}
		wait(NULL);
		char localHost[] = "127.0.0.1";
		char rsakey[] = "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";
		u_short loginPort = 1337;
		MemUtil injector(child);
		injector.writeMem(rsakey, RSA_ADDR, 310);
		for(int i = 0; i < 10; i ++) {
			injector.writeMem(localHost, LOGIN_BASE_ADDR + i * LOGIN_OFFSET, 10);
			injector.writeMem(&loginPort, PORT_BASE_ADDR + i * LOGIN_OFFSET, 2); 
		}
		ptrace(PTRACE_DETACH, child, NULL, NULL);
	}
	return child;
}
		
