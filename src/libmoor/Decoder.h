#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>

unsigned char *unbase64(char *input, int length);

unsigned char* 
base64_decode(const char* data, int length, unsigned int* read = 0);

void decrypt(unsigned char** data, unsigned int data_size,
             unsigned char* key, unsigned char* iv);
