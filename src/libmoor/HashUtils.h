#ifndef HASHUTILS_H__
#define HASHUTILS_H__

#include <string>
#include <vector>

/**
 * Calculates MD5 sum.
 * @param data Data to calc MD5 for.
 * @return MD5 sum.
 */
std::string md5(const std::string& data);

/**
 * Calculates SHA1 sum from a single byte.
 * @param data Data to calculate checksum for.
 * @return Checksum in hex.
 */
std::string sha1(unsigned char data);

std::vector<int> split(const char* str, int len);

void splitToVec(const char* str, int len, std::vector<int>& result);

#endif // HASHUTILS_H__
