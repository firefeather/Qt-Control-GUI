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

#include "tcabstractfilter.h"

TCAbstractFilter::TCAbstractFilter(QObject *parent) : QObject( parent)
{
}

TCAbstractFilter::~TCAbstractFilter()
{
}

//================================

void TCAbstractFilter::setId( qint64 p_id )
{
 	id=p_id;
}

//================================

void TCAbstractFilter::setZoomKey( const QString & p_zoomKey )
{
	zoomKey =p_zoomKey;
}

//================================

void TCAbstractFilter::setUserKey( const QString & p_userKey )
{
	userKey=p_userKey;
}

//================================

void TCAbstractFilter::setName( const QString & p_name )
{
	name=p_name;
}

//================================

void TCAbstractFilter::setHeaderFilters( QStringList p_headerFilters )
{
	headerFilters=p_headerFilters;
}

//================================

void TCAbstractFilter::setHeaderFilter( int column, QString p_headerFilter)
{
	headerFilters.replace(column,p_headerFilter);
}

//================================

void TCAbstractFilter::setHeaderAggregators( QStringList p_aggregators )
{
	aggregators=p_aggregators;
}

//================================

void TCAbstractFilter::setHeaderAggregator(int column, QString p_aggregator)
{
	aggregators.replace(column,p_aggregator);
}


//================================

void TCAbstractFilter::setMasterFilterText( QString p_masterFilter )
{
	masterFilterText=p_masterFilter;
}

//================================

void TCAbstractFilter::setMasterFilterColumn( QString p_masterFilterColumn )
{
	masterFilterColumn=p_masterFilterColumn;
}

//================================

void TCAbstractFilter::setComplexFilter( QString p_complexFilter )
{
	complexFilter=p_complexFilter;
}

//================================

void TCAbstractFilter::setSelectedRow( qint64 p_selectedRow )
{
	selectedRow=p_selectedRow;
}

//================================

void TCAbstractFilter::setState( QByteArray p_state )
{
	state=p_state;
}

//================================

void TCAbstractFilter::setWindow( int x, int y, int w, int h )
{
	window.setLeft(x);
	window.setTop(y);
	window.setWidth(w);
	window.setHeight(h);
}

void TCAbstractFilter::setWindow( QRect win)
{
	window=win;
}


//================================

void TCAbstractFilter::setReadOnly( bool p_ro )
{
	readOnly=p_ro;
}

//================================

void TCAbstractFilter::setSaveTime( QDateTime p_saveTime )
{
	saveTime=p_saveTime;
}

//==============================================
// UTILS
//==============================================

void TCAbstractFilter::startLogTimer(const QString & name)
{
	logTimer[name]=QTime::currentTime();
}

//==============================================
int TCAbstractFilter::endLogTimer(const QString & name)
{
	QTime currentTime=logTimer[name];
	int result=currentTime.msecsTo(QTime::currentTime());
	if (result > 0)
	{
		qDebug()<<name << " [msecs]: " << result;
	}
	logTimer.remove(name);
	return result;
}
