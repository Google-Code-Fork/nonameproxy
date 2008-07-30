#ifndef MEMUTILS_H_GAURD
#define MEMUTILS_H_GAURD

#include <sys/types.h>

class MemUtil {
	public:
		MemUtil(pid_t pid);

		void readMem(void* dest, u_int address, u_int n);
		void writeMem(void* source, u_int address, u_int n);
	private:
		pid_t p;
};

#endif

