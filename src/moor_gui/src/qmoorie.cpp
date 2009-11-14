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
#include "qmoorie.h"


QMoorie::QMoorie(QWidget * parent, Qt::WFlags f):QMainWindow(parent, f)
{
    setWindowIcon( QIcon(":/images/hi16-app-qmoorie.png"));
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + " - Hashcode Downloader");

    ui.setupUi(this);
    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForLocale ( QTextCodec::codecForName ("UTF-8"));

    Zmienne().LLEVEL = 8;
    Zmienne().DLEVEL = 2;
    Zmienne().KSEGMENTS = true;
    stop = true;
    idx = 0;
    m = 0;

    createTable();
    readConfigFile();
    createActions();
    createToolBars();
    setTray();
    //loadDownloads();
    unsigned int logLevel(8);
    logLevel = static_cast<unsigned int>( Log::Error ) - logLevel + 1;
    LogConsoleHandle *logh = new LogConsoleHandle(static_cast<Log::Level>( logLevel ));
    Log::getLog()->addHandle(logh);
}

void QMoorie::setTray()
{
    tray = new QSystemTrayIcon( this );
    tray->setIcon ( QIcon(":/images/hi64-app-qmoorie.png") );
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    this,SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    contextMenu = new QMenu();
    contextMenu -> addAction(addAct);
    contextMenu -> addAction(settingsAct);
    contextMenu -> addSeparator();
    contextMenu -> addAction(exitAct);
    tray -> setContextMenu(contextMenu);
    tray->show();
}
void QMoorie::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) toggleVisibility();
}
void QMoorie::toggleVisibility()
{
    if(isHidden())
    {
        show();
        if(isMinimized())
        {
            if(isMaximized()) showMaximized();
            else showNormal();
        }
        raise();
        activateWindow();
    }
    else hide();
}
 void QMoorie::createActions()
 {

     addAct = new QAction(QIcon(":/images/add.png"), tr("&Dodaj"), this);
     addAct->setShortcut(tr("Ctrl+N"));
     addAct->setStatusTip(tr("Rozpoczyna pobieranie nowego pliku"));
     connect(addAct, SIGNAL(triggered()), this, SLOT(addDialog()));

     settingsAct = new QAction(QIcon(":images/tool2.png"),tr("&Ustawienia"),this);
     settingsAct -> setShortcut(tr("Ctrl+Alt+S"));
     settingsAct -> setStatusTip(tr("Konfiguracja aplikacji"));
     connect(settingsAct, SIGNAL(triggered()), this ,SLOT(showSettings()));

     playAct = new QAction(QIcon(":images/play.png"),tr("&Wznów"),this);
     playAct -> setStatusTip(tr("Wznawia pobieranie"));

     pauseAct = new QAction(QIcon(":images/pause.png"),tr("Wstrzymaj"),this);
     pauseAct -> setStatusTip(tr("Wstrzymuje pobieranie"));

     removeAct = new QAction(QIcon(":images/remove.png"),tr("&Usuń"),this);
     removeAct -> setStatusTip(tr("Usuwa pobierany plik"));

     aboutAct = new QAction(QIcon(":/images/help_about.png"), tr("&O programie"), this);
     aboutAct->setStatusTip(tr("Pokaż informacje o aplikacji"));
     connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutDialog()));

     exitAct = new QAction(QIcon(":/images/exit.png"), tr("Wyjście"), this);
     exitAct->setShortcut(tr("Ctrl+Q"));
     exitAct->setStatusTip(tr("Wyjście z aplikacji"));
     connect(exitAct, SIGNAL(triggered()), this, SLOT(exitApp()));

     connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(setLog()));
     connect(tabela->tInfoAct, SIGNAL(triggered()), this, SLOT(infoDialog()));
}

 void QMoorie::createToolBars()
 {
     fileToolBar = addToolBar(tr("Plik"));
     fileToolBar->addAction(addAct);
     fileToolBar->addAction(removeAct);
     fileToolBar->addSeparator();
     fileToolBar->addAction(playAct);
     fileToolBar->addAction(pauseAct);
     fileToolBar->addAction(settingsAct);
     fileToolBar->addSeparator();
     fileToolBar->addAction(exitAct);
     fileToolBar->addAction(aboutAct);
     fileToolBar->addSeparator();
     fileToolBar->addAction(exitAct);
     fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
     fileToolBar->setIconSize(QSize(24,24));
 }
void QMoorie::createTable()
{
    tabela = new myTableWidget();
    QVBoxLayout *main = new QVBoxLayout;
    main -> addWidget(tabela);
    ui.tab_2->setLayout(main);
    tabela->setEditTriggers(0);
    tabela->setItemDelegate(new TrackDelegate());
    tabela->setColumnCount(7);
    QHeaderView *header = tabela->horizontalHeader();
    header->resizeSection( 0, 200 );
    header->resizeSection( 5, 120 );
    headarB  << "Nazwa pliku" << "Rozmiar" << "Pozostało"<< "Stan Pobierania " << "Prędkość" << "Status" << "Skrzynka";
    tabela->setHorizontalHeaderLabels( headarB );
}
void QMoorie::addDialog()
{
    addDownload *get = new addDownload(this);
    get->exec();
    if(get->result())addInstance(get->text->toPlainText(), get->pathEdit->text());
    delete get;
}

/**
* Tworzymy nową instację pobierania
* @param hash zawiera hashcode
* @param path zawiera ścieżkę pobierania
*/
void QMoorie::addInstance(QString hash, QString path)
{
    tInstance.append(new threadInstance(hash, path));
    tInstance.last()->start();
    const MoorhuntHash & hashcode (tInstance.last()->hashcode.toStdString());
    tInstance.last()->filename = QString::fromStdString(hashcode.getFileName());
    tInstance.last()->size = hashcode.getFileSize();
    tInstance.last()->pobrano = false;
    tInstance.last()->itemRow = tabela->rowCount();

    tabela->setRowCount(tabela->rowCount() + 1);

    QString fileSize; fileSize.sprintf("%.2f", tInstance.last()->size / 1024 / 1024);

    QTableWidgetItem *nazwaPliku = new QTableWidgetItem(tInstance.last()->filename);
    QTableWidgetItem *rozmiarPliku = new QTableWidgetItem(fileSize + " MB");
    tabela->setItem(tInstance.last()->itemRow, 0, nazwaPliku);
    tabela->setItem(tInstance.last()->itemRow, 1, rozmiarPliku);

}
void QMoorie::aboutDialog()
{
    about *get = new about(this);
    get->exec();
    delete get;
}
void QMoorie::infoDialog()
{
    InfoDialog *get = new InfoDialog(this);
    get->exec();
    delete get;
}
void QMoorie::showSettings()
{
    dialog = new ConfigDialog();
    dialog -> show();
}
void QMoorie::setConfigDir()
{
 QDir::setCurrent(QDir::homePath ());
 QDir currentDir = QDir::current();
 currentDir.mkdir(".config/Qmoorie");
 currentDir.cd(".config/Qmoorie");
 QDir::setCurrent(currentDir.path());
}

//This is called if no config file is available, contains default settings
void QMoorie::writeDefaultConfigFile()
{
 setConfigDir();
 QFile configFile("config.txt"); 

  if (configFile.open(QFile::WriteOnly)) 
  {
     QTextStream out(&configFile); 
     out.setCodec("UTF-8");
     out << "home" <<"\n"; //Folder pobierania
     out << "8" <<"\n"; //Log Level
     out << "2" <<"\n"; //Max Downloads
     out << "ON" <<"\n"; //keep segments
  
  } 
   else
    { 
      QMessageBox::warning(NULL, "QMoorie", "Nie można zapisać pliku konfiguracyjnego.", "OK");
    }; 
 readConfigFile();
}

void QMoorie::readConfigFile()
{//Reads the config file and sets the config variables which are noted all upercase starting
//with C_ (from CONFIG)
 setConfigDir();
 QFile configFile("config.txt");
 
 if (configFile.open(QFile::ReadOnly))
 {
   QTextStream in(&configFile);
   in.setCodec("UTF-8");
   Zmienne().PATH = in.readLine();
   Zmienne().LLEVEL=(in.readLine()).toInt();
   Zmienne().DLEVEL=(in.readLine()).toInt();
   QString ksegmentsString = in.readLine();
   if(ksegmentsString=="ON") Zmienne().KSEGMENTS=true;
   else Zmienne().KSEGMENTS = false;
   
 } 
 else writeDefaultConfigFile();

}

void QMoorie::closeEvent(QCloseEvent *event)
{
            //saveDownloads();
            event->ignore();
            hide();
}
void QMoorie::exitApp()
{
 //saveDownloads();
 qApp->quit();
}
QMoorie::~QMoorie()
{
  stop = true;
}

void QMoorie::setLog()
{
ui.log->setPlainText(Zmienne().logi);
}
QMoorie::LogGuiHandle::LogGuiHandle( Log::Level lvl): LogHandle(lvl)
{
}
QMoorie::LogGuiHandle::~LogGuiHandle()
{
}
void QMoorie::LogGuiHandle::log(const char *msg)
{
QString t;
t = QString::fromUtf8(msg);
QTime time = QTime::currentTime();
Zmienne().logi += time.toString("hh:mm:ss") + " " + t + "\n";
}
