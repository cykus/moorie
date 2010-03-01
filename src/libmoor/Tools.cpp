/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007 Moorie Team - http://moorie.pl/
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

char hexToAscii(char first, char second)
{
	char hex[5], *stop;
	hex[0] = '0';
	hex[1] = 'x';
	hex[2] = first;
	hex[3] = second;
	hex[4] = 0;
	return strtol(hex, &stop, 16);
}
