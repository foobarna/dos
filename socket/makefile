build:
	gcc -Wall -o client client.c
	gcc -Wall -o server server.c -lpthread

web: build
	./client

many: build
	./server
	./client
	./client
	./client
	./client

clean:
	-rm client
	-rm server
