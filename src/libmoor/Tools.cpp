#include "Tools.h"

string& str_replace(const string &search, const string &replace, string &subject)
{
    string buffer;

    int sealeng = search.length();
    int strleng = subject.length();

    if (sealeng==0)
        return subject;//no change

    for(int i=0, j=0; i<strleng; j=0 )
    {
        while (i+j<strleng && j<sealeng && subject[i+j]==search[j])
            j++;
        if (j==sealeng)//found 'search'
        {
            buffer.append(replace);
            i+=sealeng;
        }
        else
        {
            buffer.append( &subject[i++], 1);
        }
    }
    subject = buffer;
    return subject;
}

string Rot13(string str)
{
	string str2 = "";
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

