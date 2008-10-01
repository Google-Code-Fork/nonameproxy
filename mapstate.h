#ifndef __MAP_STATE_H
#define __MAP_STATE_H

#include <string>
#include <stdint.h>

class Pos
{
        public:
                Pos ();
                Pos (uint32_t xx, uint32_t yy, uint32_t zz);

                uint32_t x;
                uint32_t y;
                uint32_t z;
};

class MapState
{
        public:
                Pos& getCurPos ();
        private:
                Pos _curPos;
};
#endif

