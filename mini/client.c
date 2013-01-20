#include "meetmehere.h"

struct arg {
	int index;
	// struct msggetfile_t f;
	struct sockaddr_in addr;
};
char filename[100];
char dest[100];
int av_node = 0;
struct msggetfile_t f2;
pthread_t *th[10];
pthread_mutex_t create_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t update_mtx = PTHREAD_MUTEX_INITIALIZER;


int assignme() {
	int i;
	pthread_mutex_lock(&create_mtx);
	for(i = 0; i < 10; i++)
		if (th[i] == NULL) {
			pthread_mutex_unlock(&create_mtx);
			return i;
		}
	pthread_mutex_unlock(&create_mtx);
	return -1;
}

void work(struct arg *a) {
	int sock,i;
	struct sockaddr_in saddr;
	struct msggetfile_t f;
	struct msggetdata_t d;
	FILE *file;


	printf("%d+\n", a->index);
	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = a->addr.sin_addr.s_addr;
	saddr.sin_port = a->addr.sin_port;

	if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < -1) {
		perror("\nCould not connect to server\n");
		// return 1;
	}

	i = 0;
	strcpy(f.filename,filename);
	strcpy(&f.mtype,"4");

	printf("%s\n",f.filename );
	while(1) {
		f.from = htonl(i*av_node*100);
		file = fopen(dest,"w");

		send(sock, &f, sizeof(struct msggetfile_t), 0);
		recv(sock, &d, sizeof(struct msggetdata_t), 0);
		
		if(ntohs(d.length) == 0) break;
		
		fseek(file,ntohl(f.from),SEEK_SET);
		fwrite(d.data,1,ntohs(d.length),file);
		i++;
		
	}
	strcpy(&f.mtype,"6");
	send(sock, &f, sizeof(struct msggetfile_t), 0);
	fclose(file);
	close(sock);
	pthread_mutex_lock(&create_mtx);
	free(th[a->index]);
  	th[a->index] = NULL;
	free(a);
	pthread_mutex_unlock(&create_mtx);
}


int main(int argc, char *argv[]) {
	int sock,i,index;
	struct sockaddr_in saddr;
	struct msggetnod_t t;
	struct msgnodlst_t l;

	if(argc < 2) {
		printf("Wrong usage!\n");
		return 0;
	}

	for(i = 0; i < MAX; i++) {
		th[i] = NULL;
	}

	strcpy(filename,argv[1]);
	strcpy(dest,argv[1]);

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
	
	strcpy(&t.mtype, "2");
	send(sock, &t, sizeof(struct msggetnod_t), 0);


	recv(sock, &l, sizeof(struct msgnodlst_t), 0);
	if (l.mtype != '3') {
		printf("Error! Maybe no nodes!?\n");
		close(sock);
		return 0;
	}

	for(i = 0; i < 10; i++){
		if (ntohs(l.addr[i].sin_port) != 0) av_node++;
		printf("%d ",ntohs(l.addr[i].sin_port));
	}
	printf("\n");

	for(i = 0;i < 10; i++) {
		if (ntohs(l.addr[i].sin_port) != 0) {
			index = assignme();
			if (index < 0 ) printf("asdasd\n");
			struct arg *a = (struct arg*)malloc(sizeof(struct arg));
			a->index = index;
			a->addr = l.addr[i];
			// sprintf(a->f.filename,"%s", filename);
			// printf("%s\n",a->f.filename );
			th[a->index] = (pthread_t*)malloc(sizeof(pthread_t));
			pthread_create(th[a->index], NULL, (void *)work, a);
		}
	}

	printf("Downloading...\n");
	for(i=0; i<MAX; i++) {
    	pthread_t* t = th[i];
    	if(t != NULL) {
      		pthread_join(*t, NULL);
    	}
  	}
  	printf("Done!\n");
	close(sock);
	return 0;
}
