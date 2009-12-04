#ifndef HASH_H__
#define HASH_H__

/** @file */

#include <string>
#include "HashInfo.h"

class Hash {
	public:
		explicit Hash(HashInfo hashInfo);
		virtual ~Hash() =0;

	public:
		/**
		 * Verifies hash download password against specified.
		 * @param password String to verify hash password against.
		 * @return Whether passwords match.
		 */
		virtual bool checkAccessPassword(const std::string& password);

		/**
		 * Gets object which stores hash data.
		 * @return Object storing hash data.
		 * @see HashInfo
		 */
		const HashInfo& getInfo() const { return model_; }

	private:
		HashInfo model_; ///< Stores hash data.
};

/**
 * Compares two hashes.
 * Current implementation forwards comparison to op== working on #HashInfo
 * objects.
 * @param lhs Left side hash.
 * @param rhs Right side hash.
 * @return Comparison result.
 * @see HashInfo
 * @see operator==(const HashInfo&, const HashInfo&)
 */
inline bool operator==(const Hash& lhs, const Hash& rhs) {
	return (lhs.getInfo() == rhs.getInfo());
}

#endif // HASH_H__
