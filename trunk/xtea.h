#ifndef __XTEA_H
#define __XTEA_H

#include <stdint.h>

class XTEA {
        public:
                XTEA ();

                bool setKey (const uint32_t* key);

                bool encrypt (uint8_t* buffer, uint32_t size);
                bool decrypt (uint8_t* buffer, uint32_t size);
        private:
                uint32_t xkey[4];
                bool keySet;
};

#endif

