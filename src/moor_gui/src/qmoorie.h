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
#include "downloadinfodialog.h"
#include "uploadinfodialog.h"
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
#include <Tools.h>
//
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#if defined(unix)
    #include <unistd.h>
    inline void sleepMs(int ms) {
        static const unsigned MilliToMicro = 1000;
        ::usleep(ms * MilliToMicro);
    }
#else
    #include <windows.h>
    inline void sleepMs(int ms) {
        ::Sleep(ms);
    }
#endif


// Defines for list columns
#define ID 0
#define NAME 1
#define SIZE 2
#define REMAINING 3
#define PROGRESS 4
#define SPEED 5
#define STATUS 6
#define MAILBOX 7
#define FILEPATH 8

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

    QBrush backgroundBrush;
    quint64 allBytesRead; //!< Całkowity rozmiar pobranych danych
    quint64 allBytesSend; //!< Całkowity rozmiar wysłanych danych
    mySystemTrayIcon *tray; //!< Klasa dziedzicząca po QTableWidget
    myTableWidget *downloadTable; //!< Klasa dziedzicząca po QTableWidget
    myTableWidget *uploadTable; //!< Klasa dziedzicząca po QTableWidget
    statusesThread statuses;
    int downloadInstanceIndex;
    int uploadInstanceIndex;
    QHash<int, downloadInstance*> downloadInstanceH; //!< Hash instancji klasy downloadInstance
    QHash<int, uploadInstance*> uploadInstanceH; //!< Hash instancji klasy uploadInstance

    void createTable(); //!< Tworzy myTableview dla kart pobieranie i wysyłanie
    void createToolBars(); //!< Tworzy pasek narzędziowy
    void createActions(); //!< Tworzy akcje dla przycisków
    void readConfigFile(); //!< Wczytuje konfigurację z pliku
    void writeConfigFile(); //!< Zapisuje konfiguracje do pliku
    void setTray(); //!< Ustawia ikonkę w tacce systemowej
    void loadDownloads(); //!< Wczytuje listę plików do pobrania z poprzedniej sesji
    void saveDownloads(); //!< Zapisuje listę plików aktualnie pobieranych
    void loadUploads(); //!< Wczytuje listę plików do wysłania z poprzedniej sesji
    void saveUploads(); //!< Zapisuje listę plików aktualnie wysyłanych
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
    void showDownloadInfoDialog(); //!< Wyświetla okno z informacją o pobieranym pliku
    void showUploadInfoDialog(); //!< Wyświetla okno z informacją o wysyłanym pliku
    void showConfigDialog(); //!< Wyświetla okno konfiguracyjne
    void exitApp(); //!< Kończy program
    void toggleVisibility();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void removeDownload(); //!< Usuwa pobieranie wybranego pliku
    void removeDownloadWithData(); //!< Usuwa pobieranie wybranego pliku wraz z danymi
    void pauseDownload(); //!< Wstrzymuje/Wznawia pobieranie wybranego pliku
    void removeUpload(); //!< Usuwa wysyłanie wybranego pliku
    void removeUploadWithData(); //!< Usuwa wysyłanie wybranego pliku wraz z danymi
    void pauseUpload(); //!< Wstrzymuje/Wznawia wysyłanie wybranego pliku
    void refreshStatuses();
};
#endif
