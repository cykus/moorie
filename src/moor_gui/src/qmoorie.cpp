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

QMoorie::QMoorie(QWidget * parent, Qt::WFlags f):QMainWindow(parent, f), ui(new Ui::MainWindow)
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
    statuses.start();

    unsigned int logLevel(Zmienne().LLEVEL);
    logLevel = static_cast<unsigned int>( Log::Error ) - logLevel + 1;
    LogGuiHandle *logh = new LogGuiHandle(static_cast<Log::Level>( logLevel ) );
    Log::getLog()->addHandle(logh);
    LogConsoleHandle *logh2 = new LogConsoleHandle(static_cast<Log::Level>( logLevel ) );
    Log::getLog()->addHandle(logh2);

    loadDownloads();


}

void QMoorie::setTray()
{
    tray = new mySystemTrayIcon();
    tray->setIcon(QIcon(":/images/hi64-app-qmoorie.png") );
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    contextMenu = new QMenu();
    contextMenu->addAction(addAct);
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
    addAct = new QAction(QIcon::fromTheme("list-add", QIcon(":images/hi"+QString::number(iconSize)+"-list-add.png")), tr("&Nowy"), this);
    settingsAct = new QAction(QIcon::fromTheme("configure", QIcon(":images/hi"+QString::number(iconSize)+"-configure.png")),tr("&Ustawienia"),this);
    pauseAct = new QAction(QIcon::fromTheme("media-playback-pause", QIcon(":images/hi"+QString::number(iconSize)+"-media-playback-pause.png")),tr("Wstrzymaj wszystko"),this);
    removeAct = new QAction(QIcon::fromTheme("list-remove", QIcon(":images/hi"+QString::number(iconSize)+"-list-remove.png")),tr("&Usuń"),this);
    aboutAct = new QAction(QIcon::fromTheme("help-about", QIcon(":images/hi"+QString::number(iconSize)+"-help-about.png")), tr("&O programie"), this);
    exitAct = new QAction(QIcon::fromTheme("application-exit", QIcon(":images/hi"+QString::number(iconSize)+"-application-exit.png")), tr("Zakończ"), this);
#else
    addAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-list-add.png"), tr("&Nowy"), this);
    settingsAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-configure.png"),tr("&Ustawienia"), this);
    pauseAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-media-playback-pause.png"),tr("Wstrzymaj wszystko"), this);
    removeAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-list-remove.png"),tr("&Usuń"),this);
    aboutAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-help-about.png"), tr("&O programie"), this);
    exitAct = new QAction(QIcon(":images/hi"+QString::number(iconSize)+"-application-exit.png"), tr("Zakończ"), this);
#endif

    addAct->setShortcut(tr("Ctrl+N"));
    addAct->setStatusTip(tr("Dodanie nowego pliku"));
    connect(addAct, SIGNAL(triggered()), this, SLOT(addDialog()));

    settingsAct -> setShortcut(tr("Ctrl+Alt+S"));
    settingsAct -> setStatusTip(tr("Konfiguracja aplikacji"));
    connect(settingsAct, SIGNAL(triggered()), this ,SLOT(showSettings()));

    pauseAct->setDisabled(true);
    pauseAct -> setStatusTip(tr("Wstrzymanie/wznowienie pobierania wszystkich plików"));

    removeAct -> setStatusTip(tr("Usunięcie pobierania"));

    aboutAct->setStatusTip(tr("Informacje o aplikacji"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutDialog()));

    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Wyjście z aplikacji"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exitApp()));

    connect(tabela->tRemoveAct, SIGNAL(triggered()), this, SLOT(removeDownload()));
    connect(tabela->tPauseAct, SIGNAL(triggered()), this, SLOT(pauseDownload()));
    connect(&statuses, SIGNAL(refresh()), this, SLOT(refreshStatuses()));
}

void QMoorie::createToolBars()
{
    fileToolBar->addAction(addAct);
    fileToolBar->addAction(removeAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(pauseAct);
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
    tabela = new myTableWidget();
    QVBoxLayout *main = new QVBoxLayout;
    main -> addWidget(tabela);
    ui->tab_2->setLayout(main);
    tabela->setEditTriggers(0);
    tabela->setItemDelegate(new TrackDelegate());
    tabela->setColumnCount(7);
    tabela->horizontalHeader()->setStretchLastSection(true);
    headerH  << "Nazwa pliku" << "Rozmiar" << "Pozostało"<< "Postęp " << "Prędkość" << "Status" << "Skrzynka";
    tabela->setHorizontalHeaderLabels(headerH);
}
void QMoorie::addDialog()
{
    addDownload *get = new addDownload(this);
    get->exec();
    if(get->result())
    {
        QString path = get->pathEdit->text();
        if((path.lastIndexOf(QDir::separator()) != path.length() - 1) && (path.length() > 1)) path += QDir::separator();
        addInstance(get->text->toPlainText(), get->edit->text(), path);
        saveDownloads();
    }
    delete get;
}

/**
* Tworzymy nowy wątek pobierania
* @param hash zawiera hashcode
* @param pass zawiera hasło pobierania
* @param path zawiera ścieżkę pobierania
*/
void QMoorie::addInstance(QString hash, QString pass, QString path)
{
    tInstance.append(new threadInstance(hash, pass, path));
    boost::shared_ptr<Hash> hashcode(HashManager::fromString((tInstance.last()->hash.toStdString())));
    tInstance.last()->filename = QString::fromStdString(hashcode->getInfo().fileName);
    tInstance.last()->size = hashcode->getInfo().fileSize;
    tInstance.last()->totalSegments = hashcode->getInfo().numOfSegments;
    tInstance.last()->pobrano = false;
    tInstance.last()->itemRow = tabela->rowCount();
    tInstance.last()->start();

    tabela->setRowCount(tabela->rowCount() + 1);

    QFileInfo fileInfo;
    fileInfo.setFile(tInstance.last()->path+tInstance.last()->filename);

    if(fileInfo.exists()) tInstance.last()->pobranoLS = fileInfo.size();
    else tInstance.last()->pobranoLS = 0;

    QTableWidgetItem *nazwaPliku = new QTableWidgetItem(tInstance.last()->filename);
    tabela->setItem(tInstance.last()->itemRow, 0, nazwaPliku);
    QTableWidgetItem *rozmiarPliku = new QTableWidgetItem(fileSize(tInstance.last()->size));
    tabela->setItem(tInstance.last()->itemRow, 1, rozmiarPliku);
    QTableWidgetItem *PobranoPliku = new QTableWidgetItem("?");
    tabela->setItem(tInstance.last()->itemRow, 2, PobranoPliku);
    QTableWidgetItem *postepPobierania = new QTableWidgetItem;
    postepPobierania->setData(Qt::DisplayRole, 0);
    tabela->setItem(tInstance.last()->itemRow, 3, postepPobierania);
    QTableWidgetItem *SzybkoscPobierania = new QTableWidgetItem("?");
    tabela->setItem(tInstance.last()->itemRow, 4, SzybkoscPobierania);
    QTableWidgetItem *statusPobierania = new QTableWidgetItem();
    tabela->setItem(tInstance.last()->itemRow, 5, statusPobierania);
    QTableWidgetItem *SkrzynkaPobierania = new QTableWidgetItem();
    tabela->setItem(tInstance.last()->itemRow, 6, SkrzynkaPobierania);

}

/**
* Funkcja odpowiedzialna za wyświetlanie statusu pobierania
*/
void QMoorie::refreshStatuses()
{
    quint64 allBytesReadSession = 0;
    double allSpeed = 0;
    for (int i = 0; i < tInstance.size(); ++i)
    {
        if(tInstance.at(i)->Instance->downloadDone)
        {
            Status status = tInstance.at(i)->Instance->getStatus();

            if(status.state == Status::Finished)
            {
                for(int j = 0 ; j < 7; j++ )
                {
                    tabela->item(tInstance.at(i)->itemRow, j)->setBackground(QColor(0, 50, 0, 100));
                }
                tray->showHints("Pobrano pomyślnie", "Pobieranie pliku: <br/><b><i>"+tInstance.at(i)->filename+"</b></i><br/>zakończono pomyślnie.");
                tInstance.remove(i);
                saveDownloads();
            }
            if(status.state == Status::FileError && !tInstance.at(i)->pobrano)
            {
                QTableWidgetItem *PobranoPliku = new QTableWidgetItem("0.00 MB");
                tabela->setItem(tInstance.at(i)->itemRow, 2, PobranoPliku);
                QTableWidgetItem *postepPobierania = new QTableWidgetItem;
                postepPobierania->setData(Qt::DisplayRole, 0);
                tabela->setItem(tInstance.at(i)->itemRow, 3, postepPobierania);
                QTableWidgetItem *statusPobierania = new QTableWidgetItem("No valid account found");
                tabela->setItem(tInstance.at(i)->itemRow, 5, statusPobierania);
                QTableWidgetItem *SzybkoscPobierania = new QTableWidgetItem("0 KB/s");
                tabela->setItem(tInstance.at(i)->itemRow, 4, SzybkoscPobierania);
                for(int j = 0 ; j < 7; j++ )
                {
                    tabela->item(tInstance.at(i)->itemRow, j)->setBackground(QColor(255, 0, 0, 200));
                }
                tInstance.at(i)->pobrano = true;
                tray->showHints("Błąd pobierania", "Niestety nie udało się pobrać pliku:<br/><b><i>"+tInstance.at(i)->filename+"</i></b>");
            }
        }
    }

    for (int i = 0; i < tInstance.size(); ++i)
    {
        Status status = tInstance.at(i)->Instance->getStatus();
        if(!(tInstance.at(i)->Instance->downloadPaused) && status.state != Status::FileError)
        {
            allBytesReadSession += status.bytesRead;

            QTableWidgetItem *PobranoPliku = new QTableWidgetItem(fileSize(tInstance.at(i)->size - tInstance.at(i)->pobranoLS - status.bytesRead));
            tabela->setItem(tInstance.at(i)->itemRow, 2, PobranoPliku);
            int percentDownloaded = 100.0f * (status.bytesRead + tInstance.at(i)->pobranoLS)  / tInstance.at(i)->size;
            QTableWidgetItem *postepPobierania = new QTableWidgetItem;
            postepPobierania->setData(Qt::DisplayRole, percentDownloaded);
            tabela->setItem(tInstance.at(i)->itemRow, 3, postepPobierania);

            allSpeed += static_cast<double>( status.speed) / 1024.0f;
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
            tabela->setItem(tInstance.at(i)->itemRow, 4, SzybkoscPobierania);
            QTableWidgetItem *statusPobierania = new QTableWidgetItem(QString::fromStdString(status.getStateText()) +
                                                                      " " + QString::number(status.downloadSegment+1) + "/" + QString::number(tInstance.at(i)->totalSegments));
            if(status.state == Status::Connecting || status.state == Status::Connected)
            {
                statusPobierania->setForeground(QColor(204, 210, 55, 255));
            }
            else if(status.state == Status::Downloading || status.state == Status::Downloaded)
            {
                statusPobierania->setForeground(QColor(0, 100, 0, 255));
            }
            else if(status.state == Status::Finished)
            {
                statusPobierania->setForeground(QColor(0, 0, 200, 255));
            }
            else if(status.state == Status::ConnectionError || status.state == Status::FileError || status.state == Status::SegmentError)
            {
                statusPobierania->setForeground(QColor(255, 0, 0, 200));
            }
            tabela->setItem(tInstance.at(i)->itemRow, 5, statusPobierania);
            //qDebug() << status.downloadSegment;
            QTableWidgetItem *SkrzynkaPobierania = new QTableWidgetItem(QString::fromStdString(status.mailboxName));
            tabela->setItem(tInstance.at(i)->itemRow, 6, SkrzynkaPobierania);
        }
        else if(tInstance.at(i)->Instance->downloadPaused)
        {
            QTableWidgetItem *SzybkoscPobierania = new QTableWidgetItem("0 KB/s");
            tabela->setItem(tInstance.at(i)->itemRow, 4, SzybkoscPobierania);
            QTableWidgetItem *statusPobierania = new QTableWidgetItem("Wstrzymane");
            tabela->setItem(tInstance.at(i)->itemRow, 5, statusPobierania);
            for(int j = 0 ; j < 7; j++ )
            {
                tabela->item(tInstance.at(i)->itemRow, j)->setBackground(QColor(255, 255, 0, 100));
            }
        }
    }
    if(tInstance.size())
    {

        QSettings settings;
        if(settings.isWritable())
        {
            settings.beginGroup("VARIABLES_QMOORIE");
            settings.setValue("allBytesRead", allBytesRead+allBytesReadSession);
            settings.endGroup();
        }
        std::stringstream allS;
        allS.setf( std::ios::fixed);
        if ( allSpeed - round( allSpeed ) < 0.1f )
        {
            allS << std::setprecision( 0 );
        }
        else
        {
            allS << std::setprecision( 2 );
        }
        allS << allSpeed;

        ui->allBytesReadSession->setText(fileSize(allBytesReadSession));
        ui->maxDownloadSpeed->setText(QString::fromStdString(allS.str()) +" KB/s");

        QString tip = "<table cellpadding='2' cellspacing='2' align='center'><tr><td><b>Szybkość:</b></td><td></td></tr><tr><td>Pobieranie: <font color='#1c9a1c'>"+ QString::fromStdString(allS.str()) +" KB/s</font></td><td>Wysyłanie: <font color='#990000'>0 KB/s</font></td></tr><tr><td><b>Transfer:</b></td><td></td></tr><tr><td>Pobrano: <font color='#1c9a1c'>"+ fileSize(allBytesReadSession) +"</font></td><td>Wysłano: <font color='#990000'>0 MB</font></td></tr></table>";
        tray->setToolTip(tip);
    }
    ui->allBytesRead->setText(fileSize(allBytesRead+allBytesReadSession));
    if(Zmienne().logs != ""){
        ui->log->append(Zmienne().logs);
        Zmienne().logs = "";
    }

}
void QMoorie::removeDownload()
{
    int row = tabela->currentRow();
    for (int i = 0; i < tInstance.size(); ++i)
    {
        //qDebug() << 'tIns.size: ' << tInstance.size() << ' tabela.rowcount: ' << tabela->rowCount();
        if(tInstance.at(i)->itemRow == row)
        {
            QString fileName = tInstance.at(i)->path + tInstance.at(i)->filename;
            tInstance.at(i)->terminate();
            tInstance.remove(i);
            tabela->removeRow(row);
            if (QFile::exists(fileName)) QFile::remove(fileName);
            if (QFile::exists(fileName + ".seg")) QFile::remove(fileName + ".seg");
            saveDownloads();
        }
    }

    for (int i = 0; i < tInstance.size(); ++i)
    {
        tInstance.at(i)->itemRow = i;
        QTableWidgetItem *nazwaPliku = new QTableWidgetItem(tInstance.at(i)->filename);
        tabela->setItem(tInstance.at(i)->itemRow, 0, nazwaPliku);
        QTableWidgetItem *rozmiarPliku = new QTableWidgetItem(fileSize(tInstance.at(i)->size));
        tabela->setItem(tInstance.at(i)->itemRow, 1, rozmiarPliku);


    }
}
void QMoorie::pauseDownload()
{
    int row = tabela->currentRow();
    for (int i = 0; i < tInstance.size(); ++i)
    {
        if(tInstance.at(i)->itemRow == row)
        {
            if(tInstance.at(i)->Instance->downloadPaused)
            {
                tInstance.at(i)->Instance->unpauseDownload();
                QTableWidgetItem *nazwaPliku = new QTableWidgetItem(tInstance.last()->filename);
                tabela->setItem(tInstance.at(i)->itemRow, 0, nazwaPliku);
                QTableWidgetItem *rozmiarPliku = new QTableWidgetItem(fileSize(tInstance.at(i)->size));
                tabela->setItem(tInstance.at(i)->itemRow, 1, rozmiarPliku);
            }
            else
            {
                tInstance.at(i)->Instance->pauseDownload();
            }
        }
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

                    addInstance(hash.text(), pass.text(), pathstr);
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

    for(int i = 0; i < tInstance.size(); ++i) {
        download = dokument_xml.createElement( "download" );
        downloads.appendChild(download);

        hash = dokument_xml.createElement( "hash");
        download.appendChild(hash);
        val = dokument_xml.createTextNode(tInstance.at(i)->hash);
        hash.appendChild(val);

        pass = dokument_xml.createElement( "pass");
        download.appendChild(pass);
        val = dokument_xml.createTextNode(tInstance.at(i)->pass);
        pass.appendChild(val);

        path = dokument_xml.createElement( "path");
        download.appendChild(path);
        val = dokument_xml.createTextNode(tInstance.at(i)->path);
        path.appendChild(val);
    }
    QFile dokument(Zmienne().configPath+"hashcodes.xml");
    if(dokument.open(QFile::WriteOnly))
    {
        QTextStream ts(&dokument);
        ts << dokument_xml.toString();
    }
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
    ConfigDialog *get = new ConfigDialog();
    get->exec();
    if(!Zmienne().TRAY) tray->hide();
    else tray->show();
    delete get;
}
void QMoorie::readConfigFile()
{
    QHeaderView *header = tabela->horizontalHeader();
    header->setDefaultAlignment(Qt::AlignLeft);
    QSettings settings;
    settings.beginGroup("CONFIG_PAGE");
    Zmienne().PATH = settings.value("PATH", "home").toString();
    Zmienne().LLEVEL = settings.value("LLEVEL", 6).toInt();
    Zmienne().DLEVEL = settings.value("DLEVEL", 2).toInt();
    Zmienne().NLEVEL = settings.value("NLEVEL", 0).toInt();
    Zmienne().KSEGMENTS = settings.value("KSEGMENTS", 1).toBool();
    Zmienne().TRAY = settings.value("TRAY", true).toBool();
    settings.endGroup();

    settings.beginGroup("GEOMETRY_QMOORIE");
    resize(settings.value("size", QSize(848, 280)).toSize());
    move(settings.value("pos", QPoint(0, 0)).toPoint());
    header->resizeSection( 0, settings.value("tabela_column_0", 260).toInt() );
    header->resizeSection( 1, settings.value("tabela_column_1", 80).toInt() );
    header->resizeSection( 2, settings.value("tabela_column_2", 70).toInt() );
    header->resizeSection( 3, settings.value("tabela_column_3", 120).toInt() );
    header->resizeSection( 4, settings.value("tabela_column_4", 65).toInt() );
    header->resizeSection( 5, settings.value("tabela_column_5", 120).toInt() );
    header->resizeSection( 6, settings.value("tabela_column_6", 100).toInt() );
    settings.endGroup();
    settings.beginGroup("VARIABLES_QMOORIE");
    allBytesRead = settings.value("allBytesRead", 0).toUInt() ;
    settings.endGroup();

    writeConfigFile();

    Zmienne().configPath = settings.fileName();
    //qDebug() << "configPath: " << Zmienne().configPath;
    Zmienne().configPath.remove(Zmienne().configPath.lastIndexOf(QDir::separator())+1,Zmienne().configPath.size());
    //qDebug() << "configPath: " << Zmienne().configPath;
}
void QMoorie::writeConfigFile()
{
    QHeaderView *header = tabela->horizontalHeader();
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

        settings.beginGroup("GEOMETRY_QMOORIE");
        settings.setValue("size", size());
        settings.setValue("pos", pos());
        settings.setValue("tabela_column_0", header->sectionSize(0));
        settings.setValue("tabela_column_1", header->sectionSize(1));
        settings.setValue("tabela_column_2", header->sectionSize(2));
        settings.setValue("tabela_column_3", header->sectionSize(3));
        settings.setValue("tabela_column_4", header->sectionSize(4));
        settings.setValue("tabela_column_5", header->sectionSize(5));
        settings.setValue("tabela_column_6", header->sectionSize(6));
        settings.endGroup();
    }
    else
    {
        QMessageBox::critical(NULL, "QMoorie", "Nie można zapisać pliku konfiguracyjnego do\n "+settings.fileName(), "OK");
        this->close();
    }
}
void QMoorie::closeEvent(QCloseEvent *event)
{
    writeConfigFile();
    if(Zmienne().TRAY){
        event->ignore();
        hide();
    }
    else qApp->quit();
}
void QMoorie::exitApp()
{
    writeConfigFile();
    qApp->quit();
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
