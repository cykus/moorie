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

    pathEdit = new QLineEdit(Zmienne().PATH);
    if(Zmienne().PATH=="home") pathEdit->setText(QDir::homePath());
    QPushButton *pathButton = new QPushButton(tr("Przeglądaj"));
    pathEdit->setEnabled(false);

    QLabel *dLabel = new QLabel(tr("Maksymalna liczba pobieranych plików:"));
    dBox = new QSpinBox();
    dBox ->setRange(1,10);
    dBox -> setValue(Zmienne().DLEVEL);
    kBox = new QCheckBox();
    kBox->setText("Pozostaw segmenty po scalaniu:");
    kBox -> setChecked(Zmienne().KSEGMENTS);

    QGroupBox *logGroup = new QGroupBox(tr("Dziennik zdarzeń"));

    QLabel *lLabel = new QLabel(tr("Szczegółowość:"));
    lBox = new QSpinBox();
    lBox ->setRange(1,8);
    lBox -> setValue(Zmienne().LLEVEL);

    QGroupBox *otherGroup = new QGroupBox(tr("Inne"));

    tBox = new QCheckBox();
    tBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tBox->setText("Pokazuj ikonę w tacce systemowej");
    tBox->setChecked(Zmienne().TRAY);
    thBox = new QCheckBox();
    thBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    thBox->setText("Uruchomiaj zminimalizowane do tacki.");
    thBox->setChecked(Zmienne().RUNINTRAY);
    if(!Zmienne().TRAY) thBox->setEnabled(false);
    abcBox = new QCheckBox();
    abcBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    abcBox->setText("Pytaj o potwierdzenie przed zamnknięciem");
    abcBox->setChecked(Zmienne().ASKBEFORECLOSE);

    notifyGroup = new QGroupBox(parent);
    notifyGroup->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    xdgNotification = new QRadioButton(notifyGroup);
    hintNotification = new QRadioButton(notifyGroup);
    noneNotification = new QRadioButton(notifyGroup);
    notifyGroup->setTitle(tr("Powiadomienia"));
    xdgNotification->setText(tr("Powiadomienia systemowe"));
    hintNotification->setText(tr("Powiadomienia w dymkach"));
    noneNotification->setText(tr("Brak powiadomień"));
    xdgNotification->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hintNotification->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    noneNotification->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    if(Zmienne().NLEVEL == 2) xdgNotification->setChecked(true);
    else if(Zmienne().NLEVEL == 1) hintNotification->setChecked(true);
    else noneNotification->setChecked(true);
    if(!checkXDG()) xdgNotification->setEnabled(false);

    QHBoxLayout *pathLayout = new QHBoxLayout;
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

    QVBoxLayout *otherLayout = new QVBoxLayout;
    otherLayout->addWidget(tBox);
    otherLayout->addWidget(thBox);
    otherLayout->addWidget(abcBox);
    otherGroup->setLayout(otherLayout);

    QVBoxLayout *nGroupLayout = new QVBoxLayout(notifyGroup);
    nGroupLayout->addWidget(xdgNotification);
    nGroupLayout->addWidget(hintNotification);
    nGroupLayout->addWidget(noneNotification);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(downloadGroup);
    mainLayout->addWidget(logGroup);
    mainLayout->addWidget(notifyGroup);
    mainLayout->addWidget(otherGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    connect(pathButton,SIGNAL(clicked()),this,SLOT(setDir()));
    connect(tBox,SIGNAL(stateChanged(int)),this,SLOT(setCheckedState()));

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
void ConfigurationPage::setCheckedState()
{
    if(tBox->isChecked()) thBox->setEnabled(true);
    else thBox->setEnabled(false);
}
