#include "qaes.h"

#include "openssl/ossl_typ.h"
#include "openssl/aes.h"
#include "openssl/evp.h"


int QAes::AESEncrypt(const uint8_t *key, const uint8_t *iv, uint8_t *in, int inLen, uint8_t *out, int *outLen)
{
    int len, tmplen;
    int res;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    res = EVP_EncryptUpdate(ctx, out, &len, in, inLen);
    if (!res) {
        return -1;
    }

    res = EVP_EncryptFinal_ex(ctx, out + len, &tmplen);
    if (!res) {
        return -1;
    }

    len += tmplen;
    *outLen = len;
    EVP_CIPHER_CTX_cleanup(ctx);
    return 0;
}

int QAes::AESDecrypt(const uint8_t *key, const uint8_t *iv, uint8_t *in, int inLen, uint8_t *out, int *outLen)
{
    int len, tmplen;
    int res;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    res = EVP_DecryptUpdate(ctx, out, &len, in, inLen);
    if (!res) {
        return -1;
    }

    res = EVP_DecryptFinal_ex(ctx, out + len, &tmplen);
    if (!res) {
        return -1;
    }

    len += tmplen;
    *outLen = len;
    EVP_CIPHER_CTX_cleanup(ctx);
    return 0;
}
