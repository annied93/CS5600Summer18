#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>

int main(){
	
	char server_message[256] = "You have reached the server";

	// Create the server socket
	int server_socket;
	//			domain, type 	    ,protocol
	// domain = AF_INET(IPv4) or AF_INET6			
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// define the server address
	//
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	// Bind the socket to our specified IP and port
	printf("bind()\n");
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	// Start listening for connections
	// 27 - number of connections we can wait for
	printf("Server Listening\n");

	listen(server_socket, 27);

	char client_message[64];

	// Integer to hold the client socket
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	printf("Client_socket %d\n",client_socket);
	
	// send the message
	send(client_socket, server_message, sizeof(server_message), 0);
	
	// Receive messages from clients
	while(recv(client_socket, client_message, sizeof(client_message),0) > 0){
		printf("> %s",client_message);
		memset(client_message,0,strlen(client_message));
	}

	// Close the socket
	close(server_socket);

	return 0;
}
