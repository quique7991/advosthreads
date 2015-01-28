#include "linked_list.h"


int first = 1;///Boolean that indicates that there are no values in the list
struct node head;///The list always exists, but can be composed of null pointers
struct node *tail=NULL;///Last value in the queue, initially it does not exists. (pointer)
struct node * current=NULL;
struct node * next_thread=NULL;///Value that is going to execute next (pointer)
ucontext_t *main_thread=NULL;///Pointer to the context of the main.
struct node *sched_thread=NULL;
long thread_counter; //Counter of the thread counters required
long global_counter;
long current_max_threads=NUM_THREADS;
gtthread_t next_id=0;
struct to_join_result* join_results;

void jump_next_context(){
	if(current->next==NULL){
		current = &head;
	}
	else{
		current=current->next;
	}
	if(current->next==NULL){
		next_thread = &head;
	}
	else{
		next_thread=current->next;
	}	
	setcontext(current->thread_context); 
}

void return_handler(void* (*start_routine)(void *), void *arg){
	void * result;
	result = start_routine(arg);
	join_results[current->id].return_value = result;
	join_results[current->id].ready = 1;
	pop_node(current);
	if(thread_counter==0){
		//end();
		exit(0);
	}
	jump_next_context();
}

ucontext_t* initialize_sched_thread(void  (*timer_handler)(int)){
	sched_thread = (struct node *) malloc(sizeof(struct node));	
	sched_thread->id=-1;
	sched_thread->next = NULL;
	sched_thread->active = 1;
	int error;	
	sched_thread->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));		
	error=getcontext(sched_thread->thread_context);
	if(error==-1)
		puts("Error creating the context\n");	
	//Initialize the uc_link
	//TODO: UC_LINK should be variable					
	(sched_thread->thread_context)->uc_link=0;
	(sched_thread->thread_context)->uc_stack.ss_sp=malloc(MEM);
	if (((sched_thread->thread_context)->uc_stack.ss_sp) == NULL) {
    	int err = errno;        /* Preserve the errno from the failed malloc(). */
    	puts("Malloc failed");  /* This puts() could change the global errno. */
    	puts(strerror(err));
	}		
	///Update the other variables.		
	(sched_thread->thread_context)->uc_stack.ss_size=MEM;
	(sched_thread->thread_context)->uc_stack.ss_flags=0;
	makecontext((sched_thread->thread_context), (void*)timer_handler, 1,0);	
	return (sched_thread->thread_context);
}

//This instruction has to be always executed before any other action can be called
int push_main(ucontext_t *main_context){
	int i;
	join_results = (struct to_join_result*) malloc(NUM_THREADS*sizeof(struct to_join_result));
	for(i=0; i < NUM_THREADS; ++i){
		join_results[i].return_value = NULL;
		join_results[i].ready = 0;
		join_results[i].waiting_to_join = -1;
		join_results[i].is_running = 0.;
	}
	global_counter=1;
	if(first){
		thread_counter=1;
		first = 0;
		///Get the value of the current context, just to initialize it
		head.id=next_id;
		head.active=1;
		join_results[next_id].is_running=1;
		++next_id;
		head.thread_context = main_context;
		next_thread = &head;
		current = &head;
		tail = &head;
		head.next = NULL;
	}
	else{
        fprintf(stderr, "Main always have to be the first node in the ready list\n");
        exit(EXIT_FAILURE); //indicate failure.		
	}	
	return 0;
}

struct node * get_current(){
	return current;
}

struct node * next(){
	struct node *result;
	if(!first){///List is not empty
		result = next_thread;
	}
	else{
		result = NULL;
	}
	return result;
}

struct node * next_and_move(){
	current = next_thread;
	if((next_thread->next) == NULL)
		next_thread = &head;
	else{
		next_thread = next_thread->next;
	}
	return current;
}

/*create a new node*/
ucontext_t * push_node(void* (*fn1)(void *), void *arg, gtthread_t *value){
	struct node *tmp_ptr =NULL;
	ucontext_t * result;
	int error;
	///Push a new node into the list	
	if(first){
		thread_counter=1;
		++global_counter;
		first = 0;
		///Get the value of the current context, just to initialize it
		head.id=next_id;
		*value = next_id;
		head.active=1;
		join_results[next_id].is_running=1;
		join_results[next_id].node_information	= &head;			
		++next_id;
		head.thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
		error=getcontext((head.thread_context));
		if(error==-1)
			puts("Error creating the context\n");
		//Initialize the uc_link
		(head.thread_context)->uc_link=0;//sched_thread->thread_context;
		(head.thread_context)->uc_stack.ss_sp=malloc(MEM);
    	if (((head.thread_context)->uc_stack.ss_sp) == NULL) {
        	int err = errno;        /* Preserve the errno from the failed malloc(). */
        	puts("Malloc failed");  /* This puts() could change the global errno. */
        	puts(strerror(err));
    	}		
    	///Update the other variables.		
		(head.thread_context)->uc_stack.ss_size=MEM;
		(head.thread_context)->uc_stack.ss_flags=0;
		makecontext((head.thread_context), (void*)return_handler, 2,(void*)fn1,arg);
		next_thread = &head;
		current = &head;
		tail = &head;
		head.next = NULL;
		result = (head.thread_context);
	}
	else{
		tail->next = (struct node *) malloc(sizeof(struct node));
		tmp_ptr = tail->next;
		tmp_ptr->id = next_id;
		*value = next_id;
		join_results[next_id].is_running=1;		
		join_results[next_id].node_information	= tmp_ptr;		
		++next_id;
		tmp_ptr->active=1;
		///Get the value of the current context, just to initialize it		
		tmp_ptr->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));		
		error=getcontext((tmp_ptr->thread_context));
		if(error==-1)
			puts("Error creating the context\n");	
		//Initialize the uc_link
		//TODO: UC_LINK should be variable					
		(tmp_ptr->thread_context)->uc_link=0;//sched_thread->thread_context;
		(tmp_ptr->thread_context)->uc_stack.ss_sp=malloc(MEM);
    	if (((tmp_ptr->thread_context)->uc_stack.ss_sp) == NULL) {
        	int err = errno;        /* Preserve the errno from the failed malloc(). */
        	puts("Malloc failed");  /* This puts() could change the global errno. */
        	puts(strerror(err));
    	}		
    	///Update the other variables.		
		(tmp_ptr->thread_context)->uc_stack.ss_size=MEM;
		(tmp_ptr->thread_context)->uc_stack.ss_flags=0;
		makecontext((tmp_ptr->thread_context), (void*)return_handler, 2,(void*)fn1,arg);
		tail = tmp_ptr;
		tail->next = NULL;
		result = (tail->thread_context);
		++thread_counter;
		++global_counter;
	}
	if(global_counter==current_max_threads){
		int i;
		join_results = (struct to_join_result*) realloc(join_results,2*current_max_threads*sizeof(struct to_join_result));
		for(i=current_max_threads; i < 2*current_max_threads; ++i){
			join_results[i].return_value = NULL;
			join_results[i].ready = 0;
			join_results[i].waiting_to_join = -1;		
			join_results[i].is_running=0;		
		}
		current_max_threads = 2*current_max_threads;
	}
	return result;
}

int pop_node_gtthread(gtthread_t thread){
	///TODO: Verify what we require to do when a value is cancel, for joining afterwards
	return pop_node(join_results[thread].node_information);
}

int pop_node(struct node *toErase){
	int found = 0;
	struct node *temp_pointer;
	if(toErase==(&head)){///Node to erase is the head
		if(head.next == NULL){///It was the last context in the list.
			//free(head.thread_context);
			head.thread_context = NULL;
			head.active=0;
			head.next = NULL;///It was already null, it just for safety.
			first=1;
			tail=NULL;
			current=NULL;
			next_thread=NULL;
			--thread_counter;

		}
		else{///Substitute the head to the next value in the list.
			//free(head.thread_context);
			head.thread_context = (head.next)->thread_context;
			head.id = (head.next)->id;			
			temp_pointer = head.next;
			head.next = temp_pointer->next;
			head.active = temp_pointer->active;
			free(temp_pointer);
			//TODO verify the assignation of current, next_thread and tail
			current = &head;
			next_thread = &head;
			join_results[head.id].node_information = &head;
			--thread_counter;
		}
	}
	else{///It is not the head.
		struct node *temp_pointer = head.next;///Start with the value that follows the head..
		struct node *previous_node = &head;///Initially previous node is the head
		while(temp_pointer != NULL){//While there are more values not visited in the list
			if(temp_pointer == toErase){/// If the node is found.
				///Reasign next of previous node
				previous_node->next = temp_pointer->next;
				//Free the stack of the context that is being pop.
				free((temp_pointer->thread_context)->uc_stack.ss_sp);
				///Free the context
				free((temp_pointer->thread_context));				
				///Free the node
				free(temp_pointer);				
				found=1;///Node was found.
				thread_counter--;///Reduce the number of threads.
				break;
			}
			previous_node = temp_pointer;///Update the previous node
			temp_pointer = previous_node->next;///Update the next Node
		}
		if(!found){///If not found, error
	        fprintf(stderr, "Pointer does not exists in the linked list\n");
	        exit(EXIT_FAILURE); //indicate failure.
		}
	}
	return 0;
}


void end(){
	struct node *to_erase;
	struct node * tmp_ptr;
	///First clean the memory of the main thread context
	free(main_thread);
	to_erase=&head;
	while(to_erase!=NULL){
		tmp_ptr = to_erase;
		to_erase = to_erase->next;
		//Free the stack
		free((tmp_ptr->thread_context)->uc_stack.ss_sp);
		///Free the context
		free((tmp_ptr->thread_context));				
		if(tmp_ptr!=(&head)){///If the node is not the head.
			///Free the node
			free(tmp_ptr);				
		}
	}
	free(join_results);
	free(sched_thread);
}

long get_thread_counter(){
	return thread_counter;
}

int getValue(gtthread_t thread, void **ret){
	int result = 1;
	if(join_results[thread].is_running){
		if(join_results[thread].ready){
			if(ret != NULL){
				*ret = (join_results[thread].return_value);
			}
			result = 0;
		}
	}
	else{
		return -1; //value does not exists;
	}
	return result;
}

int copy_result(gtthread_t id, void *retval){
	join_results[id].return_value = retval;
	join_results[id].ready	= 1;	
	return 0;
}
