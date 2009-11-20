#include "MoorhuntHash.h"

MoorhuntHash::MoorhuntHash(HashInfo hashInfo)
	: Hash(hashInfo)
{
}

bool MoorhuntHash::checkAccessPassword(const std::string& password) {
	return true;
}