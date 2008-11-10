#include <stdio.h>
#include <stdint.h>
#include "adler32.h"


//load a packet into a buffer and return its size
int loadpacket (uint8_t** buffer_ptr)
{
        uint32_t s1 = 0, s2 = 0;
        //read the size
        if (scanf ("%X", &s1) == EOF) return 0;
        if (scanf ("%X", &s2) == EOF) {
                printf ("Mal formed packet\n");
                return 0;
        }

        uint32_t size = 2 + s1 + s2 * 256 ;

        *buffer_ptr = new uint8_t[size];
        (*buffer_ptr)[0] = s1;
        (*buffer_ptr)[1] = s2;

        uint32_t i;
        for (i = 2; i < size; i ++) {
                uint32_t tmp;
                if (scanf ("%X", &tmp) == EOF) {
                        printf ("Mal formed packet\n");
                        return 0;
                }
                (*buffer_ptr)[i] = tmp;
                if (i % 16 == 15) {
                        while (getchar () != '\n');
                }
        }
        if (i % 16 != 0) while (getchar () != '\n');

        return (int)size;
}

int main ()
{
        uint8_t* buffer;

        uint32_t size = loadpacket (&buffer);

        Adler32 adler;
        uint32_t checksum = adler.checksum (&buffer[6], size - 6);

        printf ("0x%X\n", checksum);
        return 0;
}


