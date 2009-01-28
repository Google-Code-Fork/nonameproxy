#ifndef __TUTORIAL1_H
#define __TUTORIAL1_H

#include <string>
#include <stdint.h>

#include "client.h"

/**
 * Note all we need are 3 functions
 * We need to export them as c functions, hence the extern "C"
 */

extern "C"
{
        void load (uint32_t id, Client *client);
        void unload ();
        const std::string &name ();
}

#endif

