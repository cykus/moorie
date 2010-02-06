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
#include "configdialog.h"

ConfigDialog::ConfigDialog()
{
    setWindowIcon( QIcon(":/images/hi64-app-qmoorie.png") );
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + " - Ustawienia programu");

    warning = new QLabel;
    warning->setText("<b>Niektóre zmiany wymagają uruchomienia aplikacji ponownie.<b>");
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(64, 64));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setWrapping(false);
    contentsWidget->setFlow(QListView::TopToBottom);
    contentsWidget->setMinimumHeight(300);
    contentsWidget->setMaximumWidth(110);
    contentsWidget->setResizeMode(QListView::Fixed);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    confpage = new ConfigurationPage();
    uploadpage = new UploadPage();
    pagesWidget->addWidget(confpage);
    pagesWidget->addWidget(uploadpage);

    //pagesWidget->addWidget(new UpdatePage);

    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *cancelButton = new QPushButton(tr("Anuluj"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    //QObject::connect(pathButton,SIGNAL(clicked()),this,SLOT(setDir()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(okButton, SIGNAL(clicked()),this,SLOT(saveConfigFile()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout -> addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(warning);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    QListWidgetItem *uploadButton = new QListWidgetItem(contentsWidget);
    QListWidgetItem *otherButton = new QListWidgetItem(contentsWidget);

#if QT_VERSION >= 0x040600
    configButton->setIcon(QIcon::fromTheme("preferences-system", QIcon(":images/preferences-system.png")));
    uploadButton->setIcon(QIcon::fromTheme("preferences-system-network", QIcon(":images/preferences-system-network.png")));
    otherButton->setIcon(QIcon::fromTheme("preferences-other", QIcon(":images/preferences-other.png")));
#else
    configButton->setIcon(QIcon(":/images/preferences-system.png"));
    uploadButton->setIcon(QIcon(":/images/preferences-system-network.png"));
    otherButton->setIcon(QIcon(":/images/preferences-other.png"));
#endif

    configButton->setText(tr("Konfiguracja"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    uploadButton->setText(tr("Wysyłanie"));
    uploadButton->setTextAlignment(Qt::AlignHCenter);
    uploadButton->setFlags(Qt::NoItemFlags);

    otherButton->setText(tr("Inne"));
    otherButton->setTextAlignment(Qt::AlignHCenter);
    otherButton->setFlags(Qt::NoItemFlags);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current){
        current = previous;
    }
    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}
void ConfigDialog::saveConfigFile()
{
    Zmienne().PATH = confpage->pathEdit->text();
    Zmienne().LLEVEL = confpage->lBox->value();
    Zmienne().DLEVEL = confpage->dBox->value();
    Zmienne().KSEGMENTS = confpage->kBox->isChecked();
    Zmienne().TRAY = confpage->tBox->isChecked();
    Zmienne().RUNINTRAY = confpage->thBox->isChecked();
    Zmienne().ASKBEFORECLOSE = confpage->abcBox->isChecked();
    if(confpage->xdgNotification->isChecked()) Zmienne().NLEVEL = 2;
    else if(confpage->hintNotification->isChecked()) Zmienne().NLEVEL = 1;
    else if(confpage->xdgNotification->isChecked()) Zmienne().NLEVEL = 0;
    Zmienne().SEGSIZE = uploadpage->segSizeSlider->value();
    Zmienne().DOWNPASS =  uploadpage->downPassEdit->text();
    Zmienne().EDITPASS =  uploadpage->editPassEdit->text();

    QSettings settings;
    if(settings.isWritable()){
        settings.beginGroup("CONFIG_PAGE");
        settings.setValue("PATH", confpage->pathEdit->text());
        settings.setValue("LLEVEL", confpage->lBox->value());
        settings.setValue("DLEVEL", confpage->dBox->value());
        settings.setValue("NLEVEL", Zmienne().NLEVEL);
        settings.setValue("KSEGMENTS", confpage->kBox->isChecked());
        settings.setValue("TRAY", confpage->tBox->isChecked());
        settings.setValue("RUNINTRAY", confpage->thBox->isChecked());
        settings.setValue("ASKBEFORECLOSE", confpage->abcBox->isChecked());
        settings.endGroup();
        settings.beginGroup("UPLOAD_PAGE");
        settings.setValue("SEGSIZE", uploadpage->segSizeSlider->value());
        settings.setValue("DOWNPASS", uploadpage->downPassEdit->text());
        settings.setValue("EDITPASS", uploadpage->editPassEdit->text());
        settings.endGroup();
        this->close();
    }
    else
    {
        QMessageBox::critical(NULL, "QMoorie", "Nie można zapisać pliku konfiguracyjnego do\n "+settings.fileName(), "OK");
        this->close();
    }
}
