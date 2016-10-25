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

#ifndef TCPSQLFILTERS_H
#define TCPSQLFILTERS_H

//Optional (after uncomment this, the filter order will in update time not in create time):
//#define MODIFIY_ORDER

#include <QtCore/QDebug>
#include <QtCore/qobject.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include "tcabstractfilter.h"

//#define MODIFIY_ORDER

/*! \class TCPSqlFilters
 *  \brief Get-Set filter parameters with Postgres Sql
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */

class TCPSqlFilters;
class TCPSqlFilters : public TCAbstractFilter
{
        Q_OBJECT
public:

        TCPSqlFilters(QString &conn, QString zoomName,QString userName, QObject *parent = 0);
        ~TCPSqlFilters();
        virtual QStringList getFilterList();
        virtual bool newFilter(const QString &name,int numCols);
        virtual bool activateFilter(const QString &name);
	virtual bool activateFilter( int cidx);
	virtual bool loadAllFilters();
        virtual bool saveFilter();
        virtual bool deleteFilter(int idx);
	virtual bool insertDefaultFilter(int numCols);
	virtual int changeCurrentIndex ( int cidx );

	virtual int numRows();
	virtual QStringList getErrors();

        //protected:
private:
        virtual void setupCurrentFilter();
	virtual bool updateWithEmpty(const QString & name,int numCols);
	QSqlDatabase pgDB;
        QSqlTableModel *filterModel;
	QStringList lastErrors;
	int currentIdx;
	
};

#endif
