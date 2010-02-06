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
#ifndef NEWUPLOADDIALOG_H
#define NEWUPLOADDIALOG_H
#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QDir>
#include <QGroupBox>
#include <QSlider>
#include <QDebug>
#include <QSpacerItem>
#include <QTextEdit>
#include "singleton.h"
#include <iostream>

class newUploadDialog: public QDialog
{
    Q_OBJECT
public:
    QTableWidget *mirrorTable;
    QGroupBox *mirrorGroup;
    QLabel *loginLabel, *passLabel, *fileLabel, *segSizeLabel, *segSizeLabel2, *downPassLabel, *editPassLabel;
    QLineEdit *loginEdit, *passEdit, *fileEdit, *mirrorEdit, *segSizeEdit, *downPassEdit, *editPassEdit;
    QPushButton *saveButton, *addButton, *deleteButton, *sendButton, *fileButton, *closeButton;
    QSlider *segSizeSlider;
    QSpacerItem *spacerItem;
    QStringList header;
    newUploadDialog(QWidget * parent = 0, Qt::WFlags f = 0 );
private Q_SLOTS:
    void ok();
    void setSliderLabel(int);
    void setFile();
    void addMailbox();
    void editMailbox();
    void saveMailbox();
    void deleteMailbox();
};
#endif // NEWUPLOADDIALOG_H

