#ifndef MAILBOX_FACTORY_H__
#define MAILBOX_FACTORY_H__

#include <map>
#include <string>

class CMailBox;

class MailboxFactory {
	public:
		typedef CMailBox* (*MailboxCreator)(const std::string&, const std::string&);

	public:
		/**
		 * Registers mailbox object creator.
		 * @param name Name of mailbox.
		 * @param creator Pointer to function that creates mailbox.
		 * @return True if registered successfuly, false otherwise.
		 */
		bool Register(const std::string& name, MailboxCreator creator);

		/**
		 * Registers single creator for multi mailbox objects.
		 * Last mailbox name in passed array should be an empty string.
		 * Registers creator until it's registered for all the mailboxes
		 * or registration fails.
		 * @param names Mailboxes' names.
		 * @param creator Pointer to function that creates mailbox.
		 * @return True if all registered, false otherwise.
		 */
		bool Register(const std::string names[], MailboxCreator creator);

		/**
		 * Checks if mailbox has been registered.
		 * @param name Name of mailbox.
		 * @return True if mailbox creator was found, false otherwise.
		 */
		bool Registered(const std::string& name);

		/**
		 * Unregisters mailbox object creator.
		 * @param name Name of creator.
		 * @return Whether operation succeeded (true) or not (false).
		 */
		bool Unregister(const std::string& name);

		/**
		 * Creates specified object.
		 * @param name Name of a creator.
		 * @param username Login for that mailbox.
		 * @param password Password for that mailbox.
		 * @return Ptr to mailbox object or NULL.
		 */
		CMailBox* Create(const std::string& name,
		                 const std::string& username,
		                 const std::string& password);

		/**
		 * Gets factory instance.
		 * Simple singleton implementation. If used in MT client
		 * has to guard its use by itself!
		 * @return Factory instance.
		 */
		static MailboxFactory& Instance();

	private:
		MailboxFactory();
		MailboxFactory(const MailboxFactory&);
		MailboxFactory& operator=(const MailboxFactory&);

	private:
		static MailboxFactory* instance_;

	private:
		typedef std::map<std::string, MailboxCreator> CreatorMap;
		CreatorMap creators_;
};

#endif // MAILBOX_FACTORY_H__
