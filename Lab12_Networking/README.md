# This is an optional ungraded lab for your learning

# Lab 12 Networking

The Internet (with a capital "I") is full of many processes running. We have previously learned in lecture how we connect to different ports in a router, and that routes packets of information across the internet. The other tool we have learned about, is a _socket_. Sockets allow for an end-to-end connection between two processes. Most frequently, sockets are used to connect a server and client process together.

## Task 1

<img src="https://www.tutorialspoint.com/unix_sockets/images/socket_client_server.gif" alt="Client-Server Model flow chart" width="350px" align="right">

The first task, is to understand the client-server model in regards to sockets.

The following flow chart (on the right) shows a nice breakdown of what the client and server do.


In short, a server:
1. Creates a socket
2. Binds a socket
3. Listens for connections to the socket
4. Accepts connections

The client:
1. Creates a socket
2. Connects to some other connection (e.g. a server)

Once a successful connection is made, a client and server can read and write messages to each other.

## Task 2

First we need to understand our tools available to us.

Skim the man pages for the following(It will be helpful to refer to them as you go through the exercise):

- `man socket`
- `man connect`
- `man listen`
- `man send`
- `man recv`
- `man 2 bind` (Note this opens up the version of 'bind' from the man page related to system calls. That's what the numbers mean next to the man pages).

```
MANUAL SECTIONS
    The standard sections of the manual include:

    1      User Commands
    2      System Calls
    3      C Library Functions
    4      Devices and Special Files
    5      File Formats and Conventions
    6      Games et. al.
    7      Miscellanea
    8      System Administration tools and Daemons
```


## Task 3

Let's go ahead and get started by building a little tcp server that runs and accepts connections from clients.

Create a C file called tcpserver.c

**Goal**: Build a tcp server that accepts a connection from a client.

### Step 1
The first thing we are going to need is some header files to provide us with networking libraries (this is for both our client and server).

```
// Our standard libraries
#include <stdio.h>
#include <stdlib.h>

// Some types and unix operations
#include <sys/types.h>
#include <unistd.h>

// A sockets library and internet protocol library
#include <sys/socket.h>
#include <netinet/in.h>
```

### Step 2

Now lets create a server socket:
```
	// Create the server socket
	int server_socket;
	//			domain, type 	    ,protocol
	// domain = AF_INET(IPv4) or AF_INET6			
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
```

socket takes three parameters.

- Domain: Which domain are we communicating over. Examples are AF_INET for IPv4 or AF_INET6 for IPv6
- Type: The type specifies how the communication will occur. SOCK_STREAM means there is a two-way connection using TCP. The communication is reliable and in-order.
- Protocol: This specifies which protocol we are using. Typically we set this to 0, which means only 1 protocol exists for communciation (you could have more in theory).

### Step 3
After we have created a socket, we need to figure out what addresse we are going to serve communications from.
To do this, we create a `sockaddr_in` struct(man 7 ip) which holds information about our server address format.

We need to make sure we match the protocol(`IF_INET`), set a port number that is not used (typically not a well known port), and then an internet address(Let's use `INADDR_ANY` which represents localhost for our machine). 


```
	// define the server address format
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002); // 9002 is choosen arbitrarily. Try other ports if this does not work
	server_address.sin_addr.s_addr = htons(INADDR_ANY);
```

> Note that the address format here that we are using, must also be matched with our clients.


### Step 4

Now that we have an address where we would like our server to exist we want to `bind` it to a socket. That is, our socket is created, but it does not yet know where to send and receive information from.

```
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	printf("Server bind()\n");
```

### Step 5

So our server is now connected to a specific port within the localhost. Since this is a server, if we review our architecture, we want clients to be able to connect. The tool for doing this is the `listen` function.

listen takes two parameters, one for a socket descriptor, and another for how many client connections can be queued to join. In this case, I have set 27, such that 27 clients could be queued up in the server to connect.

```
	listen(server_socket, 27);
```

### Step 6

For the final step, our server should accept connections from clients. That is, a client will attempt to `connect` to a server, and if this happens we will send the client a message (and then our server will shutdown).


```
	// Integer to hold a potential client socket
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);

	// send a message from the server
	char server_message[256] = "You have reached the server";

	send(client_socket, server_message, sizeof(server_message), 0);

	// Close the socket
	close(server_socket);
```


Make sure all of this code is within a main function, and then give it a test!

If your server is running, it will wait patiently forever (i.e. not do anything useful).

## Task 4

Now that our server has been built, lets build a client in a separeate c file called tcpclient.c.


### Step 1
The first step will be to load up our header files again.

```
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>
```

### Step 2
Just like with our server, we are going to create a socket. The semantics will look very similar.

```
	// Create a socket
	int client_socket;
	// Note that the protocol matches our server here
	// Also note I am doing some error checking for my socket.
	// You may consider updating your server code to do the same.
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
	}
```

### Step 3
Now we need to specify where we want to connect to. Take a moment to guess what the address format should be.







```
	// Our client will connect to a server with the following address and port.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET; // 32-bit IPV4 address
	server_address.sin_port = htons(9002); // convert to the appropriate port number using htons
	server_address.sin_addr.s_addr = htons(INADDR_ANY); // Our ip address, which is localhost using INADDR_ANY
```

### Step 4
Now that we have created a socket and have specified where we want to connect to, let's try to connect.

```
	// Attempt to connect
	// Returns an integer as an error code (zero means everything worked)
	int connection_status;

	// Report back to user if our connection was successful
	if( (connection_status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address))) <0 ){
		printf("%d Connection unsuccessful\n", connection_status);
		close(client_socket);
		exit(0);	
	}else{
		printf("Connected successfully\n");
	}
```

If the connection is received successfully, then we should see a print out of the status. Otherwise, our program will terminate.


### Step 5

Finally, when our client connects, the client can receive a message from the server and then exit.

```
// Receive data from the server
	char server_response[256];
	recv(client_socket, &server_response, sizeof(server_response), 0);
	
	// Print out the server's response
	printf("The server sent the data: %s\n",server_response);

	// Then close the socket
	close(client_socket);
```


## Task 5

### Step 1
Let us update our client to add some more customization. That is, when we launch the tcpclient, a user should enter their name.


### Step 2
When the server receives a connection from the client, the client name should be printed out.

The simplest way to do this would be to send the client name along with each message.


## Task 6


Now the problem with our tcp server is that it listens for a client to connect, but then it disconnects after a client has joined.

Let us update our server so that it runs forever(at least, in theory).

For the server, we can update this by using a while loop that runs forever.

That is, once our client connection is accepted, we continuously attempt to receive messages.

```

// Fill in the code here
while(???){
	printf ???
}
```

On the client end, there is additionally al oop where we can continously send messages. Update your code to do the following:

```

Stay in an infinite loop (or until some key is pressed)

enter a message (with the clients username appended)

Send a message across that socket

```

After this is completed, you will have setup a nice 1-1 connection using sockets. The last task links to a resource for handling multiple sockets.


## Task 7

Often we will want to build little servers so that we can test our web applications on our machines. With this in mind, we will do the following:

Create a file called `http_server.c` for this task.

1. First, we will need to run locally (if you are ssh'd, then you will need to run this exercise outside of ssh on your local machine).
2. Second, we should create a minimal html page example. Create the following html page below
```
<html><body>Welcome to the HTTP Server!</body></html>

```
3. Now we are ready to build our http server.


### Step 1

In order to run our http_server to serve html web pages, we will first need to have some html data to read in.

Read in an a file using the following:

```
	// Open a file to serve
	FILE* html_data;
	html_data=fopen("index.html","r");

	char response_data[1024];
	fgets(response_data, 1024, html_data);

	// Lets client know data is okay
	char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
	
	// http_header stores the data that we will send.
	strcat(http_header, response_data);
```

### Step 2

Next, just as in our above examples, we will need to create a socket. The port you will connect to is 8001.

And remember, because this is a server, our next steps are to bind, and then listen over a server socket.

```

Create a socket

Then define the address format

Bind our server_socket

Listen


```


### Step 3

Finally, we will have a while loop that accepts a connection when html is served.

```
int client_socket;

	while(1){
		client_socket = accept(server_socket, NULL, NULL);
		send(client_socket, http_header, sizeof(http_header),0);
		close(client_socket);
	}

```


### Step 4

1. Finally, to test if this works, fire up your browser on your machine(locally).
2. Run your code (locally on your machine--NOT over ssh)
3. Then navigate to your browser to localhost:8001.


If you have completed this step, you now have a simple http server--congratulations!

## Task 8

Now that we have written an http server which can serve pages, we will write an http client to get data and return it.

Create a new file called `http_client.c` for this task.

## Step 1

Let us write a command-line program that takes in an ip address and serves some data to us.

(Include the other headers and add a main as normal)

This time however, there is one additional header we need to include for working with addresses.
```
#include <arpa/inet.h>
```


```
// Store address from our first parameter
	char* address;
	address = argv[1];

	if(argc <= 0){
		printf("No arguments!\n");
		exit(0);
	}

	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM,0);
```

Now when we build our socket, what will we have to change here?

```
// connect to an address
	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(80); // What do you notice that is different here? Why is this 80
	// Conversion of address
	inet_aton(address, &remote_address.sin_addr.s_addr);
```

Next we will connect to our client as normal.

```
write the code to connect to client (1 line)
```

Now the difference in this application, is that we are sending a request to get information.

When we get a response, we will then receive the html from that page.

Let's have a look.

```
	// Our request to the server to get information
	char request[] = "GET / HTTP/1.1\r\n\r\n";
	// Store this data
	char response[4096];

	// Actually send a request and get a response from the server if successful.
	send(client_socket, request, sizeof(request), 0);
	recv(client_socket, &response, sizeof(response), 0);

	printf("response from the server: %s\n",response);

	// Finally close our socket
	close(client_socket);
```



Now thinking about what we have done, let us run our program.

Can we run it with ./http_client www.google.com or something else? Well, think about what address we are passing in.
In order to find an ip, you can use a command like 'ping' or look up the ip address of a website using some online tool.

## Lab Deliverable

- Follow the above steps to get a working http server and client.

### More Resources to Help

Here are some more resources on socket programming

- [Nice commented tutorial](http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)

## Going Further

For this final task, you may want to update your client-server chat to handle multiple connections.

The following is a tutorial if you have time at the end or outside of class.
[GeeksforGeeks tutorial](http://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)



