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

#ifndef __MAILRU_H
#define __MAILRU_H

#include <string>
#include "../Mailbox.h"

class MailRuMailbox: public CMailBox
{
		std::string auth, page;
		int totalEmails;
		int pgcnt;


	public:
                /*
                 * Konstruktor klasy skrzynki
                 *
                 * @param std::string $name Nazwa skrzynki
                 * @param std::string $usr Nazwa użytkownika
                 * @param std::string $passwd Hasło do skrzynki
                 */
                MailRuMailbox(const std::string &name, const std::string &usr, const std::string &passwd);
		~MailRuMailbox();

                /*
                 * Logowanie do skrzynki
                 *
                 * Funkcja logująca do skrzynki zwaraca 0 jeśli
                 * zalogowano pomyślinie
                 */
		int loginRequest();
                /*
                 * Wylogowanie ze skrzynki
                 *
                 * Narazie funkcja nieużywana
                 */
		void logoutRequest();
                /*
                 * Pobieranie nagłówków
                 *
                 * Funkcja pobiera listę wszystkich segmentów (nagłówek maila + adres do niego)
                 */
		void getHeadersRequest();
                /*
                 * Pobieranie segmentu
                 *
                 * @param int seg numer segmentu
                 */
                int downloadRequest(int seg);
                /*
                 * Wysyłanie  segmentu
                 *
                 * @param int seg nazwa pliku
                 * @param std::vector<std::string> to adresy e-mail na które ma wysłać segmenty
                 * @param int seg numer segmentu
                 */
		int uploadRequest(std::string filename, std::vector<std::string> to, int seg);

		void parseResponse();
};

#endif

