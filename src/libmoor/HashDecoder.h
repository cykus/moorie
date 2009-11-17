#ifndef HASHDECODER_H
#define HASHDECODER_H

#include <string>
#include "HashInfo.h"

class HashDecoder {
	public:
		virtual ~HashDecoder() {}

	public:
		/**
		 * Decodes supplied hashcode.
		 * @param hashcode Hash code to decode.
		 * @return Decoded hash object.
		 */
		virtual HashInfo decode(const std::string& hashcode) =0;
};

#endif // HASHDECODER_H
