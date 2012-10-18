all: client

CC = gcc
CFLAGS = -Wall -g

client: client.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o client
