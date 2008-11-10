#include <stdio.h>
#include "mapstate.h"
#include "datreader.h"

int main ()
{
        DatReader dat;
        
        MapState map;

        Tile& tile = map.getTile (0,0,0);
        
        Outfit_t o;
        printf ("a\n");
        tile.insertThing (Item (408), 0, &dat);
        printf ("b\n");
        tile.insertThing (Item (4080), 1, &dat);
        printf ("%d\n", tile.removeThing (1));

        tile.show ();
        return 0;
}

