#ifndef HASHMANAGER_H__
#define HASHMANAGER_H__

#include <list>
#include <string>

class Hash;
class HashDecoder;

class HashManager {
	public:
// 		static std::list<HashInfo> fromFile(const std::string& filename);
		/**
		 * Creates hash object for specified hashcode.
		 * @param hashcode Hashcode string.
		 * @return Hash object.
		 */
		static Hash* fromString(const std::string& hashcode);

	private:
		/**
		 * Gets decoder for specified hashcode.
		 * @param hashcode Hashcode string to decode.
		 * @return Hash decoder.
		 */
                //static HashDecoder* getHashDecoder(const std::string& hashcode);

	private:
		HashManager();
		HashManager(const HashManager&);
		HashManager& operator=(const HashManager&);
};

#endif // HASHMANAGER_H__
