#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

struct sigaction sa;
struct itimerval timer;
sigset_t signal_control;

void timer_handler (int signum)
{
 static int count = 0;
 printf ("timer expired %d times\n", ++count);
}

void setTimer(unsigned int time){
        ///Initialize signal control variable
  sigemptyset(&signal_control);
        //Add the only signal that we are interested in: virtual timer
  sigaddset(&signal_control,SIGVTALRM);

 /* Install timer_handler as the signal handler for SIGVTALRM. */
 memset (&sa, 0, sizeof (sa));
 sa.sa_handler = &timer_handler;
 sigaction (SIGVTALRM, &sa, NULL);

 /* Configure the timer to expire after 250 msec... */
 timer.it_value.tv_sec = 0;
 timer.it_value.tv_usec = time;
 /* ... and every 250 msec after that. */
 timer.it_interval.tv_sec = 0;
 timer.it_interval.tv_usec = time;
 /* Start a virtual timer. It counts down whenever this process is
   executing. */
 setitimer (ITIMER_VIRTUAL, &timer, NULL);
}

int main ()
{
 setTimer(250000);
 /* Do busy work. */
 sigprocmask(SIG_BLOCK, &signal_control, NULL);
 int counter = 0;
 while(counter < 1000000000){
	counter++;
 }
 printf("Hello\n");
 sigprocmask(SIG_UNBLOCK,&signal_control,NULL);

 while (1);
}
