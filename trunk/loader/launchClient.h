#ifndef LAUNCH_CLIENT_GUARD
#define LAUNCH_CLIENT_GUARD

#include <stdint.h>

class ClientLauncher {
	public:
		pid_t launch(const char* host, uint16_t port, bool mc, bool ot);
};

#endif

