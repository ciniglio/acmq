all: mq_client

CC = gcc
CFLAGS = -Wall -g

mq_client: client.o mq_client.o
	$(CC) $(CFLAGS) $^ -o $@

client: client.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o client
