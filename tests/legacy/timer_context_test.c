#include "timer/timer.h"
#include "linked_list/linked_list.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

ucontext_t Main;
ucontext_t scheduler_context;
ucontext_t *other;


void scheduler (int signum)
{
	deactivate_signal();
	printf("Hello\n");
	long active_threads = get_thread_counter();
	struct node* next_thread;
	struct node* current;	
	//modifyTimer(0, 500000, 0, 0);
	if(active_threads == 0)
		printf("No active threads\n");
	else if(active_threads==1){
		///Keep the same thread running
		next_thread = next();
		printf("Hello 1 threads\n");
		//set_timer(500000, (void*) &scheduler);
		activate_signal();
		swapcontext(&Main,next_thread->thread_context);
	}
	else{
		printf("Hello Multi threads\n");
		current = get_current();
		next_thread = next_and_move();
		if(current == NULL){
			printf("Current Null\n");
			//set_timer(500000, (void*) &scheduler);
			activate_signal();
			setcontext(next_thread->thread_context);
		}
		else{
			printf("Current not NULL\n");
			//set_timer(500000, (void*) &scheduler);
			activate_signal();
			swapcontext(current->thread_context,next_thread->thread_context);
		}
	}
}
	

void fn1(){
	printf("this is from 1\n");
	while(1){
		//int result = set_timer(500000, (void*) &scheduler);
		//printf("%d\n",result );
	}
}

void fn2(){
   		printf("this is from 2\n");
	while(1){
   		//usleep(100000);
	}
}

void fn3(){
	int counter;
	while(1){
		counter=0;
		while(counter < 1000000){
			counter++;
		}
		printf("this is from 3\n");
	}

}


int main(int argc, char *argv[]){
	set_timer(500000, (void*) &scheduler);
   	getcontext(&Main);
    start(&Main);
   	other = push_node(&fn1);
   	push_node(&fn2);
   	push_node(&fn3);
   	next_and_move();
	swapcontext(&Main, other);
	printf("completed\n");
	//end();
	while(1);
	exit(0);
}
