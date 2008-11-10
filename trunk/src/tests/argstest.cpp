#include <stdio.h>
#include "messenger.h"

class Client;

int main (int argc, char** argv)
{
        Client* c = NULL;
        ArgsParser ap (argv[1], c);
        Args args = ap.getArgs ();
        for (Args::iterator i = args.begin (); i != args.end (); ++ i) {
                printf ("%s\n", (*i).c_str ());
        }
        return 0;
}

        
