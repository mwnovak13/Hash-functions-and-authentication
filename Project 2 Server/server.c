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

//void substring(char [], char[], int, int);

void deriveKeyFromPassphrase(char *passphrase, char *key) {
    int i, j, k;
    
    for (i = 0; i < sizeof(key); ++i) {
        j = (i + 10) % 16;
        k = (i + 8) % 16;
        key[i] ^= passphrase[j] ^ key[k] ^ passphrase[k] ^ key[j] ^ passphrase[i];
    }
}

char* substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
   return sub;
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
	char *message = malloc(8);
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

	//-----------------SHA3 Algorithm--------------------
    
    //-----------------Convert input string to state array------
    printf("\n Enter message to Bob \n");
    fgets(buffer, sizeof(buffer), stdin);
    message = buffer;
    int x=5;
    int y=5;
    int i,j;
    uint64_t Lane[x][y];        //state array
    uint64_t S[25];             //array to take input string
    
    char substr[16];
    
    printf("beginning of test area\n");
    
    char numericValue[8]; 
    
    memset(message, 0, sizeof(message));
    //memset(tstmessage, 0, sizeof(tstmessage));
    
    message = "85121215";
    //sprintf(numericValue, "%ld", message); 
    //printf("NUmerically %ld", message);
    
    //memcpy(tstmessage,"abcdefe\x00", 8);
    
    //tstmessage = "\x61\x62\x63\x64\x65\x66\x67\x68";
    
    uint64_t temp = 0;
    //uint64_t tmp = 0;
    
    printf("contents of message are %s\n", message);
    //printf("contents of tstmessage are %s\n", tstmessage);
 
    //temp = (uint64_t) message;
    temp = atol(message);
    //tmp = (uint64_t)tstmessage;
    
    printf("Temp is: %ld \n", temp);
    //printf("Tmp is: %ld \n", tmp);
    
    printf("End of test area\n");
    
    for(i=0; i<25; i++)
    {
        S[i] = 0;
    }
    S[1] = temp;
    printf("S[1] value is %ld \n",S[1]);

    //convert the input to  Keccak-p state srray

    Lane[0][0] = S[0];
    Lane[1][0] = S[1];
    Lane[2][0] = S[2];
    Lane[3][0] = S[3];
    Lane[4][0] = S[4];

    Lane[0][1] = S[5];
    Lane[1][1] = S[6];
    Lane[2][1] = S[7];
    Lane[3][1] = S[8];
    Lane[4][1] = S[9];

    Lane[0][2] = S[10];
    Lane[1][2] = S[11];
    Lane[2][2] = S[12];
    Lane[3][2] = S[13];
    Lane[4][2] = S[14];

    Lane[0][3] = S[15];
    Lane[1][3] = S[16];
    Lane[2][3] = S[17];
    Lane[3][3] = S[18];
    Lane[4][3] = S[19];

    Lane[0][4] = S[20];
    Lane[1][4] = S[21];
    Lane[2][4] = S[22];
    Lane[3][4] = S[23];
    Lane[4][4] = S[24];

    for(i=0;i<x;i++)
    {
    for(j=0;j<y;j++)
    {
    printf("Lane[%d][%d]: %3ld \n",i,j,Lane[i][j]);
    }
    }

    //------------Step mapping row--------------

    //Step mapping



    int offset;
    uint64_t bv = 0x1;
    offset = 1;
    //executing the step mapping nr (number of rounds)=80 times
    for(int i=0;i<80;i++) {
        
    uint64_t retainedbits =  bv & Lane[1][0];
    Lane[1][0]=  Lane[1][0] >> offset;
    int diff;
    diff = 64 - offset;
    retainedbits = retainedbits << diff ;
    Lane[1][0] =  Lane[1][0] ^ retainedbits;
    }
    
    printf("after bv Value is %ld \n ",  (uint64_t)Lane[1][0]);
    
    //----------------State array to string mapping-------------
    
    S[0] = Lane[0][0];
    S[1] = Lane[1][0];
    S[2] = Lane[2][0];
    S[3] = Lane[3][0];
    S[4] = Lane[4][0];

    S[5] = Lane[0][1];
    S[6] = Lane[1][1];
    S[7] = Lane[2][1];
    S[8] = Lane[3][1];
    S[9] = Lane[4][1];

    S[10] = Lane[0][2];
    S[11] = Lane[1][2];
    S[12] = Lane[2][2];
    S[13] = Lane[3][2];
    S[14] = Lane[4][2];

    S[15] = Lane[0][3];
    S[16] = Lane[1][3];
    S[17] = Lane[2][3];
    S[18] = Lane[3][3];
    S[19] = Lane[4][3];


    S[20] = Lane[0][4];
    S[21] = Lane[1][4];
    S[22] = Lane[2][4];
    S[23] = Lane[3][4];
    S[24] = Lane[4][4];
    
    printf("Value of S[1] %ld \n ",  (uint64_t)S[1]);
    
    
    temp = S[1];
    printf("Temp is: %ld \n",temp); 
    
    //message = (char*) temp;
    //sprintf(message,"Formatted data: %ld",temp);
    
    printf("contents of message are %s\n", message);
    
  
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
    //mpz_init_set_str(d, "5617843187844953170308463622230283376298685", 10);
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
    mpz_powm(ptext, ctext,e,n);
    gmp_printf("\nThe ptext of RSA key is %Zd \n", ptext);

    //Export the plaintext into array aeskey
    memset(aeskey, 0, sizeof(aeskey));
    mpz_export(aeskey, NULL, 1,1,0,0,ptext);

    //Print the AES key 
    printf("\nThe original AES key after decryption is %s \n", aeskey);




	//-----Starting decryption code here---------//

    //Read the buffer contents again for the secret message
   /* memset(buffer, 0, sizeof(buffer));
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
    printf("\n Transmission is complete \n");*/
   
   
   
         // ------------------RSA OF HASH -----------------//

//hash = "HHHH";

//Import the value of hash  into plain text 
//mpz_import(ptext, strlen(hash),1,1,0,0,hash);
//gmp_printf("\n hash value is - %Zd \n",ptext);


//cipher hash text = encypt hash message power d mod n
//mpz_powm(ctext, ptext,d,n);
//gmp_printf("\n Ctext of hash value is - %Zd \n",ctext);


//Exporting hash encrptyed value = digest
//memset(digest, 0, sizeof(digest));
//mpz_export(digest, NULL, 1,1,0,0,ctext);
//printf("\n RSA Encryption of hash is complete. \n The digest is  - %s \n", digest);



//Encrypted the Bob message with hash

//append message with encrypted hash 

//strcat(hashmessage, digest);
//printf("print message before strcat %s\n", hashmessage);
//strcat(hashmessage, msg);
//printf("print after strcat - %s \n", hashmessage);
//msg = hashmessage;

//struct AES_ctx ctx;
//AES_init_ctx(&ctx, secretkey);
//AES_encrypt(&ctx, msg);
//send(new_socket, msg, strlen(msg), 0);
//printf("\n Transmission is complete \n");
  
	
    return 0; 
} 
