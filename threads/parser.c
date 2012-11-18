#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "list.h"

#define MAX_READERS 3
#define MAX_VALUE 2
#define MAX_ELEMS 5
pthread_mutex_t lock_deleting = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_inc = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_readers = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_reading = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_deleting = PTHREAD_COND_INITIALIZER;

int current_max_elems = 0;
int current_size = 0;
int current_max_values = 0;
int current_readers = 0;
list* l;

void lazy_job(int worker){
	printf("Thread %3d	CREATED\n", worker);
	int exists;
	int count = 0;
	do{
		exists = 0;

		if (current_readers >= MAX_READERS) {
			printf("MAX_READERS		BLOCKED\n");
			pthread_mutex_lock(&lock_readers);
			pthread_cond_wait(&cond_reading, &lock_readers);
			pthread_mutex_trylock(&lock_deleting);
			printf("MAX_READERS		UNBLOCKED\n");
		}
		pthread_mutex_lock(&lock_inc);
		current_readers++;
		pthread_mutex_unlock(&lock_inc);
		printf("Thread %3d	PARSED\n", worker);
		elem* e = l->head;
		while(e) {
			if ((e->worker == worker) && (e->value > MAX_VALUE)) {
				exists = 1;
				e->value = sqrt(e->value);
				// if (e->value <= MAX_VALUE) current_max_elems++;
				// if ((current_max_elems >= MAX_ELEMS)||(current_max_elems >= l->size) ) pthread_cond_signal(&cond_deleting);

			}
			e = e->next;
		}
		pthread_mutex_lock(&lock_inc);
		current_readers--;
		pthread_mutex_unlock(&lock_inc);


		pthread_mutex_unlock(&lock_readers);
		pthread_cond_signal(&cond_reading);
		pthread_mutex_unlock(&lock_deleting);


		count++;
	} while (exists);
	printf("Thread %3d	ENDS	%3d parses\n", worker, count);
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

	printf("\nwaiting for workers to do their lazy jobs!!\n");
	while(1){
		pthread_cond_wait(&cond_deleting,&lock_deleting);
		pthread_mutex_lock(&lock_deleting);
		printf("deleting...\n");
		delete_all(l,MAX_VALUE);
		current_max_elems = 0;
		print(l);
		if (l->size <= 1) break;
		pthread_mutex_unlock(&lock_deleting);
	// 	// pthread_cond_signal(&cond_deleting);
	}

	for (i = 0; i<w; i++) {
		pthread_cancel(da_tred[i]);
	}

	printf("\n and... their done!\n");
	print(l);
	return 0;
}
