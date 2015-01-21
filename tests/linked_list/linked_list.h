#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <errno.h>        /* errno */
#include <string.h>       /* strerror */
#define MEM 64000

struct node {
  int id;
  ucontext_t *thread_context;
  struct node *next;
};

/*
	Use this function only in the main, it initialize the enviroment with the main context 
	This function has to be called before al the others functions.
*/
void start();

/*
	This returns the next node to iterate, it treats the linked list as a circular linked list.
	It should always return a value different to NULL
	If it returns NULL the linked list is empty (serious problems if this happens)
*/
struct node * next();

/*
	This returns the next node to iterate, it treats the linked list as a circular linked list.
	It also moves the iterator to the next position
	It should always return a value different to NULL
	If it returns NULL the linked list is empty (serious problems if this happens)
*/
struct node * next_and_move();

/*
	This functions creates a new node.
	The input is the function pointer that the thread is going to execute.
	The output is a pointer to the new created context.
*/
ucontext_t * push_node(void (*fn1)());
/*
	This functions erase the node from the list
	All threads can be popped, except for the main thread.
	It returns 0 if the operations was done correctly
	Otherwise returns EXIT_FAILURE
*/
int pop_node(struct node *);
/*
	This functions is called from inside the functions that are assigned to the threads.
	It yields the processor time to the next thread in the round robbin list.	
*/

void yield();
/*
	This cleans all the memory used by threads and contexts (stack).
*/
void end();
 
 #endif