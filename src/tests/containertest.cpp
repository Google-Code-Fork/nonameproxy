#include <stdio.h>
#include "inventorystate.h"

int main ()
{
        InventoryState inventory;
        Container& c = inventory.getContainer (1);
        
        c.open (10, 1234, false, "name");
        c.show ();

        c.addThing (Item (1));
        c.addThing (Item (2));
        c.show ();

        c.insertThing (Item (3), 0);
        c.insertThing (Item (4), 3);
        c.insertThing (Item (5), 2);
        c.show ();

        c.removeThing (0);
        c.show ();
        c.removeThing (3);
        c.show ();
        c.removeThing (1);
        c.show ();

        c.close ();
        c.show ();


        c.open (10, 1234, false, "name");
        c.insertThing (Item (6), 0);
        c.show ();

        return 0;
}

