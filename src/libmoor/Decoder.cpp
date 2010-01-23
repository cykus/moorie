#include "Decoder.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

unsigned char *unbase64(char *input, int length)
{
	BIO *b64, *bmem;

	unsigned char *buffer = (unsigned char *)malloc(length);
	memset(buffer, 0, length);

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);

	BIO_read(bmem, buffer, length);

	BIO_free_all(bmem);


	return buffer;
}


unsigned char *base64(char *input, int length)
{
	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	unsigned char *buffer = (unsigned char *)malloc(bptr->length);
	memcpy(buffer, bptr->data, bptr->length-1);
	buffer[bptr->length-1] = 0;

	BIO_free_all(b64);


	return buffer;
}

