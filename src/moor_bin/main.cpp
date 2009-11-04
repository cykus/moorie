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
   	
	 Instance = new CLibMoor();

	if (Instance -> Dehash(hash) != 0) {
		std::cout << "Podany hashcode jest nieprawidlowy..." << std::endl;
		return 1;
	}
	else {
		std::cout << "Podany hashcode jest prawidlowy... Wybieranie skrzynki" << std::endl;
		Instance -> selectMailBox(4);
	}
	return 0;
}
