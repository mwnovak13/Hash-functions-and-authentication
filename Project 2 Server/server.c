#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdint.h>
#include "aes_d.h"
#include <gmp.h>

#define PORT 5000


void deriveKeyFromPassphrase(char *passphrase, char *key) {
    int i, j, k;
    
    for (i = 0; i < sizeof(key); ++i) {
        j = (i + 10) % 16;
        k = (i + 8) % 16;
        key[i] ^= passphrase[j] ^ key[k] ^ passphrase[k] ^ key[j] ^ passphrase[i];
    }
}



	//-----------------Main Function-----------------------//

int main(int argc, char const *argv[]) {
uint8_t key[16] =        { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };

    printf("This is Alice's cryptosystem.\n"); 
    
	int server_fd, new_socket;
    
	struct sockaddr_in address; 
	
    int opt = 1; 
	
    int addrlen = sizeof(address); 
	
    char buffer[2048]; 

    char rsakey[2048];

    char aeskey[10000];
char *secretkey = "";
	
    char *greeting = "\n This is a message from the Alice: Decryption complete \n"; 
	
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

	printf("\n Connected to Bob's cryptosystem over network \n");

	

	//------------------RSA Algorithm-------------------//

mpz_t ptext;
mpz_t ctext;
mpz_t  n; // public key
mpz_t  d; // private key
mpz_t  e;

mpz_init(ptext); // plain text
mpz_init(ctext); // cipher text

//Initializing values to n,d,e variables
mpz_init_set_str(n, "9516311845790656153499716760847001433441357", 10);
mpz_init_set_str(d, "5617843187844953170308463622230283376298685", 10);
mpz_init_set_str(e, "65537", 10);

//Reading the AES key from Bob(client) and storing into rsakey. After that make the value of buffer to null so that the next message from Bob can be stored
memset(buffer, 0, sizeof(buffer));
read(new_socket, buffer, 2048); 

//Printing the contents of rsakey
printf("\nThe RSA key array  recieved is %s \n", buffer);

//Importing rsakey value into cipher text- ctext 
mpz_import(ctext, strlen(buffer),1,1,0,0,buffer);
gmp_printf("\nThe ctext of RSA key is %Zd \n", ctext);

//decrypting the cipher text to plain text 
//formula - plaintext = ciphertext power d mod n
mpz_powm(ptext, ctext,d,n);
gmp_printf("\nThe ptext of RSA key is %Zd \n", ptext);

//Export the plaintext into array aeskey
memset(aeskey, 0, sizeof(aeskey));
mpz_export(aeskey, NULL, 1,1,0,0,ptext);

//Print the AES key 
printf("\nThe original AES key after decryption is %s \n", aeskey);




	//-----Starting decryption code here---------//

//Read the buffer contents again for the secret message
memset(buffer, 0, sizeof(buffer));
read(new_socket, buffer, 2048);
printf("\n The shared secret message is - %s \n", buffer);

//use the AES key and key initialized above to get the round keys
//deriveKeyFromPassphrase(aeskey, key); 
 
secretkey = aeskey;
//Calling the decrypt function in aes_e.c file	
struct AES_ctx ctx;
AES_init_ctx(&ctx, secretkey);
AES_decrypt(&ctx, buffer);

//Sending message to Bob
printf("\n Message recieved and encrypted. \n ");
    printf("\n Decryption of secret message is complete. The secret message is - %s \n", buffer);

send(new_socket, greeting, strlen(greeting), 0);

char alicebuffer[2048];
char *msg = "";
memset(alicebuffer, 0, sizeof(alicebuffer));
printf("\n Enter message to Bob \n");
fgets(alicebuffer, sizeof(alicebuffer), stdin);
msg = alicebuffer;

//Encrypted the Bob message
//deriveKeyFromPassphrase(aeskey, key); 

AES_init_ctx(&ctx, secretkey);
AES_encrypt(&ctx, msg);
send(new_socket, msg, strlen(msg), 0);
printf("\n Transmission is complete \n");
	
    return 0; 
} 
