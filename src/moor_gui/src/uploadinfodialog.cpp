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
#include "uploadinfodialog.h"

UploadInfoDialog::UploadInfoDialog(QString infoString)
{

    setWindowIcon( QIcon(":/images/hi64-app-qmoorie.png") );
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + tr(" - Informacje"));

    info = new QLabel(infoString);
    info->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    Hashcode = new QTextEdit();
    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *cancelButton = new QPushButton(tr("Anuluj"));
    okButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout -> addWidget(okButton);

    QVBoxLayout *main = new QVBoxLayout;
    main -> addWidget(info);
    main -> addSpacing(10);
    main -> addLayout(buttonLayout);
    setLayout(main);

    okButton->setFocus();

    connect( okButton, SIGNAL(clicked()), this, SLOT(close()));

    getHashInfo();
}
void UploadInfoDialog::getHashInfo()
{
}
