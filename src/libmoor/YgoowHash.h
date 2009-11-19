#ifndef YGOOWHASH_H__
#define YGOOWHASH_H__

#include "Hash.h"

class YgoowHash : public Hash {
	public:
		explicit YgoowHash(HashInfo hashInfo);

	public:
		bool checkAccessPassword(const std::string& password);
};

#endif // YGOOWHASH_H__
