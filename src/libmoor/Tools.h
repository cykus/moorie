#ifndef TOOLS_H__
#define TOOLS_H__

/** @file */

#include <string>

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

#endif // TOOLS_H__