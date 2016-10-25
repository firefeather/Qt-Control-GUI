/*
 * DataBrowser data browsing widget and filtering engine
 * Copyright (C) 2004-2008 by Gordos Kund / QnD Co Bt.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact gordos.kund@gmail.com with any questions on this license.
 */

#ifndef TCSQLCONVERTUTILS_H
#define TCSQLCONVERTUTILS_H
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QRect>

/*! \class tcsqlutils
 *  \brief utilites for sql datatype etc.
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */


/*
Postgres8 Types and qt4.3 types
QBOOLOID: QVariant::Bool;
QINT8OID: QVariant::LongLong;
QINT2OID,QINT4OID,QOIDOID,QREGPROCOID,QXIDOID,QCIDOID: QVariant::Int;
QNUMERICOID,QFLOAT4OID,QFLOAT8OID: QVariant::Double;
QABSTIMEOID,QRELTIMEOID,QDATEOID: QVariant::Date;
QTIMEOID,QTIMETZOID:QVariant::Time;
QTIMESTAMPOID, QTIMESTAMPTZOID: QVariant::DateTime;
QBYTEAOID: QVariant::ByteArray;
Other QVariant::String;
*/

class TCSqlConvertUtils;
class TCSqlConvertUtils
{

public:
        TCSqlConvertUtils();
        TCSqlConvertUtils( const QVariant &var =0 );
        TCSqlConvertUtils( const QList<int> &var );
        TCSqlConvertUtils( const QStringList &var );
        TCSqlConvertUtils( const QRect &var );
        virtual ~TCSqlConvertUtils();
        virtual QList<int> toQListInt();
        virtual QList<bool> toQListBool();
        virtual QList<qint64> toQListBigInt();
        virtual QMap<int,QStringList> toColorTypes();
        virtual QString toPgSql();
        virtual QStringList toStringList();
	virtual QStringList addDoubleQuote(QStringList in);

        enum Type {
                none = 0,
                ListInt = 1,
                StringList = 2,
                Variant = 3,
                Rect = 4,
                LastType = 0xffffffff // need this so that gcc >= 3.4 allocates 32 bits for Type
        };


private:
        QVariant variantToConvert;
        QList<int> qListToConvert;
	QRect qRectToConvert;
	QStringList qStringListToConvert;
        Type type;
};
#endif
