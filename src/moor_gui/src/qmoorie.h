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
#ifndef QMOORIE_H
#define QMOORIE_H

#include <QDomDocument>
#include <QDebug>
#include <QTextCodec>
#include <QToolBar>
#include <QSettings>
#include <QTextStream>

#include "ui_qmoorie.h"

#include "aboutdialog.h"
#include "newdownloaddialog.h"
#include "newuploaddialog.h"
#include "configdialog.h"
#include "infodialog.h"
#include "mysystemtrayicon.h"
#include "mytablewidget.h"
#include "tabledelegate.h"
#include "downloadinstance.h"
#include "uploadinstance.h"
#include "statusesthread.h"
#include "tools.h"
// moorie
#include <HashManager.h>
#include <Status.h>
#include <Log.h>
//
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#ifdef Q_OS_WIN
    #include <windows.h>
    inline void sleepMs(int ms) {
    {
        ::Sleep(ms);
    }
#else
    #include <unistd.h>
    inline void sleepMs(int ms) {
        static const unsigned MilliToMicro = 1000;
        ::usleep(ms * MilliToMicro);
    }
#endif

namespace Ui
{
    class MainWindow;
}
class QMoorie:public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *contextMenu;
    QMenu *tableMenu;

    QAction *addDownloadAct;
    QAction *addUploadAct;
    QAction *aboutAct;
    QAction *exitAct;
    QAction *aboutQtAct;
    QAction *settingsAct;
    QAction *pauseAct;
    QAction *infoAct;
    QAction *removeAct;

    QToolBar *fileToolBar;

    QStringList headerH;

    quint64 allBytesRead; //!< Całkowity rozmiar pobranych danych
    mySystemTrayIcon *tray; //!< Klasa dziedzicząca po QTableWidget
    myTableWidget *downloadTable; //!< Klasa dziedzicząca po QTableWidget
    myTableWidget *uploadTable; //!< Klasa dziedzicząca po QTableWidget
    statusesThread statuses;
    QVector<downloadInstance*> downloadInstanceV; //!< Wektor instancji klasy downloadInstance
    QVector<uploadInstance*> uploadInstanceV; //!< Wektor instancji klasy uploadInstance

    void createTable(); //!< Tworzy myTableview dla kart pobieranie i wysyłanie
    void createToolBars(); //!< Tworzy pasek narzędziowy
    void createActions(); //!< Tworzy akcje dla przycisków
    void readConfigFile(); //!< Wczytuje konfigurację z pliku
    void writeConfigFile(); //!< Zapisuje konfiguracje do pliku
    void setTray(); //!< Ustawia ikonkę w tacce systemowej
    void loadDownloads(); //!< Wczytuje listę plików do pobrania z poprzedniej sesji
    void saveDownloads(); //!< Zapisuje listę plików aktualnie pobieranych
    void addDownloadInstance(QString, QString, QString = ""); //!< Tworzymy nową instację pobierania
    void addUploadInstance(QString, QVector<mirrorMailbox*>, QString, QString, int, int); //!< Tworzymy nową instację wysyłania
    bool showExitAppConfirmDialog();


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
    
public Q_SLOTS:
    void showNewDownloadDialog(); //!< Wyświetla okno dodawania pliku do pobrania
    void showNewUploadDialog(); //!< Wyświetla okno dodawania pliku do wysłania
    void showAboutDialog(); //!< Wyświetla okno O programie
    void showInfoDialog(); //!< Wyświetla okno z informacją o danym pliku
    void showConfigDialog(); //!< Wyświetla okno konfiguracyjne
    void exitApp(); //!< Kończy program
    void toggleVisibility();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void removeDownload(); //!< Usuwa pobieranie wybranego pliku
    void pauseDownload(); //!< Wstrzymuje/Wznawia pobieranie wybranego pliku
    void refreshStatuses();
};
#endif
