#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <signal.h>
#include "timer.h"
#include "linked_list.h"
#include "definitions.h"

/*
This function initiates the scheduler (inclding the timer and the signal handler)
Time is in microseconds
*/
void init_timer_scheduler(long time);

#endif