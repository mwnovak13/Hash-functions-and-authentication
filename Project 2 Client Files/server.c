#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#define PORT 5000

int main(int argc, char const *argv[]) {
    printf("This is Alice's cryptosystem.\n"); 
    
	int server_fd, new_socket;
    
	struct sockaddr_in address; 
	
    int opt = 1; 
	
    int addrlen = sizeof(address); 
	
    char buffer[2048]; 
	
    char *greeting = "This is a message from the server: I am ready to start the cryptographic handshake"; 
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		perror("setsockopt failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT); 
	
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if (listen(server_fd, 3) < 0) { 
		perror("listen failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen))<0) { 
		perror("accept failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	read(new_socket, buffer, 2048); 
	
    printf("%s\n", buffer); 
    buffer[2048] = '\0';
    read(new_socket, buffer, 2048); 
	
    printf("%s\n", buffer); 
	
    send(new_socket, greeting, strlen(greeting), 0); 
	
    return 0; 
} 
