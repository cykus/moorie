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
#include "uploadpage.h"

UploadPage::UploadPage(QWidget *parent)
    : QWidget(parent)
{
    mailboxGroup = new QGroupBox(tr("Skrzynki do wysyłania"));

    mailboxTable = new QTableWidget();
    mailboxTable->setEditTriggers(0);
    mailboxTable->setColumnCount(2);
    mailboxTable->hideColumn(1);
    mailboxTable->horizontalHeader()->setStretchLastSection(true);
    header  << "Użytkownik" << "Hasło" ;
    mailboxTable->setHorizontalHeaderLabels(header);

    loginLabel = new QLabel("Użytkownik:");
    loginEdit = new QLineEdit();

    passLabel = new QLabel("Hasło:");
    passEdit = new QLineEdit();

    saveButton = new QPushButton("Zapisz");
    addButton = new QPushButton("Dodaj");
    deleteButton = new QPushButton("Usuń");

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    dataGroup = new QGroupBox(tr("Dane do wysyłania"));

    segSizeLabel = new QLabel("Rozmiar segmentu:");
    segSizeSlider = new QSlider(Qt::Horizontal);
    segSizeSlider->setMinimum(1);
    segSizeSlider->setMaximum(10);
    segSizeSlider->setValue(Zmienne().SEGSIZE);
    segSizeLabel2 = new QLabel();
    setSliderLabel(Zmienne().SEGSIZE);

    downPassLabel = new QLabel("Hasło pobierania:");
    downPassEdit = new QLineEdit(Zmienne().DOWNPASS);

    editPassLabel = new QLabel("Hasło edycji:");
    editPassEdit = new QLineEdit(Zmienne().EDITPASS);

    QHBoxLayout *loginLayout = new QHBoxLayout;
    loginLayout -> addWidget(loginLabel);
    loginLayout -> addWidget(loginEdit);

    QHBoxLayout *passLayout = new QHBoxLayout;
    passLayout -> addWidget(passLabel);
    passLayout -> addWidget(passEdit);

    QHBoxLayout *mailboxButtonLayout = new QHBoxLayout;
    mailboxButtonLayout -> addWidget(saveButton);
    mailboxButtonLayout -> addWidget(addButton);
    mailboxButtonLayout -> addWidget(deleteButton);

    QVBoxLayout *mailboxRightLayout = new QVBoxLayout;
    mailboxRightLayout -> addLayout(loginLayout);
    mailboxRightLayout -> addLayout(passLayout);
    mailboxRightLayout -> addLayout(mailboxButtonLayout);
    mailboxRightLayout -> addItem(spacerItem);

    QHBoxLayout *mailboxLayout = new QHBoxLayout;
    mailboxLayout->addWidget(mailboxTable);
    mailboxLayout->addLayout(mailboxRightLayout);
    mailboxGroup->setLayout(mailboxLayout);
////
    QHBoxLayout *segSizeLayout = new QHBoxLayout;
    segSizeLayout -> addWidget(segSizeLabel);
    segSizeLayout -> addWidget(segSizeSlider);
    segSizeLayout -> addWidget(segSizeLabel2);

    QHBoxLayout *downPassLayout = new QHBoxLayout;
    downPassLayout -> addWidget(downPassLabel);
    downPassLayout -> addWidget(downPassEdit);

    QHBoxLayout *editPassLayout = new QHBoxLayout;
    editPassLayout -> addWidget(editPassLabel);
    editPassLayout -> addWidget(editPassEdit);

    QVBoxLayout *dataLayout = new QVBoxLayout;
    dataLayout -> addLayout(segSizeLayout);
    dataLayout -> addLayout(downPassLayout);
    dataLayout -> addLayout(editPassLayout);
    dataGroup->setLayout(dataLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mailboxGroup);
    mainLayout->addWidget(dataGroup);
    setLayout(mainLayout);

    connect(addButton,SIGNAL(clicked()),this,SLOT(addMailbox()));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveMailbox()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteMailbox()));
    connect(mailboxTable,SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),this,SLOT(editMailbox()));
    connect(segSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(setSliderLabel(int)));

    loadMailboxesFromFile();
}
void UploadPage::setSliderLabel(int value)
{
    segSizeLabel2->setText(QString::number(value)+" MB");
}
void UploadPage::addMailbox()
{
    int row = mailboxTable->rowCount();
    mailboxTable->setRowCount(mailboxTable->rowCount() + 1);
    QTableWidgetItem *mailboxLogin = new QTableWidgetItem(loginEdit->text());
    mailboxTable->setItem(row, 0, mailboxLogin);
    QTableWidgetItem *mailboxPass = new QTableWidgetItem(passEdit->text());
    mailboxTable->setItem(row, 1, mailboxPass);
    loginEdit->clear();
    passEdit->clear();
    saveMailboxesInFile();
}
void UploadPage::editMailbox()
{
    int row = mailboxTable->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *mailboxLogin = mailboxTable->item(row, 0);
        loginEdit->setText(mailboxLogin->text());
        QTableWidgetItem *mailboxPass = mailboxTable->item(row, 1);
        passEdit->setText(mailboxPass->text());
    }
}
void UploadPage::saveMailbox()
{
    int row = mailboxTable->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *mailboxLogin = new QTableWidgetItem(loginEdit->text());
        mailboxTable->setItem(row, 0, mailboxLogin);
        QTableWidgetItem *mailboxPass = new QTableWidgetItem(passEdit->text());
        mailboxTable->setItem(row, 1, mailboxPass);
        saveMailboxesInFile();
    }

}
void UploadPage::deleteMailbox()
{
    int row = mailboxTable->currentRow();
    if(row >= 0)
    {
        mailboxTable->removeRow(row);
    }
}
void UploadPage::saveMailboxesInFile()
{
    QDomDocument dokument_xml;
    QDomElement mailboxes, mailbox, login, password;
    QDomText val;
    QTableWidgetItem *mailboxLogin;
    QTableWidgetItem *mailboxPass;

    mailboxes = dokument_xml.createElement( "mailboxes" );
    dokument_xml.appendChild(mailboxes);

    for(int i = 0; i < mailboxTable->rowCount(); ++i) {

        mailboxLogin = mailboxTable->item(i, 0);
        mailboxPass = mailboxTable->item(i, 1);

        mailbox = dokument_xml.createElement( "mailbox" );
        mailboxes.appendChild(mailbox);

        login = dokument_xml.createElement( "login");
        mailbox.appendChild(login);
        val = dokument_xml.createTextNode(mailboxLogin->text());
        login.appendChild(val);

        password = dokument_xml.createElement( "password");
        mailbox.appendChild(password);
        val = dokument_xml.createTextNode(mailboxPass->text());
        password.appendChild(val);
    }
    QFile dokument(Zmienne().configPath+"mailboxes.xml");
    if(dokument.open(QFile::WriteOnly))
    {
        QTextStream ts(&dokument);
        ts << dokument_xml.toString();
    }
}
void UploadPage::loadMailboxesFromFile()
{
    QDomDocument dokument_xml;
    QFile dokument(Zmienne().configPath+"mailboxes.xml");
    dokument.open( QIODevice::ReadOnly );
    dokument_xml.setContent( &dokument );
    dokument.close();

    QDomNode mailboxes;
    mailboxes = dokument_xml.documentElement();

    QDomNode mailbox, item;
    mailbox = mailboxes.firstChild();

    while(!mailbox.isNull())
    {
        QDomElement login,pass;

        item = mailbox.namedItem("login");
        login = item.toElement();
        item = mailbox.namedItem("password");
        pass = item.toElement();

        int row = mailboxTable->rowCount();
        mailboxTable->setRowCount(mailboxTable->rowCount() + 1);
        QTableWidgetItem *mailboxLogin = new QTableWidgetItem(login.text());
        mailboxTable->setItem(row, 0, mailboxLogin);
        QTableWidgetItem *mailboxPass = new QTableWidgetItem(pass.text());
        mailboxTable->setItem(row, 1, mailboxPass);

        mailbox = mailbox.nextSibling();
    }
}
