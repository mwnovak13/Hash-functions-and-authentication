#include <stdint.h>

#define AES128 1

#define AES_BLOCKLEN 16 

#define AES_KEYLEN 16   
#define AES_keyExpSize 176


struct AES_ctx
{
  uint8_t RoundKey[AES_keyExpSize];
  uint8_t Iv[AES_BLOCKLEN];
};

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);

void AES_encrypt(struct AES_ctx* ctx, uint8_t* buf);
