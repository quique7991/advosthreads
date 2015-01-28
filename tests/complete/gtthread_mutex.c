#include "gtthread.h"
#include "mutex.h"
/* Just an example. Nothing to see here. */


/* see man pthread_mutex(3); except init does not have the mutexattr parameter,
 * and should behave as if mutexattr is NULL (i.e., default attributes); also,
 * static initializers do not need to be implemented */

 /*
pthread_mutex_init initializes the mutex object pointed to by mutex according to t
he mutex attributes specified in mutexattr. If mutexattr is NULL, default attributes are used instead.
The mutex type determines what happens if a thread attempts to lock a mutex it 
already owns with pthread_mutex_lock(3) . If the mutex is of the “normal” or “fast” type, 
pthread_mutex_lock(3) simply suspends the calling thread forever.
 */
int  gtthread_mutex_init(gtthread_mutex_t *mutex){
	int result = init_lock(mutex);
	return result;
}
/*
pthread_mutex_lock locks the given mutex. If the mutex is currently unlocked, it becomes 
locked and owned by the calling thread, and pthread_mutex_lock returns immediately. If the mutex 
is already locked by another thread, pthread_mutex_lock suspends the calling thread until the mutex 
is unlocked.
*/
int  gtthread_mutex_lock(gtthread_mutex_t *mutex){
	int result = mutex_lock(mutex);
	return result;
}

/*
pthread_mutex_unlock unlocks the given mutex. The mutex is assumed to be locked and owned by
the calling thread on entrance to pthread_mutex_unlock. If the mutex is of the “normal” type, 
pthread_mutex_unlock always returns it to the unlocked state.
*/
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex){
	int result = mutex_unlock(mutex);
	return result;
}
/* gtthread_mutex_destroy() and gtthread_mutex_trylock() do not need to be
 * implemented */
