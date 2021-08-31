#ifndef QAES_H
#define QAES_H
#include <stdint.h>

class QAes {
public:
    static int AESEncrypt(const uint8_t *key, const uint8_t *iv, uint8_t *in, int inLen, uint8_t *out, int *outLen);
    static int AESDecrypt(const uint8_t *key, const uint8_t *iv, uint8_t *in, int inLen, uint8_t *out, int *outLen);
};
#endif // QAES_H
