// Test10
// Producer and consumer.

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <gtthread.h>

#define LOOP 100

gtthread_t g_th1, g_th2;
gtthread_mutex_t g_mutex;
int g_num = 0;

void* producer(void* arg)
{
        printf("Should not print any errors.\n");
	int i, j;

	for(i=0; i < LOOP; i++)
	{
		gtthread_mutex_lock(&g_mutex);
		++g_num;
		gtthread_mutex_unlock(&g_mutex);

		for(j=0; j < rand() % 999999; ++j);
	}
        return NULL;
}

void* consumer(void* arg)
{
	int i, j;

	for(i=0; i < LOOP; i++)
	{
		gtthread_mutex_lock(&g_mutex);
		--g_num;
		gtthread_mutex_unlock(&g_mutex);

		for(j=0; j < rand() % 999999; ++j);
	}
        return NULL;
}

int main()
{
	gtthread_t th1, th2;

	srand(time(NULL));

	gtthread_init(1000);

	gtthread_mutex_init(&g_mutex);

	gtthread_create(&th1, producer, NULL);
	gtthread_create(&th2, consumer, NULL);

	gtthread_join(th1, NULL);
	printf("Join 1\n");
	gtthread_join(th2, NULL);
	printf("Join 2\n");

	if (g_num != 0) {
		fprintf(stderr, "!ERROR! Wrong result! %d != 0\n", g_num);
	}
	return 0;
}
