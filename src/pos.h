#ifndef __POS_H
#define __POS_H

#include <stdint.h>

#define POS_2_INDEX(p) ((uint64_t)p.x << 32 | \
                        (uint64_t)p.y << 16 | \
                        (uint64_t)p.z)

class Pos
{
        public:
                Pos ();
                Pos (uint32_t _x, uint32_t _y, uint32_t _z);

                uint32_t x;
                uint32_t y;
                uint32_t z;

                const bool operator== (const Pos& a) const;
                const bool operator!= (const Pos& a) const;
};

#endif

