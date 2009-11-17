#ifndef HASHMANAGER_H__
#define HASHMANAGER_H__

#include <list>
#include "HashInfo.h"

class HashDecoder;

class HashManager {
	public:
// 		static std::list<HashInfo> fromFile(const std::string& filename);
		static HashInfo fromString(const std::string& hashcode);

	private:
		static HashDecoder* getHashDecoder(const std::string& hashcode);
		
	private:
		HashManager();
		HashManager(const HashManager&);
		HashManager& operator=(const HashManager&);
};

#endif // HASHMANAGER_H__
