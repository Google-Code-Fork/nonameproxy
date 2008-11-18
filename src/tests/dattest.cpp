#include <stdio.h>
#include "datreader.h"

int main ()
{
        DatReader* dat = new DatReader ();

        uint32_t itemid;
        while (scanf ("%d", &itemid)) {
                dat->getItemData (itemid).show ();
        }
        delete dat;
        return 0;
}

