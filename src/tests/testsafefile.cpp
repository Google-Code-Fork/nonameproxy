#include <stdio.h>
#include "safefile.h"

int main (int argc, char** argv)
{
        //output some stuff
        char test[] = "abcdefghijklmnopqrstuvwxyz";

        SafeFile* f = new SafeFile ("test.txt", SafeFile::m_write_b);

        f->write (test, 26);
        f->close ();

        char test2[27];
        test2[26] = 0;
        f->open ("test.txt", SafeFile::m_read_b);
        f->read (test2, 26);
        printf ("%s\n", test2);

        delete f;
        return 0;
}

