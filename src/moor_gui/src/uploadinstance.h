/*
 *   Copyright (C) 2008-2010 by Patryk Połomski
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
#ifndef UPLOADINSTANCE_H
#define UPLOADINSTANCE_H
#include <QThread>
#include <QTime>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <LibMoor.h>
#include "singleton.h"
#include <iostream>
#include <QMessageBox>
#include <QTextCodec>

struct mirrorMailbox{
    QString username;
    QString password;
};
class uploadInstance : public QThread
{
    void loadMailboxesFromFile();
    QString getToUsernames();
    QString generateInfo();
public:
    uploadInstance(QString, QVector<mirrorMailbox*>, QString, QString, int, unsigned int);
    CLibMoor * Instance; //!< Instancja klasy CLibMoor

    void run();
    bool done;
    QString infoString; //!< Przechowuje informacje o pliku
    QString file; //!< Nazwa pliku wraz ze ścieżką do wysłania
    QString fileName; //!< Nazwa pliku do wysłania
    quint64 fileSize; //!< Rozmiar pliku do wysłania
    QString fileCRC; //!< CRC pliku
    QString user; //!< Nazwa użytkownika do skrzynki
    QString pass; //!< Hasło do skrzynki
    QString dpass; //!< Hasło pobierania
    QString epass; //!< Hasło edycji
    QString to; //!< ciąg mirrorów
    QVector<mirrorMailbox*> mirrorMailboxes; //!< Wektor mirrorów
    QVector<mirrorMailbox*> uploadMailboxes; //!< Wektor skrzynek do uploadu
    int totalSegments; //!< Liczba wszystkich segmentów
    int msize; //!< Rozmiar segmentu
    unsigned int fromseg; //!< Od którego segmentu
    quint64 wyslanoLS; //!< Ile wysłano w poprzedniej sesji
};

#endif // UPLOADINSTANCE_H
