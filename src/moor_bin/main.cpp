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
        unsigned int logLevel( 0 );
	
	boost::program_options::options_description desc("Moorie 0.2 (C)by Moorie Team (http://moorie.mahho.net/) \n\nOptions");
	desc.add_options()
			("hash,f",          boost::program_options::value<std::string>(), "Hash file")
			("shash,s",	    boost::program_options::value<std::string>(), "Hash string")
			("password,p",      boost::program_options::value<std::string>(), "Hash password")
                        ("path,u",      boost::program_options::value<std::string>(), "Download path")
			("log-level,l",     boost::program_options::value<unsigned int>( &logLevel )->default_value( 0 ), "Log level (0-8)")
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
	}
	else if (vars.count("shash"))
	{
		strhash = vars["shash"].as<std::string>();
		hash = strhash;
	}
	else if (strhash.length() == 0)
	{
		std::cerr << "Hash not specified" << std::endl;
		return 1;
	}
	curl_global_init(CURL_GLOBAL_ALL);
	
        try
        {
            boost::shared_ptr<Hash> hhash(HashManager::fromString(hash)); // TODO: double parsed hash here1
            if (hhash->getInfo().valid)
            {
                if(hhash->checkAccessPassword(pass))
                {
                    Instance = new CLibMoor();
                    Instance -> Dehash(hash); // TODO: double parsed hash here2
                    Instance -> selectMailBox(0,path);
                }
                else std::cerr << "Hasło nieprawidłowe" << std::endl;
                }
            else std::cerr << "Niepoprawny hashcode" << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << "Nieobsługiwany hashcode" << std::endl;
        }

	return 0;
}
