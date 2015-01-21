#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#define MEM 64000

ucontext_t Main;
ucontext_t a;
ucontext_t T[3];
int counter=0;
int globalCounter;



void yield(){
	int previous = counter;
	globalCounter++;
	counter = (counter + 1)%3;
	if(globalCounter < 24)
		swapcontext(&(T[previous]), &T[counter]);
	else
		swapcontext(&(T[previous]),&Main);
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
	getcontext(&(T[0]));
	(T[0]).uc_link=0;
	(T[0]).uc_stack.ss_sp=malloc(MEM);
	(T[0]).uc_stack.ss_size=MEM;
	(T[0]).uc_stack.ss_flags=0;
	makecontext(&(T[0]), (void*)&fn1, 0);
	//Function 2
	getcontext(&(T[1]));
	(T[1]).uc_link=0;
	(T[1]).uc_stack.ss_sp=malloc(MEM);
	(T[1]).uc_stack.ss_size=MEM;
	(T[1]).uc_stack.ss_flags=0;
	makecontext(&(T[1]), (void*)&fn2, 0);
	//Function 3
	getcontext(&(T[2]));
	(T[2]).uc_link=0;
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