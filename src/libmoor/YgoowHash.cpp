#include "YgoowHash.h"


YgoowHash::YgoowHash(HashInfo hashInfo)
	: Hash(hashInfo)
{
}

bool YgoowHash::checkAccessPassword(const std::string& password) {
	return true;
}