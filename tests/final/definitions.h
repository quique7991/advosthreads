#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <ucontext.h>

///Define the thread
typedef long gtthread_t;

///Define the mutex type
typedef struct {
    int is_locked;
    long id;
    gtthread_t owner;
} gtthread_mutex_t;

//Define the nodes for the linked list
struct node{
  gtthread_t id;
  int active;
  ucontext_t *thread_context;
  struct node *next;
};

//Define the structure for the result storage
struct to_join_result{
	void * return_value;
	int ready;
	int is_running;
	struct node *node_information;
	gtthread_t waiting_to_join;
};
#endif