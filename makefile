CC = gcc
CFLAGS = -Wall -Wextra

server: server.c
	$(CC) $(CFLAGS) -o server server.c

.PHONY: clean

clean: 
	rm -rf server