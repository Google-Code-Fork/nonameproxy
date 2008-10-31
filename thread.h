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

#ifndef __THREAD_H
#define __THREAD_H

#include <stdint.h>
#include <pthread.h>

class Thread;
class Mutex;

enum MutexError
{
        MUTEX_NO_ERROR = 0,
        MUTEX_INVALID,
        MUTEX_DEAD_LOCK,
        MUTEX_BUSY,
        MUTEX_UNLOCKED,
        MUTEX_MISC_ERROR
};

enum MutexKind
{
        MUTEX_NORMAL, //recursive = deadlock, unlocking unowned = deadlock
        MUTEX_ERRORCHECK, //recursive = error, unlocking unowned = error 
        MUTEX_RECURSIVE, //recursive = ok, unlocking unowned = undefined 
        MUTEX_DEFAULT //recursive = undefined, unlocking unowned = undefined 
};

enum ThreadError
{
        THREAD_NO_ERROR = 0,
        THREAD_NO_RESOURCE,
        THREAD_RUNNING,
        THREAD_NOT_RUNNING,
        THREAD_KILLED,
        THREAD_MULTI_JOINS,
        THREAD_MISC_ERROR
};

//a joinable thread can be joined to get a return value
//a detatchable thread deletes itself after termination,
//and therefore must be allocated on the heap
enum ThreadKind
{
        THREAD_DETATCHABLE,
        THREAD_JOINABLE
};

enum ThreadState
{
        STATE_NEW,
        STATE_RUNNING,
        STATE_EXITED
};

class Mutex
{
        public:
                Mutex (MutexKind kind = MUTEX_DEFAULT);
                MutexError lock ();
                MutexError tryLock ();
                MutexError unlock ();

                bool isOk () const;
        private:
                pthread_mutex_t mutex;
                bool _isOk;
};

class Thread
{
        public:
                typedef void* ExitCode;

                //initializes the thread as either detatched or joinable
                Thread (ThreadKind kind = THREAD_DETATCHABLE);

                //don't use this
                virtual ~Thread ();
                
                //give the remainder of the thread's time slice back to the OS
                static void yield ();
                //suspend thread for useconds microseconds
                static void uSleep (uint32_t useconds);
                //suspend thread for seconds seconds
                static void sSleep (uint32_t seconds);


                //creates and runs the thread. Optionally the
                //stack size can be explicitly specified
                ThreadError create (uint32_t stackSize = 0);

                //the nice way to kill a thread. a subsequent call to
                //testDestroy () by the thread will return true. It is
                //the programmers responsibility to ensure that the thread
                //exits as soon as possible
                //
                //if the thread is detatched its memory will
                //automatically be cleaned up.
                //
                //If the thread is joinable it will be stopped and the exit
                //code will be written to rc, unless rc is NULL
                //Afterwards it is the programmers responsibility to free
                //up the memory used by the thread (ie call delete)
                ThreadError destroy (ExitCode* rc = (ExitCode*)NULL);

                //waits until thread terminates and returns its exit code
                ExitCode join ();

                //the messy way to terminate a thread. a detatchable thread will
                //delete itself, a joinable thread must be deleted manually
                //node onExit will not be called
                ThreadError kill ();

                //returns true if the thread is running
                bool isRunning () const;

                //returns true if destroy has been previously called
                bool testDestroy ();
        protected:
        //these functions should be overridden to provide the functionality
        //of the thread
                //onEntry is called from the new thread after a call to run ()
                virtual void onEntry () = 0;
                //onExit is called when the thread is destroyed
                virtual void onExit () = 0;

        private:
                pthread_t       _threadId;
                ThreadState     _state;

                //flag is set by destroy
                bool _cancelled;
                //because destroy can be called from multiple threads we also
                //need a mutex to protect the flag
                Mutex _cancelMutex;

                //somewhere to store the exit code
                Thread::ExitCode _exitCode;

                //it is an error for multiple threads to attempt to join a
                //single thread. wx does something a bit dodgy, it allows
                //multiple threads to sit on a conditional variable until a
                //single thread joins, and then returns the exit code to all
                //threads. I will be adhearing more to the true nature of 
                //join, and multiple calls to join will return an error. I
                //still need a flag to record if join has been called or not
                bool _isJoining;
                bool _isDetatched;
};
#endif

