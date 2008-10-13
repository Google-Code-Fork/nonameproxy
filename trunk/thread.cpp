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

#include "thread.h"


//to avoid having to #include "pthread.h" in the header, internal variables
//are defined in their own class
class ThreadInternal
{
        public:
                
        private:
                pthread_t _threadId;

                //stores the state of the thread
                ThreadState _state;
                //because several threads may try to access the state we
                //need a mutex to protect it
                Mutex _stateMutex

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
}

ThreadInternal::ThreadInternal (ThreadKind kind = THREAD_DETATCHABLE)
{
        if (kind == THREAD_DETATCHABLE) {
                _isDetatched = true;
        } else {
                _isDetatched = false;
        }
        //_stateMutex.lock ();
        _state = STATE_NEW;
        //_stateMutex.unlock ();
        //_cancelMutex.lock ();
        _cancelled = false;
        //_cancelMutex.unlock ();
}

ThreadError ThreadInternal::create (uint32_t stacksize = 0)
{
        if (_state != NEW) {
                //don't run a thread twice
                return THREAD_RUNNING;
        }

        // set up the threads attributes
        pthread_attr_t attr;
        pthread_attr_init (&attr);

        if (stacksize != 0) {
                pthread_attr_setstacksize (&attr, stacksize);
        }
        if (_isDetatched) {
                if (pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED != 0)) {
                        printf ("error: pthread_attr_setdetachstate (DETACHED) failed");
                }
        } //threads are joinable by default, no need for an else

        int32_t rc = pthread_create (_threadId, &attr, 
        
        
        
        
