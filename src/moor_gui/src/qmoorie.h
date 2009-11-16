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
#ifndef QMOORIE_H
#define QMOORIE_H
#include <QtGui>
#include <QtCore>

#include "ui_qmoorie.h"

#include "about.h"
#include "addDownload.h"
#include "configdialog.h"
#include "infoDialog.h"
#include "myTableWidget.h"
#include "tabledelegate.h"
#include "threadinstance.h"
#include "threadlogs.h"
// moorie
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <MoorhuntHash.h>
#include <Status.h>
//
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

namespace Ui
{
    class MainWindow;
}

class QMoorie:public QMainWindow
{
    Q_OBJECT

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *contextMenu;
    QMenu *tableMenu;

    QAction *addAct;
    QAction *aboutAct;
    QAction *exitAct;
    QAction *aboutQtAct;
    QAction *settingsAct;
    QAction *pauseAct;
    QAction *playAct;
    QAction *infoAct;
    QAction *removeAct;

    QStringList headarH;
    QSystemTrayIcon *tray;
    QToolBar *fileToolBar;

    Ui::MainWindow *ui;
    boost::thread *statusesThread;

    void refreshStatuses();

protected:
    void closeEvent(QCloseEvent *event);

public:
    QMoorie(QWidget * parent = 0, Qt::WFlags f = 0 );
    ~QMoorie();

    void addInstance(QString, QString = ""); //!< Tworzymy nową instację pobierania
    void createTable(); //!< Tworzy myTableview dla karty pobieranie
    void createToolBars(); //!< Tworzy pasek narzędziowy
    void createActions(); //!< Tworzy akcje dla przycisków
    void loadDownloads();
    void readConfigFile();
    void writeConfigFile();
    void setTray();
    void setLog();

    addDownload *dodaj;
    myTableWidget *tabela;
    QVector<threadInstance*> tInstance; //!< Wektor instancji klasy threadInstance
    threadLogs tLogs; //!< Wątek odpowiedzialny za wyświetlanie logów

public Q_SLOTS:
    void addDialog();
    void aboutDialog();
    void infoDialog();
    void showSettings();
    void exitApp();
    void toggleVisibility();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);

};
#endif
