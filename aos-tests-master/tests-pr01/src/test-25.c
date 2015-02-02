/*
 * pthread_cancel_test.c
 * Copyright (C) 2015 benjamin <benjamin@assus>
 *
 * Distributed under terms of the MIT license.
 */

#include "gtthread.h"
#include <stdio.h>

gtthread_t child;
gtthread_t parent;

void* worker(void* arg)
{
  printf("Hello 1\n");
  gtthread_yield();
  printf("Hello 2\n");
  gtthread_cancel(parent);
  printf("Parent canceled\n");
  int i = 0;
  for (i = 0; i < 1; ++i) {
    printf("Child running\n");
    gtthread_yield();
  }
  return NULL;
}

int main()
{
  printf("Should print 'Parent running' for 1 or 2 times, then 'Child running' and exit smoothly.\n");
  gtthread_init(1000);
  parent = gtthread_self();
  gtthread_create(&child, worker, NULL);
  while(1) {
    printf("Parent running\n");
    gtthread_yield();
  }
  return 0;
}
