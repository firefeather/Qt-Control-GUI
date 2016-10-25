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

#include <QtCore/QAbstractListModel>
#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtCore/QDebug>
#include <QtCore/QCache>

#include "tcabstractfilter.h"
#include "tcabstractzoomparams.h"
#include "tcabstractdatamodel.h"

#define POSTGRES_QUERY_SWITCH_LIMIT 1000
#define POSTGRES_SQL_LIMIT_AT_ONCE "100"

/*! \class TCPSqlDataModel
 *  \brief Data Model. Transfer data from handler to widget.
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */

class TCPSqlDataModel;
class TCPSqlDataModel : public TCAbstractDataModel
{
        Q_OBJECT

public:
	TCPSqlDataModel(QString &conn, TCAbstractZoomParams *params, TCAbstractFilter *filters, QObject *parent = 0);
	~TCPSqlDataModel();
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QStringList getErrors();
	virtual void closeConnections();
	virtual QSqlRecord getRecord(int row);
	virtual bool testActiveColumnExists();
	virtual int fieldIndex(const QString & name);

private:
	virtual bool setFieldTypes();
	virtual qint64 realRowCount() const;
	virtual QVariant fetch(const QModelIndex &index) const;
	virtual QVariant getVariantFromHash(const QModelIndex &index) const;
	virtual bool makeTransaction() const;
	virtual QString constructAggrSelect() const;
	virtual QString constructFields() const;
	virtual QString constructFrom() const;
	virtual QString constructWhere() const;
	virtual QString constructOrderBy() const;
	virtual QList<QVariant> constructOffsetLimit(int offset) const;
	virtual QString formatHeaderFilter(int section, bool html = true) const; 
        QSqlDatabase pgDB;
        QSqlDatabase *p_pgDB;
	QSqlDatabase pgDBForTransaction;
	QSqlDatabase *p_pgDBForTransaction;
        QSqlQueryModel *zoomModel;
	mutable bool hasTransaction;
        mutable QStringList lastErrors;
	mutable QCache<qint64,QSqlRecord> cache;
	mutable QHash<qint64,QSqlRecord> hash;
	mutable int bgColorColumn;
	mutable int fgColorColumn;
	mutable int previousColumn;
	mutable QString oldFrom;
	mutable QString oldFields;
	mutable QString oldWhere;
	mutable QString previousCountQuery;
	mutable QString previousTransactionQuery;

signals:
	virtual void rowNumberChanged(qint64 oldNumRows,qint64 newNumRows) const;
	virtual void beginQuery(QString query) const;
	virtual void endQuery(QString query) const;
	virtual void aggregatorValue(int column, QVariant value) const;
};
