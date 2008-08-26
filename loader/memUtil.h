#ifndef MEMUTILS_H_GAURD
#define MEMUTILS_H_GAURD

#include <stdint.h>
#include <sys/types.h>

class MemUtil {
	public:
		MemUtil(pid_t pid);

		bool readMem(void* dest, uint32_t address, uint32_t n);
		bool writeMem(const void* source, uint32_t address, uint32_t n);
	private:
		pid_t p;
};

#endif

