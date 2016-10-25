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

#include "tcpsqlfilters.h"
#include "tcsqlconvertutils.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>


//0 ms
TCPSqlFilters::TCPSqlFilters(QString &conn, QString zoomName, QString userName, QObject *parent) : TCAbstractFilter( parent)
{
	lastErrors.clear();
        pgDB=QSqlDatabase::database (conn);

        filterModel = new QSqlTableModel(this,pgDB);
        filterModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

        setZoomKey(zoomName);
        setUserKey(userName);
        //default save mode is insert:
        currentIdx=-1;
}

//================================

TCPSqlFilters::~TCPSqlFilters()
{
        pgDB = QSqlDatabase();
}

//=====================================================================
//PUBLIC, constrained by abstract class
//=====================================================================

//Record order and comboBox item order must be equal ! Do not sort combo items.
QStringList TCPSqlFilters::getFilterList( )
{
	QStringList ret;
        for ( int i = 0; i <filterModel->rowCount(); i++ ) {
                ret.append(filterModel->record(i).value("filters_name").toString());
        }
        return ret;
}

//================================

bool TCPSqlFilters::newFilter( const QString & name, int numCols )
{
        return updateWithEmpty(name,numCols);
}
//================================

bool TCPSqlFilters::insertDefaultFilter(int numCols)
{
        return updateWithEmpty("DEFAULT",numCols);
}

//================================
//20ms
bool TCPSqlFilters::loadAllFilters()
{
	//startLogTimer("TCPSqlFilters::loadAllFilters");
	
	QString filter;
        filterModel->clear();
        //fill model from zoomfilters table
        filterModel->setTable("zoom.filters");
        filter="filters_zoom_pn='"+getZoomKey()+"' AND filters_user_pn='"+getUserKey()+"' AND filters_active=TRUE";
        filterModel->setFilter(filter);

        //last used is the first
        filterModel->setSort(filterModel->fieldIndex("filters_savetime"),Qt::DescendingOrder);

        if (!filterModel->select()) {
                lastErrors.append(filterModel->lastError().text());
                return false;
        }

        //if no record, DEFAULT must insert
        if (filterModel->rowCount()==0) {
                currentIdx=-1;
                return true;
        }

        //first is the current
        currentIdx=0;
	
	//endLogTimer("TCPSqlFilters::loadAllFilters");
	return true;
}

//================================

bool TCPSqlFilters::activateFilter( const QString & name )
{
        //search
        QModelIndex m_Search = filterModel->index(0,filterModel->fieldIndex("filters_name"));
        QModelIndexList match = filterModel->match( m_Search,Qt::DisplayRole,name,1,Qt::MatchExactly);

        if (!match.count()) {
                return false;
        }

	return activateFilter(match.value(0).row());
}

//================================
//0ms
bool TCPSqlFilters::activateFilter( int cidx )
{
	currentIdx=cidx;
        setupCurrentFilter();
	return true;
}

//================================

int TCPSqlFilters::changeCurrentIndex ( int cidx )
{
        currentIdx=cidx;
        int ret=currentIdx;
        return ret;
}


//================================
//Insert record or save current filter
//80-150ms
bool TCPSqlFilters::saveFilter()
{
	QStringList names=getFilterList();

        QSqlRecord record;
	
	//Check for duplicates
	//insert
        if (currentIdx==-1) {
                //empty record
        	record=filterModel->record();

                if (names.contains(getName())) {
                        lastErrors.append("Insert: Filter name ("+getName()+") must be unique !");
                        return false;
                }
	}
        record=filterModel->record(currentIdx);
	//update: rename or save
        if (currentIdx!=-1) {
		names.removeAt(currentIdx);
                if (names.contains(getName())) {
                        lastErrors.append("Update: Filter name ("+getName()+") must be unique !");
                        return false;
                }
        }


        record.setValue("filters_name",getName());
        record.setValue("filters_zoom_pn",getZoomKey());
        record.setValue("filters_user_pn",getUserKey());
        record.setValue("filters_selectedid",getSelectedRow());
        record.setValue("filters_state",getState());
        record.setValue("filters_masterfilter",getMasterFilterText());
        record.setValue("filters_masterfiltercolumn",getMasterFilterColumn());
        record.setValue("filters_readonly",getReadOnly());
        record.setValue("filters_complexfilters",getComplexFilter());
        record.setValue("filters_filters",TCSqlConvertUtils(getHeaderFilters()).toPgSql()); //QStringList
	record.setValue("filters_aggregators",TCSqlConvertUtils(getHeaderAggregators()).toPgSql()); //QStringList
	record.setValue("filters_window",TCSqlConvertUtils(getWindow()).toPgSql()); //QRect
#ifdef MODIFIY_ORDER
        record.setValue("filters_savetime",QDateTime::currentDateTime()); //QList<int>
#endif

        //commit record
        if (currentIdx==-1) {
                //insert
                filterModel->insertRecord(-1,record);
        } else {
                //update
                filterModel->setRecord(currentIdx,record);
        }

        if (!filterModel->submitAll()) {
                lastErrors.append(filterModel->lastError().text());
                return false;
        }

	return true;
}

//================================
//50-60 ms
bool TCPSqlFilters::deleteFilter(int idx )
{
	if (filterModel->rowCount()==1)
	{
		lastErrors.append("Filter must contains one row !");
		return false;
	}
        if (!filterModel->removeRows(idx,1)) {
                lastErrors.append(filterModel->lastError().text());
                return false;
        }
        if (!filterModel->submitAll()) {
                lastErrors.append(filterModel->lastError().text());
                return false;
        }
        currentIdx=-1;
	
        return true;
}

//================================

int TCPSqlFilters::numRows()
{
	int ret=filterModel->rowCount();
	return ret;
}

//================================

QStringList TCPSqlFilters::getErrors()
{
        QStringList ret=lastErrors;
        lastErrors.clear();
        return ret;
}

//=====================================================================
//PRIVATE
//=====================================================================
bool TCPSqlFilters::updateWithEmpty(const QString & name,int numCols)
{
        //zoomKey,userKey, must set in constructor
        setName(name);
        setSelectedRow(0);
        setState(0);
        setMasterFilterText("");
        setComplexFilter("");
        setReadOnly(false);

        //Widget default pos and size
        setWindow(10,10,400,300);


        QStringList flt;
	QStringList agg;
	for ( int i = 0; i <numCols; i++ ) {
                flt.append("*");
		agg.append("-");
        }
        setHeaderFilters(flt);
	setHeaderAggregators(agg);

        //save
        currentIdx=-1;
        return saveFilter();
}

//================================

void TCPSqlFilters::setupCurrentFilter()
{
        QSqlRecord record=filterModel->record(currentIdx);
        setName(record.value("filters_name").toString());
        setZoomKey(record.value("filters_zoom_pn").toString());
        setUserKey(record.value("filters_user_pn").toString());
        setSelectedRow(record.value("filters_selectedid").toLongLong());
        setState(record.value("filters_state").toByteArray());
        setReadOnly(record.value("filters_readonly").toBool());
        setMasterFilterText(record.value("filters_masterfilter").toString());
        setMasterFilterColumn(record.value("filters_masterfiltercolumn").toString());
        setComplexFilter(record.value("filters_complexfilters").toString());
        setWindow(record.value("filters_window").toRect());
        setHeaderFilters(TCSqlConvertUtils(record.value("filters_filters")).toStringList());
	setHeaderAggregators(TCSqlConvertUtils(record.value("filters_aggregators")).toStringList());
}
