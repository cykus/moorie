/*
 * This file is a part of Moorie.
 *
 * Copyright (C) 2007 Krzysztof Konieczny <silent@aol.pl>
 *
 * Moorie is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#include <cctype>
#include <boost/lexical_cast.hpp>
//#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "EmailHeader.h"
#include "Log.h"

#include <iostream>
std::string EmailHeader::decode( const std::string& code )
{
    std::string code_temp( code );
    boost::erase_all( code_temp, " " );
    std::string code1;

	int digits;
	digits = 0;
    int l = code_temp.length();
    if (code_temp.length() > 18 ) {
		for (int i = 0; i < code_temp.length()-18; i++) {
			if (!isdigit (code_temp[i]) )
				digits++;
		}
		int cut = code_temp.length() - digits;
		code1 = code_temp.substr(l-cut, l);
	}
    else
		code1 = code_temp;

    try{

    std::string sa,sb,sc;
    std::string crc1,crc2;
    int iseg;
    sc = code1;
    for( int i = 0; i< sc.length(); ++i){
       char ch = sc.at(i);
        isdigit( ch ) ? sa += ch : sb += ch;
    }
    if ( sb.length() == 17 )
//        code1 = code1.substr( sb.length() - ( 0x10 + sa.length() ) );
	code1 = code_temp.substr(l-17, l);
    else if ( sb.length() == 15)
	code1 = code_temp.substr(l-19, l);
    int i;
    std::string s1,s2;
    for ( int j=0; j < code1.length(); ++j ){
        if ( isdigit(code1.substr(j,1).at(0) ) ){
            char ch = code1.substr(j,1).at(0);
            i=ch;
            s1+=ch;
        }
        else
            s2 += code1.substr(j,1);
    }
    std::string s3,s4;
    s2 = asciiDecode( s2 );
    for ( int j=0; s2.length() == 0x10 && j<s2.length();j+=2 ){
        s3 += s2.substr(j,1);
        s4 += s2.substr(j+1,1);
    }
    iseg = boost::lexical_cast<int>(s1);
    if ( (iseg % 2) == 1 ){
        crc1 = s4;
        crc2 = s3;
    }
    else {
        crc1 = s3;
        crc2 = s4;
    }
    boost::to_upper(crc1);
    boost::to_upper(crc2);
	LOG(Log::Debug, "Segment: ["+crc1+"]"+"["+crc2+"]"+"["+boost::lexical_cast<std::string>(iseg)+"]");
    return "["+crc1+"]"+"["+crc2+"]"+"["+boost::lexical_cast<std::string>(iseg)+"]";
    }
    catch(...){
	return code;
    }
}

std::string EmailHeader::asciiDecode( const std::string& code )
{
    std::string code1 = code;
    int j;
    for( int i = 0; i < code1.length(); ++i )
    {
        if ( ! (( code1.at(i) < 0x41 ) || ( code.at(i) > 0x4A ))){
            j = i;
            code1.at(i) = code1.at(j) - 0x11;
        }
        else {
            if ( ! (( code.at(i) < 0x6D ) || ( code.at(i) > 0x7A ))){
                j = i;
                code1.at(i) = code1.at(j) - 0xD;
            }
            else {
                if ( ! (( code.at(i) < 0x61 ) || ( code.at(i) > 0x6D))){
                    j=i;
                    code1.at(i) = code1.at(j) + 0xD;
                }
            }
        }
    }
    return code1;
}


