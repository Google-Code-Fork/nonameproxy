#ifndef __ADLER_32_H
#define __ADLER_32_H

#include <stdint.h>

class Adler32
{
        public:
                uint32_t checksum (uint8_t* buffer, uint32_t len);
};

#endif

