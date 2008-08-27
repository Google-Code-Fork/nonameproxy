#include <stdio.h>
#include "datreader.h"

int main ()
{
        DatReader* dat = new DatReader ();
        for (int i = 0; i < dat->getNIds (); i ++) {
                dat->getItemData (i + 100)->show ();
        }
        delete dat;
        return 0;
}

