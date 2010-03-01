/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
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

#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>

unsigned char *unbase64(char *input, int length);
unsigned char *base64(char *input, int length);

/**
 * Decodes base64 data.
 * @param data Data to decode.
 * @param length Data size.
 * @param[out] read Number of decoded bytes.
 * @return Decoded data.
 */
unsigned char* 
base64_decode(const char* data, int size, unsigned int* read = 0);

/**
 * Decrypts data using Rijndael-256 algorithm.
 * @param[in,out] data Pointer to data to decrypt.
 * @param size Data size.
 * @param key Encryption key (should be 32-bytes long).
 * @param iv Encryption initialization vector.
 */
void decrypt(unsigned char** data, unsigned int size,
             unsigned char* key, unsigned char* iv);



