#ifndef STRINGUTILS_H__
#define STRINGUTILS_H__

#include <string>
#include <vector>

/**
 * Replaces every @c needle occurance in @c baseString to @c replacemenet.
 * @param baseString String to parse.
 * @param needle Searched string (passed to @c boost::regex object).
 * @param replacement Replacement string.
 * @param isRegex If needle should be evaluated as regular expression.
 * @return Replaced string.
 */
std::string strReplace(const std::string& baseString,
                       const std::string& needle,
                       const std::string& replacement,
                       bool isRegex = false);

/**
 * Replaces every one of @c needles occurance in @c baseString to appropriate
 * @c replacemenet string.
 * @param baseString String to parse.
 * @param needles Searched strings (passed to @c boost::regex object).
 * @param replacements Replacement strings.
 * @param isRegex If needle should be evaluated as regular expression.
 * @return Replaced string.
 */
std::string strReplace(const std::string& baseString,
                       const char* needles[],
                       const char* replacements[],
                       bool isRegex = false);

/**
 * Split string by supplied deliminator.
 * @param baseString String to split.
 * @param delim Deliminator to split by.
 * @return Vector of splitted strings.
 */
std::vector<std::string> strSplit(const std::string& baseString,
                                  const char delim);

/**
 * Parses string to integer.
 * @param baseString String to parse.
 * @throw std::bad_cast Unable to parse.
 * @return Parsed value.
 */
int strToInt(const std::string& baseString);

/**
 * Parses integer to string.
 * @param value value to parse.
 * @throw std::bad_cast Unable to parse.
 * @return Parsed string.
 */
std::string intToStr(int value);

/**
 * Formats data as string consisting of hex values.
 * @param data Data to format.
 * @param size Size of data.
 * @return Formatted string.
 */
std::string hashToStr(unsigned char* data, unsigned int size = 16);

#endif // STRINGUTILS_H__
