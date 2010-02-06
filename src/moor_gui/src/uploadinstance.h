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

struct mirrorMailbox{
    QString username;
    QString password;
    //getToUsernames();
};
class uploadInstance : public QThread
{
    void loadMailboxesFromFile();
public:
    uploadInstance(QString, QVector<mirrorMailbox*>, QString, QString, int, int);
    CLibMoor * Instance; //!< Instancja klasy CLibMoor

    void run();
    bool wyslano;
    QString file; //!< Nazwa pliku do wysłania
    QString user; //!< Nazwa użytkownika do skrzynki
    QString pass; //!< Hasło do skrzynki
    QString dpass; //!< Hasło pobierania
    QString epass; //!< Hasło edycji
    QString to; //!< ciąg mirrorów
    QVector<mirrorMailbox*> mirrorMailboxes; //!< Wektor mirrorów
    QVector<mirrorMailbox*> uploadMailboxes; //!< Wektor skrzynek do uploadu
    int msize; //!< Rozmiar segmentu
    int fromseg; //!< Od którego segmentu
};

#endif // UPLOADINSTANCE_H
