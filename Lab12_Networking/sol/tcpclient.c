#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>

int main(){

	// Ask a user there name
	char username[10];
	printf("What is your username:");
	fgets(username,10,stdin);	


	// Create a socket
	int client_socket;
	// 0 - default protocol for TCP)
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
	}

	// We need to specify somewhere to connect to.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET; // 32-bit IPV4 address
	server_address.sin_port = htons(9002); // convert to the appropriate port number using htons
	server_address.sin_addr.s_addr = htons(INADDR_ANY); // Our ip address, which is localhost using INADDR_ANY

	// First we connect
	// Returns an integer as an error code (zero means everything worked)
	int connection_status;

	// Report back to user if our connection was successful
	if( (connection_status =	connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address))) <0 ){
		printf("%d Connection unsuccessful\n", connection_status);
		close(client_socket);
		exit(0);	
	}else{
		printf("Connected successfully\n");
	}

	// Receive data from the server
	char server_response[256];
	recv(client_socket, &server_response, sizeof(server_response), 0);
	
	// Print out the server's response
	printf("The server sent the data: %s\n",server_response);
	
	char messagebuffer[64];

	while(1){
		printf("Send msg to server>");
		fgets(messagebuffer,64,stdin);
	
		char* buffer = calloc(strlen(messagebuffer)+strlen(username),sizeof(char));
		strcpy(buffer,username);
		strcat(buffer,messagebuffer);
	
		if(send(client_socket,buffer,strlen(buffer),0) <0 ){
			printf("Send failed. Disconnecting\n");
			break;
		}
		
		free(buffer);
	}

	// Then close the socket
	close(client_socket);

	return 0;
}
