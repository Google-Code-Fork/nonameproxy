/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

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

class Timer
{
        public:
                int gettimeofday (struct timeval *tv, struct timezone *tz);

};

#endif

