#include "Utils.h"

const unsigned char* getVerByte(const unsigned char data[][34],
																unsigned char vermaj,
																unsigned char vermin)
{
	for (int i = 0; data[i][0]; ++i)
		if ((data[i][0] == vermaj) && (data[i][1] == vermin))
			return &data[i][2];

	return 0;
}