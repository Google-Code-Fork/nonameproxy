#include <stdio.h>
#include <string.h>
#include "rsa.h"
#include "rsakeys.h"

int main() {
        char test[129] = "abcdefghijklmnopqrstuvwxyz";
        test[128] = 0;
        
        RSA crypto;
        crypto.setPublicKey (TIBKEY, OTMOD);
        crypto.setPrivateKey (OTKEY, OTMOD);
        printf ("%s\n", test);

        //for(int i = 0; i < 128; i++) {
        //        printf("%02X ", test[i]);
        //}
        //printf("\n\n");
        crypto.encrypt((uint8_t*)test, 128);
        //for(int i = 0; i < 128; i++) {
        //        printf ("%02X ", test[i]);
        //}
        //printf ("\n\n");
        crypto.decrypt((uint8_t*)test, 128);
        //for(int i = 0; i < 128; i++) {
        //        printf ("%02X ", test[i]);
        //}
        //printf ("\n\n");
        
        //for(int i = 0; i < 128; i++) {
        //        printf("%c ", test[i]);
        //}
        //printf("\n");
        printf("%s\n", test);
        return 0;
}

