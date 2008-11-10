#include "plugin.h"

#include <string>

int main (int argc, char** argv)
{
        Plugin p (1);
        std::string path ("./plugins/dummy/dummy.so");
        
        p.load (path, (Client*)NULL);
        return 0;
}

