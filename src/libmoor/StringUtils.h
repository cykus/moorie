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
