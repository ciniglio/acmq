all: cmd

CC = gcc
CFLAGS = -Wall -g

cmd: cmd.o client.o mq_client.o
	$(CC) $(CFLAGS) $^ -o $@

mq_client: client.o mq_client.o
	$(CC) $(CFLAGS) $^ -o $@

client: client.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o client
