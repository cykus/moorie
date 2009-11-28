#include "MailboxFactory.h"

MailboxFactory* MailboxFactory::instance_ = 0;

MailboxFactory& MailboxFactory::Instance() {
	if (!instance_)
		instance_ = new MailboxFactory;

	return *instance_;
}

bool
MailboxFactory::Register(const std::string& name, MailboxCreator creator) {
	return creators_.insert(CreatorMap::value_type(name, creator)).second;
}

bool
MailboxFactory::Register(const std::string names[], MailboxCreator creator) {
	bool result;
	for (unsigned int i = 0; !names[i].empty(); ++i) {
		result = creators_.insert(CreatorMap::value_type(names[i], creator)).second;
		if (!result)
			break;
	}

	return result;
}

bool MailboxFactory::Registered(const std::string& name) {
	return (creators_.find(name) != creators_.end());
}

bool MailboxFactory::Unregister(const std::string& name) {
	return (creators_.erase(name) == 1);
}

CMailBox* MailboxFactory::Create(const std::string& name,
                                 const std::string& username,
                                 const std::string& password)
{
	CreatorMap::const_iterator it = creators_.find(name);
	if (it != creators_.end()) {
		try {
			return (it->second)(username, password);
		}
		catch (...) {
			// Hopefully only std::bad_alloc can be thrown here.
			return 0;
		}
	}

	return 0;
}

MailboxFactory::MailboxFactory() {}
