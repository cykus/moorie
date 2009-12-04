#ifndef HASHUTILS_H__
#define HASHUTILS_H__

/** @file */

#include <string>
#include <vector>

/**
 * Calculates MD5 sum.
 * @param data Data to calc MD5 for.
 * @return MD5 sum in hex.
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

/**
 * Formats data as string consisting of hex values.
 * @param data Data to format.
 * @param size Size of data.
 * @return Formatted string.
 */
std::string hashToStr(unsigned char* data, unsigned int size = 16);

#endif // HASHUTILS_H__
