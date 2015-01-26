#ifndef MUTEX_H
#define MUTEX_H

#define MAX_LOCKS 100

#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "definitions.h"

///Initialize a new mutex
int init_lock(gtthread_mutex_t* mutex);
///Lock the mutex
int mutex_lock(gtthread_mutex_t* mutex);
//Unlock the mutex
int mutec_unlock(gtthread_mutex_t* mutex);

#endif