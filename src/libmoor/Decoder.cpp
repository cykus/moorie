/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007 Moorie Team - http://moorie.pl/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
   
#include "Decoder.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <mcrypt.h>
#include <cassert>

unsigned char *unbase64(char *input, int length)
{
	BIO* b64 = NULL;
	BIO* bmem = NULL;

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

unsigned char*
base64_decode(const char* data, int size, unsigned int* read) {
	assert(data);
	assert(size > 0);

	BIO* b64 = NULL;
	BIO* bmem = NULL;

	unsigned char* buffer = new unsigned char[size];
	memset(buffer, 0, size);

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new_mem_buf(const_cast<char*>(data), size);
	bmem = BIO_push(b64, bmem);

	if (read)
		*read = BIO_read(bmem, buffer, size);
	else
		BIO_read(bmem, buffer, size);

	BIO_free_all(bmem);

	return buffer;
}
void decrypt(unsigned char** data, unsigned int size,
             unsigned char* key, unsigned char* iv)
{
	assert(data && *data && key && iv);

	MCRYPT thread = mcrypt_module_open(const_cast<char*>("rijndael-256"), NULL,
	                                   const_cast<char*>("cbc"), NULL);
	mcrypt_generic_init(thread, key, 32, iv);
	mdecrypt_generic(thread, *data, size);
	mcrypt_generic_deinit(thread);
	mcrypt_module_close(thread);
}
