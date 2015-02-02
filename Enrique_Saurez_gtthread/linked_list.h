/*
Enrique Saurez Apuy
gtid: 903061622
Spring 2015
*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "definitions.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>
#include <errno.h>        /* errno */
#include <string.h>       /* strerror */
/*
	Use this function only in the main, it initialize the enviroment with the main context 
	This function has to be called before al the others functions.
*/
void start();

/*
	This returns the next struct node to iterate, it treats the linked list as a circular linked list.
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
ucontext_t * push_node(void* (*fn1)(void *), void *arg, gtthread_t *value);
/*
	This functions erase the node from the list
	All threads can be popped, except for the main thread.
	It returns 0 if the operations was done correctly
	Otherwise returns EXIT_FAILURE
*/
int pop_node(struct node *);
/*
	This cleans all the memory used by threads and contexts (stack).
*/
void end();
/*
	Return the number of active threads
*/
long get_thread_counter();
/*
	Return the currently active node (executing)
*/
struct node * get_current();
/*
	Push the main as the first node.
*/
int push_main(ucontext_t *main);
 /*
	This function is used to join a thread.
	It returns 0 if the thread already finished.
	It returns 1 if the thread is not yet ready
	If the function returns 1, then ret (if different to null) contains the result of the thread.
 */
int getValue(gtthread_t thread, void **ret,gtthread_t myself); 
/*
	This function eliminates the thread with id thread, from the system
*/
int pop_node_gtthread(gtthread_t thread);
/*
	This function copy back the result of a thread to the corrresponding item.
*/
int copy_result(gtthread_t id, void *retval);
/*
	It is the function wrapper for the thread execution.
*/
void return_handler(void* (*start_routine)(void *), void *arg);

void jump_next_context();
 #endif
