#include "timer.h"

struct sigaction signal_action;
struct itimerval timer;
sigset_t signal_control;

void yield(){
	raise(SIGVTALRM);//Call the time handler
}

void activate_signal(){
	sigprocmask(SIG_UNBLOCK,&signal_control,NULL);
}

void deactivate_signal(){
	sigprocmask(SIG_BLOCK, &signal_control, NULL);
}

int set_timer(long seconds,long microseconds, void  (*timer_handler)(int) ){
	int result;
	///Initialize signal control variable
	sigemptyset(&signal_control);
	//Add the only signal that we are interested in: virtual timer
	sigaddset(&signal_control,SIGVTALRM);
	///Initialize struct
	memset (&signal_action, 0, sizeof (signal_action));
	///initialize the mask for the sigaction
	sigemptyset(&signal_action.sa_mask);	
	///Empty the flags
	signal_action.sa_flags=0;
	//Assign the function handler
 	signal_action.sa_handler = timer_handler;
 	///Indicate the use of the virtual time
 	if(sigaction (SIGVTALRM, &signal_action, NULL)== -1){
         printf("error with: sigaction\n");
        exit(EXIT_FAILURE);		
 	}
 	/*Configure the first expiration time*/
	timer.it_value.tv_sec = seconds;
 	timer.it_value.tv_usec = microseconds;
 	/* Configure sucesive times */
 	timer.it_interval.tv_sec = seconds;
 	timer.it_interval.tv_usec = microseconds;
 	/* Starts the timer */
 	result = setitimer (ITIMER_VIRTUAL, &timer, NULL);
 	return result;
}

void modify_timer(long value_tv_sec, long value_tv_usec, long interval_tv_sec, long interval_tv_usec){
 	/*Reconfigure the first expiration time*/
 	timer.it_value.tv_sec = value_tv_sec;
 	timer.it_value.tv_usec = value_tv_usec;
 	/* Reconfigure sucesive times */
 	timer.it_interval.tv_sec = interval_tv_sec;
 	timer.it_interval.tv_usec = interval_tv_usec;	
 	/* Starts the timer */
	setitimer (ITIMER_VIRTUAL, &timer, NULL);
}