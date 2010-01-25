#include "Tools.h"

// std::string& str_replace(const std::string &search,
//                          const std::string &replace,
//                          std::string &subject)
// {
// 	std::string buffer;
// 
// 	int sealeng = search.length();
// 	int strleng = subject.length();
// 
// 	if (sealeng==0)
// 			return subject;//no change
// 
// 	for(int i=0, j=0; i<strleng; j=0 )
// 	{
// 			while (i+j<strleng && j<sealeng && subject[i+j]==search[j])
// 					j++;
// 			if (j==sealeng)//found 'search'
// 			{
// 					buffer.append(replace);
// 					i+=sealeng;
// 			}
// 			else
// 			{
// 					buffer.append( &subject[i++], 1);
// 			}
// 	}
// 	subject = buffer;
// 	return subject;
// }

std::string Rot13(std::string str)
{
	std::string str2 = "";
	for (int i = 0; i < str.length(); i++)
	{
		char ch;
		if ((str[i] >= 'A') && (str[i] < '['))
		{
			ch = (char) (str[i] + '\r');
			if (ch >= '[')
			{
				ch = (char) (ch - '\x001a');
			}
		}
		else if ((str[i] >= 'a') && (str[i] < '{'))
		{
			ch = (char) (str[i] + '\r');
			if (ch >= '{')
			{
				ch = (char) (ch - '\x001a');
			}
		}
		else
		{
			ch = str[i];
		}
		str2 = str2 + ch;
	}
	return str2;
}
std::string rot13(const std::string& str) {
	std::string result = "";

	for (int i = 0; i < str.size(); ++i) {
		result += static_cast<char>(str[i] - 1 / (~(~(str[i]) | 32) / 13 * 2 - 11) * 13);
	}

	return result;
}
