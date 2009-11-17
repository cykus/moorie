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

std::vector<int> split(const char* str, int len);

void splitToVec(const char* str, int len, std::vector<int>& result);

#endif // HASHUTILS_H__
