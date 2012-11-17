#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"

#define MAX_READERS 3
#define MAX_VALUE 2
pthread_mutex_t lock_inc = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_readers = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_reading = PTHREAD_COND_INITIALIZER;

int current_size = 0;
int current_max_values = 0;
int current_readers = 0;
list* l;

void increment(int x) {
	while (x < 10) {
		printf("%d\n", x);
		x = x + 1;
	}
	printf("succesfully incremented! and done\n");
}

void lazy_job(int worker){
	printf("Thread %3d	CREATED\n", worker);
	int exists;
	int count = 0;
	do{
		exists = 0;
		if (current_readers == MAX_READERS) {
			printf("MAX_READERS		BLOCKED\n");
			pthread_mutex_lock(&lock_readers);
			pthread_cond_wait(&cond_reading, &lock_readers);
			printf("MAX_READERS		UNBLOCKED\n");
		}
		pthread_mutex_lock(&lock_inc);
		current_readers++;
		pthread_mutex_unlock(&lock_inc);
		printf("Thread %3d	PARSING\n", worker);
		elem* e = l->head;
		while(e) {
			if (e->worker == worker) exists = 1;
			e = e->next;
		}
		if (exists) sleep(3);
		sleep(1);
		pthread_mutex_lock(&lock_inc);
		current_readers--;
		pthread_mutex_unlock(&lock_inc);
		// if (exists) {
		// 	printf("Thread %3d	START	\n", worker);
		// 	sleep(2);
		// 	printf("Thread %3d	ENDS	\n", worker);
		// 	pthread_mutex_lock(&lock_inc);
		// 	current_readers--;
		// 	pthread_mutex_unlock(&lock_inc);
		// } else {
		// 	printf("Thread %3d	SKIPPED\n",worker);
		// 	pthread_mutex_lock(&lock_inc);
		// 	current_readers--;
		// 	pthread_mutex_unlock(&lock_inc);
		// }
		pthread_cond_signal(&cond_reading);
		pthread_mutex_unlock(&lock_readers);
		count++;
		if (count>=(worker%5)) break;
	} while (exists);
	printf("Thread %3d	ENDS\n", worker);
	pthread_exit(NULL);
}

int main() {
	l = malloc(sizeof(list));
	l->head = NULL;
	int e,w,i;

	printf("Give number of elements: ");
	scanf("%d",&e);
	printf("Give number of workers: ");
	scanf("%d",&w);

	generate_all(l,e,w);
	print(l);
	printf("sizeof: %d\n",l->size);

	pthread_t da_tred[w];
	for (i = 0; i<w; i++) {
		pthread_create((pthread_t *) & da_tred[i], NULL, (void *) lazy_job, (void*) i);
	}

	// printf("\nwaiting for workers to do their lazy jobs!!\n");
	for (i = 0; i<w; i++) {
		pthread_join(da_tred[i],NULL);
	}
	printf("\n and... their done!\n");
	return 0;
}
