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



