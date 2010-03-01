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

#include "HashManager.h"

#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

#include "HashDecoder.h"
#include "MoorhuntHashDecoder.h"
#include "YgoowHashDecoder.h"

const boost::regex ygoowHashRegex("^[yY]goow://.*$");
const boost::regex moorhuntHashRegex(".*(<<a[a-h].*?>>).*");

Hash* HashManager::fromString(const std::string& hashcode) {

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
