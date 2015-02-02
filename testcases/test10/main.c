
#include <stdio.h>
#include <time.h>
#include <gtthread.h>

#define LOOP 10000 

gtthread_t g_th1, g_th2;
gtthread_mutex_t g_mutex;
int producing = 1;
int g_num = 0;

void* producer(void* arg)
{
	int i, j,ran;

	for(i=0; i < LOOP; i++)
	{
		gtthread_mutex_lock(&g_mutex);
		++g_num;
		printf("Producer %lu produced\n",gtthread_self());
		gtthread_mutex_unlock(&g_mutex);
		ran = rand()%99999; 
		for(j=0; j < ran; ++j);
	}
	producing=0;
	gtthread_exit(NULL);
}

void* consumer(void* arg)
{
	int j,ran;
	printf("It is consumer %lu\n", gtthread_self());
	while(1)
	{
		gtthread_mutex_lock(&g_mutex);
			if(g_num > 0){
				--g_num;
				printf("Consumer %lu consumed\n",gtthread_self());
			}
		gtthread_mutex_unlock(&g_mutex);
		ran = rand()%99999;	
		for(j=0; j < ran; ++j);
		gtthread_mutex_lock(&g_mutex);	
		if(!producing && !g_num){
	                gtthread_mutex_unlock(&g_mutex);
			break;
		}
                gtthread_mutex_unlock(&g_mutex);
	}
	gtthread_exit(NULL);
}

int main()
{
	gtthread_t th1, th2,th3;

	srand(time(NULL));

	gtthread_init(100);

	gtthread_mutex_init(&g_mutex);
	gtthread_create(&th1, &producer, NULL);
	gtthread_create(&th2, &consumer, NULL);
	gtthread_create(&th3, &consumer, NULL);


	gtthread_join(th1, NULL);
	printf("Thread 1 joined\n");
	gtthread_join(th2,NULL);
	printf("Thread 1 joined\n");
	gtthread_join(th3,NULL);
	printf("Thread 1 joined\n");

	if (g_num != 0) {
		fprintf(stderr, "!ERROR! Wrong result! %d != 0\n", g_num);
	}
	return 0;
}
