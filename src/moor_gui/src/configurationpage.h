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
#ifndef CONFIGURATIONPAGE_H
#define CONFIGURATIONPAGE_H
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QRadioButton>
#include "singleton.h"
#include "tools.h"

class ConfigurationPage : public QWidget
{
    Q_OBJECT
public:
    QCheckBox *kBox, *tBox, *thBox, *abcBox;
    QSpinBox *dBox, *lBox;
    QGroupBox *notifyGroup;
    QRadioButton *xdgNotification, *hintNotification, *noneNotification ;
    QLineEdit *pathEdit;
    ConfigurationPage(QWidget *parent = 0);
private:  

public Q_SLOTS :
    void setDir();
    void setCheckedState();
};
#endif
