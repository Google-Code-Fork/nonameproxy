#ifndef __NONAME_TIMER_H
#define __NONAME_TIMER_H

#ifdef WIN32
        #include <sys/time.h>
#endif
#include <time.h>

#include <stdint.h>

/****************************************************************************
 * This class was made to remove the gettimeofday call used to seed srand ()
 * struct timeval is defined in both sys/time.h and time.h
 ****************************************************************************/

#ifdef WIN32
struct timezone 
{
        int tz_minuteswest; /* minutes W of Greenwich */
        int  tz_dsttime;     /* type of dst correction */
};
#endif

class Timer
{
        public:
                int gettimeofday (struct timeval *tv, struct timezone *tz);

};

#endif

