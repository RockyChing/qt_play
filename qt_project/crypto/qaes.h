#ifndef QAES_H
#define QAES_H
#include <stdint.h>

class QAes {
public:
    static int AESEncrypt(const uint8_t *key, int keyLen, const uint8_t *iv, int ivLen, 
			uint8_t *in, int inLen, uint8_t *out, int *outLen);
    static int AESDecrypt(const uint8_t *key, int keyLen, const uint8_t *iv, int ivLen, 
			uint8_t *in, int inLen, uint8_t *out, int *outLen);
};
#endif // QAES_H
