#include <iostream>

#include "../libmoor/LibMoor.h"
#include "../libmoor/Log.h"
#include <boost/program_options.hpp>

#define VERSION 0.2

int main(int argc, char **argv) {
	unsigned int logLevel(8);
	logLevel = static_cast<unsigned int>( Log::Error ) - logLevel + 1;
	LogConsoleHandle *logh = new LogConsoleHandle(static_cast<Log::Level>( logLevel ));
	Log::getLog()->addHandle(logh);
    	
	std::string file;   // hash input file
	std::string strhash;
	std::string mhpass; // moorhunt password
	std::string hash;
	
	boost::program_options::options_description desc("Moorie 0.2 (C)by Moorie Team (http://moorie.mahho.net/) \n\nOptions");
	desc.add_options()
			("hash,f",          boost::program_options::value<std::string>(), "Hash file")
			("shash,s",	    boost::program_options::value<std::string>(), "Hash string")
			("password,p",      boost::program_options::value<std::string>(), "Hash password")
//			("max-downloads,m", boost::program_options::value<unsigned int>(), "Maximum number of concurrent downloads")
			("log-level,l",     boost::program_options::value<unsigned int>( &logLevel )->default_value( 0 ), "Log level (0-8)")
//			("keep,k",          "Keep segments on disk after merging")
//			("creator,c",	    "Simple Creator")
			("info,i",          "Display detailed hash information")
			("verify,v",        "Verify hash correctness, then exit")
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
		std::cout << "help" << std::endl;
		return 0;
	}
	
	if (vars.count("version"))
	{
		std::cout << "Moorie " << VERSION << std::endl;
		return 0;
	}
	if (vars.count("password"))
	{
		mhpass = vars["password"].as<std::string>();
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
	
	if (file!="") 
	{

	}
	else
	{
		
	}
	
	CLibMoor * Instance;
   			
	string ygoowhash = "";
	
	string opt = "moorhunt";
	if (opt == "ygoow") {
	 	Instance = new CLibMoor();
		Instance -> DehashYgoow(ygoowhash);
	} else if (opt == "moorhunt") {
		Instance = new CLibMoor();
		Instance -> Dehash(hash);
		Instance -> selectMailBox(0);
	}
/*
	if (Instance -> Dehash(hash) != 0) {
		std::cout << "Podany hashcode jest nieprawidlowy..." << std::endl;
		return 1;
	}
	else {
		std::cout << "Podany hashcode jest prawidlowy... Wybieranie skrzynki" << std::endl;
		Instance -> selectMailBox(4);
	}
	 */
	 
	return 0;
}
