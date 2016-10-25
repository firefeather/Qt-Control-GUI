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

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include "tcpsqldatamodel.h"
#include "twdateinterval.h"

//=====================================================================
//61 ms
TCPSqlDataModel::TCPSqlDataModel (QString &conn, TCAbstractZoomParams *p_params, TCAbstractFilter *p_filters, QObject *parent) : TCAbstractDataModel(p_params,p_filters,parent)
{
	lastErrors.clear();
        oldFrom="";
	pgDB=QSqlDatabase::database (conn);
	pgDBForTransaction=QSqlDatabase::cloneDatabase (pgDB,conn+"-FORTRANSACTION");
	p_pgDB=&pgDB;
        p_pgDBForTransaction=&pgDBForTransaction;
	
	//57ms
	p_pgDBForTransaction->open();
	
	hasTransaction=false;
        zoomModel = new QSqlQueryModel(this);
	setDoRealCount(true,TCAbstractDataModel::ConstructorRole);
	setDoRealFetch(true,TCAbstractDataModel::ConstructorRole);
	previousCountQuery="";
        previousTransactionQuery="";

	//3ms
        setFieldTypes();
}

//=====================================================================
//
//=====================================================================
TCPSqlDataModel::~TCPSqlDataModel ()
{
}

//=====================================================================
//
//=====================================================================
void TCPSqlDataModel::closeConnections()
{
        p_pgDBForTransaction->commit();
        p_pgDBForTransaction->close();
        p_pgDB->close();
        pgDBForTransaction = QSqlDatabase();
        pgDB = QSqlDatabase();
}

//=====================================================================
//
//=====================================================================
//3 ms
bool TCPSqlDataModel::setFieldTypes()
{
	QString sqlQuery="SELECT" + constructFields() + constructFrom() + " OFFSET 0 LIMIT 1";
	emit beginQuery(sqlQuery);
        QSqlQuery query(sqlQuery,pgDB);
        QSqlRecord rec;
        if (query.next()) {
                rec=query.record();
        } else
	{
		lastErrors.append(sqlQuery);
		lastErrors.append(query.lastError().text());
		qDebug()<<query.lastError().text();
		emit endQuery(sqlQuery);
		return false;
	}

        emit endQuery(sqlQuery);

        QStringList fieldTypes;
        for (int i = 0; i < rec.count(); i++) {
		QSqlField fld=rec.field(i);
                //do not insert FG & BG color types
                if (fld.name()!=params->getBGColorColumn() && fld.name()!=params->getFGColorColumn() ) {
                        fieldTypes.append(QString(QVariant::typeToName(fld.type())));
                }
        }
        params->setColumnTypes(fieldTypes);

	return true;
}

//=====================================================================
//Return type is not qint64 !
//=====================================================================
int TCPSqlDataModel::rowCount(const QModelIndex &/*parent*/) const
{
	qint64 ret=0;

        //set CountPuffer
        if (getDoRealCount()) {
		setDoRealCount(false,TCAbstractDataModel::RowCountRole);
		setCountPuffer(realRowCount());
        }

        ret=getCountPuffer();
        if (getShowAggregators()) {
                ret++;
        }
	
        return ret;
}

//=====================================================================
//
//=====================================================================
qint64 TCPSqlDataModel::realRowCount() const
{
        QString sqlQuery="SELECT count(*) AS count "+constructFrom()+constructWhere()+";";	
	//here you can't eliminate duplicates because refreshAll() after insert row in table construct same sql as before
	
        previousCountQuery=sqlQuery;

        emit beginQuery(sqlQuery);
        zoomModel->setQuery(sqlQuery,pgDB);
        emit endQuery(sqlQuery);

        if (zoomModel->lastError().isValid()) {
                lastErrors.append(sqlQuery);
                lastErrors.append(zoomModel->lastError().text());
                qDebug()<<zoomModel->lastError().text();
                return -1;
        }
        QSqlRecord record=zoomModel->record(0);
	setDoRealCount(false,TCAbstractDataModel::RealRowCountRole);
        qint64 newNumRows = record.value("count").toLongLong();
        qint64 oldNumRows=getCountPuffer();

        //?makeTransaction();

        Q_ASSERT (newNumRows>=0);
        setCountPuffer(newNumRows);

        //call non const function , this will call indirect via qt widget fetch()
        emitRefreshAll(oldNumRows,newNumRows);
	
        return newNumRows;
}

//=====================================================================
//
//=====================================================================
int TCPSqlDataModel::columnCount(const QModelIndex &/*parent*/) const
{
        Q_ASSERT(params->getColumns().count()>=0);
        return params->getColumns().count();
}

//=====================================================================
//
//=====================================================================
bool TCPSqlDataModel::makeTransaction() const
{
        //declare cursor
        QString sqlQuery="DECLARE zoomcursor SCROLL CURSOR FOR SELECT " + constructFields() + constructFrom() + constructWhere() + constructOrderBy() + ";";

        // Do nothing if no query change
        //eliminate double queries
        if (previousTransactionQuery==sqlQuery) {
                return false;
        }

        previousTransactionQuery=sqlQuery;

        if (hasTransaction) {
                p_pgDBForTransaction->commit();
        }
        p_pgDBForTransaction->transaction();

        hasTransaction=true;

        emit beginQuery(sqlQuery);
        QSqlQuery gq(sqlQuery,pgDBForTransaction);
        emit endQuery(sqlQuery);

        if (gq.lastError().isValid()) {
                lastErrors.append(gq.lastError().text());
                qDebug()<<gq.lastError().text();
                return false;
        }

        return true;
}

//=====================================================================
//
//=====================================================================
QVariant TCPSqlDataModel::fetch(const QModelIndex &index) const
{
        //if mster filter changed we must clear cache and construct new Sql
        if (getDoRealFetch()) {
                hash.clear();
		
		// make transaction in case rowCount > POSTGRES_QUERY_SWITCH_LIMIT
		if (rowCount()>=POSTGRES_QUERY_SWITCH_LIMIT)
		{
                	makeTransaction();
		}
		
		setDoRealFetch(false,TCAbstractDataModel::FetchRole);
                previousColumn=-1;
        }

        //eliminate duplicate fetches in columns
        if (index.column()!=previousColumn) { 
                previousColumn=index.column();

                //check hash
                if (!hash.contains(index.row())) {
                        //do Aggreagtor sql in last row
			if (index.row()==rowCount()-1 && getShowAggregators()) {
				if (rowCount()-1!=0)
				{
                                        QString sqlQuery=constructAggrSelect()+constructFrom() + constructWhere()+ " LIMIT 1";
                                        emit beginQuery(sqlQuery);
                                        QSqlQuery query(sqlQuery,pgDB);
                                        if (query.next()) {
                                                hash.insert(index.row(),query.record());
                                        }
                                        emit endQuery(sqlQuery);

                                        if (query.lastError().isValid()) {
                                                lastErrors.append(query.lastError().text());
                                                //Error to Table item
                                                return query.lastError().text();
                                        }
				}
                        } else {

                                if (index.row() > POSTGRES_QUERY_SWITCH_LIMIT) {
                                        QString sqlQuery="FETCH ABSOLUTE "+QString::number(index.row()+1)+" FROM zoomcursor;";

                                        emit beginQuery(sqlQuery);
                                        QSqlQuery query(sqlQuery,pgDBForTransaction);
                                        query.next();
                                        emit endQuery(sqlQuery);

                                        if (query.lastError().isValid()) {
                                                lastErrors.append(query.lastError().text());
                                                //Error to Table item
                                                return query.lastError().text();
                                        }


                                        //put to hash
                                        bgColorColumn=query.record().indexOf(params->getBGColorColumn());
                                        fgColorColumn=query.record().indexOf(params->getFGColorColumn());
                                        hash.insert(index.row(),query.record());
                                } else {
                                        QList<QVariant> limitOffset=constructOffsetLimit(index.row());
                                        QString sqlQuery="SELECT "+constructFields()+constructFrom() + constructWhere() + constructOrderBy() + limitOffset.first().toString()+";";

                                        emit beginQuery(sqlQuery);
                                        QSqlQuery query(sqlQuery,pgDB);

                                        //result -> hash

                                        int pos=limitOffset.last().toInt();
                                        while (query.next()) {
                                                bgColorColumn=query.record().indexOf(params->getBGColorColumn());
                                                fgColorColumn=query.record().indexOf(params->getFGColorColumn());
                                                hash.insert(pos,query.record());
                                                pos++;
                                        }
                                        emit endQuery(sqlQuery);

                                        if (query.lastError().isValid()) {
                                                lastErrors.append(query.lastError().text());
                                                //Error to Table item
                                                return query.lastError().text();
                                        }

                                }
                        }

                }
        }

        return getVariantFromHash(index);
}

//=====================================================================
//
//=====================================================================
QVariant TCPSqlDataModel::getVariantFromHash(const QModelIndex &index) const
{
        QSqlRecord ret=hash.value(index.row());


        if (getShowAggregators()) {
                //last row ?
                if (index.row()==rowCount()-1) {
                        QString aggType=filters->getHeaderAggregator(index.column());
			if (aggType=="-" || aggType=="") {
                                return QVariant("-");
                        } else {
                                return QVariant(aggType+": "+ret.value(index.column()).toString());
                        }
                }
        }
	
        return ret.value(index.column());
}

//=====================================================================
//Construct complet select query for aggregators
//=====================================================================
QString TCPSqlDataModel::constructAggrSelect() const
{
        QStringList columns=params->getColumns();
	Q_ASSERT(columns.size()>0);
	
	QStringList aggregators=filters->getHeaderAggregators();
	Q_ASSERT(aggregators.size()>0);
        
	QStringList retSL;
        QString ret=" SELECT ";
        for (int i = 0; i < columns.size(); ++i) {
                if (i < aggregators.size()) {
                        if (aggregators.at(i)=="-" || aggregators.at(i)=="") {
                                retSL.append("\'-\'");
                        } else {
                                retSL.append(aggregators.at(i)+"("+columns.at(i)+")");
                        }
                } else {
                        retSL.append("\'-\'");
                }
        }
        ret.append(retSL.join(","));
        return ret;
}

//=====================================================================
//
//=====================================================================
QString TCPSqlDataModel::constructFields() const
{
        QStringList columns=params->getColumns();
        QString ret;
        ret=" "+columns.join(",")+" ";
        if (params->getFGColorColumn()!="") {
                ret.append(","+params->getFGColorColumn());
        }

        if (params->getBGColorColumn()!="") {
                ret.append(","+params->getBGColorColumn());
        }

        if (oldFields!=ret) {
		setDoRealCount(true,TCAbstractDataModel::ConstructFieldsRole);
		setDoRealFetch(true,TCAbstractDataModel::ConstructFieldsRole);
                oldFields=ret;
        }

        return ret;
}

//=====================================================================
//
//=====================================================================
QList<QVariant> TCPSqlDataModel::constructOffsetLimit(int offset) const
{
        QList<QVariant> ret;
        QString sqlRet;
        int begin;

        if (offset <= 50) {
                begin=0;
        } else {
                begin=offset-50;
        }

        sqlRet.append(" OFFSET "+QString::number(begin));
        sqlRet.append(" LIMIT "+QString(POSTGRES_SQL_LIMIT_AT_ONCE)+" ");
        ret.append(sqlRet);
        ret.append(begin);
        return ret;
}

//=====================================================================
// Must be a FAST routine !
//TODO Optimizie calls
//=====================================================================
QString TCPSqlDataModel::constructWhere() const
{
        QStringList whereList;
        QString ret;

        QString masterFilter=filters->getMasterFilterText();
        QString selColName=params->getColumnByName(filters->getMasterFilterColumn());

        //if masterFilter is empty, then do not filter
        if (masterFilter!="") {
                QString regexp;
                masterFilter.replace ("  "," ");
                masterFilter.replace (" ",".*\\\\s");
                regexp.append("^");
                regexp.append(masterFilter);
                whereList.append(selColName + " ~'"+regexp+"'");
        }

        QStringList headerFilters=filters->getHeaderFilters();
        for (int i = 0; i < headerFilters.size(); ++i) {
                QString currentFilter=formatHeaderFilter(i,false);
                if (currentFilter!="*") {
                        whereList.append(params->getColumnName(i)+currentFilter);
                }
        }

	//Inactive Where clause

	if (!getShowInactives())
	{
		QString activeColumn=params->getActiveColumn();
		if (activeColumn!="")
		{
			whereList.append(activeColumn + "='true'");
		}
	}

        ret=whereList.join(" AND ");

        if (ret!="") {
                ret=" WHERE "+ret+" ";
        }

        ret.replace("*","%");


	//Eliminate duplicate fetcheses
        if (oldWhere!=ret) {
		setDoRealCount(true,TCAbstractDataModel::ConstructWhereRole);
		setDoRealFetch(true,TCAbstractDataModel::ConstructWhereRole);
                oldWhere=ret;
        }
        return ret;
}

//=====================================================================
//
//=====================================================================
QString TCPSqlDataModel::constructOrderBy() const
{
        QString orderby;
        int selectedColumn = getSelectedColumn();
        QStringList columnNames = params->getColumns();
        orderby=columnNames[selectedColumn];

        QString ascDesc="";
        if (getOrderDirection()==TCAbstractDataModel::Descending) {
                ascDesc="DESC";
        }

        if (getOrderDirection()==TCAbstractDataModel::Ascending) {
                ascDesc="ASC";
        }

        if (orderby!="") {
                orderby=" ORDER BY "+orderby+" "+ascDesc+" ";
        }

        if (ascDesc=="") {
                orderby=" ";
        }
        return orderby;
}

//=====================================================================
//
//=====================================================================
QString TCPSqlDataModel::constructFrom() const
{
        QString from=" FROM "+params->getDataTable();
        if (oldFrom!=from) {
		setDoRealCount(true,TCAbstractDataModel::ConstructFromRole);
		setDoRealFetch(true,TCAbstractDataModel::ConstructFromRole);
                oldFrom=from;
        }
        return from;
}

//=====================================================================
//
//=====================================================================
QVariant TCPSqlDataModel::data(const QModelIndex &index, int role) const
{
        if (!index.isValid()) {
                return QVariant();
        }

        switch (role) {
        case Qt::DisplayRole: {
                if ( params->getColumnType(index.column())=="bool") {
                        return QVariant("");
                }
		QVariant ret=fetch(index);
		if (getShowAggregators()) {
			if (index.row()==rowCount()-1) {
				emit aggregatorValue(index.column(), ret);
			}
		}
                return ret;
                break;
        }
        case Qt::TextColorRole: {
                //get color from database (via hash)
                QVariant fetchResult=fetch(index);
                QSqlRecord record=hash.value(index.row());
                QString fgColorName=record.value(fgColorColumn).toString();
                QColor fgColor(fgColorName);

                QColor result = QColor("black");
                //if no color but in actual column

                if (fgColorName!="") {
                        result=fgColor;
                }
                return result;
                break;
        }
        case Qt::BackgroundColorRole: {
                //get color from database (via hash)
                QVariant fetchResult=fetch(index);
                QSqlRecord record=hash.value(index.row());
                QString bgColorName=record.value(bgColorColumn).toString();
                QColor bgColor(bgColorName);
                QColor result = QColor("white");

                //if no color but in last row
                if (getShowAggregators()) {
                        if (bgColorName=="" && index.row()==rowCount()-1) {
                                result=QColor("yellow");
                        }
                }

                //if no color but in actual column
                if (bgColorName=="" && index.column()==getSelectedColumn()) {
                        //result=QBrush(Qt::Dense1Pattern);
                        result=result.darker(110);
                }



                //if must colorize & in actual column
                if (bgColorName!="" && index.column()==getSelectedColumn()) {
                        //result=QBrush(bgColor,Qt::Dense4Pattern);
                        result=bgColor.darker(110);

                }

                //must colorize & not in actual column
                if (bgColorName!="" && index.column()!=getSelectedColumn()) {
                        result=bgColor;
                }

                return result;
                break;
        }
        case Qt::TextAlignmentRole: {
                if (getShowAggregators()) {
                        if (index.row()==rowCount()-1) {
				return QVariant(Qt::AlignRight | Qt::AlignVCenter);
                        }
                }
		return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
                break;
        }
        case Qt::CheckStateRole: {
                if ( params->getColumnType(index.column())=="bool") {
                        QVariant fetchResult=fetch(index);
                        QSqlRecord record=hash.value(index.row());
                        bool result=record.value(index.column()).toBool();
                        if (result==true) {
                                return true;
                        } else {
                                return false;
                        }
                }
                return QVariant();

                //for checkBox:                return true;
                break;
        }
        case Qt::DecorationRole: {
                return QVariant();
                break;
        }
        case Qt::ToolTipRole: {
                return fetch(index);
                break;
        }
        case Qt::StatusTipRole: {
                return "StatusTip";
                break;
        }
        case Qt::WhatsThisRole: {
                return "WhatsThis";
                break;
        }
        }
	
        return QVariant();
}

//=====================================================================
//
//=====================================================================
//0 ms
QVariant TCPSqlDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
        switch (role) {
        case Qt::DisplayRole:
                if (orientation == Qt::Horizontal)
                        return params->getHeaderName(section);
                else
                        return QString("Row %1").arg(section);
                break;

        case Qt::ToolTipRole:
                if (orientation == Qt::Horizontal) {
                        QString tooltip = "<p style='white-space:pre' align=center>Column:<br><b>[";
                        tooltip.append(params->getHeaderName(section)+"]");
                        tooltip.append("<p style='white-space:pre' align=center>Current filter: <br><font color=red>" );
                        tooltip.append(formatHeaderFilter(section) + "<\b><\font>");
			if (getShowAggregators())
			{
                        	tooltip.append("<p style='white-space:pre' align=center>Current aggregator:<br><font color=red>");
                        	tooltip.append(filters->getHeaderAggregator(section)+ "<\b><\font>");
			}
                        return tooltip;
                } else {
                        QString numRows=QString("%1").arg(rowCount());
                        QString tooltip = "<p style='white-space:pre' align=center><b>Total number of rows: <br>"+ numRows+"<\b>";
                        return tooltip;
                }
                return QVariant();
                break;

        case Qt::DecorationRole:
                if (orientation == Qt::Horizontal && filters->getHeaderFilter(section)!="*") {
                        return getFilterIcon();
                }
                return QVariant();
                break;

        case Qt::TextColorRole: {
                if (orientation == Qt::Horizontal && filters->getMasterFilterColumn()==params->getHeaderName(section)) {
                        return QColor("red");
                }
                return QVariant();
                break;
        }

        case Qt::StatusTipRole:
                return "StatusTip";
                break;
        case Qt::WhatsThisRole:
                return "WhatsThis";
                break;
        }
	
        return QVariant();
}

//=====================================================================
//
//=====================================================================
QString TCPSqlDataModel::formatHeaderFilter(int section, bool html) const
{
        QString result=filters->getHeaderFilter(section);


        if (result.contains( "×")) {
                result.remove(0,2);
                TWDateInterval di;
                //calculate dates
                QList<QDate> dates=di.getDates(result.toInt());
                //join dates
                result=dates.first().toString(QLocale::system().dateFormat(QLocale::ShortFormat))+"¤"+dates.last().toString(QLocale::system().dateFormat(QLocale::ShortFormat));
        }

        //construct tooltip or sql string
        QStringList splitted=result.split("¤");
        if (splitted.count()==2) {
                if (html) {
                        //Tooltip
                        result=splitted.first()+" &le; "+params->getHeaderName(section)+" &le; "+splitted.last();
                } else {
                        //Sql
                        result=" >= '"+splitted.first()+"' AND "+params->getColumnName(section)+" <= '"+splitted.last()+"'";
                }
        }
        return result;
}

//=====================================================================
//
//=====================================================================
int TCPSqlDataModel::fieldIndex(const QString & name)
{
	QStringList columns=params->getColumns();
	return columns.indexOf(name);
}

//=====================================================================
//
//=====================================================================
QStringList TCPSqlDataModel::getErrors()
{
        QStringList ret=lastErrors;
        lastErrors.clear();
        return ret;
}

//=====================================================================
//
//=====================================================================
QSqlRecord TCPSqlDataModel::getRecord(int row)
{
	return hash.value(row);
}

//=====================================================================
//
//=====================================================================
bool TCPSqlDataModel::testActiveColumnExists()
{
	if (params->getActiveColumn()=="")
	{
		return false;
	}
		
	QString sqlQuery="SELECT " + params->getActiveColumn() + constructFrom() + " OFFSET 0 LIMIT 1"+";";
	
	QSqlQuery query(sqlQuery,pgDB);
	QSqlRecord rec;
	if (query.next()) 
	{
		rec=query.record();
	}

	
	for (int i = 0; i < rec.count(); i++) {
		QSqlField fld=rec.field(i);

		if (fld.name()==params->getActiveColumn())
		{
			return true;
		}
	}
	QString err=QString ("Error: Active column name (%1) is given in zoom, but no column with these name !").arg(params->getActiveColumn());
	lastErrors.append(err);
	qDebug()<<err;
	return false;
}

