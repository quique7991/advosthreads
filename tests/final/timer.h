#ifndef TIMER_H
#define TIMER_H

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "definitions.h"

///Warning: Never use this function in combination with either sleep or usleep, this clean the signals and other configurations

/*
This function set the the time period for each signal and the function that behaves
It returns 0 if it was correctly executed (the setitime) or -1 otherwise
*/
int set_timer(long seconds,long microseconds, void (*timer_handler)(int));
/*
	This function reset the parameters of the timer withouth modifying the signal actions
*/
void modify_timer(long value_tv_sec, long value_tv_usec, long interval_tv_sec, long interval_tv_usec);
/*
	Reactivates the signal obtained from the timeout of the virtual clock
*/
void activate_signal();
/*
	Deactivates the signal obtained from the timeout of the virtual clock
*/
void deactivate_signal();
/*
	Call the timer handler, by raising the virtual timeout signal
*/
void yield();
 #endif