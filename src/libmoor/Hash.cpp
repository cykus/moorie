#include "Hash.h"

Hash::Hash(HashInfo hashInfo)
	: model_(hashInfo)
{
}

Hash::~Hash() { }

bool Hash::checkAccessPassword(const std::string& password) {
	return true;
}