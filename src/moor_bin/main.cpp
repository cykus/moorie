#include <iostream>

#include "../libmoor/LibMoor.h"
#include "../libmoor/Log.h"
#include "../libmoor/HashManager.h"
#include <boost/program_options.hpp>

#include "../libmoor/Tools.h"

#define VERSION 0.3

int main(int argc, char **argv) {

        CLibMoor * Instance;

        unsigned int logLevel(6);
		/** zmienne dla downloadu */
		std::string file;   // hash input file
		std::string strhash;
		unsigned int mailbox (1) ;
		std::string pass; // moorhunt password
		std::string hash;
		std::string path = "";
		bool download = false;

  		/** zmienne dla uploadu */
		int upmailbox;
		std::string upload_filename;
        std::string ul, up, dp, ep, to;
        unsigned int ss (7);
        unsigned int fromseg (1);
        bool upload = false;

		/** zmienne dla edycji hashcode'a */
		std::string orighash;
		std::string editpass;
		std::string mboxaddr;
		std::string mboxpass;
		bool edit = false;

        boost::program_options::options_description general_option("General options");
        general_option.add_options()
                        ("version,v", "Show version information")
                        ("help,h", "Show help")
                        ("log-level,l", boost::program_options::value<unsigned int>( &logLevel )->default_value(6), "Log level (0-8)");
        boost::program_options::options_description download_option("Download options");
        download_option.add_options()
                        ("hash,f", boost::program_options::value<std::string>(), "Hash file")
                        ("shash,s", boost::program_options::value<std::string>(), "Hash string")
                        ("password,p", boost::program_options::value<std::string>(), "Hash password")
                        ("mailbox,m", boost::program_options::value<unsigned int>( &mailbox )->default_value( 1 ), "Select mailbox");
                        ("path,fp",      boost::program_options::value<std::string>(), "Download path");

		boost::program_options::options_description upload_option("Upload options");
        upload_option.add_options()
                        ("upload,u", boost::program_options::value<std::string>(), "Upload file")
                        ("ul", boost::program_options::value<std::string>(), "Upload mailbox login")
                        ("up", boost::program_options::value<std::string>(), "Upload mailbox password")
                        ("to", boost::program_options::value<std::string>(), "Upload adressee")
                        ("dp", boost::program_options::value<std::string>(), "Download password")
                        ("ep", boost::program_options::value<std::string>(), "Edit password")
                        ("ss", boost::program_options::value<unsigned int>( &ss )->default_value( 7 ), "Upload segment size (1-10 mb), default 7mb")
                        ("fromseg", boost::program_options::value<unsigned int>( &fromseg )->default_value( 1 ),  "Upload  from given segment");

		boost::program_options::options_description hashcode_option("Hashcode edition options");
		hashcode_option.add_options()
						("orighash", boost::program_options::value<std::string>(), "Original hashcode")
						("editpass", boost::program_options::value<std::string>(), "Edit password")
						("mboxaddr", boost::program_options::value<std::string>(), "Mirror address (example@example.com)")
						("mboxpass", boost::program_options::value<std::string>(), "Password for mirror");

        boost::program_options::options_description all("Moorie 0.2.1 (C)by Moorie Team (http://moorie.pl)");
        all.add(general_option).add(download_option).add(upload_option).add(hashcode_option);
        boost::program_options::variables_map vars;
        try
        {
                boost::program_options::store(boost::program_options::parse_command_line(argc, argv, all), vars);
        }
        catch (boost::program_options::error e)
        {
                std::cerr << e.what() << std::endl;
                return 1;
        }

        if (vars.count("help"))
        {
                std::cout << all << std::endl;
                return 0;
        }

        if (vars.count("version"))
        {
                std::cout << "Moorie " << VERSION << std::endl;
                return 0;
        }

        if ( vars.count("log-level") )
    {
        logLevel = vars["log-level"].as<unsigned int>();
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
                mailbox = vars["mailbox"].as<unsigned int>();
                std::string buf, line;
                std::ifstream f(file.c_str());
                while(std::getline(f,line))
                        hash += line;

                download = true;
        }
        else if (vars.count("shash"))
        {
                strhash = vars["shash"].as<std::string>();
                mailbox = vars["mailbox"].as<unsigned int>();
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
                if ( !boost::filesystem::exists( vars["upload"].as<std::string>() ) ) {
                        std::cout << "File does not exist! Terminating" << std::endl;
                        return 1;
                } else {
                        upload_filename = vars["upload"].as<std::string>();
                        upload = true;
                }

                if (!vars.count ("ul")) {
// 			std::cout << "Upload Mailbox login (-ul) is not set" << std::endl;
                        std::cout << "Podaj login: ";
                        std::cin >> ul;
// 			return 1;
                } else
                        ul = vars["ul"].as<std::string>();

                if (!vars.count ("up")) {
// 			std::cout << "Upload Mailbox password (-up) is not set" << std::endl;
                        std::cout << "Podaj haslo: ";
                        std::cin >> up;
// 			return 1;
                } else
                        up = vars["up"].as<std::string>();
                if (!vars.count ("to")) {
    // 			std::cout << "Upload adresse (-to) is not set" << std::endl;
                            std::cout << "Podaj adresatow: ";
                            std::cin >> to;
    // 			return 1;
                    } else
                            to = vars["to"].as<std::string>();
                if (!vars.count ("dp")) {
                        std::cout << "Podaj hasło pobierania: ";
                        std::cin >> dp;
                } else
                        dp = vars["dp"].as<std::string>();

                if (!vars.count ("ep")) {
// 			std::cout << "Upload Mailbox password (-up) is not set" << std::endl;
                        std::cout << "Podaj haslo edycji: ";
                        std::cin >> ep;
// 			return 1;
                } else
                        ep = vars["ep"].as<std::string>();

                if (ss < 1 || ss > 10) {
                        std::cout << "Zla wielkosc segmentu (tylko 1-10)! Koncze program." << std::endl;
                        return 1;
                }
        } else if (vars.count("orighash")) {
				edit = true;
				orighash = vars["orighash"].as<std::string>();

				if (!vars.count("editpass")) {
					std::cout << "Podaj haslo edycji: " << std::endl;
					std::cin >> editpass;
				} else
					editpass = vars["editpass"].as<std::string>();

				if (!vars.count("mboxaddr")) {
					std::cout << "Podaj adres skrzynki do dodania: " << std::endl;
					std::cin >> mboxaddr;
				} else
					mboxaddr = vars["mboxaddr"].as<std::string>();

				if (!vars.count("mboxpass")) {
					std::cout << "Podaj haslo skrzynki: " << std::endl;
					std::cin >> mboxpass;
				} else
					mboxpass = vars["mboxpass"].as<std::string>();
		}

        if (!vars.count("hash") && !vars.count("shash") && !vars.count("upload") && !vars.count("orighash")) {
                std::cout << all << std::endl;
        }

        curl_global_init(CURL_GLOBAL_ALL);

        if (download) {
                try
                {
                    boost::shared_ptr<Hash> hhash(HashManager::fromString(hash));
                    if (hhash->getInfo().valid)
                    {
					std::string myPass;
					if (hhash->getInfo().accessPasswd.length() == 32) {
						myPass = getMD5((unsigned char*)pass.c_str());
					} else
						myPass = pass;

                        if(myPass.compare( hhash->getInfo().accessPasswd) == 0)
                        {
                            if((path.find_last_of("/") != path.length() - 1) && (path.length() > 1)) path += "/";
                            Instance = new CLibMoor();
                            Instance -> Dehash(hash);
                            Instance -> selectDownloadMailBox(mailbox-1, path);
                        }
                        else std::cerr << std::endl << "Blad: Podane haslo pobierania jest nieprawidlowe!" << std::endl << std::endl;
                        }
                    else std::cerr << "Niepoprawny hashcode" << std::endl;
                }
				catch (std::exception& e)
				{
					std::cerr << "Blad! " << e.what() << std::endl;
				}
		} else if (upload) {
             try
                {
                        Instance = new CLibMoor();
                        Instance -> splitFile(upload_filename, ss);
                        if(!Instance -> selectUploadMailBox(ul, up, to, dp, ep)){
                            std::cerr << Instance->generateCleanHashcode() << "\n";
                            Instance -> startUpload(fromseg);
                        }
                        delete Instance;

                }
                catch (std::exception& e)
                {
                                std::cerr << "Blad! " << e.what() << std::endl;
                }
        } else if (edit) {
			try {
				Instance = new CLibMoor();
				Instance->addMirror(editpass, orighash, mboxaddr, mboxpass);
// 				delete Instance;

			}
			catch (std::exception& e)
			{
				std::cerr << "Blad! " << e.what() << std::endl;
			}
		}
        return 0;
}
