#ifndef HASHINFO_H__
#define HASHINFO_H__

#include <string>
#include <vector>

struct HashInfo {

	struct MboxAccount {
		std::string name;
		std::string login;
		std::string password;
	};

	bool valid; //!< flag indicating whether hash is valid, set on decoding
	char verMaj;
	char verMin;
	std::string fileName; //!< downloaded file name (as provided in hashfile)
	long fileSize;
	unsigned int crc;
	int numOfSegments;
	int segmentSize;
	std::vector<MboxAccount> accounts;
	std::vector<int> ids;
	std::string forWhom;
	std::string accessPasswd;
	std::string editPasswd;
	std::string coverURL;
	std::string descURL;
	std::string fullTitle;
	std::string uploader;
	std::string comment;
	std::string hashString; //!< raw hash std::string (base64 encoded)
};

inline bool operator==(const HashInfo& lhs, const HashInfo& rhs) {
	return (lhs.fileName == rhs.fileName);
}

#endif // HASHINFO_H__