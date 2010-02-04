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
#include <QLabel>
#include <QDir>
#include <QTextEdit>
#include "singleton.h"
#include <boost/shared_ptr.hpp>
#include <HashManager.h>
#include <Hash.h>
#include <iostream>

 class newUploadDialog: public QDialog
 {
    Q_OBJECT
public:
    QLineEdit *edit;
    QPushButton *pathButton;
    QLineEdit *pathEdit;
    QLabel *label[3];
    QPushButton *button[2];
    QTextEdit *text;
    QHBoxLayout *lay[6];
    newUploadDialog(QWidget * parent = 0, Qt::WFlags f = 0 );
private Q_SLOTS:
    void ok();
    void setDir();
};
#endif // NEWUPLOADDIALOG_H

