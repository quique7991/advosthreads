Project 1: GTTHREADS
Enrique Saurez
gtid: 903061622

==What Linux platform do you use?==
Ubuntu 14.10 64 bits
Kernel: 3.16.0-30-generic

==How the preemptive scheduler is implemented.==

The scheduler was implemented using the tools suggested: signals and the *context()* functions. 
Each time a thread is created (including the main), it is included into a linked list. The library have three pointers in this linked list: current thread executing, the head and the tail of the linked list. They are used to implement the scheduler. Each time time a new thread is to be add to the system, is added following the pointer of the current tail.

The round robbin preempty scheduler is achive by using a virtual time one-shot alarm signal (achieved by using sigaction and setitimer). Each time the timer expires a scheduler handler is called and the context is changed from the current thread (using the pointer exposed in the previous paragraph) and the next context in the linked list. When we reach the end of the linked list, it starts over with the head (equivalently to a circular linked list). If there is only one thread executing, the scheduler does not makes a context switch and fall through the handler, that returns to the point before the handler was called. 

Each time the scheduler handler is called the one-shot timer is restarted. During this time the SIGVTALRM signal is disabled, so the system can not do a recursive handler call, if the time used during the handler is too long. If there are more than two threads running, then on the call the handler stores the current context in the linked list, move the pointer and swap it for the next context to be executed. All of this is done in a round robin fasion.


The implementation given the use of context functions and the signal handler could lead to signal-unsafe situations, but in all the cases tested it worked correctly.

==How to compile your library and run your program==
To compile the library:
make clean
make

To run the Dining Philosophers code:
./philosophers.o

Before compiling the code of the dining philosophers, on the top of the file philosophers.c, it is
define 2 variables, INIT\_TIME and MAX\_RANDOM\_LOOP that defines the time between each virtual signal
and the max time between states of the philosophers, respectively, they can be changed, to obtain
a different time behavior. Primarly MAX\_RANDOM\_LOOP defines how fast the threads write in average
to the screen.

==How you prevent deadlocks in your Dining Philosophers solution==

We assume that each chopstick can only be used by the person to its right
and the person to its left. We define a protocol for the philosophers
such that each time one of them try to start eating, they always grab an specific
 chopstick first and then when they acquire it, they grab the 
other chopstick.
Each different philosopher is always deterministic in such, that each of them
always pick the same side first. Also not all the philosophers are leftie, 
some of them start with the right, and others start with the left.
This avoid deadlocks as long as one person waits for left fork first 
and another waits for right first, cycles can't form. This is true only because two differents 
philosophers never contend for the same pair of chopsticks.
To start with right or left is define by the parity of the id of the current
philosopher (thread\_id).


== Any thoughts you have on the project, including things that work especially well or which don't work. ==

The mutex work really well, because they benefit from the characteristics of the round 
robbin scheduler. Given that the system is working as an user level thread library, its behaviour
is equivalent to a uniprocessor system. So we know that if a thread is not able to obtain a thread,
it does not make sense to keep spining, because two threads are never executing at the same time. 
Then what is done, is that the processor yields each times it tries to obtain a lock and fails.
Improving considerably over a naive spin-lock implementation.
Everything wokrs, but there are some behaviours in the API used (context functions) that are 
not described thoroughly, the information is sparse in different documentations and this makes 
the programming more difficult. So there could be unexpected behaviours due to uncommented
situations for combining signals and contexts, and any other design decision of the library.
