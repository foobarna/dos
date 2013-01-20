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

#define PORT 31114
#define IP "127.0.0.1"
#define MAX 100
#define N 5

struct triplet {
	int a,b,c;
};

struct largest {
	int sum;
	struct sockaddr_in addr;
};