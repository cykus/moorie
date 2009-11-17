#include "HashUtils.h"

#include <mhash.h>
#include <sstream>
#include <boost/scoped_array.hpp>
#include <boost/format.hpp>

std::string md5(const std::string& data) {
	// TODO: make it static (?)
	MHASH mh = mhash_init(MHASH_MD5);
	
	if (mh != MHASH_FAILED) {
		boost::scoped_array<unsigned char> md5hash(new unsigned char[mhash_get_block_size(MHASH_MD5)]);
		mhash(mh, data.c_str(), data.size());
		mhash_deinit(mh, md5hash.get());
		std::stringstream ss;
		// TODO get rid of boost::format
		for (unsigned int i = 0; i < mhash_get_block_size( MHASH_MD5 ); i++) {
			ss <<  ( boost::format( "%02x" ) % static_cast<unsigned int>( md5hash[i] ) );
		}
		return ss.str();
	}
	
	return std::string(); //TODO
}

std::vector<int> split(const char* str, int len)
{
	std::vector<int> result;
	splitToVec(str, len, result);

	return result;
}

void splitToVec(const char* str, int len, std::vector<int>& result)
{
	const char delim = '|';
	int begin = 0;
	int i = 0;
	int tokens = 0;
	while (i < len)	{
		if (str[i] == delim) {
			result.push_back(begin);
			result.push_back(i - begin);
			begin = i + 1;
			++tokens;
			// special case for crc byte - always 4, may contain 0x7c character
			if (tokens == 1) {
				i += 5;
				++tokens;
				continue;
			}
		}
		++i;
	}
}