#include "rsa.h"
#include "rsakeys.h"
#include <string.h>
#include <stdint.h>

int main ()
{
        RSA* rsa = new RSA ();
        rsa->setPublicKey (OTKEY, OTMOD);
        rsa->setPrivateKey (TIBKEY, OTMOD);

        uint8_t test[128];
        memset (test, 0, 128);
        strcpy ((char*)test + 1, "abcdefg");

        printf ("%s\n", test + 1);
        rsa->encrypt (test, 128);
        //printf ("%s\n", test);
        rsa->decrypt (test, 128);

        printf ("%s\n", test + 1);
        return 0;
}
