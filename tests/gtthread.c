#include "gtthread.h"

/*
Many of these pthread functions either return an error code or return non-zero and set errno 
to an error code when they have an error. These error codes tell the caller what, specifically, went wrong. 
However, you do not need to return proper error codes in your gtthread library; simply returning non-zero 
when an error occurrs is good enough, and you do not ever need to set errno. (It's also okay for a function to 
always return zero if it is written in a way that it cannot ever fail. Error returns are only needed when errors 
occur.)
*/

void gtthread_init(long period){
  init_timer_scheduler(period);
}

/*
Taken from the pthreads linux man

The  gthread_create()  function  starts  a  new thread in the calling process.  The new thread
starts execution by invoking start_routine(); arg is passed as the sole argument of 
start_routine().
The new thread terminates in one of the following ways:

* It  calls  gthread_exit(3),  specifying  an  exit  status value that is available to another
 thread in the same process that calls pthread_join(3).

* It returns from start_routine().  This is equivalent to  calling  pthread_exit(3)  with  the
 value supplied in the return statement.

* It is canceled (see pthread_cancel(3)).

* Any  of  the threads in the process calls exit(3), or the main thread performs a return from
 main().  This causes the termination of all threads in the process.

 Before returning, a successful call to pthread_create() stores the ID of the new thread in the
 buffer pointed to by thread; this identifier is used to refer  to  the  thread  in  subsequent
 calls to other pthreads functions.

The new thread inherits a copy of the creating thread's signal mask (pthread_sigmask(3)).  The
set of pending signals for the new thread is empty (sigpending(2)).  The new thread  does  not
inherit the creating thread's alternate signal stack (sigaltstack(2)).

The new thread inherits the calling thread's floating-point environment (fenv(3)).

The initial value of the new thread's CPU-time clock is 0 (see pthread_getcpuclockid(3)).

The  new  thread inherits copies of the calling thread's capability sets (see capabilities(7))
and CPU affinity mask (see sched_setaffinity(2)).

RETURN VALUE
    On success, pthread_create() returns 0; on error, it returns an error number, and the contents
    of *thread are undefined.
    EAGAIN Insufficient  resources to create another thread, or a system-imposed limit on the 
    number of  threads  was  encountered. 

 On Linux/x86-32, the default stack size for a new thread  is  2  megabytes. 

*/

int  gtthread_create(gtthread_t *thread, void *(*start_routine)(void *), void *arg){
  //Push the node to the linked list
  deactivate_signal();///Deactivate signals to be sure that the node is created correctly
  push_node(start_routine, arg, thread);
  activate_signal();
  return 0;
}
/*

The  gtthread_join()  function  waits for the thread specified by thread to terminate.  If that
thread has already terminated, then pthread_join() returns immediately.  The thread  specified
by thread must be joinable.

If  retval is not NULL, then pthread_join() copies the exit status of the target thread (i.e.,
the value that the target thread supplied to pthread_exit(3)) into the location pointed to  by
*retval.  If the target thread was canceled, then PTHREAD_CANCELED is placed in *retval.
RETURN VALUE
       On success, pthread_join() returns 0; on error, it returns an error number.

ERRORS
       EDEADLK
        A  deadlock  was  detected (e.g., two threads tried to join with each other); or thread
        specifies the calling thread.

       EINVAL Another thread is already waiting to join with this thread.

       ESRCH  No thread with the ID thread could be found.

Failure  to  join  with a thread that is joinable (i.e., one that is not detached), produces a
"zombie thread".  Avoid doing this, since each zombie thread consumes some  system  resources,
and  when  enough zombie threads have accumulated, it will no longer be possible to create new
threads (or processes).

*/

int  gtthread_join(gtthread_t thread, void **status){
  int result = getValue(thread,status);
  while(result == 1){
    ///If the value is not yet ready, yield the processor, 
    ///Given the round-robbin scheduler, the value is not going to change, while we hold 
    ///the processor. So it is better to yield.
    yield();
    result = getValue(thread,status);
  }
  return result;
}
/*
The  pthread_exit() function terminates the calling thread and returns a value via retval that
(if the thread is joinable) is available to another thread in  the  same  process  that  calls
pthread_join(3).

When a thread terminates, process-shared resources (e.g., mutexes, condition variables,  sema‐
phores,  and  file descriptors) are not released, and functions registered using atexit(3) are
not called.

After the last thread in a process terminates, the process terminates as  by  calling  exit(3)
with  an exit status of zero; thus, process-shared resources are released and functions regis‐
tered using atexit(3) are called.

Performing a return from the start function of any thread other than the main  thread  results
in  an implicit call to pthread_exit(), using the function's return value as the thread's exit
status.

To allow other threads to continue execution, the main  thread  should  terminate  by  calling
pthread_exit() rather than exit(3).

The  value pointed to by retval should not be located on the calling thread's stack, since the
contents of that stack are undefined after the thread terminates.


*/
void gtthread_exit(void *retval){
  ///Given that we are cleaning a thread we turn off the signals 
  deactivate_signal();
  ///Get current context information
  struct node* current_node = get_current(); 
  //Pop thread from the queue
  pop_node(current_node);
  ///Push results so other threads can be the retval return
  copy_result(current_node->id,retval);
  //Reactivate signals
  deactivate_signal();
  if(get_thread_counter() == 0){///If this was the last thread, exit
    exit(0);
  }
  else{///Yield the processor to the next thread.
    yield();
  }
}


/*
    pthread_yield()  causes the calling thread to relinquish the CPU.  The thread is placed at the
    end of the run queue for its static priority and another thread is scheduled to run. 
    On success, pthread_yield() returns 0; on error, it returns an error number.    
*/
int gtthread_yield(void){
  yield();
  return 0;
}
/*
DESCRIPTION
       The pthread_equal() function compares two thread identifiers.

RETURN VALUE
       If  the  two  thread  IDs  are  equal,  pthread_equal() returns a nonzero value; otherwise, it
       returns 0.
*/
int  gtthread_equal(gtthread_t t1, gtthread_t t2){
	/*In this library gtthreads are treated as long integers, then normal comparison function 
	works as expected*/
	return (t1==t2);
}

/*
Deferred cancelation does not need to be implemented; all threads are canceled immediately.
The  pthread_cancel() function sends a cancellation request to the thread thread.

The  return
status of pthread_cancel() merely informs the caller whether the cancellation request was suc‐
cessfully queued.

After a canceled thread has terminated, a join with that thread using pthread_join(3)  obtains
PTHREAD_CANCELED  as the thread's exit status.  (Joining with a thread is the only way to know
that cancellation has completed.)

RETURN VALUE
       On success, pthread_cancel() returns 0; on error, it returns a nonzero error number.

ERRORS
       ESRCH  No thread with the ID thread could be found.

*/

int  gtthread_cancel(gtthread_t thread){
	///We just need to pop the thread from the queue.
  return pop_node_gtthread(thread);
}

/*
DESCRIPTION
       The pthread_self() function returns the ID of the calling thread.  This is the same value that
       is returned in *thread in the pthread_create(3) call that created this thread.

RETURN VALUE
       This function always succeeds, returning the calling thread's ID.
*/
gtthread_t gtthread_self(void){
	///Just get the current context and get the ID of that thread :)
  gtthread_t return_value = get_current()->id;
  return return_value;
}
