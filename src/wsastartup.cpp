#include "wsastartup.h"

#ifdef WIN32

#include <stdio.h>
#include <winsock2.h>

int32_t iWSAStartup ()
{
        /* Winsock init */
        WORD wVersionRequested;
        WSADATA wsaData;

        /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
        wVersionRequested = MAKEWORD(2, 0);

        int32_t err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
                printf("WSAStartup failed with error: %d\n", err);
                return 1;
        }
        return err;
}

#else

int32_t iWSAStartup ()
{
        return 0;
}

#endif

