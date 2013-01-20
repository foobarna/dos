#include "meetmehere.h"

struct th_arg {
	int index;
	int socket;
	int port;
};

int av_node = 0;
void *msg;
char mtype;
struct msgreg_t r;
struct msggetnod_t t;
struct msgnodlst_t l;
struct sockaddr_in saddr;
pthread_t *client_th[MAX];
pthread_t *node_th[N];
pthread_mutex_t create_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t update_mtx = PTHREAD_MUTEX_INITIALIZER;

int assignme_client() {
	int i;
	pthread_mutex_lock(&create_mtx);
	for(i = 0; i < MAX; i++)
		if (client_th[i] == NULL) {
			pthread_mutex_unlock(&create_mtx);
			return i;
		}
	pthread_mutex_unlock(&create_mtx);
	return -1;
}

int assignme_node() {
	int i;
	pthread_mutex_lock(&create_mtx);
	for(i = 0; i < N; i++)
		if (node_th[i] == NULL) {
			pthread_mutex_unlock(&create_mtx);
			av_node++;
			return i;
		}
	pthread_mutex_unlock(&create_mtx);
	return -1;
}

void node_worker(struct th_arg* arg) {
	//l.addr[arg->index] = saddr;
	l.addr[arg->index].sin_port = arg->port;
	printf("%d\n", ntohs(l.addr[arg->index].sin_port));
}

void client_worker(struct th_arg* arg) {
	strcpy(&l.mtype,"3");
	send(arg->socket, &l, sizeof(struct msgnodlst_t), 0);
	pthread_mutex_lock(&create_mtx);
	free(client_th[arg->index]);
	client_th[arg->index] = NULL;
	close(arg->socket);
	free(arg);
	pthread_mutex_unlock(&create_mtx);
}

int main() {
	int i,sock,rv_sock;
	unsigned int len;
	struct sockaddr_in saddr;

	for(i = 0; i < MAX; i++) {
		client_th[i] = NULL;
	}

	for(i = 0; i < N; i++) {
		node_th[i] = NULL;
	}

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

	if (listen(rv_sock, MAX) < 0) {
		perror ("\nCould not listen to rv sock\n");
		return 1;
	}

	len = sizeof(saddr);

	for(i = 0; i<10; i++) {
		l.addr[i] = saddr;
		l.addr[i].sin_port = htons(0);
	}


	while(1){
		sock = accept(rv_sock, (struct sockaddr *)&saddr, &len);
		if (sock < 0) {
			printf("Some error while accepting connection. Skiped it and moved on.\nGoin'on!\n");
			continue;
		}
		recv(sock, &msg, sizeof(void*), 0);
		memcpy(&mtype, &msg, sizeof(char));
		if (mtype == '2') {
			printf("A client connected!\n");
			if (av_node == 0) {
				printf("Can not handle client! Nodes not available!\n");
				strcpy(&l.mtype, "6");
				send(sock, &l, sizeof(struct msgnodlst_t), 0);
				close(sock);
				continue;
			}
			i = assignme_client();
			if (i == -1) {
				printf("Can not handle client! No more room for threads!\n");
				strcpy(&l.mtype, "6");
				send(sock, &l, sizeof(struct msgnodlst_t), 0);
				close(sock);
				continue;
			}
			struct th_arg *arg = (struct th_arg*)malloc(sizeof(struct th_arg));
			arg->index = i;
			arg->socket = sock;
			client_th[arg->index] = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_create(client_th[arg->index], NULL, (void *)client_worker, arg);

		}
		if (mtype == '1') {
			printf("A node connected\n");
			i = assignme_node();
			if (i == -1) {
				printf("Can not handle node! No more room for threads!\n");
				// send(sock, &error, sizeof(struct msgerror_t), 0);
				close(sock);
				continue;
			}
			memcpy(&r, &msg, sizeof(struct msgreg_t));
			struct th_arg *arg = (struct th_arg*)malloc(sizeof(struct th_arg));
			arg->index = i;
			arg->socket = sock;
			arg->port = r.rvport;
			node_th[arg->index] = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_create(node_th[arg->index], NULL, (void *)node_worker, arg);
		}
	}
	return 0;
}