/*
 *   Copyright (C) 2008-2009 by Patryk Połomski
 *   cykuss@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifndef THREADINSTANCE_H
#define THREADINSTANCE_H
#include <QtCore>
#include <QThread>
#include <QTime>

#include <LibMoor.h>
#include <iostream>

class threadInstance : public QThread
{
public:
    threadInstance(QString, QString, QString = "");
    CLibMoor vInstance; //!< Instancja klasy CLibMoor

    void run();
    bool pobrano;
    float size; //!< rozmiar pliku
    int totalSegments; //!< liczba wszystkich segmentów
    unsigned int itemRow; //!< Nr. wiersza w tabeli
    long pobranoLS; //!< Ile pobrano w poprzedniej sesji
    QString hash; //!< hashcode pliku
    QString path; //!< Ścieżka pobierania pliku
    QString pass; //!< hasło pliku
    QString filename; //!< nazwa pliku
};

#endif // THREADINSTANCE_H
