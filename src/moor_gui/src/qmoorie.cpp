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
#include "qmoorie.h"

QMoorie::QMoorie(QWidget * parent, Qt::WFlags f):QMainWindow(parent, f), ui(new Ui::MainWindow),
downloadInstanceIndex(0), uploadInstanceIndex(0)
{
    setWindowIcon( QIcon(":/images/hi64-app-qmoorie.png"));
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + " - Klient P2M");

    ui->setupUi(this);
    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForLocale ( QTextCodec::codecForName ("UTF-8"));

    createTable();
    createActions();
    createToolBars();
    readConfigFile();
    setTray();

    unsigned int logLevel(Zmienne().LLEVEL);
    logLevel = static_cast<unsigned int>( Log::Error ) - logLevel + 1;
    LogGuiHandle *logh = new LogGuiHandle(static_cast<Log::Level>( logLevel ) );
    Log::getLog()->addHandle(logh);
    LogConsoleHandle *logh2 = new LogConsoleHandle(static_cast<Log::Level>( logLevel ) );
    Log::getLog()->addHandle(logh2);

    loadDownloads();
    loadUploads();

    statuses.start();
}

void QMoorie::setTray()
{
    tray = new mySystemTrayIcon();
    tray->setIcon(QIcon(":/images/hi64-app-qmoorie.png") );
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    contextMenu = new QMenu();
    contextMenu->addAction(addDownloadAct);
    contextMenu->addAction(settingsAct);
    contextMenu->addSeparator();
    contextMenu->addAction(exitAct);
    tray->setContextMenu(contextMenu);
    if(Zmienne().TRAY) tray->show();
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

    fileToolBar = addToolBar(tr("Plik"));
    int iconSize = fileToolBar->iconSize().width();
    if(iconSize <= 16) iconSize = 16;
    if(iconSize > 16 && iconSize <= 22) iconSize = 22;
    if(iconSize > 22 && iconSize <= 32) iconSize = 32;
    if(iconSize > 32) iconSize = 48;

#if QT_VERSION >= 0x040600
    addDownloadAct = new QAction(QIcon::fromTheme("list-add", QIcon(":images/hi"+QString::number(iconSize)+"-list-add.png")), tr("&Pobierz"), this);
    addUploadAct = new QAction(QIcon::fromTheme("list-add", QIcon(":images/hi"+QString::number(iconSize)+"-list-add.png")), tr("&Wyślij"), this);
    settingsAct = new QAction(QIcon::fromTheme("configure", QIcon(":images/hi"+QString::number(iconSize)+"-configure.png")),tr("&Ustawienia"),this);
    pauseAct = new QAction(QIcon::fromTheme("media-playback-pause", QIcon(":images/hi"+QString::number(iconSize)+"-media-playback-pause.png")),tr("Wstrzymaj wszystko"),this);
    removeAct = new QAction(QIcon::fromTheme("list-remove", QIcon(":images/hi"+QString::number(iconSize)+"-list-remove.png")),tr("&Usuń"),this);
    aboutAct = new QAction(QIcon::fromTheme("help-about", QIcon(":images/hi"+QString::number(iconSize)+"-help-about.png")), tr("&O programie"), this);
    exitAct = new QAction(QIcon::fromTheme("application-exit", QIcon(":images/hi"+QString::number(iconSize)+"-application-exit.png")), tr("Zakończ"), this);
#else
    addDownloadAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-list-add.png"), tr("&Pobierz"), this);
    addUploadAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-list-add.png"), tr("&Wyślij"), this);
    settingsAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-configure.png"),tr("&Ustawienia"), this);
    pauseAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-media-playback-pause.png"),tr("Wstrzymaj wszystko"), this);
    removeAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-list-remove.png"),tr("&Usuń"),this);
    aboutAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-help-about.png"), tr("&O programie"), this);
    exitAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-application-exit.png"), tr("Zakończ"), this);
#endif

    addDownloadAct->setShortcut(tr("Ctrl+N"));
    addDownloadAct->setStatusTip(tr("Dodanie nowego pliku do pobrania"));
    connect(addDownloadAct, SIGNAL(triggered()), this, SLOT(showNewDownloadDialog()));

    addUploadAct->setStatusTip(tr("Dodanie nowego pliku do wysłania"));
    connect(addUploadAct, SIGNAL(triggered()), this, SLOT(showNewUploadDialog()));

    settingsAct -> setShortcut(tr("Ctrl+Alt+S"));
    settingsAct -> setStatusTip(tr("Konfiguracja aplikacji"));
    connect(settingsAct, SIGNAL(triggered()), this ,SLOT(showConfigDialog()));

    pauseAct->setDisabled(true);
    pauseAct -> setStatusTip(tr("Wstrzymanie/wznowienie pobierania wszystkich plików"));

    removeAct -> setStatusTip(tr("Usunięcie pobierania"));

    aboutAct->setStatusTip(tr("Informacje o aplikacji"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Wyjście z aplikacji"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exitApp()));

    connect(downloadTable->tRemoveAct, SIGNAL(triggered()), this, SLOT(removeDownload()));
    connect(downloadTable->tPauseAct, SIGNAL(triggered()), this, SLOT(pauseDownload()));
    connect(uploadTable->tRemoveAct, SIGNAL(triggered()), this, SLOT(removeUpload()));
    connect(uploadTable->tPauseAct, SIGNAL(triggered()), this, SLOT(pauseUpload()));
    connect(&statuses, SIGNAL(refresh()), this, SLOT(refreshStatuses()));
}

void QMoorie::createToolBars()
{
    fileToolBar->addAction(addDownloadAct);
    fileToolBar->addAction(addUploadAct);
    fileToolBar->addAction(removeAct);
    fileToolBar->addSeparator();
    //fileToolBar->addAction(pauseAct);
    fileToolBar->addAction(settingsAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(exitAct);
    fileToolBar->addAction(aboutAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(exitAct);
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileToolBar->setMovable(false);
}
void QMoorie::createTable()
{

    QStringList downloadHeaderH;
    downloadTable = new myTableWidget();
    QVBoxLayout *main = new QVBoxLayout;
    main -> addWidget(downloadTable);
    ui->tab_2->setLayout(main);
    downloadTable->setEditTriggers(0);
    downloadTable->setItemDelegate(new TrackDelegate());
    downloadTable->setColumnCount(8);
    downloadTable->horizontalHeader()->setStretchLastSection(true);
    downloadHeaderH << "ID" << "Nazwa pliku" << "Rozmiar" << "Pozostało"<< "Postęp " << "Prędkość" << "Status" << "Skrzynka";
    downloadTable->setHorizontalHeaderLabels(downloadHeaderH);
    downloadTable->hideColumn( 0 );
    downloadTable->setSortingEnabled( 1 );
    downloadTable->sortItems( 1 );

    QStringList uploadHeaderH;
    uploadTable = new myTableWidget();
    QVBoxLayout *main2 = new QVBoxLayout;
    main2 -> addWidget(uploadTable);
    ui->tab_4->setLayout(main2);
    uploadTable->setEditTriggers(0);
    uploadTable->setItemDelegate(new TrackDelegate());
    uploadTable->setColumnCount(8);
    uploadTable->horizontalHeader()->setStretchLastSection(true);
    uploadHeaderH << "ID" << "Nazwa pliku" << "Rozmiar" << "Wysłano"<< "Postęp " << "Prędkość" << "Status" << "Skrzynka";
    uploadTable->setHorizontalHeaderLabels(uploadHeaderH);
    uploadTable->hideColumn ( 0 );
    uploadTable->setSortingEnabled( 1 );
    uploadTable->sortItems( 1 );
}
void QMoorie::showNewUploadDialog()
{
    newUploadDialog *get = new newUploadDialog(this);
    get->exec();
    if(get->result())
    {
        QTableWidgetItem *mailboxLogin;
        QTableWidgetItem *mailboxPass;
        QVector<mirrorMailbox*> mirrorMailboxes;
        for(int i = 0; i < get->mirrorTable->rowCount(); ++i)
        {
            mirrorMailboxes.append(new mirrorMailbox());

            mailboxLogin = get->mirrorTable->item(i, 0);
            mailboxPass = get->mirrorTable->item(i, 1);

            mirrorMailboxes.last()->username = mailboxLogin->text();
            mirrorMailboxes.last()->password = mailboxPass->text();
        }
        addUploadInstance(get->fileEdit->text(), mirrorMailboxes, get->downPassEdit->text(), get->editPassEdit->text(), get->segSizeSlider->value(), 1);
        saveUploads();
    }
    delete get;
}
void QMoorie::showNewDownloadDialog()
{
    newDownloadDialog *get = new newDownloadDialog(this);
    get->exec();
    if(get->result())
    {
        QString path = get->pathEdit->text();
        if((path.lastIndexOf(QDir::separator()) != path.length() - 1) && (path.length() > 1)) path += QDir::separator();
        addDownloadInstance(get->text->toPlainText(), get->edit->text(), path);
        saveDownloads();
    }
    delete get;
}
/**
* Tworzymy nowy wątek wysyłania
*/
void QMoorie::addUploadInstance(QString file, QVector<mirrorMailbox*> mirrorMailboxes, QString downPass, QString editPass, int msize, int fromSeg)
{
    int itemNumber = uploadInstanceIndex;
    int itemRow = uploadTable->rowCount();
    QFileInfo fileInfo;
    fileInfo.setFile(file);

    uploadInstanceH.insert(itemNumber, new uploadInstance(file, mirrorMailboxes, downPass, editPass, msize, fromSeg));
    uploadInstanceH[itemNumber]->fileName = fileInfo.fileName();
    uploadInstanceH[itemNumber]->fileSize = fileInfo.size();
//  uploadInstanceH[itemNumber]->totalSegments = uploadInstanceH[itemNumber]->Instance->getMyUploadNumOfSeg();
//  uploadInstanceH[itemNumber]->fileCRC = QString::fromStdString(uploadInstanceH[itemNumber]->Instance->getMyUploadFileCRC());
    uploadInstanceH[itemNumber]->start();
    uploadTable->setRowCount(uploadTable->rowCount() + 1);
    
    uploadTable->setItem(itemRow, ID, new QTableWidgetItem(QString::number(itemNumber)));
    uploadTable->setItem(itemRow, NAME, new QTableWidgetItem(uploadInstanceH[itemNumber]->fileName));
    uploadTable->setItem(itemRow, SIZE, new QTableWidgetItem(fileSize(uploadInstanceH[itemNumber]->fileSize)));
    uploadTable->setItem(itemRow, REMAINING, new QTableWidgetItem("?"));
    QTableWidgetItem *postepWysylania = new QTableWidgetItem;

    postepWysylania->setData(Qt::DisplayRole, 0);
    uploadTable->setItem(itemRow, PROGRESS, postepWysylania);
    uploadTable->setItem(itemRow, SPEED, new QTableWidgetItem("?"));
    uploadTable->setItem(itemRow, STATUS, new QTableWidgetItem());
    uploadTable->setItem(itemRow, MAILBOX, new QTableWidgetItem());
    uploadInstanceIndex++;
}

/**
* Tworzymy nowy wątek pobierania
* @param hash zawiera hashcode
* @param pass zawiera hasło pobierania
* @param path zawiera ścieżkę pobierania
*/
void QMoorie::addDownloadInstance(QString hash, QString pass, QString path)
{
    int itemNumber = downloadInstanceIndex;
    int itemRow = downloadTable->rowCount();
    downloadInstanceH.insert(itemNumber, new downloadInstance(hash, pass, path));
    boost::shared_ptr<Hash> hashcode(HashManager::fromString((downloadInstanceH[itemNumber]->hash.toStdString())));
    downloadInstanceH[itemNumber]->filename = QString::fromStdString(hashcode->getInfo().fileName);
    downloadInstanceH[itemNumber]->size = hashcode->getInfo().fileSize;
    downloadInstanceH[itemNumber]->totalSegments = hashcode->getInfo().numOfSegments;
    downloadInstanceH[itemNumber]->pobrano = false;
    itemRow = downloadTable->rowCount();
    downloadInstanceH[itemNumber]->start();
    
    downloadTable->setRowCount(downloadTable->rowCount() + 1);

    QFileInfo fileInfo;
    fileInfo.setFile(downloadInstanceH[itemNumber]->path+downloadInstanceH[itemNumber]->filename);

    if(fileInfo.exists()) downloadInstanceH[itemNumber]->pobranoLS = fileInfo.size();
    else downloadInstanceH[itemNumber]->pobranoLS = 0;

    downloadTable->setItem(itemRow, ID, new QTableWidgetItem(QString::number(itemNumber)));
    downloadTable->setItem(itemRow, NAME, new QTableWidgetItem(downloadInstanceH[itemNumber]->filename));
    downloadTable->setItem(itemRow, SIZE, new QTableWidgetItem(fileSize(downloadInstanceH[itemNumber]->size)));
    downloadTable->setItem(itemRow, REMAINING, new QTableWidgetItem("?"));
    QTableWidgetItem *postepPobierania = new QTableWidgetItem;
    postepPobierania->setData(Qt::DisplayRole, 0);
    downloadTable->setItem(itemRow, PROGRESS, postepPobierania);
    downloadTable->setItem(itemRow, SPEED, new QTableWidgetItem("?"));
    downloadTable->setItem(itemRow, STATUS, new QTableWidgetItem());
    downloadTable->setItem(itemRow, MAILBOX, new QTableWidgetItem());
    downloadInstanceIndex++;
}

/**
* Funkcja odpowiedzialna za wyświetlanie statusu pobierania
*/
void QMoorie::refreshStatuses()
{
    quint64 allBytesReadSession = 0;
    quint64 allBytesSendSession = 0;
    double allSpeedRead = 0;
    double allSpeedSend = 0;
    for (int i = 0; i < downloadTable->rowCount(); ++i)
    {
        int itemNumber = downloadTable->item(i,ID)->text().toInt();
        if(itemNumber < 0) continue;
        Status status = downloadInstanceH[itemNumber]->Instance->getDownloadStatus();
        if(downloadInstanceH[itemNumber]->Instance->downloadDone && !downloadInstanceH[itemNumber]->pobrano && downloadInstanceH[itemNumber]->Instance->started)
        {
            if(status.state == Status::Downloaded || status.state == Status::Finished)
            {
                downloadTable->item( i, REMAINING )->setText( "0.00 MB" );
                downloadTable->item( i, PROGRESS )->setData(Qt::DisplayRole, 100);
                downloadTable->item( i, SPEED )->setText("0 KB/s");
                downloadTable->item( i, STATUS )->setText(QString::fromStdString(status.getStateText()) +
                               "\n " + QString::number(downloadInstanceH[itemNumber]->totalSegments) +
                               "/" + QString::number(downloadInstanceH[itemNumber]->totalSegments));
                downloadTable->item( i, STATUS )->setForeground(QColor(0, 0, 200, 255));
            }
            if(status.state == Status::Finished)
            {

                for(int j = 0 ; j < 8; j++ )
                {
                    downloadTable->item(i, j)->setBackground(QColor(0, 50, 0, 100));
                }
                tray->showHints("Pobrano pomyślnie", "Pobieranie pliku: <br/><b><i>"+downloadInstanceH[itemNumber]->filename+"</b></i><br/>zakończono pomyślnie.");
                downloadInstanceH.remove(itemNumber);
                downloadTable->item(i,ID)->setText("-1");
                saveDownloads();
            }
            if(status.state == Status::FileError)
            {
                downloadTable->item( i, REMAINING )->setText( "0.00 MB" );
                downloadTable->item( i, PROGRESS )->setData(Qt::DisplayRole, 0);
                downloadTable->item( i, SPEED )->setText("0 KB/s");
                downloadTable->item( i, STATUS )->setText("Nie udało się pobrać pliku z żadnej ze skrzynek");
                for(int j = 0 ; j < 8; j++ )
                {
                    downloadTable->item(i, j)->setBackground(QColor(255, 0, 0, 200));
                }
                downloadInstanceH[itemNumber]->pobrano = true;
                tray->showHints("Błąd pobierania", "Niestety nie udało się pobrać pliku:<br/><b><i>"+downloadInstanceH[itemNumber]->filename+"</i></b>");
            }
        }
        else if(!downloadInstanceH[itemNumber]->Instance->downloadDone && downloadInstanceH[itemNumber]->Instance->started)
        {
            if(!(downloadInstanceH[itemNumber]->Instance->downloadPaused) && status.state != Status::FileError)
            {
                allBytesReadSession += status.bytesRead;

                downloadTable->item( i, REMAINING )->setText( fileSize(downloadInstanceH[itemNumber]->size - downloadInstanceH[itemNumber]->pobranoLS - status.bytesRead) );
                int percentDownloaded = 100.0f * (status.bytesRead + downloadInstanceH[itemNumber]->pobranoLS)  / downloadInstanceH[itemNumber]->size;
                downloadTable->item( i, PROGRESS )->setData(Qt::DisplayRole, percentDownloaded);

                allSpeedRead += static_cast<double>( status.speed) / 1024.0f;
                const double speed( static_cast<double>( status.speed) / 1024.0f );
                std::stringstream s;
                s.setf( std::ios::fixed);
                if ( speed - round( speed ) < 0.1f )
                {
                    s << std::setprecision( 0 );
                }
                else
                {
                    s << std::setprecision( 2 );
                }
                s << speed;
                downloadTable->item( i, SPEED )->setText( QString::fromStdString(s.str()) + " KB/s" );
                downloadTable->item( i, STATUS )->setText(QString::fromStdString(status.getStateText()) +
                               "\n " + QString::number(status.downloadSegment+1) +
                               "/" + QString::number(downloadInstanceH[itemNumber]->totalSegments));

                if(status.state == Status::Connecting || status.state == Status::Connected)
                {
                    downloadTable->item( i, STATUS )->setForeground(QColor(204, 210, 55, 255));
                }
                else if(status.state == Status::Downloading)
                {
                    downloadTable->item( i, STATUS )->setForeground(QColor(0, 100, 0, 255));
                }
                else if(status.state == Status::ConnectionError || status.state == Status::FileError || status.state == Status::SegmentError)
                {
                    downloadTable->item( i, STATUS )->setForeground(QColor(255, 0, 0, 200));
                }
                downloadTable->item( i, MAILBOX )->setText( QString::fromStdString(status.mailboxName) );
            }
            else
            {

                downloadTable->item( i, SPEED )->setText( "0 KB/s" );
                downloadTable->item( i, STATUS )->setText("Wstrzymane");
                for(int j = 0 ; j < 8; j++ )
                {
                    downloadTable->item(i, j)->setBackground(QColor(255, 255, 0, 100));
                }
            }
        }
    }
    for (int i = 0; i < uploadTable->rowCount(); ++i)
    {
        int itemNumber = uploadTable->item(i,ID)->text().toInt();
        if(itemNumber < 0) continue;
        if(uploadInstanceH[itemNumber]->Instance->downloadDone && !uploadInstanceH[itemNumber]->wyslano && uploadInstanceH[itemNumber]->Instance->started)
        {
            Status status = uploadInstanceH[itemNumber]->Instance->getUploadStatus();
            if(status.state == Status::Uploaded || status.state == Status::Finished)
            {
                QTableWidgetItem *PozostaloPliku = new QTableWidgetItem("0.00 MB");
                uploadTable->setItem(i, REMAINING, PozostaloPliku);
                QTableWidgetItem *postepPobierania = new QTableWidgetItem;
                postepPobierania->setData(Qt::DisplayRole, 100);
                uploadTable->setItem(i, PROGRESS, postepPobierania);
                QTableWidgetItem *SzybkoscPobierania = new QTableWidgetItem("0 KB/s");
                uploadTable->setItem(i, SPEED, SzybkoscPobierania);
                QTableWidgetItem *statusPobierania = new QTableWidgetItem(QString::fromStdString(status.getStateText()) +
                "\n " + QString::number(uploadInstanceH[itemNumber]->totalSegments) + "/" + QString::number(uploadInstanceH[itemNumber]->totalSegments));
                statusPobierania->setForeground(QColor(0, 0, 200, 255));
                uploadTable->setItem(i, STATUS, statusPobierania);
            }
            if(status.state == Status::Finished)
            {
                for(int j = 0 ; j < 8; j++ )
                {
                    uploadTable->item(i, j)->setBackground(QColor(0, 50, 0, 100));
                }
                tray->showHints("Wysłano pomyślnie", "Wysyłanie pliku: <br/><b><i>"+uploadInstanceH[itemNumber]->fileName+"</b></i><br/>zakończono pomyślnie.");
                uploadInstanceH.remove(i);
                downloadTable->item(i,ID)->setText("-1");
                saveUploads();
            }
            if(status.state == Status::FileError && !uploadInstanceH[itemNumber]->wyslano)
            {
                QTableWidgetItem *PozostaloPliku = new QTableWidgetItem("0.00 MB");
                uploadTable->setItem(i, REMAINING, PozostaloPliku);
                QTableWidgetItem *postepPobierania = new QTableWidgetItem;
                postepPobierania->setData(Qt::DisplayRole, 0);
                uploadTable->setItem(i, PROGRESS, postepPobierania);
                QTableWidgetItem *statusPobierania = new QTableWidgetItem("Nie udało się Wysłać pliku");
                uploadTable->setItem(i, STATUS, statusPobierania);
                QTableWidgetItem *SzybkoscPobierania = new QTableWidgetItem("0 KB/s");
                uploadTable->setItem(i, SPEED, SzybkoscPobierania);
                for(int j = 0 ; j < 8; j++ )
                {
                    uploadTable->item(i, j)->setBackground(QColor(255, 0, 0, 200));
                }
                uploadInstanceH[itemNumber]->wyslano = true;
                tray->showHints("Błąd wysyłania", "Niestety nie udało się wysłać pliku:<br/><b><i>"+uploadInstanceH[itemNumber]->fileName+"</i></b>");
            }
        }
        else if(!uploadInstanceH[itemNumber]->Instance->downloadDone && uploadInstanceH[itemNumber]->Instance->started)
        {
            Status status = uploadInstanceH[itemNumber]->Instance->getUploadStatus();
            if(!(uploadInstanceH[itemNumber]->Instance->downloadPaused) && status.state != Status::FileError)
            {
                allBytesSendSession += status.bytesRead;

                QTableWidgetItem *WyslanoPliku = new QTableWidgetItem(fileSize(status.bytesRead));
                uploadTable->setItem(i, REMAINING, WyslanoPliku);
                int percentDownloaded = 100.0f * status.bytesRead  / uploadInstanceH[itemNumber]->fileSize;
                QTableWidgetItem *postepWysylania = new QTableWidgetItem;
                postepWysylania->setData(Qt::DisplayRole, percentDownloaded);
                uploadTable->setItem(i, PROGRESS, postepWysylania);

                allSpeedSend += static_cast<double>( status.speed) / 1024.0f;
                const double speed( static_cast<double>( status.speed) / 1024.0f );
                std::stringstream s;
                s.setf( std::ios::fixed);
                if ( speed - round( speed ) < 0.1f )
                {
                    s << std::setprecision( 0 );
                }
                else
                {
                    s << std::setprecision( 2 );
                }
                s << speed;
                QTableWidgetItem *SzybkoscPobierania = new QTableWidgetItem(QString::fromStdString(s.str()) + " KB/s");
                uploadTable->setItem(i, SPEED, SzybkoscPobierania);
                uploadInstanceH[itemNumber]->fromseg = status.downloadSegment;
                QTableWidgetItem *statusPobierania = new QTableWidgetItem(QString::fromStdString(status.getStateText()) +
                                                                          "\n " + QString::number(status.downloadSegment) + "/" + QString::number(uploadInstanceH[itemNumber]->totalSegments));
                if(status.state == Status::Connecting || status.state == Status::Connected)
                {
                    statusPobierania->setForeground(QColor(204, 210, 55, 255));
                }
                else if(status.state == Status::Uploading)
                {
                    statusPobierania->setForeground(QColor(0, 100, 0, 255));
                }
                else if(status.state == Status::ConnectionError || status.state == Status::FileError || status.state == Status::SegmentError)
                {
                    statusPobierania->setForeground(QColor(255, 0, 0, 200));
                }
                uploadTable->setItem(i, STATUS, statusPobierania);
                //qDebug() << status.downloadSegment;
                QTableWidgetItem *SkrzynkaPobierania = new QTableWidgetItem(QString::fromStdString(status.mailboxName));
                uploadTable->setItem(i, MAILBOX, SkrzynkaPobierania);
            }
            else if(uploadInstanceH[itemNumber]->Instance->downloadPaused)
            {
                QTableWidgetItem *SzybkoscPobierania = new QTableWidgetItem("0 KB/s");
                uploadTable->setItem(i, SPEED, SzybkoscPobierania);
                QTableWidgetItem *statusPobierania = new QTableWidgetItem("Wstrzymane");
                uploadTable->setItem(i, STATUS, statusPobierania);
                for(int j = 0 ; j < 8; j++ )
                {
                    uploadTable->item(i, j)->setBackground(QColor(255, 255, 0, 100));
                }
            }
        }
    }
    if(downloadInstanceH.size() || uploadInstanceH.size())
    {

        QSettings settings;
        if(settings.isWritable())
        {
            settings.beginGroup("VARIABLES_QMOORIE");
            settings.setValue("allBytesRead", allBytesRead+allBytesReadSession);
            settings.setValue("allBytesSend", allBytesSend+allBytesSendSession);
            settings.endGroup();
        }
        std::stringstream allS;
        allS.setf( std::ios::fixed);
        if ( allSpeedRead - round( allSpeedRead ) < 0.1f )
        {
            allS << std::setprecision( 0 );
        }
        else
        {
            allS << std::setprecision( 2 );
        }
        allS << allSpeedRead;
        std::stringstream allSs;
        allSs.setf( std::ios::fixed);
        if ( allSpeedSend - round( allSpeedSend ) < 0.1f )
        {
            allSs << std::setprecision( 0 );
        }
        else
        {
            allSs << std::setprecision( 2 );
        }
        allSs << allSpeedSend;
        ui->allBytesReadSession->setText(fileSize(allBytesReadSession));
        ui->allBytesWriteSession->setText(fileSize(allBytesSendSession));
        ui->maxDownloadSpeed->setText(QString::fromStdString(allS.str()) +" KB/s");
        ui->maxUploadSpeed->setText(QString::fromStdString(allSs.str()) +" KB/s");

        QString tip = "<table cellpadding='2' cellspacing='2' align='center'><tr><td><b>Szybkość:</b></td><td></td></tr><tr><td>Pobieranie: <font color='#1c9a1c'>"+ QString::fromStdString(allS.str()) +" KB/s</font></td><td>Wysyłanie: <font color='#990000'>"+ QString::fromStdString(allSs.str()) +" KB/s</font></td></tr><tr><td><b>Transfer:</b></td><td></td></tr><tr><td>Pobrano: <font color='#1c9a1c'>"+ fileSize(allBytesReadSession) +"</font></td><td>Wysłano: <font color='#990000'>"+ fileSize(allBytesSendSession) +"</font></td></tr></table>";
        tray->setToolTip(tip);
    }
    ui->allBytesRead->setText(fileSize(allBytesRead+allBytesReadSession));
    ui->allBytesWrite->setText(fileSize(allBytesSend+allBytesSendSession));
    if(Zmienne().logs != ""){
        ui->log->append(Zmienne().logs);
        Zmienne().logs = "";
    }
}
void QMoorie::removeDownload()
{
    int row = downloadTable->currentRow();
    int itemNumber = downloadTable->item(row, ID)->text().toInt();
    qDebug() << "row: " << row << "  itemNumber: " << itemNumber;
    QString fileName = downloadInstanceH[itemNumber]->path + downloadInstanceH[itemNumber]->filename;
    if(downloadInstanceH[itemNumber]->Instance->downloadDone)
    {
        downloadTable->removeRow(row);
        downloadInstanceH.remove(itemNumber);

    }
    else
    {
        downloadTable->removeRow(row);
        downloadInstanceH[itemNumber]->terminate();
        downloadInstanceH.remove(itemNumber);
        if (QFile::exists(fileName)) QFile::remove(fileName);
        if (QFile::exists(fileName + ".seg")) QFile::remove(fileName + ".seg");
    }
    saveDownloads();
}
void QMoorie::pauseDownload()
{
    int itemNumber = downloadTable->item(uploadTable->currentRow(),ID)->text().toInt();

    if(downloadInstanceH[itemNumber]->Instance->downloadPaused)
    {
        downloadInstanceH[itemNumber]->Instance->unpauseDownload();
        for(int j = 0 ; j < 8; j++ )
        {
            downloadTable->item(itemNumber, j)->setBackground(backgroundBrush);
        }
    }
    else
    {
        downloadInstanceH[itemNumber]->Instance->pauseDownload();
    }
}
void QMoorie::removeUpload()
{
    int row = uploadTable->currentRow();
    int itemNumber = uploadTable->item(row, ID)->text().toInt();

    if(uploadInstanceH[itemNumber]->Instance->downloadDone)
    {
        uploadInstanceH.remove(itemNumber);
        uploadTable->removeRow(row);
    }
    else
    {
        uploadInstanceH[itemNumber]->terminate();
        uploadInstanceH.remove(itemNumber);
        uploadTable->removeRow(row);
    }
    saveUploads();
}
void QMoorie::pauseUpload()
{
    int itemNumber = uploadTable->item(uploadTable->currentRow(), ID)->text().toInt();

    if(uploadInstanceH[itemNumber]->Instance->downloadPaused)
    {
        uploadInstanceH[itemNumber]->Instance->unpauseDownload();
        for(int j = 0 ; j < 8; j++ )
        {
            uploadTable->item(itemNumber, j)->setBackground(backgroundBrush);
        }
    }
    else
    {
        uploadInstanceH[itemNumber]->Instance->pauseDownload();
    }
}
void QMoorie::loadDownloads()
{
    QDomDocument dokument_xml;
    QFile dokument(Zmienne().configPath+"hashcodes.xml");
    dokument.open( QIODevice::ReadOnly );
    dokument_xml.setContent( &dokument );
    dokument.close();

    QDomNode downloads;
    downloads = dokument_xml.documentElement();

    QDomNode download, item;
    download = downloads.firstChild();

    while(!download.isNull())
    {
        QDomElement hash,pass,path;

        item = download.namedItem("hash");
        hash = item.toElement();
        item = download.namedItem("pass");
        pass = item.toElement();
        item = download.namedItem("path");
        path = item.toElement();
        
        try
        {
            boost::shared_ptr<Hash> hhio(HashManager::fromString(hash.text().toStdString()));
            if (hhio->getInfo().valid)
            {
                if(hhio->checkAccessPassword(pass.text().toStdString()))
                {
                    QString pathstr = path.text();
                    
                    if((pathstr.lastIndexOf(QDir::separator()) != pathstr.length() - 1) && (pathstr.length() > 1)) pathstr += QDir::separator();

                    addDownloadInstance(hash.text(), pass.text(), pathstr);
                }
                else QMessageBox::about(this, tr("Błąd"),tr("Hasło nieprawidłowe! "));
            }
            else QMessageBox::about(this, tr("Błąd"),tr("Źle skopiowany lub niepoprawny hashcode!"));
        }
        catch (std::exception& e)
        {
            QMessageBox::about(this, tr("Błąd"),tr("Nieobsługiwany hashcode!"));
        }
        download = download.nextSibling();
    }
}
void QMoorie::saveDownloads()
{
    QDomDocument dokument_xml;
    QDomElement downloads, download, hash,pass,path;
    QDomText val;

    downloads = dokument_xml.createElement( "downloads" );
    dokument_xml.appendChild(downloads);

    QHashIterator<int, downloadInstance*> it(downloadInstanceH);
    while (it.hasNext()) {
        it.next();
        download = dokument_xml.createElement( "download" );
        downloads.appendChild(download);

        hash = dokument_xml.createElement( "hash");
        download.appendChild(hash);
        val = dokument_xml.createTextNode(it.value()->hash);
        hash.appendChild(val);

        pass = dokument_xml.createElement( "pass");
        download.appendChild(pass);
        val = dokument_xml.createTextNode(it.value()->pass);
        pass.appendChild(val);

        path = dokument_xml.createElement( "path");
        download.appendChild(path);
        val = dokument_xml.createTextNode(it.value()->path);
        path.appendChild(val);
    }
    QFile dokument(Zmienne().configPath+"hashcodes.xml");
    if(dokument.open(QFile::WriteOnly))
    {
        QTextStream ts(&dokument);
        ts << dokument_xml.toString();
    }
}
void QMoorie::loadUploads()
{
    QDomDocument dokument_xml;
    QFile dokument(Zmienne().configPath+"uploads.xml");
    dokument.open( QIODevice::ReadOnly );
    dokument_xml.setContent( &dokument );
    dokument.close();

    QDomNode uploads;
    uploads = dokument_xml.documentElement();

    QDomNode upload, item;
    upload = uploads.firstChild();

    while(!upload.isNull())
    {
        QDomElement file,dpass,epass,msize,fromseg;

        item = upload.namedItem("file");
        file = item.toElement();

        QDomNode mailboxes;
        mailboxes = upload.namedItem("mailboxes");

        QDomNode mailbox, item;
        mailbox = mailboxes.firstChild();

        QVector<mirrorMailbox*> mirrorMailboxes;
        while(!mailbox.isNull())
        {
            QDomElement username,password;
            mirrorMailboxes.append(new mirrorMailbox());

            item = mailbox.namedItem("username");
            username = item.toElement();

            item = mailbox.namedItem("password");
            password = item.toElement();

            mirrorMailboxes.last()->username = username.text();
            mirrorMailboxes.last()->password = password.text();

            mailbox = mailbox.nextSibling();
        }
        item = upload.namedItem("dpass");
        dpass = item.toElement();
        item = upload.namedItem("epass");
        epass = item.toElement();
        item = upload.namedItem("msize");
        msize = item.toElement();
        item = upload.namedItem("fromseg");
        fromseg = item.toElement();

        addUploadInstance(file.text(), mirrorMailboxes, dpass.text(),epass.text(), msize.text().toInt(), fromseg.text().toInt());

        upload = upload.nextSibling();
    }
}
void QMoorie::saveUploads()
{
    QDomDocument dokument_xml;
    QDomElement uploads, upload, mailboxes, mailbox, file, username, password, dpass, epass, msize, fromseg;
    QDomText val;

    uploads = dokument_xml.createElement( "uploads" );
    dokument_xml.appendChild(uploads);

    QHashIterator<int, uploadInstance*> it(uploadInstanceH);
    while (it.hasNext()) {
        it.next();
        upload = dokument_xml.createElement( "upload" );
        uploads.appendChild(upload);

        file = dokument_xml.createElement( "file");
        upload.appendChild(file);
        val = dokument_xml.createTextNode(it.value()->file);
        file.appendChild(val);

        mailboxes = dokument_xml.createElement( "mailboxes" );
        upload.appendChild(mailboxes);

        for(int j = 0; j < it.value()->mirrorMailboxes.size(); ++j)
        {
            mailbox = dokument_xml.createElement( "mailbox" );
            mailboxes.appendChild(mailbox);

            username = dokument_xml.createElement( "username");
            mailbox.appendChild(username);
            val = dokument_xml.createTextNode(it.value()->mirrorMailboxes.at(j)->username);
            username.appendChild(val);

            password = dokument_xml.createElement( "password");
            mailbox.appendChild(password);
            val = dokument_xml.createTextNode(it.value()->mirrorMailboxes.at(j)->password);
            password.appendChild(val);

        }
        dpass = dokument_xml.createElement( "dpass");
        upload.appendChild(dpass);
        val = dokument_xml.createTextNode(it.value()->dpass);
        dpass.appendChild(val);

        epass = dokument_xml.createElement( "epass");
        upload.appendChild(epass);
        val = dokument_xml.createTextNode(it.value()->epass);
        epass.appendChild(val);

        msize = dokument_xml.createElement( "msize");
        upload.appendChild(msize);
        val = dokument_xml.createTextNode(QString::number(it.value()->msize));
        msize.appendChild(val);

        fromseg = dokument_xml.createElement( "fromseg");
        upload.appendChild(fromseg);
        val = dokument_xml.createTextNode(QString::number(it.value()->fromseg));
        fromseg.appendChild(val);
    }
    QFile dokument(Zmienne().configPath+"uploads.xml");
    if(dokument.open(QFile::WriteOnly))
    {
        QTextStream ts(&dokument);
        ts << dokument_xml.toString();
    }
}
void QMoorie::showAboutDialog()
{
    aboutDialog *get = new aboutDialog(this);
    get->exec();
    delete get;
}
void QMoorie::showInfoDialog()
{
    InfoDialog *get = new InfoDialog(this);
    get->exec();
    delete get;
}
void QMoorie::showConfigDialog()
{
    ConfigDialog *get = new ConfigDialog();
    get->exec();
    if(!Zmienne().TRAY) tray->hide();
    else tray->show();
    delete get;
}
void QMoorie::readConfigFile()
{
    QHeaderView *header = downloadTable->horizontalHeader();
    QHeaderView *header2 = uploadTable->horizontalHeader();
    header->setDefaultAlignment(Qt::AlignLeft);
    header2->setDefaultAlignment(Qt::AlignLeft);
    QSettings settings;
    settings.beginGroup("CONFIG_PAGE");
    Zmienne().PATH = settings.value("PATH", "home").toString();
    Zmienne().LLEVEL = settings.value("LLEVEL", 6).toInt();
    Zmienne().DLEVEL = settings.value("DLEVEL", 2).toInt();
    Zmienne().NLEVEL = settings.value("NLEVEL", 1).toInt();
    Zmienne().RUNINTRAY = settings.value("RUNINTRAY", false).toBool();
    Zmienne().ASKBEFORECLOSE = settings.value("ASKBEFORECLOSE", false).toBool();
    Zmienne().KSEGMENTS = settings.value("KSEGMENTS", true).toBool();
    Zmienne().TRAY = settings.value("TRAY", true).toBool();
    settings.endGroup();

    settings.beginGroup("UPLOAD_PAGE");
    Zmienne().SEGSIZE = settings.value("SEGSIZE", 7).toInt();
    Zmienne().DOWNPASS =  settings.value("DOWNPASS", "").toString();
    Zmienne().EDITPASS =  settings.value("EDITPASS", "").toString();
    settings.endGroup();

    settings.beginGroup("GEOMETRY_QMOORIE");
    resize(settings.value("size", QSize(848, 280)).toSize());
    move(settings.value("pos", QPoint(0, 0)).toPoint());
    header->resizeSection( 1, settings.value("tabela_column_0", 260).toInt() );
    header->resizeSection( 2, settings.value("tabela_column_1", 80).toInt() );
    header->resizeSection( 3, settings.value("tabela_column_2", 70).toInt() );
    header->resizeSection( 4, settings.value("tabela_column_3", 120).toInt() );
    header->resizeSection( 5, settings.value("tabela_column_4", 65).toInt() );
    header->resizeSection( 6, settings.value("tabela_column_5", 120).toInt() );
    header->resizeSection( 7, settings.value("tabela_column_6", 100).toInt() );
    header2->resizeSection( 1, settings.value("tabela2_column_0", 260).toInt() );
    header2->resizeSection( 2, settings.value("tabela2_column_1", 80).toInt() );
    header2->resizeSection( 3, settings.value("tabela2_column_2", 70).toInt() );
    header2->resizeSection( 4, settings.value("tabela2_column_3", 120).toInt() );
    header2->resizeSection( 5, settings.value("tabela2_column_4", 65).toInt() );
    header2->resizeSection( 6, settings.value("tabela2_column_5", 120).toInt() );
    header2->resizeSection( 7, settings.value("tabela2_column_6", 100).toInt() );
    settings.endGroup();
    settings.beginGroup("VARIABLES_QMOORIE");
    allBytesRead = settings.value("allBytesRead", 0).toUInt() ;
    allBytesSend = settings.value("allBytesSend", 0).toUInt() ;
    settings.endGroup();

    writeConfigFile();

    Zmienne().configPath = settings.fileName();
    //qDebug() << "configPath: " << Zmienne().configPath;
    Zmienne().configPath.remove(Zmienne().configPath.lastIndexOf(QDir::separator())+1,Zmienne().configPath.size());
    //qDebug() << "configPath: " << Zmienne().configPath;
}
void QMoorie::writeConfigFile()
{
    QHeaderView *header = downloadTable->horizontalHeader();
    QHeaderView *header2 = uploadTable->horizontalHeader();
    QSettings settings;
    if(settings.isWritable()){
        settings.beginGroup("CONFIG_PAGE");
        settings.setValue("PATH", Zmienne().PATH);
        settings.setValue("LLEVEL", Zmienne().LLEVEL);
        settings.setValue("DLEVEL", Zmienne().DLEVEL);
        settings.setValue("NLEVEL", Zmienne().NLEVEL);
        settings.setValue("KSEGMENTS", Zmienne().KSEGMENTS);
        settings.setValue("TRAY", Zmienne().TRAY);
        settings.endGroup();

        settings.beginGroup("UPLOAD_PAGE");
        settings.setValue("SEGSIZE", Zmienne().SEGSIZE);
        settings.setValue("DOWNPASS", Zmienne().DOWNPASS);
        settings.setValue("EDITPASS", Zmienne().EDITPASS);
        settings.endGroup();

        settings.beginGroup("GEOMETRY_QMOORIE");
        settings.setValue("size", size());
        settings.setValue("pos", pos());
        settings.setValue("tabela_column_0", header->sectionSize(1));
        settings.setValue("tabela_column_1", header->sectionSize(2));
        settings.setValue("tabela_column_2", header->sectionSize(3));
        settings.setValue("tabela_column_3", header->sectionSize(4));
        settings.setValue("tabela_column_4", header->sectionSize(5));
        settings.setValue("tabela_column_5", header->sectionSize(6));
        settings.setValue("tabela_column_6", header->sectionSize(7));
        settings.setValue("tabela2_column_0", header2->sectionSize(1));
        settings.setValue("tabela2_column_1", header2->sectionSize(2));
        settings.setValue("tabela2_column_2", header2->sectionSize(3));
        settings.setValue("tabela2_column_3", header2->sectionSize(4));
        settings.setValue("tabela2_column_4", header2->sectionSize(5));
        settings.setValue("tabela2_column_5", header2->sectionSize(6));
        settings.setValue("tabela2_column_6", header2->sectionSize(7));
        settings.endGroup();
    }
    else
    {
        QMessageBox::critical(NULL, "QMoorie", "Nie można zapisać pliku konfiguracyjnego do\n "+settings.fileName(), "OK");
        this->close();
    }
}
bool QMoorie::showExitAppConfirmDialog()
{
    QMessageBox msgBox;

    msgBox.setText("Potwierdzenie zakończenia");
    msgBox.setInformativeText("Czy na pewno chcesz zakończyć aplikację?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        return true;
        break;
    case QMessageBox::No:
        return false;
        break;
    default:
        // should never be reached
        break;
    }

}
void QMoorie::closeEvent(QCloseEvent *event)
{
    writeConfigFile();
    if(Zmienne().TRAY){
        event->ignore();
        hide();
    }
    else
    {
        if(Zmienne().ASKBEFORECLOSE)
        {
            if(showExitAppConfirmDialog()) qApp->quit();
        }
        else qApp->quit();
    }
}
void QMoorie::exitApp()
{
    writeConfigFile();
    if(Zmienne().ASKBEFORECLOSE)
    {
        if(showExitAppConfirmDialog()) qApp->quit();
    }
    else qApp->quit();
}
QMoorie::~QMoorie()
{
}
QMoorie::LogGuiHandle::LogGuiHandle( Log::Level lvl): LogHandle(lvl)
{
}
QMoorie::LogGuiHandle::~LogGuiHandle()
{
}
void QMoorie::LogGuiHandle::log(const char *msg)
{
    QTime time = QTime::currentTime();
    Zmienne().logs += "<font color=\"red\"><b>" + time.toString("hh:mm:ss") + "</b></font> - "
                      + QString::fromUtf8(msg) + "<br/>";

}
