#include "HashManager.h"

#include <boost/regex.hpp>
#include "HashDecoder.h"
#include "MoorhuntHashDecoder.h"
#include "YgoowHashDecoder.h"

const boost::regex ygoowHashRegex("Ygoow://.*$");
const boost::regex moorhuntHashRegex("^<<a[a-h].*>>");

Hash* HashManager::fromString(const std::string& hashcode) {
	HashDecoder* decoder = getHashDecoder(hashcode);
	if (!decoder)
		throw std::exception();

	return decoder->decode(hashcode);
}

HashDecoder* HashManager::getHashDecoder(const std::string& hashcode) {
	if (boost::regex_search(hashcode, ygoowHashRegex))
		return new YgoowHashDecoder();
	else if (boost::regex_search(hashcode, moorhuntHashRegex))
		return new MoorhuntHashDecoder();

	return 0;
}