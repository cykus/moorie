/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007 Moorie Team - http://moorie.pl/
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
