#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "list.h"

#define MAX_PARSERS 3
#define MAX_VALUE 35
#define MAX_ELEMS 5

pthread_mutex_t parser_mutex[MAX_PARSERS];
pthread_mutex_t deleting_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t inc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_deleting2 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_deleting2 = PTHREAD_COND_INITIALIZER;

int current_max_elems = 0;
int current_readers = 0;
list* l;


int get_parsing_mutex() {
	int i,x = -1;
	while (1) {
		if (pthread_mutex_trylock(&deleting_mutex) == 0)
			pthread_mutex_unlock(&deleting_mutex);
		else continue;
		x = -1;
		for (i = 0; i < MAX_PARSERS; i++){
			if (pthread_mutex_trylock(&parser_mutex[i]) == 0) {
				x = i;
				break;
			}
		}
		if (x != -1) break;
	}
	return x;
}

void block_parsers() {
	int i,locks = 0;
	while (locks < MAX_PARSERS) {
		for (i = 0; i < MAX_PARSERS; i++){
			if (pthread_mutex_trylock(&parser_mutex[i]) == 0) {
				locks++;
			}
		}
	}
}

void unblock_parsers() {
	int i;
	for (i = 0; i < MAX_PARSERS; i++)
		pthread_mutex_unlock(&parser_mutex[i]);
}


void lazy_job(int worker){
	printf("Thread %3d	CREATED\n", worker);
	int mutex_no;
	do{
		printf("Thread %3d	BLOCKED\n",worker);

		mutex_no = get_parsing_mutex();

		printf("Thread %3d	UNBLOCKED on mutex %d\n",worker,mutex_no);

		printf("Thread %3d	PARSING\n", worker);

		elem* e = l->head;
		while(e) {
			if (e->worker == worker) {
				e->value = sqrt(e->value);
			}
			if (e->value <= MAX_VALUE) {
				pthread_mutex_lock(&inc_mutex);
				current_max_elems++;
				pthread_mutex_unlock(&inc_mutex);
			}
			if ((current_max_elems >= MAX_ELEMS)||(current_max_elems >= l->size) ) pthread_cond_signal(&cond_deleting2);
			e = e->next;
		}

		printf("Thread %3d	DONE parsing\n", worker);

		pthread_mutex_unlock(&parser_mutex[mutex_no]);
	} while (1);
}

int main() {
	int e,w,i,count_del=0;

	for(i = 0; i < MAX_PARSERS; i++) {
		pthread_mutex_init(&parser_mutex[i], NULL);
	}

	l = malloc(sizeof(list));
	l->head = NULL;

	printf("Give number of elements: ");
	scanf("%d",&e);
	printf("Give number of workers: ");
	scanf("%d",&w);

	generate_all(l,e,w);
	print(l);

	pthread_t da_tred[w];
	for (i = 0; i<w; i++) {
		pthread_create((pthread_t *) & da_tred[i], NULL, (void *) lazy_job, (void*) i);
	}

	while(l->head != NULL){
		printf("MAIN Thread WAITING for signal\n");
		pthread_cond_wait(&cond_deleting2,&lock_deleting2);
		pthread_mutex_lock(&deleting_mutex);
		block_parsers();
		printf("MAIN Thread DELETING from list\n");
		count_del++;
		delete_all(l,MAX_VALUE);
		current_max_elems = 0;
		print(l);
		pthread_mutex_unlock(&deleting_mutex);
		unblock_parsers();
	}

	printf("MAIN Thread DONE deleting list, so it starts KILLING\n");

	for (i = 0; i<w; i++) {
		pthread_cancel(da_tred[i]);
	}

	print(l);
	printf("deleted %d times\n", count_del);
	return 0;
}
