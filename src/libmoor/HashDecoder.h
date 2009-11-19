#ifndef HASHDECODER_H__
#define HASHDECODER_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include "Hash.h"

class HashDecoder {
	public:
		virtual ~HashDecoder() {}

	public:
		/**
		 * Decodes supplied hashcode string.
		 * @param hashcode Hashcode string to decode.
		 * @return Decoded hash object.
		 */
		virtual Hash* decode(const std::string& hashcode) =0;
};

#endif // HASHDECODER_H__
