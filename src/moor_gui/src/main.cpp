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
#include <QApplication>
#include <QTimer>
#include <QSplashScreen>
#include "qmoorie.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.png"));
    splash->show();
    app.setApplicationName("Qmoorie");
    app.setOrganizationName("Moorie Team");
    app.setOrganizationDomain("moorie.pl");
    app.setApplicationVersion ("0.3.0");
    app.setQuitOnLastWindowClosed(true);
    Q_INIT_RESOURCE(application);
    QMoorie mw;
    splash->showMessage( "Uruchamianie aplikacji...", Qt::AlignBottom | Qt::AlignRight );
    QTimer::singleShot(2500, splash, SLOT(close()));
    if(!Zmienne().RUNINTRAY || (Zmienne().RUNINTRAY && !Zmienne().TRAY)) QTimer::singleShot(2500, &mw, SLOT(show()));
    return app.exec();
}

