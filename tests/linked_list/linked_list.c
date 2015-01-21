#include "linked_list.h"


int first = 1;///Boolean that indicates that there are no values in the list
struct node head;///The list always exists, but can be composed of null pointers
struct node *tail=NULL;///Last value in the queue, initially it does not exists. (pointer)
struct node * current=NULL;///Value that is going to execute next (pointer)
ucontext_t *main_thread=NULL;///Pointer to the context of the main.
long thread_counter;
int globalCounter;
//TODO verify if a long int is required.
int next_id=1;

void start(){	
	main_thread = (ucontext_t *) malloc(sizeof(ucontext_t));
	getcontext(main_thread);
}

struct node * next(){
	struct node *result;
	if(!first){///List is not empty
		result = current;
	}
	else{
		result = NULL;
	}
	return result;
}

struct node * next_and_move(){
	struct node * result;
	result = current;
	if((current->next) == NULL)
		current = &head;
	else{
		current = current->next;
	}
	return result;
}

void yield(){
	struct node * previous;
	globalCounter++;
	if(globalCounter == 12)
		pop_node(&head);
	previous = current;
	if((current->next) == NULL){
		current = &head;
	}
	else{
		current = previous->next;
	}
	printf("%d\n", (current->id));
	if(globalCounter < 24)
		swapcontext((previous->thread_context), (current->thread_context));
	else
		swapcontext((previous->thread_context),main_thread);
} 

/*create a new node*/
ucontext_t * push_node(void (*fn1)()){
	struct node *tmp_ptr =NULL;
	ucontext_t * result;
	int error;
	///Push a new node into the list	
	if(first){
		thread_counter=1;
		first = 0;
		///Get the value of the current context, just to initialize it
		head.id=next_id;
		++next_id;
		head.thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
		error=getcontext((head.thread_context));
		if(error==-1)
			puts("Error creating the context\n");
		//Initialize the uc_link
		//TODO: UC_LINK should be variable					
		(head.thread_context)->uc_link=0;
		(head.thread_context)->uc_stack.ss_sp=malloc(MEM);
    	if (((head.thread_context)->uc_stack.ss_sp) == NULL) {
        	int err = errno;        /* Preserve the errno from the failed malloc(). */
        	puts("Malloc failed");  /* This puts() could change the global errno. */
        	puts(strerror(err));
    	}		
    	///Update the other variables.		
		(head.thread_context)->uc_stack.ss_size=MEM;
		(head.thread_context)->uc_stack.ss_flags=0;
		makecontext((head.thread_context), (void*)fn1, 0);
		current = &head;
		tail = &head;
		head.next = NULL;
		result = (head.thread_context);
	}
	else{
		tail->next = (struct node *) malloc(sizeof(struct node));
		tmp_ptr = tail->next;
		tmp_ptr->id = next_id;
		++next_id;
		///Get the value of the current context, just to initialize it		
		tmp_ptr->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));		
		error=getcontext((tmp_ptr->thread_context));
		if(error==-1)
			puts("Error creating the context\n");	
		//Initialize the uc_link
		//TODO: UC_LINK should be variable					
		(tmp_ptr->thread_context)->uc_link=0;
		(tmp_ptr->thread_context)->uc_stack.ss_sp=malloc(MEM);
    	if (((tmp_ptr->thread_context)->uc_stack.ss_sp) == NULL) {
        	int err = errno;        /* Preserve the errno from the failed malloc(). */
        	puts("Malloc failed");  /* This puts() could change the global errno. */
        	puts(strerror(err));
    	}		
    	///Update the other variables.		
		(tmp_ptr->thread_context)->uc_stack.ss_size=MEM;
		(tmp_ptr->thread_context)->uc_stack.ss_flags=0;
		makecontext((tmp_ptr->thread_context), (void*)fn1, 0);
		tail = tmp_ptr;
		tail->next = NULL;
		result = (tail->thread_context);
		++thread_counter;
	}
	return result;
}

int pop_node(struct node *toErase){
	int found = 0;
	struct node *temp_pointer;
	if(toErase==(&head)){///Node to erase is the head
		if(head.next == NULL){///It was the last context in the list.
			free(head.thread_context);
			head.thread_context = NULL;
			head.next = NULL;///It was already null, it just for safety.
			first=1;
			tail=NULL;
			current=NULL;
		}
		else{///Substitute the head to the next value in the list.
			printf("Hello\n");
			free(head.thread_context);
			head.thread_context = (head.next)->thread_context;
			head.id = (head.next)->id;			
			temp_pointer = head.next;
			head.next = (head.next)->next;
			free(temp_pointer);
		}
	}
	else{///It is not the head.
		struct node *temp_pointer = head.next;///Start with the value that follows the head..
		struct node *previous_node = &head;///Previous node is the head
		while(temp_pointer != NULL){//While there are more values not visited in the list
			if(temp_pointer == toErase){/// If the node is found.
				if(toErase == tail){///If current is the tail, then there is a new tail that is the previous value
					tail=previous_node;
				}
				if(toErase == current){///If toErase is the one to eliminate
					if(toErase->next == NULL){///If there is no next, the next is the head
						current = &head;
					}
					else{
						current = toErase->next;
					}
				}
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
}