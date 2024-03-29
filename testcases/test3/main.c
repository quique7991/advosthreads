// Test3
// Main exit. Program must terminate smoothly.

#include <stdio.h>
#include <gtthread.h>

void* worker(void* arg)
{
	gtthread_join(0,NULL);
}

int main()
{
	gtthread_t th1;

	gtthread_init(1000);

	gtthread_create(&th1, worker, NULL);

	printf("main exit\n");

	gtthread_exit(NULL);
	return 0;
}
