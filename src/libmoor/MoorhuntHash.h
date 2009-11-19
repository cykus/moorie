#ifndef MOORHUNTHASH_H__
#define MOORHUNTHASH_H__

#include "Hash.h"

class MoorhuntHash : public Hash {
	public:
		explicit MoorhuntHash(HashInfo hashInfo);

	public:
		bool checkAccessPassword(const std::string& password);
};

#endif // MOORHUNTHASH_H__
