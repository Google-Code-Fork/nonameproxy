#include <sys/ptrace.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "memUtil.h"

#define min(x,y) ({ \
	const typeof(x) _x = (x);       \
	const typeof(y) _y = (y);       \
	(void) (&_x == &_y);            \
	_x < _y ? _x : _y; })


MemUtil::MemUtil(pid_t pid) {
	p = pid;
}

void MemUtil::readMem(void* dest, u_int address, u_int n) {
	u_int bufferPos = 0;
	u_int offset = 0;
	u_int start = address - address % 4;
	
	//copy the first oddlot of bytes into the buffer
	u_int temp = ptrace(PTRACE_PEEKDATA, p, start, NULL);
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
		*((u_int*)((u_int)dest + bufferPos)) = ptrace(PTRACE_PEEKDATA, p, start + offset, NULL);
		bufferPos += 4;
		offset += 4;
	}

	//finally copy the last oddlot of data
	if(bufferPos < n) {
		temp = ptrace(PTRACE_PEEKDATA, p, start + offset, NULL);
		memcpy((void*)((u_int)dest + bufferPos), &temp, n - bufferPos);
	}
}

void MemUtil::writeMem(void* source, u_int address, u_int n) {
	u_int bufferPos = 0;
	u_int offset = 0;
	u_int start = address - address % 4;
	//write the first oddlot of bytes to the process
	//its a bit more tricky this time because if we are
	//writing an oddlot we have to be careful not to destroy
	//the surounding bytes
	u_int temp = ptrace(PTRACE_PEEKDATA, p, start, NULL);
	if (address - start + n < 4) {
		memcpy((void*)((u_int)&temp + address - start), source, n);
		bufferPos += n;
	} else {
		memcpy((void*)((u_int)&temp + address - start), source, 4 - (address - start));
		bufferPos += 4 - (address - start);
	}
	ptrace(PTRACE_POKEDATA, p, start, temp);
	offset += 4;

	//copy the middle bulk of the data into the buffer
	while(bufferPos < n - n % 4) {
		ptrace(PTRACE_POKEDATA, p, start + offset, *((u_int*)((u_int)source + bufferPos)));
		bufferPos += 4;
		offset += 4;
	}

	//finally copy the last oddlot of data
	if(bufferPos < n) {
		temp = ptrace(PTRACE_PEEKDATA, p, start + offset, NULL);
		memcpy(&temp, (void*)((u_int)source + bufferPos), n - bufferPos);
		ptrace(PTRACE_POKEDATA, p, start + offset, temp);
	}
}

