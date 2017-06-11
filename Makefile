client.o:client.c
	gcc $^ -o $@
server.o:server.c
	gcc $^ -o $@ -lwiringPi
clean:
	rm -rf client.o server.o
