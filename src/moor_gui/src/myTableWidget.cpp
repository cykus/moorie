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
#include <QtGui>
#include "myTableWidget.h"
#include <QMessageBox>
#include "infoDialog.h"

myTableWidget::myTableWidget(QWidget *parent) : QTableWidget(parent)
{
createActions();
setUpContextMenu();
}

void myTableWidget::setUpContextMenu()
{
addAction(tPauseAct);
addAction(tInfoAct);
addAction(tRemoveAct);
setContextMenuPolicy(Qt::ActionsContextMenu);
}

void myTableWidget::createActions()
{
tInfoAct = new QAction(QIcon(":/images/help_about.png"), "&Informacje", this);
tInfoAct->setStatusTip(tr("Wyświetla informacje o uploadzie"));
tPauseAct = new QAction(QIcon(":/images/pause.png"), "Wstrzymaj/Wznów", this);
tPauseAct->setStatusTip(tr("Wstrzymuje/Wznawia pobieranie uploadu"));
tRemoveAct = new QAction(QIcon(":/images/exit.png"), "&Usuń", this);
tRemoveAct->setStatusTip(tr("Usuwa zadanie"));
}

