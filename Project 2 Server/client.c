#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <math.h>

#define PORT 5000 
int gcd(int a, int h) 
{ 
    int temp; 
    while (1) 
    { 
        temp = a%h; 
        if (temp == 0) 
          return h; 
        a = h; 
        h = temp; 
        } 
} 
double keyExchangeByRsa() {
    // Two random prime numbers 
    double p = 3; 
    double q = 7; 
  
    // First part of public key: 
    double n = p*q; 
  
    // Finding other part of public key. 
    // e stands for encrypt 
    double e = 2; 
    double phi = (p-1)*(q-1); 
    while (e < phi) 
    { 
        // e must be co-prime to phi and 
        // smaller than phi. 
        if (gcd(e, phi)==1) 
            break; 
        else
            e++; 
    } 
  
    // Private key (d stands for decrypt) 
    // choosing d such that it satisfies 
    // d*e = 1 + k * totient 
    int k = 2;  // A constant value 
    double d = (1 + (k*phi))/e; 
  
    // Message to be encrypted 
    /*double msg = 20; 
  
    printf("Message data = %lf", msg); 
  
    // Encryption c = (msg ^ e) % n 
    double c = pow(msg, e); 
    c = fmod(c, n); 
    printf("\nEncrypted data = %lf", c); 
  
    // Decryption m = (c ^ d) % n 
    double m = pow(c, d); 
    m = fmod(m, n); 
    printf("\nOriginal Message Sent = %lf", m);*/ 
    
    return d;
}
int main(int argc, char const *argv[]) { 
    printf("This is Bob's cryptosystem.\n");
    
	struct sockaddr_in address; 

    int sock = 0; 
	
    struct sockaddr_in serv_addr; 
	
    char *message = "";
    
    char buffer[2048]; 
	
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation failed \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	if(inet_pton(AF_INET, "10.0.2.15", &serv_addr.sin_addr) <=0) { 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	else if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) { 
		printf("\nConnected to Alice's cryptosystem over network. \n"); 
		
	} 
	
	//--------------------RSA Key Exchange code here------------------
	double dec = keyExchangeByRsa();
    printf("Sending the secret key %lf",dec);
    
    char arrayOfRSAKey[330];
    sprintf(arrayOfRSAKey,"%2.13f",dec);
    send(sock, arrayOfRSAKey, sizeof(dec), 0 );
    
    printf("\nKey Exhange is complete.");
    
	printf("\nEnter a message for Alice\n");
    scanf("%2048[^\n]",buffer);
    message = buffer;
    
    //encryptor(dec,message);
	send(sock, message, strlen(message), 0 ); 
	
    read(sock, buffer, 2048); 
	
    printf("%s\n", buffer); 
	
    return 0; 
} 
