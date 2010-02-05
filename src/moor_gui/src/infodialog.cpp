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
#include "infodialog.h"

InfoDialog::InfoDialog(QMoorie* parent)
{

    setWindowIcon( QIcon(":/images/hi64-app-qmoorie.png") );
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + " - Konfiguracja");

    info[0] = new QLabel(tr("Tytuł: "));
    info[1] = new QLabel(tr("Nazwa pliku: "));
    info[2] = new QLabel(tr("Rozmiar: "));
    info[3] = new QLabel(tr("Liczba segmentów: "));
    info[4] = new QLabel(tr("Liczba mirrorów: "));
    info[5] = new QLabel(tr("Mirrory: \n"));
    info[6] = new QLabel(tr("Uploader: "));
    info[7] = new QLabel(tr("Opis: "));
    info[8] = new QLabel(tr("Link do okładki: "));
    info[9] = new QLabel(tr("Komentarz: "));
    info[10] = new QLabel(tr("Hashcode: "));
    Hashcode = new QTextEdit();
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton(tr("Anuluj"));
    okButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout -> addWidget(okButton);

    QVBoxLayout *main = new QVBoxLayout;
    main -> addWidget(info[0]);
    main -> addWidget(info[1]);
    main -> addWidget(info[2]);
    main -> addWidget(info[3]);
    main -> addWidget(info[4]);
    main -> addWidget(info[5]);
    main -> addWidget(info[6]);
    main -> addWidget(info[7]);
    main -> addWidget(info[8]);
    main -> addWidget(info[9]);
    main -> addWidget(info[10]);
    main -> addWidget(Hashcode);
    main -> addSpacing(10);
    main -> addLayout(buttonLayout);
    setLayout(main);

    okButton->setFocus();

    connect( okButton, SIGNAL(clicked()), this, SLOT(close()));

    getHashInfo();
}
void InfoDialog::getHashInfo()
{
}
