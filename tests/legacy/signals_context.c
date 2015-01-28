#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#define MEM 64000

ucontext_t yield_thread;
ucontext_t Main;
ucontext_t a;
ucontext_t T[3];
int counter=0;
int globalCounter;



void yield(){
	int previous = counter;
	globalCounter++;
	counter = (counter + 1)%3;
	if(globalCounter < 9)
		swapcontext(&(T[previous]), &T[counter]);
	else if(globalCounter < 18)
		if(globalCounter%2==0)
			swapcontext(&(T[0]), &T[1]);
		else
			swapcontext(&(T[1]),&T[0]);
	else if(globalCounter < 27)
		swapcontext(&T[0],&T[0]);
	else
		swapcontext(&T[0],&Main);
} 

void fn1(){
	int counter=0;
	while(counter < 3){
		printf("this is from 1:%i\n",counter);
		counter++;
		yield();
	}
}

void fn2(){
	int counter=0;
	while(counter < 6){
		printf("this is from 2:%i\n",counter);
		counter++;
		yield();
	}
}

void fn3(){
	int counter=0;
	while(counter < 9){
		printf("this is from 3:%i\n",counter);
		counter++;
		yield();
	}

}
 
void start(){
	getcontext(&(yield_thread));
	(yield_thread).uc_link=0;
	(yield_thread).uc_stack.ss_sp=malloc(MEM);
	(yield_thread).uc_stack.ss_size=MEM;
	(yield_thread).uc_stack.ss_flags=0;
	makecontext(&(yield_thread), (void*)&yield, 0);
	//Function 1
	getcontext(&(T[0]));
	(T[0]).uc_link=&(yield_thread);
	(T[0]).uc_stack.ss_sp=malloc(MEM);
	(T[0]).uc_stack.ss_size=MEM;
	(T[0]).uc_stack.ss_flags=0;
	makecontext(&(T[0]), (void*)&fn1, 0);
	//Function 2
	getcontext(&(T[1]));
	(T[1]).uc_link=&(yield_thread);
	(T[1]).uc_stack.ss_sp=malloc(MEM);
	(T[1]).uc_stack.ss_size=MEM;
	(T[1]).uc_stack.ss_flags=0;
	makecontext(&(T[1]), (void*)&fn2, 0);
	//Function 3
	getcontext(&(T[2]));
	(T[2]).uc_link=&(yield_thread);
	(T[2]).uc_stack.ss_sp=malloc(MEM);
	(T[2]).uc_stack.ss_size=MEM;
	(T[2]).uc_stack.ss_flags=0;
	makecontext(&(T[2]), (void*)&fn3, 0);	
}

void end(){
	int i;
	for(i=0;i<3;++i)
		free((T[i]).uc_stack.ss_sp );
}
 
int main(int argc, char *argv[]){
   	start();
   	getcontext(&Main);
	swapcontext(&Main, &T[0]);
	printf("completed\n");
	end();
	exit(0);
}