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

#ifndef TOOLS_H__
#define TOOLS_H__

/** @file */

#include <string>
#include <cstdlib>
#include <openssl/md5.h>
#include <cstring>
#include <cstdio>

/**
 * Implementation of ROT13 cipher.
 * @warning It's buggy in my opinion - localghost.
 * @param str String to encipher.
 * @return Enciphered text.
 */
std::string Rot13(std::string str);

/**
 * Implementation of ROT13 cipher.
 * @remarks Had to reimplement it as version produced by #Rot13() didn't work
 *          for me in the process of Ygoow hashes decoding.
 * @param str String to encipher.
 * @return Enciphered text.
 */
std::string rot13(const std::string& str);
std::string getMD5(unsigned char* string);
/**
*/
char hexToAscii(char first, char second);
#endif // TOOLS_H__
