#include "meetmehere.h"

struct arg {
	int index;
	int socket;
};

pthread_t *th[100];
pthread_mutex_t create_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t update_mtx = PTHREAD_MUTEX_INITIALIZER;
struct largest l;
struct sockaddr_in saddr;

int assignme() {
	int i;
	pthread_mutex_lock(&create_mtx);
	for(i = 0; i < MAX; i++)
		if (th[i] == NULL) {
			pthread_mutex_unlock(&create_mtx);
			return i;
		}
	pthread_mutex_unlock(&create_mtx);
	return -1;
}

void work(struct arg *a) {
	struct triplet t;
	int sum;

	recv(a->socket, &t, sizeof(struct triplet), 0);
	t.a = ntohl(t.a);
	t.b = ntohl(t.b);
	t.c = ntohl(t.c);
	printf("%d %d %d\n", t.a, t.b, t.c);
	// sleep(3);
	sum = t.a + t.b + t.c;
	pthread_mutex_lock(&update_mtx);
	if(sum > l.sum) {
		l.sum = sum;
		l.addr = saddr;
	}
	l.sum = htonl(l.sum);
	send(a->socket, &l, sizeof(struct largest), 0);
	l.sum = ntohl(l.sum);
	pthread_mutex_unlock(&update_mtx);
	pthread_mutex_lock(&create_mtx);
	free(th[a->index]);
  	th[a->index] = NULL;
	close(a->socket);
	free(a);
	pthread_mutex_unlock(&create_mtx);
}


int main() {
	int i,sock,rv_sock;
	unsigned int len;

	/*th = (pthread_t*) malloc(sizeof(pthread_t)*MAX);*/
	for(i = 0; i < MAX; i++) {
		// th[i] = (pthread_t*) malloc(sizeof(pthread_t));
		th[i] = NULL;
	}

	l.sum 	= 0;
	// l.ip 	= 0;
	// l.port 	= 0;

	rv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (rv_sock < 0) {
		perror("\nCould not make a socket\n");
		return 1;
	}
	memset(&saddr,0,sizeof(saddr));

	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons (PORT);
	saddr.sin_family = AF_INET;

	if (bind (rv_sock, (struct sockaddr *) &saddr, sizeof (saddr)) < 0) {
		perror("\nCould not bind the rv socket\n");
		return 1;
	}

	if (listen(rv_sock, N) < 0) {
		perror ("\nCould not listen to rv sock\n");
		return 1;
	}

	len = sizeof(saddr);

	while(1) {
		sock = accept (rv_sock, (struct sockaddr *)&saddr,  &len);
		if(sock < 0) continue;
		printf("Accept connection from ip %s and port %d\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
		i = assignme();
		if (i < 0) continue;
		struct arg *a = (struct arg*)malloc(sizeof(struct arg));
		a->index = i;
		a->socket = sock;
		th[a->index] = (pthread_t*)malloc(sizeof(pthread_t));
		pthread_create(th[a->index], NULL, (void *)work, a);
	}

	return 0;	
}
