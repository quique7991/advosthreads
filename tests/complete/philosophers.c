/*
Dinning philosophers
Enrique Saurez Apuy
gtid: 903061622
*/
/*
How it is solved?
Given that each chopstick can only be used by the person to its right
and the person to its left. We define a protocol for the philosophers
such that each time they try to start eating, they always grab an specific
 chopstick first and then when they acquire it, they grab the 
other chopstick.
Each different philosopher is always deterministic in such, that each of them
always pick the same side first. Also not all the philosophers are leftie, 
some of them start with the right, and others start with the left.
This avoid deadlocks because as long as one person waits for left fork first 
and another waits for right first, cycles can't form. Because two differents 
philosophers never contend for the same pair of chopsticks.
To start with right or left is define by the parity of the id of the current
philosopher (thread_id)
*/
#define PHILOSOPHER_NUM 5
#include <stdio.h>
#include <time.h>
#include "gtthread.h"

/*One thread for each philosopher*/
gtthread_t philosophers[PHILOSOPHER_NUM];
/*One boolean for each chopstick*/
int chopsticks[PHILOSOPHER_NUM];
/*One mutex for each chopstick*/
gtthread_mutex_t chopstick_mutex[PHILOSOPHER_NUM];


long first_chopstick(gtthread_t id){
	/*gtthread id starts in 1*/
	long result;
	if(id%2==0){/*left chopstick*/
		result = id-2;/*Left chopstick*/
	}
	else{//rightie
		result = id-1;
	}

	return result;
}

long second_chopstick(gtthread_t id){
	long result;
	if(id%2==0){
		result = id-1;/*Right chopstick*/
	}
	else{
		if(id==1){
			result =  (PHILOSOPHER_NUM-1);/*If it is the first one the left is the last chopstick*/
		}
		else{
			result = id -2;/*left chopstick*/
		}
	}
	return result;/*Ids start in 1, so we substract 1 to each id*/
}

int acquire_chopsticks(){
	int first,second;
	/*Get the index for both chopsticks*/
	first = first_chopstick();
	second = second_chopstick();
	gtthread_mutex_lock(&chopstick_mutex[first]);
	gtthread_mutex_lock(&chopstick_mutex[second]);

}


int release_chopsticks(){
	int first,second;
	/*Get the index for both chopsticks*/
	first = first_chopstick();
	second = second_chopstick();
	gtthread_mutex_unlock(&chopstick_mutex[first]);
	gtthread_mutex_unlock(&chopstick_mutex[second]);
}

void* dinner_philospher(void* arg){
	while(1){
		printf("Philosopher %s is thinking\n", gtthread_self());
		/*Think for a random amount of time*/
		for(j=0; j < rand() % 999999; ++j);
		/*Now the philosopher is hungry*/
		printf("Philosopher %s is hungry\n", gtthread_self());
		acquire_chopsticks();
		/*Eat for a random amount of time*/
		for(j=0; j < rand() % 999999; ++j);		
		release_chopsticks();/* Give up forks*/
	}
	return NULL;
}

int main(){
	/*Variable creation*/
	int i;
	/*Initialize seed*/
	srand(time(NULL));
	/*Inititalize the gtthread library*/
	gtthread_init(1000);
	/*Initialize all mutexes and threads*/
	for(i=0;i<PHILOSOPHER_NUM;++i){
		/*Mutex init for the chopstick*/
		gtthread_mutex_init(&chopstick_mutex[i]);
	}
	for(i=0;i<PHILOSOPHER_NUM;++i){
		/*Thread creation*/
		gtthread_create(&philosophers[i],&dinner_philospher,NULL);
	}
	while(1);//SPIN
	return 0;
}
