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
#include "addDownload.h"

#include <Hash.h>

addDownload::addDownload(QWidget * parent, Qt::WFlags f):QDialog(parent, f)
{
    setWindowIcon( QIcon(":/images/hi64-app-qmoorie.png") );
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + " - Dodaj plik");
    QSize size(500,250);
    resize(size);
    label[0]=new QLabel(tr("Hashcode: "));
    text = new QTextEdit();
    label[1]=new QLabel(tr("Hasło: "));
    edit = new QLineEdit();
    label[2] = new QLabel(tr("Folder Pobierania:"));
    pathEdit = new QLineEdit(Zmienne().PATH);
    if(Zmienne().PATH=="home") pathEdit->setText(QDir::homePath());
    pathButton = new QPushButton(tr("Przeglądaj"));
    pathEdit->setEnabled(false);

    button[0] = new QPushButton(tr("Ok"));
    button[1] = new QPushButton(tr("Anuluj"));
    QVBoxLayout *main = new QVBoxLayout;
    lay[0] = new QHBoxLayout;
    lay[0] -> addWidget(label[0]);
    main -> addLayout(lay[0]);
    lay[1] = new QHBoxLayout;
    lay[1] -> addWidget(text);
    main -> addLayout(lay[1]);
    lay[2] = new QHBoxLayout;
    lay[2] -> addWidget(label[1]);
    lay[2] -> addWidget(edit);
    main -> addLayout(lay[2]);
    lay[3] = new QHBoxLayout;
    lay[3] -> addWidget(label[2]);
    lay[3] -> addWidget(pathEdit);
    lay[3] -> addWidget(pathButton);
    main -> addLayout(lay[3]);
    lay[4] = new QHBoxLayout;
    lay[4] -> addWidget(button[0]);
    lay[4] -> addWidget(button[1]);
    main -> addLayout(lay[4]);
    setLayout(main);

    QObject::connect( button[1], SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect( button[0], SIGNAL(clicked()), this, SLOT(ok()));
    QObject::connect(pathButton,SIGNAL(clicked()),this,SLOT(setDir()));


}
void addDownload::ok()
{
    try {
        boost::shared_ptr<Hash> hash(HashManager::fromString(text->toPlainText().toStdString()));
        if (hash->getInfo().valid)
        {
            if(hash->checkAccessPassword( edit->text().toStdString()))
            {
                accept();
            }
            else QMessageBox::about(this, tr("Błąd"),tr("Hasło nieprawidłowe! "));
        }
        else QMessageBox::about(this, tr("Błąd"),tr("Źle skopiowany lub niepoprawny hashcode!"));
    }
    catch (std::exception& e)
    {
        QMessageBox::about(this, tr("Błąd"),tr("Nieobsługiwany hashcode!"));
    }
}
void addDownload::setDir()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this,
                                 tr("Wybierz folder"),
                                 "",
                                 options);
    if (!directory.isEmpty()) pathEdit->setText(directory);
}
