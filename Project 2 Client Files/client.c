#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <math.h>
#include <stdint.h>
#include <gmp.h>
#include "aes_d.h"


#define PORT 5000 

//Combining the passphrase and key
void deriveKeyFromPassphrase(char *passphrase, char *key) {
    int i, j, k;
    
    for (i = 0; i < sizeof(key); ++i) {
        j = (i + 10) % 16;
        k = (i + 8) % 16;
        key[i] ^= passphrase[j] ^ key[k] ^ passphrase[k] ^ key[j] ^ passphrase[i];
    }
}

		//---------------Main Function----------------
int main(int argc, char const *argv[]) {

uint8_t key[16] =        { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
    
    struct sockaddr_in address; 

    int sock = 0; 
	
    struct sockaddr_in serv_addr; 
	
    char *message = "";
    char *rsamessage = "";
	char *aeskey = "";
    
    char buffer[2048]; 
    char msgbuffer[2048]; 
    char rsamsg[2048];
    char rsabuffer[2048]; 
char secretkey[2048];

    printf("This is Bob's cryptosystem.\n");


	//-------Code for Socket creation with Alice-server------------
	
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation failed \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	if(inet_pton(AF_INET, "10.0.2.9", &serv_addr.sin_addr) <=0) { 
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
    
mpz_t ptext;
mpz_t ctext;
mpz_t  n; // public key of Alice
mpz_t  e;

mpz_init(ptext); // plain text
mpz_init(ctext); // cipher text

//Initialzing the values of n,e which are the public key of Alice
mpz_init_set_str(n, "9516311845790656153499716760847001433441357", 10);
mpz_init_set_str(e, "65537", 10);

//Getting the RSA input from Bob and storing into rsamessage
memset(buffer, 0, sizeof(buffer));
//memset(rsamessage, 0, sizeof(rsamessage));
//printf("\nEnter RSA Key\n");
//fgets(buffer, sizeof(buffer), stdin);
aeskey = "AAAAAAAAAAAAAAA";
printf("\n Original AES key is - %s \n",aeskey);
//rsamessage = buffer;

//Import the value of rsamessage into plain text 
mpz_import(ptext, strlen(aeskey),1,1,0,0,aeskey);
gmp_printf("\n Ptext of AES key is - %Zd \n",ptext);

//encrypting the plain text into cipher text using Alice public key - e,n
//formula - ptext power to e mod n = ctext
//cipher text = message power e mod n
mpz_powm(ctext, ptext,e,n);
gmp_printf("\n Ctext of AES key is - %Zd \n",ctext);

//Exporting the value of cipher text into rsabuffer and printing its value
memset(rsabuffer, 0, sizeof(rsabuffer));
mpz_export(rsabuffer, NULL, 1,1,0,0,ctext);
printf("\n RSA Encryption is complete. \n The RSA key array - %s \n", rsabuffer);

//Sending the RSA encrypted key(stored in rsabuffer) to Alice
send(sock, rsabuffer, strlen(rsabuffer), 0 ); 
printf("\nKey Exchange is complete.\n");

//secretkey = aeskey;
   
		//------------Starting AES Encrytion code here------------ 

//Input message to be encrypted and store in message 
memset(msgbuffer, 0, sizeof(msgbuffer));   
printf("\nEnter a message for Alice\n");
fgets(msgbuffer, sizeof(msgbuffer), stdin);
message = msgbuffer;
    
    printf("\nEncrypting Bob's message \n");
 
//Use the RSA key(rsamessage) and key initialized above to derive the round keys
     //deriveKeyFromPassphrase(rsamessage, key);
   
//Calling the encryptor.c file for encrypting the round keys and secret message
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, aeskey);
    	AES_encrypt(&ctx, message);    

	printf("Encryption is complete. The ciphertext is - %s \n", message);

//sending the encrptyed secret message to alice
    send(sock, message, strlen(message), 0 ); 

//Read the message from Alice
memset(msgbuffer, 0, sizeof(msgbuffer));
read(sock, msgbuffer, 2048);
printf("\n %s \n", msgbuffer);

//Read encrypted message from Alice
memset(msgbuffer, 0, sizeof(msgbuffer));
read(sock, msgbuffer, 2048);
//printf("\n Encrypted Message from Alice is %s \n", msgbuffer);

//Decrpted the messgae from Alice
 //deriveKeyFromPassphrase(rsamessage, aeskey);
AES_init_ctx(&ctx, aeskey);
    	AES_decrypt(&ctx, msgbuffer);
printf("Message recieved from Alice is - %s \n", msgbuffer);
printf("\nTransmission is complete.\n");
    return 0; 
} 
