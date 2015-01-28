#include "mutex.h"
/*
On a uni-processor, an operation is atomic if no context-switch
is allowed in the middle of the operation
Context switch occurs because of:
* Internal events: system calls and exceptions
* External events: interrupts
Mutual exclusion can be achieved by preventing context
switch
Because we are at user-level with one thread, it is
equivalent to working in a uniprocessor.
Hardware delays the processing of interrupts until
interrupts are enabled
Another possibility is to implement either busy wait,
or some kind of test and set, with a 

Given the way the libraries is designed then the locks are going to behaves as follows
acquire(){
Disable signals
while(value != Free){
	enable signals
	yield
	disable signals
}
value=Busy
Enable signals
}
The procesor yields after just trying once, because it is not going to change given the 
round robbin scheduler, until the current thread yields
For Releasing is more easy
release(){
	disable signals
	value = FREE
	enable signals
}
*/

//Create variable that is going to contain the locks
gtthread_mutex_t** locks;
///Variable that contains the number of locks in the system
long counter=0;
long current_max = MAX_LOCKS;

///This function creates a new mutex into the array.
int init_lock(gtthread_mutex_t* mutex){
	if(counter == 0){///First lock in the system, need to initialize the array
		///Allocate the memory for both the array and the lock
		locks = (gtthread_mutex_t **) malloc(current_max*sizeof(gtthread_mutex_t*));
		locks[counter]= (gtthread_mutex_t*) malloc(sizeof(gtthread_mutex_t));
		mutex = locks[counter];///assign the pointer to the input variable
		mutex->is_locked = 0;//is not locked
		mutex->id = counter;//ID of the mutex
		mutex->owner = -1;///There is no owner
		++counter;///Increase the counter of locks
	}
	else if(counter < MAX_LOCKS){///Normal addition
		//Allocate memory for the new lock
		locks[counter]= (gtthread_mutex_t*) malloc(sizeof(gtthread_mutex_t));
		mutex = locks[counter];///assign the pointer to the input variable
		mutex->is_locked = 0;//is not locked
		mutex->owner = -1;///There is no owner		
		mutex->id = counter;//Id of the mutex
		++counter;///Increase the counter of locks
	}
	else{
		if(counter < current_max){///Normal addition
			//Allocate memory for the new lock
			locks[counter]= (gtthread_mutex_t*) malloc(sizeof(gtthread_mutex_t));
			mutex = locks[counter];///assign the pointer to the input variable
			mutex->is_locked = 0;//is not locked
			mutex->owner = -1;///There is no owner			
			mutex->id = counter;//ID of the mutex
			++counter;///Increase the counter of locks
		}	
		else{//Needs to increase the size of the available locks, and copy them back
			int i;
			gtthread_mutex_t** tmp_container = locks;
			current_max = 2*current_max;///Duplicate the size
			locks = (gtthread_mutex_t **) malloc(current_max*sizeof(gtthread_mutex_t*));
			for(i=0;i<counter;++i){
				locks[i]=tmp_container[i];///They are only pointers so a direct coy is correct.
			}
			free(tmp_container);///Just free the double pointer not the locks
			//Allocate memory for the new lock
			locks[counter]= (gtthread_mutex_t*) malloc(sizeof(gtthread_mutex_t));
			mutex = locks[counter];///assign the pointer to the input variable
			mutex->is_locked = 0;//is not locked
			mutex->owner = -1;///There is no owner			
			mutex->id = counter;//ID of the mutex
			++counter;///Increase the counter of locks			
		}
	}
	return 0;
}

///Lock the mutex
int mutex_lock(gtthread_mutex_t* mutex){
	///Deactivate interrupts
	deactivate_signal();
	while((mutex->is_locked)){
		///If it is already locked, activate signals again and yield the processor, 
		///Given the round-robbin scheduler, the value is not going to change, while we hold 
		///the processor. So it is better to yield.
		activate_signal();
		yield();
		///After yield deactivate signal and check again if the mutex is locked.
		deactivate_signal();
	}
	///When the mutex is available, locked it
	mutex->is_locked = 1;
	mutex->owner = (get_current()->id);///There is no owner	
	///Activate the signals again
	activate_signal();
	return 0;
}

///Unlock the mutex
int mutex_unlock(gtthread_mutex_t* mutex){
	deactivate_signal();
	if((mutex->owner) == (get_current()->id)){///Verify if it is the owner of the lock.
		mutex->is_locked = 0;
	} 
	else{///ERROR! Wrong thread
        fprintf(stderr, "Wrong thread released the lock\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
	}
	activate_signal();
	return 0;
}