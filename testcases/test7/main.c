// Test7
// gtthread_cancel. The program should terminate smoothly.

#include <stdio.h>
#include <gtthread.h>

void* worker2(void* arg)
{
	while(1);
}

void* worker(void* arg)
{
	void *ret;
	gtthread_t th;

	gtthread_create(&th, worker2, NULL);
	printf("Worker 2 created\n");
	gtthread_cancel(th);
	printf("Worker 2 canceled\n");
	gtthread_join(th, &ret);
	printf("Worker 2 returned: %i\n",(int) ret);
	while(1);
}

int main()
{
	void *ret;
	gtthread_t th;

	gtthread_init(1);
	gtthread_create(&th, worker, NULL);
	printf("Worker 1 created\n");
	gtthread_cancel(th);
	printf("Worker 1 canceled\n");
	gtthread_join(th, &ret);
	printf("Worker 1 returned: %i\n",(int) ret);
	return 0;
}
