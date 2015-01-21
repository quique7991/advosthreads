#include "linked_list/linked_list.h"
ucontext_t Main;
ucontext_t *other;

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

int main(int argc, char *argv[]){
   	getcontext(&Main);
    start(&Main);
   	other = push_node(&fn1);
   	push_node(&fn2);
   	push_node(&fn3);
	swapcontext(&Main, other);
	printf("completed\n");
	//end();
	exit(0);
}
