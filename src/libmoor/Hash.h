#ifndef HASH_H__
#define HASH_H__

#include <string>
#include "HashInfo.h"

class Hash {
	public:
		explicit Hash(HashInfo hashInfo);
		virtual ~Hash() =0;

	public:
		virtual bool checkAccessPassword(const std::string& password);
		const HashInfo& getInfo() const { return model_; }

	private:
		HashInfo model_;
};

inline bool operator==(const Hash& lhs, const Hash& rhs) {
	return (lhs.getInfo() == rhs.getInfo());
}

#endif // HASH_H__
