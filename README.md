# A C Message Queue

###A simple message queue written in C. 

## Components

### Server

The server (`server.c`) is written to be async and handle any generic
callback function. This allows it to wrap things that aren't queues
(e.g. a bloom filter).

The router component (`mq_router.c`) is what maps strings received by
the server to commands from the data structure. To use a different
data structure, a new router needs to be created.

`Main.c` ties the server to the router and data structure, by calling
`create_server` and passing the router handler function, along with a
pointer to the data structure as `(void *) state`. 

### Client

The client (`client.c`) is also written to be generic, and simply
sends strings to a specified socket.

`mq_client` maps intents to strings. So, for example, a call to `push("this")` will
send the string `"PUSH this"` to the client which will send it to the
server. 

`cmd.c` is a simple command line app that wraps mq_client. E.g. `$ cmd
-u "This long string"` will send the command `"PUSH This long string"`
to the server via the client's socket.

## Notes about the protocol

### Definition

Defintion of the protocol happens in the router, and the client must
adhere to it. The server will append a `NULL` to the end of the
recieved buffer, so string operations on that buffer should be
generally safe. 

### MQ Protocol

The protocol that the message queue adheres to has statements that are
delimited by newlines (`\n`). This means that input with newlines will
be interpreted incorrectly, and information after the newline is
likely to be lost. For this reason, it's recommended that you use JSON
to encode and decode the messages.

#### Push

The push command takes the form "`PUSH $body`". `$body` will then be
added to the queue.

#### Pop

The pop command takes the form "`POP`" with no arguments. The server
will then return the message at the front of the queue.

## Persistence

There is a persistence layer that each structure is responsible
for. The message queue structure implements this and serializes the
queue to disk after each transaction. Upon starting the program, we
will attempt to deserialize the queue from disk if possible. 

### `Persistence.c`

This file has helper functions that make it easy to read and write
from disk. Each structure should have methods to serialize and
deserialize the structure to and from a `char *` buffer, respectively.

For an example usage, see `mq_persist.c`.

#### Credits

credits to **@coreylynch** for the bloom* files and implementation.
credits to **@itairos** for inspiring me to be asynchronous.

