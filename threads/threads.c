#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// structura

void *myFunction(void *x) {
	int *y = (int *) x;
	int w = *y;
	
	printf("T%d: Incerc sa blochez!\n", w);
	int r = pthread_mutex_trylock(&mutex);	// daca e 0 e libera; != 0 e blocata
	printf("T%d: r = %d"\n,w, r);
	printf("T%d: BLOCAT\n", w);
	w += 5;
	printf("Incep sa dorm %d secunde\n", w);
	sleep(w);
	printf("M-am trezit dupa %d secunde\n", w);
	pthread_mutex_unlock(&mutex);
}

int main() {
	pthread_t threads[3];
	int i;
	
	for (i = 0; i < 3; i++) {
		int rc;
		int sleep = i + 5;
		rc = pthread_create(&threads[i], NULL, myFunction, &sleep);
		
		if (rc != 0) {
			printf("Eroare la crearea thread-ului %d\n", i);
		}
		
		sleep(1);	// doar pt acest exemplu
	}
	
	//printf("Astept dupa thread-uri\n");
	for (i = 0; i < 3; i++) {
		//printf("Astept dupa thread-ul %d\n", i);
		pthread_join(threads[i], NULL);
	}
	
	//printf("Am terminat de asteptat\n");
	


	return 0;
}


// gcc -lpthread ...