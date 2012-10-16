all: queue

CC = gcc
CFLAGS = -Wall

queue_HDR = queue.h

queue: queue.o $(queue_HDR)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o queue
