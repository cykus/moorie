#include <iostream>

#include "../libmoor/LibMoor.h"

int main() {
    	CLibMoor * Instance;
   	 std::string hash;
hash = "<<ahbid5tYiBHTj0MPV4rNiqs/0pu3FI-RJ8Mij7SJXzp4jY04VjeyEx43FYTDja\
BAho7GpeDuCjtNimyEKHTdiGSWfYrSgtUpxMja9SpjYKCtXcdb l8yMBRcup5JEU7\
vychhA1NA/mYvUxWFGYFZAcdeh/ifNoA-IxyOwEPCiBym-YRFfkF0ft0wFH9XuCd\
pznqaOwesYpW72p5CdInYACddhZvQ6QjwmzhnxU5zuGwptxb1X sUWRKpPVCERU7e\
EmtLNRZNK/2tNiKxrDFizuuVzy6WrzSkopKYxUuJY0r4vTYELSA72NHiAbhX ceML\
GAkZSiL2Qy2KEa2Mi6coFBwU0adwM9YL2YMoZGIpYfPokn10X/amxhyYlmXF2x-y\
W1191lOLwE6/hq5eKyCjfsH0WVNc5/Q0mlMLXLeP2L3d/I0=>>";
   	
   	std::string ygoowhash;
	ygoowhash = "ygoow://|The Exploited Solaufein_for ufs.rar|c1dfd96eeac13261256r278|==jNnMrClSW\
			mRYLPYkbic9jcrih5CkNkvh/Vlfcucqny01fOHaezSYZkmBMP16hNRELbuoKURZ6tzMU39ls4G9PRX\
			slBrFKkEBGkrDMOQdNv6y/MCFKgnqrGqO0WGb7Z2cZHLujgDTDT1I2TKKCxuKD1+qMpjDZZ252Q68g\
			arnTcpD0SgdgNrXcoy/yIAb1VfzwwJ778lkHK/hp9249dYYbMpnyKIUhidPIjGipSj6IPxnAct5cgr\
			UtoVar732tHGI5bSTJEBNVhaeNg8T8k8bq1gTPQRVm83akeAS6oTblAV9QvyZszmyw0JkVqqRHsghy\
			iBNcASBKiKOKPUl135FMJIUgl0kcxZNtD19eZq+NvUiHVn969gKjgXBm2LnKngifmr/KQCGbJc3bao\
			HwOKXDB5+qCyBnO8UTrlRDTRcqWifIhHev7eLjAworBRjCRzXUWBMYBVPoCg+hGhWR/insufdicvJO\
			fmeH6ZvRHVK9+FfEofXfcDGjvrrivQQ+E62+o9RpDmGON+xeqxYh3wG5qigYSwn7NuDl7fSLtb9DBp\
			CqmKZDgFyH3cgBeZOKs6bp7OohD1I3TpWacg3ErjmPI|/";
			
	string opt = "moorhunt";
	if (opt == "ygoow") {
	 	Instance = new CLibMoor();
		Instance -> DehashYgoow(ygoowhash);
	} else if (opt == "moorhunt") {
		Instance = new CLibMoor();
		Instance -> Dehash(hash);
		Instance -> selectMailBox(5);
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
