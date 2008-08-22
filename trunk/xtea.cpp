#include <string.h>
#include "xtea.h"


//these are almost the exact functions taken from wikipedia with the type
//signatures changed
void encipher(uint32_t num_rounds, uint32_t* v, uint32_t* k) {
        uint32_t v0=v[0], v1=v[1], i;
        uint32_t sum=0, delta=0x9E3779B9;
        for(i=0; i<num_rounds; i++) {
                v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
                sum += delta;
                v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
        }
        v[0]=v0; v[1]=v1;
}
 
void decipher(uint32_t num_rounds, uint32_t* v, uint32_t* k) {
        uint32_t v0=v[0], v1=v[1], i;
        uint32_t delta=0x9E3779B9, sum=delta*num_rounds;
        for(i=0; i<num_rounds; i++) {
                v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
                sum -= delta;
                v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        }
        v[0]=v0; v[1]=v1;
}

XTEA::XTEA ()
{
        keySet = false;
}

bool XTEA::setKey (const uint32_t* key)
{
        memcpy (&xkey, key, 16);
        keySet = true;
        return true;
}

bool XTEA::encrypt (uint8_t* buffer, uint32_t size)
{
        if (keySet) {
                for (uint32_t i = 0; i < size; i += 8) {
                        encipher (32, (uint32_t*)&buffer[i], xkey);
                }
                return true;
        } else {
                return false;
        }
}


bool XTEA::decrypt (uint8_t* buffer, uint32_t size)
{
        if (keySet) {
                for (uint32_t i = 0; i < size; i += 8) {
                        decipher (32, (uint32_t*)&buffer[i], xkey);
                }
                return true;
        } else {
                return false;
        }
}

