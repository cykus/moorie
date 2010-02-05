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
#include "mytablewidget.h"

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
#if QT_VERSION >= 0x040600
    tInfoAct = new QAction(QIcon::fromTheme("help-about", QIcon(":images/hi16-help-about.png")), "&Informacje", this);
    tPauseAct = new QAction(QIcon::fromTheme("media-playback-pause", QIcon(":images/hi16-media-playback-pause.png")), "Wstrzymaj/Wznów", this);
    tRemoveAct = new QAction(QIcon::fromTheme("list-remove", QIcon(":images/hi16-list-remove.png")), "&Usuń", this);
#else
    tInfoAct = new QAction(QIcon(":/images/hi16-help-about.png"), "&Informacje", this);
    tPauseAct = new QAction(QIcon(":/images/hi16-media-playback-pause.png"), "Wstrzymaj/Wznów", this);
    tRemoveAct = new QAction(QIcon(":/images/hi16-list-remove.png"), "&Usuń", this);
#endif
    tInfoAct->setStatusTip(tr("Wyświetla informacje o uploadzie"));
    tPauseAct->setStatusTip(tr("Wstrzymuje/Wznawia pobieranie uploadu"));
    tRemoveAct->setStatusTip(tr("Usuwa zadanie"));
}

