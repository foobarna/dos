#include "meetmehere.h"

int main() {
	int sock;
	struct sockaddr_in saddr;
	struct triplet t;
	struct largest l;

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

	srand(t.a+t.b+t.c);
	t.a = htonl(rand()%100);
	srand(t.a+t.b+t.c);
	t.b = htonl(rand()%100);
	srand(t.b+t.c);
	t.c = htonl(rand()%100);
	
	send(sock, &t, sizeof(struct triplet), 0);

	t.a = ntohl(t.a);
	t.b = ntohl(t.b);
	t.c = ntohl(t.c);

	printf("I send: %2d %2d %2d\n", t.a, t.b, t.c);

	recv(sock, &l, sizeof(struct largest), 0);
	
	l.sum = ntohl(l.sum);
	// l.port = ntohs(l.port);
	// l.ip = ntohl(l.ip);
	
	printf("The largest sum is %d from ip %s with port %d\n", l.sum, inet_ntoa(l.addr.sin_addr), ntohs(l.addr.sin_port));

	close(sock);
	return 0;
}
