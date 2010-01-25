#include "HashUtils.h"

#include <mhash.h>
#include <sstream>
#include <boost/scoped_array.hpp>
#include <boost/format.hpp>

#include "StringUtils.h"

namespace {
        std::string hash(hashid id, const std::string& data) {
                MHASH td = mhash_init(id);

                if (td != MHASH_FAILED) {
                        uint32_t hash_size = mhash_get_block_size(id);
                        boost::scoped_array<unsigned char> hash(new unsigned char[hash_size]);
                        mhash(td, data.c_str(), data.size());
                        mhash_deinit(td, hash.get());
                        return hashToStr(hash.get(), hash_size);
                }

                return std::string();
        }
}

std::string md5(const std::string& data) {
        return hash(MHASH_MD5, data);
}

std::string sha1(unsigned char data) {
        return hash(MHASH_SHA1, std::string() + static_cast<char>(data));
}

std::vector<int> split(const char* str, int len) {
	std::vector<int> result;
	splitToVec(str, len, result);

	return result;
}

void splitToVec(const char* str, int len, std::vector<int>& result) {
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

std::string hashToStr(unsigned char* data, unsigned int size) {
	std::stringstream ss;
	boost::format fmtr("%02x");
	for (unsigned int i = 0; i < size; ++i)
		ss << fmtr % static_cast<unsigned int>(data[i]);

	return ss.str();
}
