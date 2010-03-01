/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
                        return (it->second)(name, username, password);
		}
		catch (...) {
			// Hopefully only std::bad_alloc can be thrown here.
			return 0;
		}
	}

	return 0;
}

MailboxFactory::MailboxFactory() {}
