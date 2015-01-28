#include "scheduler.h"
/*
	This is the round robbin scheduler 
*/
long seconds=0;
long microseconds=0;
ucontext_t Main;
ucontext_t* sched;
///TODO Fix the main access to the sistem
void scheduler (int signum){
	///Deactivate the signal, similar to disabling interrupts
	deactivate_signal();
	//Get the number active threads
	long active_threads = get_thread_counter();
	//Initialize variables
	struct node* next_thread;
	struct node* current;	
	///If there are no active threads
	if(active_threads==1){
		///Keep the same thread running
		next_thread = next();
		///reinitilize timer, may not be required.
		//modify_timer(seconds, microseconds, 0, 0);		
		set_timer(seconds,microseconds, (void*) &scheduler);
		activate_signal(); // activate signal again
		//swapcontext(next_thread->thread_context,next_thread->thread_context); ///go to the next context;
	}
	else{
		///get the value of the current context
		current = get_current();
		//get the context to go
		next_thread = next_and_move();
		///The system is already runnign
		modify_timer(seconds, microseconds, 0, 0);
		set_timer(seconds,microseconds, (void*) &scheduler);//reinitialize timer, may not be required
		//activate_signal();///reactivate signal handler.
		//TODO:Verify why this is required?
		//if(current->active==1){
		//swapcontext(current->thread_context,next_thread->thread_context);////Swap to the next thread.
		if(current!=next_thread){
			swapcontext(current->thread_context,next_thread->thread_context);
		}///Else go ahead :)
	}
}

///Initiate
void init_timer_scheduler(long time){
	if(time > 1000000){
		seconds = time/1000000;
		microseconds = time%1000000;
	}
	else{
		seconds=0;
		microseconds=time;
	}
   	getcontext(&Main);	
   	///TODO push MAIN
   	push_main(&Main);
   	sched = initialize_sched_thread(&scheduler);
	set_timer(seconds,microseconds, (void*) &scheduler);
}

