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
// moorie
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <Log.h>
#include <LibMoor.h>
#include <MoorhuntHash.h>
//
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

using namespace Ui;

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
    void setConfigDir();
    void setTray();
    void showLog();
    void writeDefaultConfigFile();

    addDownload *dodaj;
    ConfigDialog *dialog;
    MainWindow ui;
    myTableWidget *tabela;

    std::string hstr ,pass;
    int refreshInterval, idx, n, m, LLEVEL, mbox;
    unsigned int DLEVEL;
    volatile bool stop, KSEGMENTS;
    QString PATH;
    QStringList headarB, headarV;
    QSystemTrayIcon *tray;
    QToolBar *fileToolBar;

    QVector<threadInstance*> tInstance; //!< Wektor instancji klasy threadInstance

    QTableWidgetItem *idItem;
    QTableWidgetItem *nazwaItem;
    QTableWidgetItem *rozmiarItem;
    QTableWidgetItem *pozostaloItem;
    QTableWidgetItem *stanItem;
    QTableWidgetItem *predkoscItem;
    QTableWidgetItem *statusItem;
    QTableWidgetItem *skrzynkaItem;

    class LogGuiHandle: public LogHandle
    {
    public:
        LogGuiHandle(Log::Level lvl);
        virtual ~LogGuiHandle();
        virtual void log(const char *msg);
        bool seg;
    };


public Q_SLOTS:
    void setLog();
    void addDialog();
    void aboutDialog();
    void infoDialog();
    void showSettings();
    void exitApp();
    void toggleVisibility();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);

};
#endif
