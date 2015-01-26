#include "library_context.h"

ucontext_t a;
struct node T;
struct node * current;
int counter=0;
int globalCounter;

void yield(){
	struct node * previous;
	previous = current;
	globalCounter++;
	if((current->next) == NULL){
		current = &T;
	}
	else{
		current = previous->next;
	}
	if(globalCounter < 24)
		swapcontext(&(previous->thread_context), &(current->thread_context));
	else
		swapcontext(&(previous->	thread_context),&Main);
} 

void fn1(){
	while(1){
		printf("this is from 1\n");
		yield();
	}
}

void fn2(){
	while(1){
   		printf("this is from 2\n");
		yield();
	}
}

void fn3(){
	while(1){
		printf("this is from 3\n");
		yield();
	}

}
 
void start(){
	//Function 1
	getcontext(&(T.thread_context));
	(T.thread_context).uc_link=0;
	(T.thread_context).uc_stack.ss_sp=malloc(MEM);
	(T.thread_context).uc_stack.ss_size=MEM;
	(T.thread_context).uc_stack.ss_flags=0;
	makecontext(&(T.thread_context), (void*)&fn1, 0);
	//Function 2
	T.next = (struct node *) malloc(sizeof(struct node));
	getcontext(&((T.next)->thread_context));
	((T.next)->thread_context).uc_link=0;
	((T.next)->thread_context).uc_stack.ss_sp=malloc(MEM);
	((T.next)->thread_context).uc_stack.ss_size=MEM;
	((T.next)->thread_context).uc_stack.ss_flags=0;
	makecontext(&((T.next)->thread_context), (void*)&fn2, 0);
	//Function 3
	T.next->next = (struct node *) malloc(sizeof(struct node));
	getcontext(&(((T.next)->next)->thread_context));
	(((T.next)->next)->thread_context).uc_link=0;
	(((T.next)->next)->thread_context).uc_stack.ss_sp=malloc(MEM);
	(((T.next)->next)->thread_context).uc_stack.ss_size=MEM;
	(((T.next)->next)->thread_context).uc_stack.ss_flags=0;
	makecontext(&(((T.next)->next)->thread_context), (void*)&fn3, 0);	
}

void end(){
	int i;
	for(i=0;i<3;++i)
		printf("Hello\n");
		//free((T[i].thread_context).uc_stack.ss_sp );
}
 