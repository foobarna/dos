#include "meetmehere.h"

struct arg {
	int index;
	int socket;
};

char dir[50];
pthread_t *th[100];
pthread_mutex_t create_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t update_mtx = PTHREAD_MUTEX_INITIALIZER;

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
	struct msggetfile_t f;
	struct msggetdata_t d;
	char filename[150];
	FILE *file;
	
	while(1) {
		recv(a->socket, &f, sizeof(struct msggetfile_t), 0);
		sprintf(filename,"%s/%s",dir,f.filename);
		printf("%s\n",filename );
		if(f.mtype != '4') break;
		file = fopen(filename,"r");
		fseek(file, ntohl(f.from), SEEK_SET);
		d.length = fread(&d.data,1,100,file);
		fclose(file);
		d.length = htons(d.length);
		send(a->socket, &d, sizeof(struct msggetdata_t), 0);
	}
	printf("Done with this client..\n");
	pthread_mutex_lock(&create_mtx);
	free(th[a->index]);
  	th[a->index] = NULL;
	close(a->socket);
	free(a);
	pthread_mutex_unlock(&create_mtx);
}

int main(int argc, char *argv[]) {
	int sock,rv_sock,cl_sock,i;
	unsigned int len;
	struct sockaddr_in saddr;
	struct sockaddr_in naddr;
	struct msgreg_t r;

	if(argc < 2) {
		printf("Wrong usage!\n");
		return 0;
	}

	strcpy(dir, argv[1]);

	for(i = 0; i < MAX; i++) {
		th[i] = NULL;
	}

	rv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (rv_sock < 0) {
		perror("\nCould not make a socket\n");
		return 1;
	}
	memset(&naddr,0,sizeof(naddr));

	naddr.sin_addr.s_addr = INADDR_ANY;
	naddr.sin_port = htons(0);
	naddr.sin_family = AF_INET;	

	if (bind (rv_sock, (struct sockaddr *) &naddr, sizeof (naddr)) < 0) {
		perror("\nCould not bind the rv socket\n");
		return 1;
	}

	if (listen(rv_sock, MAX) < 0) {
		perror ("\nCould not listen to rv sock\n");
		return 1;
	}

	len = sizeof(naddr);


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("\nCould not make a socket\n");
		return 1;
	}

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(IP);
	saddr.sin_port = htons(PORT);

	if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < -1) {
		perror("\nCould not connect to server\n");
		return 1;
	}
	
	getsockname(rv_sock,(struct sockaddr *)&naddr,&len);
	strcpy(&r.mtype, "1");
	r.rvport = naddr.sin_port;
	printf("%d\n", ntohs(r.rvport));
	send(sock, &r, sizeof(struct msgreg_t), 0);

	while(1) {
		cl_sock = accept(rv_sock, (struct sockaddr*)&naddr, &len);
		if(cl_sock < 0) continue;
		i = assignme();
		if (i < 0) continue;
		struct arg *a = (struct arg*)malloc(sizeof(struct arg));
		a->index = i;
		a->socket = cl_sock;
		th[a->index] = (pthread_t*)malloc(sizeof(pthread_t));
		pthread_create(th[a->index], NULL, (void *)work, a);
	}

	close(rv_sock);
	close(sock);
	return 0;
}