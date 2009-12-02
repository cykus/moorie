#ifndef HASHINFO_H__
#define HASHINFO_H__

/** @file */

#include <string>
#include <vector>

/**
 * Stores information decoded from hash.
 */
struct HashInfo {
	/**
	 * Stores data regarding mailbox.
	 */
	struct MboxAccount {
		std::string name;     ///< Mailbox address (hostname).
		std::string login;    ///< Mailbox login.
		std::string password; ///< Mailbox password.
	};
	
	bool valid; //!< Flag indicating whether hash is valid, set on decoding.
	char verMaj;
	char verMin;
	std::string fileName; //!< Downloaded file name (as provided in hashfile).
	long fileSize;  ///< Size of the upload.
	unsigned int crc;  ///< Control checksum.
	int numOfSegments;  ///< Number of segments the upload was splitted into.

	/**
	 * Stores size of segments.
	 * If this variable is set to 0 (and hash IS #valid) then it most probably
	 * means that segments don't have the same size and one must try checking
	 * member #segmentSizes.
	 * @see segmentSizes
	 */
	int segmentSize;

	/**
	 * Stores sizes of each segment.
	 * This member is set if segments are of different sizes.
	 * @see segmentSize
	 */
	std::vector<int> segmentSizes;
	
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

/**
 * Compares two hashes.
 * @param lhs Left side hash.
 * @param rhs Right side hash.
 * @return If equal (true) or not (false).
 */
inline bool operator==(const HashInfo& lhs, const HashInfo& rhs) {
	return (lhs.fileName == rhs.fileName);
}

#endif // HASHINFO_H__