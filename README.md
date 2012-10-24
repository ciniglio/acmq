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

#### Credits

credits to **@itairos** for inspiring me to be asynchronous.
