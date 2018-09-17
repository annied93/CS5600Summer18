// Run by navigating to localhost:80001 on your local browser
// Note that this will not work well over ssh, you need to download
// locally to your machine.


#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <netinet/in.h>

int main(){

	// Open a file to serve
	FILE* html_data;
	html_data=fopen("index.html","r");

	char response_data[1024];
	fgets(response_data, 1024, html_data);

	// Lets client know data is okay
	char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
	
	strcat(http_header, response_data);

	// create a socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);	
	// Define the address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8001);
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	bind(server_socket, (struct sockaddr*) & server_address, sizeof(server_address));

	listen(server_socket,27);

	int client_socket;

	while(1){
		client_socket = accept(server_socket, NULL, NULL);
		send(client_socket, http_header, sizeof(http_header),0);
		close(client_socket);
	}

	return 0;
}
