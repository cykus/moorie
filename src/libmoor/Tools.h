#include <iostream>
#include <string>

using namespace std;

string& str_replace(const string &search, const string &replace, string &subject);
string Rot13(string str);

/**
 * Implementation of ROT13 cipher.
 * @param str String to encipher.
 * @return Enciphered text.
 */
std::string rot13(const std::string& str);
