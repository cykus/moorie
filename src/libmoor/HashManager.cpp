#include "HashManager.h"

#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

#include "HashDecoder.h"
#include "MoorhuntHashDecoder.h"
#include "YgoowHashDecoder.h"

const boost::regex ygoowHashRegex("^[yY]goow://.*$");
const boost::regex moorhuntHashRegex(".*(<<a[a-h].*?>>).*");

Hash* HashManager::fromString(std::string& hashcode) {

    if (boost::regex_search(hashcode, ygoowHashRegex)) {
        boost::shared_ptr<HashDecoder> decoder(new YgoowHashDecoder());
        return decoder->decode(hashcode);
    }
    else if (boost::regex_search(hashcode, moorhuntHashRegex)) {
        boost::smatch what;
        boost::regex_match(hashcode, what, moorhuntHashRegex);
        boost::shared_ptr<HashDecoder> decoder(new MoorhuntHashDecoder());
        return decoder->decode(what[1]);
    }
    else
        throw std::exception();
}
/*
HashDecoder* HashManager::getHashDecoder(const std::string& hashcode) {
        if (boost::regex_search(hashcode, ygoowHashRegex))
                return new YgoowHashDecoder();
        else if (boost::regex_search(hashcode, moorhuntHashRegex))
                return new MoorhuntHashDecoder();

        return 0;
}
*/
