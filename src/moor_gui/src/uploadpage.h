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
#ifndef UPLOADPAGE_H
#define UPLOADPAGE_H
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QSpacerItem>
#include <QSlider>
#include <QPushButton>
#include <QDir>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QRadioButton>
#include <QHeaderView>
#include <QDomDocument>
#include <QTextStream>
#include "singleton.h"

class UploadPage : public QWidget
{
    Q_OBJECT
public:

    QTableWidget *mailboxTable;
    QLabel *loginLabel, *passLabel, *segSizeLabel, *segSizeLabel2, *downPassLabel, *editPassLabel;
    QLineEdit *loginEdit, *passEdit, *segSizeEdit, *downPassEdit, *editPassEdit;
    QPushButton *saveButton, *addButton, *deleteButton;
    QGroupBox *mailboxGroup, *dataGroup;
    QSlider *segSizeSlider;
    QSpacerItem *spacerItem;
    QStringList header;

    UploadPage(QWidget *parent = 0);

    void saveMailboxesInFile();
    void loadMailboxesFromFile();
public Q_SLOTS :
    void setSliderLabel(int);
    void addMailbox();
    void editMailbox();
    void saveMailbox();
    void deleteMailbox();
};
#endif // UPLOADPAGE_H
