#include "StringUtils.h"

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <sstream>
#include <cstdio>

// TODO: when "isRegex == false" some other inlined and more efficient
// algorithm should be used.
std::string strReplace(const std::string& baseString,
                       const std::string& needle,
                       const std::string& replacement,
                       bool isRegex) {
	boost::regex re(needle, (isRegex ? boost::regex_constants::normal
		                               : boost::regex_constants::literal));
	return boost::regex_replace(baseString, re, replacement);
}

std::string strReplace(const std::string& baseString,
                       const char* needles[],
                       const char* replacements[],
                       bool isRegex) {
	std::string result = baseString;
	unsigned int i = 0;
	while (needles[i] != 0) {
		boost::regex re(needles[i], (isRegex ? boost::regex_constants::normal
		                                     : boost::regex_constants::literal));
		result = boost::regex_replace(result, re, replacements[i]);
		++i;
	}
	
	return result;
}

std::vector<std::string> strSplit(const std::string& baseString,
                                  const char delim) {
	std::vector<std::string> result;
  unsigned int pos = 0, start = 0;
  while ((pos = baseString.find(delim, start)) != std::string::npos) {
    std::string s = baseString.substr(start, (pos - start));
    if (!s.empty())
      result.push_back(s);
    start = pos + 1;
  }
	if (start != 0)
		result.push_back(baseString.substr(start));
	else
    result.push_back(baseString);

	return result;
}

int strToInt(const std::string& baseString) {
	int result = 0;
	std::istringstream ss(baseString);
	if (!(ss >> result))
		throw std::bad_cast();

	return result;
}

std::string intToStr(int value) {
	std::ostringstream os;
 	if (!(os << value))
 		throw std::bad_cast();

	return os.str();
}

std::string hashToStr(unsigned char* data, unsigned int size) {
	std::stringstream ss;
	boost::format fmtr("%02x");
	for (unsigned int i = 0; i < size; ++i)
		ss << fmtr % static_cast<unsigned int>(data[i]);

	return ss.str();
}
