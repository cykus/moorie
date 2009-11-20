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
#include <QDomDocument>

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
#include <HashManager.h>
#include <Status.h>
#include <Log.h>
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

    QStringList headerH;
    QSystemTrayIcon *tray;
    QToolBar *fileToolBar;

    Ui::MainWindow *ui;
    boost::thread *statusesThread; //!< Wątek odpowiedzialny za wyświetlanie statusów
    boost::thread *logsThread; //!< Wątek odpowiedzialny za wyświetlanie logów

    void refreshStatuses();
    void refreshLogs();

    class LogGuiHandle: public LogHandle
    {
    public:
        LogGuiHandle(Log::Level lvl);
        virtual ~LogGuiHandle();
        virtual void log(const char *msg);
    };

protected:
    void closeEvent(QCloseEvent *event);

public:
    QMoorie(QWidget * parent = 0, Qt::WFlags f = 0 );
    ~QMoorie();

    void addInstance(QString, QString, QString = ""); //!< Tworzymy nową instację pobierania
    void createTable(); //!< Tworzy myTableview dla karty pobieranie
    void createToolBars(); //!< Tworzy pasek narzędziowy
    void createActions(); //!< Tworzy akcje dla przycisków
    void readConfigFile(); //!< Wczytuje konfigurację z pliku
    void writeConfigFile(); //!< Zapisuje konfiguracje do pliku
    void loadDownloads(); //!< Wczytuje listę plików do pobrania z poprzedniej sesji
    void saveDownloads(); //!< Zapisuje listę plików aktualnie pobieranych
    void setTray();

    bool stop;
    addDownload *dodaj;
    myTableWidget *tabela; //!< Klasa dziedzicząca po QTableWidget
    QVector<threadInstance*> tInstance; //!< Wektor instancji klasy threadInstance
    
public Q_SLOTS:
    void addDialog();
    void aboutDialog();
    void infoDialog();
    void showSettings();
    void exitApp();
    void toggleVisibility();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void removeDownload(); //!< Usuwa wybrane pobieranie

};
#endif
