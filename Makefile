all: main

CC = gcc
CFLAGS = -Wall -g

queue_HDR = queue.h

main: server.o mq_router.o queue.o main.o
	$(CC) $(CFLAGS) $^ -o $@

strip: server.o mq_router.o
	$(CC) $(CFLAGS) $^ -o $@

queue: queue.o $(queue_HDR)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o queue strip main
