#include "adler32.h"

#define MOD_ADLER 65521

//copy/pasted from wikipedia
uint32_t Adler32::checksum (uint8_t* buffer, uint32_t len)
{
        uint32_t a = 1, b = 0;
 
        while (len > 0) 
        {
                uint32_t tlen = len > 5552 ? 5552 : len;
                len -= tlen;
                do 
                {
                        a += *buffer++;
                        b += a;
                } while (--tlen);
 
                a %= MOD_ADLER;
                b %= MOD_ADLER;
        }
 
        return (b << 16) | a;
 
}

