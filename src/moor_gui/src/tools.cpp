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
#include "tools.h"

QString fileSize( quint64 bytes )
{
    const quint64 kb = 1024;
    const quint64 mb = 1024 * kb;
    const quint64 gb = 1024 * mb;
    const quint64 tb = 1024 * gb;
    if( bytes >= tb )
        return QString( "%1 TB" ).arg( qreal(bytes) / tb, 0, 'f', 3 );
    if( bytes >= gb )
        return QString( "%1 GB" ).arg( qreal(bytes) / gb, 0, 'f', 2 );
    if( bytes >= mb )
        return QString( "%1 MB" ).arg( qreal(bytes) / mb, 0, 'f', 2 );
    if( bytes >= kb )
        return QString( "%1 KB" ).arg( bytes / kb );
    if( bytes < kb )
        return QString( "0 KB" );
    return QString::number( bytes );
}
bool checkXDG()
{   
#if defined(unix)
    QDBusInterface *notify;

    notify = new QDBusInterface("org.kde.VisualNotifications",
                                "/VisualNotifications", "org.kde.VisualNotifications");
    if (!notify->isValid())
    {
        delete (notify);
        notify = new QDBusInterface("org.freedesktop.Notifications",
                                    "/org/freedesktop/Notifications", "org.freedesktop.Notifications");

    }
    if (notify->isValid())
    {
        delete(notify);
        return true;
    }
    else
    {
        delete(notify);
        return false;
    }
#else
    return false;
#endif
}
QString checkLanguage()
{
        QSettings settings;
        settings.beginGroup("CONFIG_PAGE");
        QString lang = settings.value("LANG", "pl").toString();
        settings.endGroup();
        saveLanguage(lang);
        return lang;
}
void saveLanguage(QString lang)
{
    QSettings settings;
    if(settings.isWritable()){
        settings.beginGroup("CONFIG_PAGE");
        settings.setValue("LANG", lang);
        settings.endGroup();
    }
}
