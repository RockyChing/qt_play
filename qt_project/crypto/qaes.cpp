#include <stdio.h>
#include <string.h>
#include "openssl/ossl_typ.h"
#include "openssl/aes.h"
#include "openssl/evp.h"
#include "openssl/err.h"
#include "qaes.h"

int QAes::AESEncrypt(const uint8_t *key, int keyLen, const uint8_t *iv, int ivLen, 
		uint8_t *in, int inLen, uint8_t *out, int *outLen)
{
    int len, tmplen;
    int res;
	uint8_t keyBuf[AES_BLOCK_SIZE + 1], ivBuf[AES_BLOCK_SIZE + 1];

	memset(keyBuf, 0, sizeof(keyBuf));
	memcpy(keyBuf, key, (keyLen > AES_BLOCK_SIZE) ? AES_BLOCK_SIZE : keyLen);

	memset(ivBuf, 0, sizeof(ivBuf));
	memcpy(ivBuf, iv, (ivLen > AES_BLOCK_SIZE) ? AES_BLOCK_SIZE : ivLen);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, keyBuf, ivBuf);

    res = EVP_EncryptUpdate(ctx, out, &len, in, inLen);
    if (!res) {
        return -1;
    }

    res = EVP_EncryptFinal_ex(ctx, out + len, &tmplen);
    if (!res) {
        return -2;
    }

    len += tmplen;
    *outLen = len;
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

int QAes::AESDecrypt(const uint8_t *key, int keyLen, const uint8_t *iv, int ivLen, 
		uint8_t *in, int inLen, uint8_t *out, int *outLen)
{
    int len, tmplen;
    int res;
	uint8_t keyBuf[AES_BLOCK_SIZE + 1], ivBuf[AES_BLOCK_SIZE + 1];

	memset(keyBuf, 0, sizeof(keyBuf));
	memcpy(keyBuf, key, (keyLen > AES_BLOCK_SIZE) ? AES_BLOCK_SIZE : keyLen);

	memset(ivBuf, 0, sizeof(ivBuf));
	memcpy(ivBuf, iv, (ivLen > AES_BLOCK_SIZE) ? AES_BLOCK_SIZE : ivLen);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, keyBuf, ivBuf);

    res = EVP_DecryptUpdate(ctx, out, &len, in, inLen);
    if (!res) {
        return -1;
    }

    res = EVP_DecryptFinal_ex(ctx, out + len, &tmplen);
    if (!res) {
        //unsigned long err = ERR_get_error();
        //char buff[256];
        //memset(buff, 0, 256);
        //ERR_error_string(err, buff);
        //qDebug("err: %s", buff);
        return -2;
    }

    len += tmplen;
    *outLen = len;
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
