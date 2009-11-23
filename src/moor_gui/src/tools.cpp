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
        return QString::number( bytes );
}
