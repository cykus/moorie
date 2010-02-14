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
#include "newuploaddialog.h"

newUploadDialog::newUploadDialog(QWidget * parent, Qt::WFlags f):QDialog(parent, f)
{
    setWindowIcon( QIcon(":/images/hi64-app-qmoorie.png") );
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + " - Dodaj plik");

    fileLabel = new QLabel("Plik:");
    fileEdit = new QLineEdit();
    fileButton = new QPushButton(tr("Przeglądaj"));
    fileEdit->setEnabled(false);

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

    loginLabel = new QLabel("Użytkownik:");
    loginEdit = new QLineEdit();

    passLabel = new QLabel("Hasło:");
    passEdit = new QLineEdit();

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    mirrorGroup = new QGroupBox(tr("Mirrory:"));
    mirrorTable = new QTableWidget();
    mirrorTable->setEditTriggers(0);
    mirrorTable->setColumnCount(2);
    mirrorTable->hideColumn(1);
    mirrorTable->horizontalHeader()->setStretchLastSection(true);
    header  << "Użytkownik" << "Hasło" ;
    mirrorTable->setHorizontalHeaderLabels(header);

    saveButton = new QPushButton("Zapisz");
    addButton = new QPushButton("Dodaj");
    deleteButton = new QPushButton("Usuń");

    sendButton = new QPushButton("Wyślij");
    closeButton = new QPushButton("Zamknij");

    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout -> addWidget(fileLabel);
    fileLayout -> addWidget(fileEdit);
    fileLayout -> addWidget(fileButton);

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

    QHBoxLayout *loginLayout = new QHBoxLayout;
    loginLayout -> addWidget(loginLabel);
    loginLayout -> addWidget(loginEdit);

    QHBoxLayout *passLayout = new QHBoxLayout;
    passLayout -> addWidget(passLabel);
    passLayout -> addWidget(passEdit);

    QHBoxLayout *mirrorButtonLayout = new QHBoxLayout;
    mirrorButtonLayout -> addWidget(saveButton);
    mirrorButtonLayout -> addWidget(addButton);
    mirrorButtonLayout -> addWidget(deleteButton);

    QVBoxLayout *mirrorRightLayout = new QVBoxLayout;
    mirrorRightLayout -> addLayout(loginLayout);
    mirrorRightLayout -> addLayout(passLayout);
    mirrorRightLayout -> addLayout(mirrorButtonLayout);
    mirrorRightLayout -> addItem(spacerItem);

    QHBoxLayout *mirrorLayout = new QHBoxLayout;
    mirrorLayout->addWidget(mirrorTable);
    mirrorLayout->addLayout(mirrorRightLayout);
    mirrorGroup->setLayout(mirrorLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout -> addWidget(sendButton);
    buttonLayout -> addWidget(closeButton);

    QVBoxLayout *main = new QVBoxLayout;

    main -> addLayout(fileLayout);
    main -> addLayout(segSizeLayout);
    main -> addLayout(downPassLayout);
    main -> addLayout(editPassLayout);
    main -> addWidget(mirrorGroup);
    main -> addLayout(buttonLayout);
    setLayout(main);

    //connect( button[1], SIGNAL(clicked()), this, SLOT(reject()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(ok()));
    connect(fileButton,SIGNAL(clicked()),this,SLOT(setFile()));
    connect(addButton,SIGNAL(clicked()),this,SLOT(addMailbox()));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveMailbox()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteMailbox()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(mirrorTable,SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),this,SLOT(editMailbox()));
    connect(segSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(setSliderLabel(int)));

}
void newUploadDialog::ok()
{
    if (!fileEdit->text().isEmpty())
    {
        if(!editPassEdit->text().isEmpty())
        {
            accept();
        }
        else QMessageBox::about(this, tr("Błąd"),tr("Musisz podać hasło edycji"));

    }
    else QMessageBox::about(this, tr("Błąd"),tr("Musisz wybrać plik do wysłania"));
}
void newUploadDialog::setSliderLabel(int value)
{
    segSizeLabel2->setText(QString::number(value)+" MB");
}
void newUploadDialog::setFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik"));
    if (!fileName.isEmpty()) fileEdit->setText(fileName);
}
void newUploadDialog::addMailbox()
{
    int row = mirrorTable->rowCount();
    mirrorTable->setRowCount(mirrorTable->rowCount() + 1);
    QTableWidgetItem *mailboxLogin = new QTableWidgetItem(loginEdit->text());
    mirrorTable->setItem(row, 0, mailboxLogin);
    QTableWidgetItem *mailboxPass = new QTableWidgetItem(passEdit->text());
    mirrorTable->setItem(row, 1, mailboxPass);
    loginEdit->clear();
    passEdit->clear();
}
void newUploadDialog::editMailbox()
{
    int row = mirrorTable->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *mailboxLogin = mirrorTable->item(row, 0);
        loginEdit->setText(mailboxLogin->text());
        QTableWidgetItem *mailboxPass = mirrorTable->item(row, 1);
        passEdit->setText(mailboxPass->text());
    }
}
void newUploadDialog::saveMailbox()
{
    int row = mirrorTable->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *mailboxLogin = new QTableWidgetItem(loginEdit->text());
        mirrorTable->setItem(row, 0, mailboxLogin);
        QTableWidgetItem *mailboxPass = new QTableWidgetItem(passEdit->text());
        mirrorTable->setItem(row, 1, mailboxPass);
    }

}
void newUploadDialog::deleteMailbox()
{
    int row = mirrorTable->currentRow();
    if(row >= 0)
    {
        mirrorTable->removeRow(row);
    }
}
