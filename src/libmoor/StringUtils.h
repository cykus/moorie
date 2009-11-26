#ifndef STRINGUTILS_H__
#define STRINGUTILS_H__

#include <string>
#include <vector>

std::string strReplace(const std::string& baseString,
                       const std::string& needle,
                       const std::string& replacement);

std::string strReplace(const std::string& baseString,
                       const char* needles[],
                       const char* replacements[]);

std::vector<std::string> strSplit(const std::string& baseString,
                                  const char delim);

int strToInt(const std::string& baseString);

std::string intToStr(int value);

std::string hashToStr(unsigned char* data, unsigned int size = 16);

#endif // STRINGUTILS_H__
