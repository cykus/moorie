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
