#include <errno.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <string.h>
#include "memUtil.h"

#define min(x,y) ({ \
	const typeof(x) _x = (x);       \
	const typeof(y) _y = (y);       \
	(void) (&_x == &_y);            \
	_x < _y ? _x : _y; })


MemUtil::MemUtil(pid_t pid) {
	p = pid;
}

bool MemUtil::readMem(void *dest, uint32_t address, uint32_t n) {
	uint32_t bufferPos = 0;
	uint32_t offset = 0;
	uint32_t start = address - address % 4;
	
	//copy the first oddlot of bytes into the buffer
        errno = 0;
	int32_t temp = ptrace(PTRACE_PEEKDATA, p, start, NULL);
        if (temp == -1) {
                if (errno != 0) {
                        perror ("ptrace (PEEK):");
                        return false;
                }
        }
                
	memcpy(dest, (void*)((uint8_t *)&temp + address - start), min(n, address - start));
	if (address - start + n < 4) {
		memcpy(dest, (void *)((uint8_t *)&temp + address - start), n);
		bufferPos += n;
	} else {
		memcpy(dest, (void *)((uint8_t *)&temp + address - start), 4 - (address - start));
		bufferPos += 4 - (address - start);
	}
	offset += 4;

	//copy the middle bulk of the data into the buffer
	while(bufferPos < n - n % 4) {
                errno = 0;
		*((uint32_t *)((uint8_t *)dest + bufferPos)) = ptrace(PTRACE_PEEKDATA, p, start + offset, NULL);
                if (temp == -1) {
                        if (errno != 0) {
                                perror ("ptrace (PEEK):");
                                return false;
                        }
                }
		bufferPos += 4;
		offset += 4;
	}

	//finally copy the last oddlot of data
	if(bufferPos < n) {
                errno = 0;
		temp = ptrace(PTRACE_PEEKDATA, p, start + offset, NULL);
                if (temp == -1) {
                        if (errno != 0) {
                                perror ("ptrace (PEEK):");
                                return false;
                        }
                }
		memcpy((void *)((uint8_t *)dest + bufferPos), &temp, n - bufferPos);
	}
        return true;
}

bool MemUtil::writeMem(const void* source, uint32_t address, uint32_t n) {
	uint32_t bufferPos = 0;
	uint32_t offset = 0;
	uint32_t start = address - address % 4;
	//write the first oddlot of bytes to the process
	//its a bit more tricky this time because if we are
	//writing an oddlot we have to be careful not to destroy
	//the surounding bytes
        errno = 0;
	int32_t temp = ptrace(PTRACE_PEEKDATA, p, start, NULL);
        if (temp == -1) {
                if (errno != 0) {
                        perror ("ptrace (PEEK):");
                        return false;
                }
        }
	if (address - start + n < 4) {
		memcpy((void *)((uint8_t *)&temp + address - start), source, n);
		bufferPos += n;
	} else {
		memcpy((void *)((uint8_t *)&temp + address - start), source, 4 - (address - start));
		bufferPos += 4 - (address - start);
	}
	if (ptrace(PTRACE_POKEDATA, p, start, temp) == -1) {
                perror ("ptrace (POKE):");
                return false;
        }

	offset += 4;

	//copy the middle bulk of the data into the buffer
	while(bufferPos < n - n % 4) {
		if (ptrace(PTRACE_POKEDATA, p, 
                        start + offset, *((uint32_t *)((uint8_t *)source + bufferPos))) == -1) 
                { 
                        perror ("ptrace (POKE):");
                        return false;
                }
		bufferPos += 4;
		offset += 4;
	}

	//finally copy the last oddlot of data
	if(bufferPos < n) {
                errno = 0;
		temp = ptrace(PTRACE_PEEKDATA, p, start + offset, NULL);
                if (temp == -1) {
                        if (errno != 0) {
                                perror ("ptrace (PEEK):");
                                return false;
                        }
                }
		memcpy(&temp, (void *)((uint8_t *)source + bufferPos), n - bufferPos);
		if (ptrace(PTRACE_POKEDATA, p, start + offset, temp) == -1) {
                        perror ("ptrace (POKE):");
                        return false;
                }
	}
        return true;
}

