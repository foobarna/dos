#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cnd = PTHREAD_COND_INITIALIZER;

int counter = 0;

void inc(int c){
	int i;
	while (1) {
	for (i = 0; i < 1; i++) {

		pthread_mutex_lock(&mtx);
		counter++;
		printf("inc: %d by %d\n", counter,c);
		if (counter % 13 == 0) pthread_cond_signal(&cnd);

		pthread_mutex_unlock(&mtx);
		sleep(1.5+c%2);
	}
	// pthread_mutex_lock(&mtx);
	// printf("----%d-----\n",counter);
	// pthread_mutex_unlock(&mtx);
	}
}


void inc2() {
	int i;
	// pthread_mutex_lock(&mtx);
	while (1) {
	pthread_cond_wait(&cnd, &mtx);
	for (i= 0; i < 3; i++) {
		printf("------------------multiple of 13\n");
		// counter++;
	}
}
	// pthread_mutex_unlock(&mtx);
	// pthread_cond_signal(&cnd);
}

int main() {
	pthread_t t1[10],t2;
	int i;
	for(i=0; i <10 ; i++) {
		pthread_create((pthread_t *) & t1[i], NULL, (void *) inc,(void*) i);
	}
	pthread_create((pthread_t *) & t2, NULL, (void *) inc2, NULL);

	for(i=0; i <5 ; i++) pthread_join(t1[i],NULL);
	pthread_join(t2,NULL);

	printf("They are done\n");
	return 0;
}
