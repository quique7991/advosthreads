#include "timer/timer.h"
ucontext_t Main;
ucontext_t *other;

void timer_handler (int signum)
{
 static int count = 0;
 printf ("timer expired %d times\n", ++count);
 if(count > 10){
 	modifyTimer(0, 250000, 0, 0);
 }
 else{
 	modifyTimer(0, 500000, 0, 0);
 }
}

int main(int argc, char *argv[]){
	setTimer(500000, (void*) &timer_handler);
	while(1);
}
