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

#ifndef __YANDEX_RU_H
#define __YANDEX_RU_H

#include <string>
#include "../Mailbox.h"

class YandexRuMailbox: public CMailBox
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
                YandexRuMailbox(const std::string &name, const std::string &usr, const std::string &passwd);
		~YandexRuMailbox();

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

