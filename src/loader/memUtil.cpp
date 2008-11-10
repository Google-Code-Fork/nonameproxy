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

bool MemUtil::readMem(void* dest, u_int address, u_int n) {
	u_int bufferPos = 0;
	u_int offset = 0;
	u_int start = address - address % 4;
	
	//copy the first oddlot of bytes into the buffer
        errno = 0;
	int temp = ptrace(PTRACE_PEEKDATA, p, start, NULL);
        if (temp == -1) {
                if (errno != 0) {
                        perror ("ptrace (PEEK):");
                        return false;
                }
        }
                
	memcpy(dest, (void*)((u_int)&temp + address - start), min(n, address - start));
	if (address - start + n < 4) {
		memcpy(dest, (void*)((u_int)&temp + address - start), n);
		bufferPos += n;
	} else {
		memcpy(dest, (void*)((u_int)&temp + address - start), 4 - (address - start));
		bufferPos += 4 - (address - start);
	}
	offset += 4;

	//copy the middle bulk of the data into the buffer
	while(bufferPos < n - n % 4) {
                errno = 0;
		*((u_int*)((u_int)dest + bufferPos)) = ptrace(PTRACE_PEEKDATA, p, start + offset, NULL);
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
		memcpy((void*)((u_int)dest + bufferPos), &temp, n - bufferPos);
	}
        return true;
}

bool MemUtil::writeMem(const void* source, u_int address, u_int n) {
	u_int bufferPos = 0;
	u_int offset = 0;
	u_int start = address - address % 4;
	//write the first oddlot of bytes to the process
	//its a bit more tricky this time because if we are
	//writing an oddlot we have to be careful not to destroy
	//the surounding bytes
        errno = 0;
	int temp = ptrace(PTRACE_PEEKDATA, p, start, NULL);
        if (temp == -1) {
                if (errno != 0) {
                        perror ("ptrace (PEEK):");
                        return false;
                }
        }
	if (address - start + n < 4) {
		memcpy((void*)((u_int)&temp + address - start), source, n);
		bufferPos += n;
	} else {
		memcpy((void*)((u_int)&temp + address - start), source, 4 - (address - start));
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
                        start + offset, *((u_int*)((u_int)source + bufferPos))) == -1) 
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
		memcpy(&temp, (void*)((u_int)source + bufferPos), n - bufferPos);
		if (ptrace(PTRACE_POKEDATA, p, start + offset, temp) == -1) {
                        perror ("ptrace (POKE):");
                        return false;
                }
	}
        return true;
}

