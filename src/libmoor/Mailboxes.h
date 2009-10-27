#ifndef _MAILBOXES_H_
#define _MAILBOXES_H_

#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "MailBoxes/MailRu.h"

enum MBoxID {
	Walla        = 0x01,
	Yahoo        = 0x02,
	GMail        = 0x03,
	O2           = 0x04,
	WP           = 0x09,
	GMX          = 0x0c,
	M2X          = 0x0d,
	Lycos        = 0x0f,
	PortugalMail = 0x10,
	Rediff 		= 0x11,
	CentrumSk 	= 0x12,
	CentrumCz 	= 0x14,
	BolComBr  = 0x22,
	PostSk    = 0x43,
	OiComBr   = 72,
	MyNet     = 22,
	MailRu    = 24,
	AOLPL     = -27,
	AOLDE     = -12,
	Gazeta    = -93,
	Gala      = -31,
	Conexcol  = -46,
	Yandex    = 53,
	Sify      = 44,
	MailCom   = 51,
	Nifmail   = -24,
	Interia   = -95,
	Bigmir    = -29,
	AzetSk    = -56,
	PocztaOnet= -86,
	OnetEu    = 71,
	GmxCom    = 70,
	RockCom   = 23
};

std::string getMailboxName(int id);

#endif
