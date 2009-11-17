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
		 * @param name Name of creator.
		 * @param creator Pointer to function that creates mailbox.
		 * @return True if registered successfuly, false otherwise.
		 */
		bool Register(const std::string& name, MailboxCreator creator);

		/**
		 * Unregisters mailbox object creator.
		 * @param name Name of creator.
		 * @return Whether operation succeeded (true) or not (false).
		 */
		bool Unregister(const std::string& name);

		/**
		 * Creates specified object.
		 * @param name Name of a creator.
		 * @param browser Additional data passed to the object.
		 * @return Ptr to mailbox object.
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
