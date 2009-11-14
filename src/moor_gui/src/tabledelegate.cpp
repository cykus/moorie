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
#include "tabledelegate.h"
#include <QStyleOptionProgressBarV2>
#include <QStyle>
#include <QApplication>
#include <assert.h>

TrackDelegate::TrackDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}
void TrackDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if( index.column() == 4 )
    {
        QStyleOptionProgressBarV2 opts;
        opts.text = index.model()->data(index, Qt::DisplayRole).toString() + " %";
        opts.maximum = 100;
        opts.minimum =   0;
        opts.progress = index.model()->data(index, Qt::DisplayRole).toInt();
        opts.rect = option.rect;
        opts.rect.setRight(option.rect.right()-1);
        opts.rect.setHeight(option.rect.height()-1);
        opts.textVisible = true;
        opts.textAlignment = Qt::AlignCenter;
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &opts, painter);

        return;
	}
	QItemDelegate::paint(painter, option, index);
}
