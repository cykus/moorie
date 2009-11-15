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
#include "configurationpage.h"

ConfigurationPage::ConfigurationPage(QWidget *parent)
    : QWidget(parent)
{
    QGroupBox *downloadGroup = new QGroupBox(tr("Pobieranie"));

    QLabel *pathLabel = new QLabel(tr("Folder Pobierania:"));
    pathEdit = new QLineEdit(Zmienne().PATH);
    if(Zmienne().PATH=="home") pathEdit->setText(QDir::homePath());
    QPushButton *pathButton = new QPushButton(tr("Przeglądaj"));
    pathEdit->setEnabled(false);

    QLabel *dLabel = new QLabel(tr("Maksymalna liczba pobierań:"));
    dBox = new QSpinBox();
    dBox ->setRange(1,10);
    dBox -> setValue(Zmienne().DLEVEL);
    QLabel *kLabel = new QLabel(tr("Pozostaw segmenty po scalaniu:"));
    kBox = new QCheckBox();
    kBox -> setChecked(Zmienne().KSEGMENTS);

    QGroupBox *logGroup = new QGroupBox(tr("Dziennik zdarzeń"));

    QLabel *lLabel = new QLabel(tr("Szczegółowość:"));
    lBox = new QSpinBox();
    lBox ->setRange(1,8);
    lBox -> setValue(Zmienne().LLEVEL);

    QGroupBox *otherGroup = new QGroupBox(tr("Inne"));

    QLabel *tLabel = new QLabel(tr("Ikonka systemowa:"));
    tBox = new QCheckBox();
    tBox->setChecked(Zmienne().TRAY);

    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout -> addWidget(pathLabel);
    pathLayout -> addWidget(pathEdit);
    pathLayout -> addWidget(pathButton);

    QHBoxLayout *dLayout = new QHBoxLayout;
    dLayout -> addWidget(dLabel);
    dLayout -> addWidget(dBox);

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addLayout(pathLayout);
    configLayout->addLayout(dLayout);
    downloadGroup->setLayout(configLayout);

    QHBoxLayout *lLayout = new QHBoxLayout;
    lLayout -> addWidget(lLabel);
    lLayout -> addWidget(lBox);

    QVBoxLayout *logLayout = new QVBoxLayout;
    logLayout->addLayout(lLayout);
    logGroup->setLayout(logLayout);

    QHBoxLayout *tLayout = new QHBoxLayout;
    tLayout -> addWidget(tLabel);
    tLayout -> addWidget(tBox);

    QVBoxLayout *trayLayout = new QVBoxLayout;
    trayLayout->addLayout(tLayout);
    otherGroup->setLayout(trayLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(downloadGroup);
    mainLayout->addWidget(logGroup);
    mainLayout->addWidget(otherGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    QObject::connect(pathButton,SIGNAL(clicked()),this,SLOT(setDir()));

}
void ConfigurationPage::setDir()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this,
                                 tr("Wybierz folder"),
                                 "",
                                 options);
    if (!directory.isEmpty()) pathEdit->setText(directory);
}
