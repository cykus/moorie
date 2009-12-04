#ifndef STRINGUTILS_H__
#define STRINGUTILS_H__

/** @file */

#include <string>
#include <vector>

namespace str {
/**
 * Replaces every @c needle occurance in @c baseString to @c replacemenet.
 * @param baseString String to parse.
 * @param needle Searched string (passed to @c boost::regex object).
 * @param replacement Replacement string.
 * @param isRegex If needle should be evaluated as regular expression.
 * @return Replaced string.
 */
std::string replace(const std::string& baseString,
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
std::string replace(const std::string& baseString,
                    const char* needles[],
                    const char* replacements[],
                    bool isRegex = false);

/**
 * Split string by supplied deliminator.
 * @param baseString String to split.
 * @param delim Deliminator to split by.
 * @return Vector of splitted strings.
 */
std::vector<std::string> split(const std::string& baseString,
                               const char delim);

/**
 * Parses string to integer.
 * @param baseString String to parse.
 * @throw std::bad_cast Unable to parse.
 * @return Parsed value.
 */
int toInt(const std::string& baseString);

/**
 * Parses integer to string.
 * @param value Value to parse.
 * @throw std::bad_cast Unable to parse.
 * @return Parsed string.
 */
std::string parse(int value);
}

#endif // STRINGUTILS_H__
