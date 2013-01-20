build:
	gcc -Wall -o client client.c -lpthread
	gcc -Wall -o hub hub.c -lpthread
	gcc -Wall -o node node.c -lpthread

web: build
	./hub

clean:
	-rm client
	-rm hub
