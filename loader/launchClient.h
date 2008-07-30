#ifndef LAUNCH_CLIENT_GUARD
#define LAUNCH_CLIENT_GUARD

#include <sys/types.h>

class ClientLauncher {
	public:
		pid_t launch(const char* workingDirectory, const char* name);
};

#endif

