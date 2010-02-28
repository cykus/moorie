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
#include "mysystemtrayicon.h"
diall::diall(QRect pos, QWidget *parent): pos(pos)
{
    setWindowFlags(Qt::ToolTip | Qt::Widget);
    setWindowModality(Qt::NonModal);
    //setStyleSheet( QString("QLabel {border: 2px solid  %1}" ).arg(QColor(QPalette::WindowText).name()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(closeD()));

    label = new QLabel();
    QVBoxLayout *main = new QVBoxLayout;
    main->addWidget(label);
    setLayout(main);
}
void diall::mousePressEvent(QMouseEvent * event)
{
    close();
}
void diall::setPosition()
{
    int x = 0,y = 0;
    if(pos.x() > width())
    {
     x -= width();
    }
    if(pos.y() > height())
    {
     y -= height();
    }
    else
    {
     y += pos.height();
    }
    move(pos.x()+x, pos.y()+y);
}
mySystemTrayIcon::mySystemTrayIcon(QWidget *parent) : QSystemTrayIcon(parent)
{
    UseFreedesktopStandard = false;
}
void mySystemTrayIcon::showHints(QString  title, QString  message, int seconds )
{
    if(!Zmienne().NLEVEL) return;
    else if(Zmienne().NLEVEL == 2)
    {
        KNotify = new QDBusInterface("org.kde.VisualNotifications",
                                     "/VisualNotifications", "org.kde.VisualNotifications");
        if (!KNotify->isValid())
        {
            delete (KNotify);
            KNotify = new QDBusInterface("org.freedesktop.Notifications",
                                         "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
            UseFreedesktopStandard = true;
        }
        if (KNotify->isValid())
        {
            QList<QVariant> args;
            args.append("Qmoorie");
            args.append(0U);
            args.append("qmoorie");
            /* the new spec doesn't have this */
            if (!UseFreedesktopStandard)
                args.append("");
            args.append("Qmoorie - " + title);
            args.append(message);
            args.append(QStringList());
            args.append(QVariantMap());
            args.append(seconds*1000);
            QDBusReply<unsigned int> reply = KNotify->callWithArgumentList(QDBus::Block, "Notify", args);
            if (reply.isValid())
            {
            }
        }
        else{
            Zmienne().NLEVEL = 1;
        }
    }
    if(Zmienne().NLEVEL == 1 && seconds>0)
    {
        diall *dial = new diall(geometry());
        dial->label->setText("<b>"+ title + "</b><br/>" + message) ;
        dial->timer->start(seconds*1000);
        dial->show();
        dial->setPosition();
    }
}
void diall::closeD()
{
    close();
}
