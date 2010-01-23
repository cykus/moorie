#include <iostream>

#include "../libmoor/LibMoor.h"
#include "../libmoor/Log.h"
#include "../libmoor/HashManager.h"
#include <boost/program_options.hpp>

#define VERSION 0.2

int main(int argc, char **argv) {

        CLibMoor * Instance;
	std::string file;   // hash input file
	std::string strhash;
        std::string pass; // moorhunt password
	std::string hash;
        std::string path = "";
		std::string upload_filename;
        unsigned int logLevel( 8 );

	bool download = false, upload = false;

	boost::program_options::options_description desc("Moorie 0.2 (C)by Moorie Team (http://moorie.mahho.net/) \n\nOptions");
	desc.add_options()
			("hash,f",          boost::program_options::value<std::string>(), "Hash file")
			("shash,s",	    boost::program_options::value<std::string>(), "Hash string")
			("password,p",      boost::program_options::value<std::string>(), "Hash password")
//                         ("path,u",      boost::program_options::value<std::string>(), "Download path")
			("upload,u", boost::program_options::value<std::string>(), "Upload file")
                        ("log-level,l",     boost::program_options::value<unsigned int>( &logLevel )->default_value( 8 ), "Log level (0-8)")
			("version",         "Show version information")
			("help,h",          "Show help");
	boost::program_options::variables_map vars;
	try
	{
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vars);
		boost::program_options::notify(vars);
	}
	catch (boost::program_options::error e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	if (vars.count("help"))
	{
                std::cout << desc << std::endl;
                return 0;
	}

	if (vars.count("version"))
	{
		std::cout << "Moorie " << VERSION << std::endl;
		return 0;
	}
        if ( logLevel > 0 )
        {
                logLevel = static_cast<unsigned int>( Log::Error ) - logLevel + 1;
                LogFileHandle *logh = new LogFileHandle( "moorie.log", static_cast<Log::Level>( logLevel ) );
                Log::getLog()->addHandle(logh);
                LogConsoleHandle *logh2 = new LogConsoleHandle(static_cast<Log::Level>( logLevel ) );
                Log::getLog()->addHandle(logh2);
        }
	if (vars.count("password"))
	{
                pass = vars["password"].as<std::string>();
	}
        if (vars.count("path"))
        {
                path = vars["path"].as<std::string>();
        }
	if (vars.count("hash"))
	{
		file = vars["hash"].as<std::string>();
		std::string buf, line;
		std::ifstream f(file.c_str());
		while(std::getline(f,line))
			hash += line;

		download = true;
	}
	else if (vars.count("shash"))
	{
		strhash = vars["shash"].as<std::string>();
		hash = strhash;
		download = true;
	}
	else if (download && strhash.length() == 0)
	{
		std::cerr << "Hash not specified" << std::endl;
		return 1;
	}
	else if (vars.count ("upload"))
	{
		if ( !boost::filesystem::exists( vars["upload"].as<std::string>() ) )
		{
			std::cout << "File does not exist! Terminating" << std::endl;
			return 1;
		} else {
			upload_filename = vars["upload"].as<std::string>();
			upload = true;
		}
	}

	curl_global_init(CURL_GLOBAL_ALL);

	if (download) {
        try
        {
			boost::shared_ptr<Hash> hhash(HashManager::fromString(hash));
            if (hhash->getInfo().valid)
            {
                if(hhash->checkAccessPassword(pass))
                {
                    Instance = new CLibMoor();
                    Instance -> Dehash(hash);
                    Instance -> selectDownloadMailBox(0,path);
                }
                else std::cerr << "Hasło nieprawidłowe" << std::endl;
                }
            else std::cerr << "Niepoprawny hashcode" << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << "Nieobsługiwany hashcode" << std::endl;
        }
	} else if (upload) {
		Instance = new CLibMoor();
		std::cout << "Select mailbox: " << std::endl;
		std::cout << "1. mail.ru" << std::endl;
		Instance -> selectUploadMailBox(0x03, "moorie", "moorie123"); // wybieramy mail.ru
		// mail.ru - 24 / mooriemoorie/moorie123
		// google - 0x03 / moorietest/moorie123
		// gaezta - -93 / moorie/moorie123
		// mooriemoorie@poczta.onet.pl /
		Instance -> splitFile(upload_filename, 7);
		Instance -> startUpload();

		try
		{
		}
		catch (std::exception& e)
		{
				std::cerr << "Blad!" << std::endl;
		}
	}
	return 0;
}
