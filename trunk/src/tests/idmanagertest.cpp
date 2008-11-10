#include "idmanager.h"

int main (int argc, char** argv)
{
        IdManager ids (3);

        for (int i = 0; i < 10; i ++) {
                printf ("%d\n", ids.newId ());
        }

        ids.recycleId (2);
        ids.recycleId (3);
        ids.recycleId (5);
        ids.recycleId (7);

        for (int i = 0; i < 10; i ++) {
                printf ("%d\n", ids.newId ());
        }
}




                
