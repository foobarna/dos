#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 33311
#define IP "127.0.0.1"
#define MAX 100
#define N 10

//type = 6
//this gets client/node when an error occurs. And the specified error.
struct msgerror_t {
	char mtype;
	char error[100];
};

//sent from node to hub
//type = 1
struct msgreg_t {
	char mtype;
	short rvport;
};

//sent from clients to hub
//type = 2
struct msggetnod_t {
	char mtype;
};

//sent by the hub to the req client
//type = 3
struct msgnodlst_t {
	char mtype;
	struct sockaddr_in addr[10];
};

//sent by the client to the node
//type = 4
struct msggetfile_t {
	char mtype;
	long from;
	char filename[100];
};

//sent by the node to the req client
//type = 5
struct msggetdata_t {
	char mtype;
	char data[100];
	int length;
};
