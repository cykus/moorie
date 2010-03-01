/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
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

#include "Mailboxes.h"
#include <iostream>

std::string getMailboxName(int id) {
	switch (id)
	{
//      case Walla: return "walla.com";
		case Yahoo: return "yahoo.com";
		case GMail: return "gmail.com";
		case O2:    return "o2.pl";
		case WP:    return "wp.pl";
		case OiComBr: return "oi.com.br";
		case MyNet: return "mynet.com";
//      case GMX:   return "gmx.net";
//      case M2X:   return "m2x.net";
//      case Lycos: return "lycos.com";
//      case Rediff: return "rediff";
//      case CentrumSk: return "centrum.sk";
//      case CentrumCz: return "centrum.cz";
//      case BolComBr:  return "bol.com.br";
//      case PostSk: return "post.sk";
		case AOLPL: return "aol.pl";
		case AOLDE: return "aol.de";
		case Gazeta: return "gazeta.pl";
		case Gala: return "gala.net";
//              case Conexcol: return "conexcol.com";
		case Yandex: return "yandex.ru";
//              case MailCom: return "mail.com";
		case MailRu: return "mail.ru";
//      case Sify:  return "sify.com";
//      case Nifmail: return "nifmail.jp";
		case Bigmir: return "bigmir.net";
		case AzetSk: return "azet.sk";
		case PocztaOnet: return "poczta.onet.pl";
		case OnetEu: return "onet.eu";
//      case GmxCom: return "Gmx.com";
		case RockCom: return "rock.com";
                case MaiboxHu : return "mailbox.hu";
//                case HotmailCom: return "hotmail.com";
//                case InterfreeIt: return "interfree.it";
//                case TiscaliIt: return "tiscali.it";
                case VivapolskaTv: return "vivapolska.tv";
 //               case WippiesCom: return "wippies.com";
                case GdeRu : return "gde.ru";
                case KlikniCz: return "klikni.cz";
                case LivedoorCom: return "livedoor.com";
                case OneindiaIn : return "oneindia.in";
                case VolnyCz : return "volny.cz";
               case PortugalMail : return  "portugalmail.pt";
 		default:    return "Nieobslugiwana !";
	}

}
