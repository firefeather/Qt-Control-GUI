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

#include "tcsqlconvertutils.h"

TCSqlConvertUtils::TCSqlConvertUtils()
{
        type=TCSqlConvertUtils::none;
}

//========================================

TCSqlConvertUtils::TCSqlConvertUtils(const QVariant &value)
{
        variantToConvert=value;
        type=TCSqlConvertUtils::Variant;
}

//========================================

TCSqlConvertUtils::TCSqlConvertUtils(const QList<int> &value)
{
        qListToConvert=value;
        type=TCSqlConvertUtils::ListInt;
}

//========================================

TCSqlConvertUtils::TCSqlConvertUtils(const QStringList &value)
{
        qStringListToConvert=value;
        type=TCSqlConvertUtils::StringList;
}

//========================================

TCSqlConvertUtils::TCSqlConvertUtils(const QRect &value)
{
        qRectToConvert=value;
        type=TCSqlConvertUtils::Rect;
}

//========================================

TCSqlConvertUtils::~TCSqlConvertUtils()
{}

//========================================
//Only from QVariant
QList<int> TCSqlConvertUtils::toQListInt()
{
        QList <int> ret;
        QString tmp= variantToConvert.toString();
        tmp.remove(0,1); //remove "{"
        tmp.remove(tmp.length()-1,1); //remove "}"
        QStringList tmpList = tmp.split(",");

        QStringListIterator tmpListIt(tmpList);
        while (tmpListIt.hasNext()) {
                ret.append(tmpListIt.next().toInt());
        }
        return ret;
}

//========================================
//Only from QVariant
QList<bool> TCSqlConvertUtils::toQListBool()
{
        QList <bool> ret;
        QString tmp= variantToConvert.toString();
        tmp.remove(0,1); //remove "{"
        tmp.remove(tmp.length()-1,1); //remove "}"
        QStringList tmpList = tmp.split(",");

        QStringListIterator tmpListIt(tmpList);
        while (tmpListIt.hasNext()) {
		QString current=tmpListIt.next();
		if (current=="true" || current=="True" || current=="TRUE" || current=="t" || current=="T")
		{
                	ret.append(true);
		} else 
		{
                	ret.append(false);
		}
        }
        return ret;
}

//========================================
//Only from QVariant
QList<qint64> TCSqlConvertUtils::toQListBigInt()
{
        QList <qint64> ret;
        QString tmp= variantToConvert.toString();
        tmp.remove(0,1); //remove "{"
        tmp.remove(tmp.length()-1,1); //remove "}"
        QStringList tmpList = tmp.split(",");

        QStringListIterator tmpListIt(tmpList);
        while (tmpListIt.hasNext()) {
                ret.append(tmpListIt.next().toLongLong());
        }
        return ret;
}

//========================================
//Only from QVariant
QStringList TCSqlConvertUtils::toStringList()
{
        QStringList ret;
        QString tmp=variantToConvert.toString();
//        tmp.replace("\"","");
        ret= tmp.split(",");
        return ret;
}


//========================================
//Only from QVariant
QMap<int,QStringList> TCSqlConvertUtils::toColorTypes()
{
        QMap<int,QStringList> ret;
        QString tmp= variantToConvert.toString();
        tmp.remove(0,2); //remove first "{{"
        tmp.remove(tmp.length()-2,2); //remove last"}}"

        QStringList tmpList = tmp.split("},{");

        QStringListIterator tmpListIt(tmpList);
        int i=0;
        while (tmpListIt.hasNext()) {
                QStringList tmp2=tmpListIt.next().split(",");
                ret.insert(i,tmp2);
                i++;
        }
        return ret;
}

//========================================

QString TCSqlConvertUtils::toPgSql()
{
        QStringList retsl;
        QString ret;

        switch (type) {
        case TCSqlConvertUtils::ListInt: {
                        for ( int i = 0; i < qListToConvert.count(); ++i ) {
                                retsl.append(QString::number(qListToConvert[i]));
                        }
			retsl=addDoubleQuote(retsl);
                        return "{"+retsl.join(",")+"}";
                }

        case TCSqlConvertUtils::StringList:
		retsl=qStringListToConvert;
                return retsl.join(",");

        case TCSqlConvertUtils::Rect: {
                        ret=QString("{%1,%2,%3,%4}")
                            .arg(qRectToConvert.x())
                            .arg(qRectToConvert.y())
                            .arg(qRectToConvert.width())
                            .arg(qRectToConvert.height());
                        return ret;
                }

        case TCSqlConvertUtils::Variant: {
                        return "{"+variantToConvert.toString()+"}";
                }

        default:
                return "";
        }
}

//========================================

QStringList TCSqlConvertUtils::addDoubleQuote(QStringList in)
{
        QStringList ret;
        for ( int i = 0; i < qListToConvert.count(); ++i ) {
		ret.append("\""+in[i]+"\"");
        }
	return ret;
}
