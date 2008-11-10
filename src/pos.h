#ifndef __POS_H
#define __POS_H

#include <stdint.h>

class Pos
{
        public:
                Pos ();
                Pos (uint32_t _x, uint32_t _y, uint32_t _z);

                uint32_t x;
                uint32_t y;
                uint32_t z;
};

#endif

