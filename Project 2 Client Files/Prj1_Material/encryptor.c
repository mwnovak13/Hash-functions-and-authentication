#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aes_e.h"

void deriveKeyFromPassphrase(char *, char *);
void readPlainText(uint8_t *);
void writeCipherText(uint8_t *);


int main(void)
{
    uint8_t key[16] =        { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
       
    char passphrase [16];
  
    memset(passphrase, 0, sizeof(passphrase));
    
    printf("Acme Corporation AES Encryptor System\n");
    printf("\nEncrypting file plaintext.dat\n");
    printf("\nEnter passphrase: ");
    
    fgets(passphrase, sizeof(passphrase), stdin);

    deriveKeyFromPassphrase(passphrase, key); 
    
    uint8_t in[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    readPlainText(in);
    
    struct AES_ctx ctx;

    AES_init_ctx(&ctx, key);
    
    AES_encrypt(&ctx, in);    
    
    writeCipherText(in);
    
    printf("Encryption is complete. The ciphertext is in file ciphertext.dat\n");
    
    return 0;
}


void deriveKeyFromPassphrase(char *passphrase, char *key) {
    int i, j, k;
    
    for (i = 0; i < sizeof(key); ++i) {
        j = (i + 10) % 16;
        k = (i + 8) % 16;
        key[i] ^= passphrase[j] ^ key[k] ^ passphrase[k] ^ key[j] ^ passphrase[i];
    }
}


void readPlainText(uint8_t * plain) {
    FILE *fp;
    fp = fopen("plaintext.dat", "rb");

    fread(plain, 16, 1, fp);
  
    fclose(fp);
}


void writeCipherText(uint8_t * ctxt) {
    FILE *fp;
    fp = fopen("ciphertext.dat", "wb");

    fwrite(ctxt, 16, 1, fp);
  
    fclose(fp);
}


