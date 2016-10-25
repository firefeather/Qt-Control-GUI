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

#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>

#include "tcabstractdatamodel.h"


TCAbstractDataModel::TCAbstractDataModel ( TCAbstractZoomParams *p_params, TCAbstractFilter *p_filters, QObject *parent) : QAbstractTableModel(parent)
{
	Q_CHECK_PTR(p_params);
        params=p_params;
	
	Q_CHECK_PTR(p_filters);
	filters=p_filters;
	
	countPuffer=0;
        showAggregators=true;
	showInactives=true;
        //red transparent image (for gaming)
        redPixmap = QPixmap(20, 20);
        redPixmap.fill(Qt::transparent);
        QRadialGradient gradient(10, 10, 10, 10, 10);
        gradient.setColorAt(0, QColor::fromRgbF(1, 0, 0, 1));
        gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
        QPainter painter(&redPixmap);
        painter.fillRect(0, 0, 20, 20, gradient);
}

//=============================================

void TCAbstractDataModel::emitRefreshRows(qint64 oldRowCount,qint64 newRowCount) const
{
	Q_ASSERT (oldRowCount>=0);
	Q_ASSERT (newRowCount>=0);

	//remove rows
	if (oldRowCount>0) {
                //emit signal rowsAboutToBeRemoved
                const_cast<TCAbstractDataModel*>(this)->beginRemoveRows(QModelIndex(),0,oldRowCount);
                //emit signal rowsRemoved
                const_cast<TCAbstractDataModel*>(this)->endRemoveRows();
	}

        //insert rows
        if (newRowCount>0) {
                //emit signal rowsAboutToBeInserted
                const_cast<TCAbstractDataModel*>(this)->beginInsertRows(QModelIndex(),0,newRowCount);
                //emit signal rowsInserted
                const_cast<TCAbstractDataModel*>(this)->endInsertRows();
        }
	
	//Vertical Header
	emit const_cast<TCAbstractDataModel*>(this)->headerDataChanged( Qt::Vertical,0,newRowCount );

}

//=============================================

void TCAbstractDataModel::emitRefreshData(qint64 newRowCount) const
{
	Q_ASSERT (newRowCount>=0);
	
        QModelIndex rowFirstColumn = index( 0, 0, QModelIndex() );
        QModelIndex rowLastColumn = index( newRowCount, 3, QModelIndex() );
	
        emit const_cast<TCAbstractDataModel*>(this)->dataChanged( rowFirstColumn, rowLastColumn );
}

//=============================================

void TCAbstractDataModel::emitLayoutChanged() const
{
	emit const_cast<TCAbstractDataModel*>(this)->layoutChanged();
}

//=============================================

void TCAbstractDataModel::emitRefreshAll(qint64 oldRowCount,qint64 newRowCount) const
{
	Q_ASSERT (oldRowCount>=0);
	Q_ASSERT (newRowCount>=0);
	
	emitRefreshRows(oldRowCount,newRowCount);
	emitRefreshData(newRowCount);
	emitLayoutChanged();
}
//=============================================

void TCAbstractDataModel::emitRefreshHorizontalHeader(int lastPos) const
{
	Q_ASSERT (lastPos>=0);
	emit const_cast<TCAbstractDataModel*>(this)->headerDataChanged( Qt::Horizontal,0, lastPos);
}

//=============================================
// If keysearch is enabled on tableView, this method will call by typing text (search in actual column)
// Now it returns only Start index, to go to the first row. Data search made by another way.
QModelIndexList TCAbstractDataModel::match ( const QModelIndex & start, int /*role*/, const QVariant & /*value*/, int /*hits*/, Qt::MatchFlags /*flags*/ ) const
{
        QModelIndexList ret;
        ret.append (start);
        return ret;
}

//=============================================

void TCAbstractDataModel::setSelectedColumn( int p_selectedColumn )
{
        selectedColumn=p_selectedColumn;
}

//=============================================

void TCAbstractDataModel::setOrderDirection( order p_orderDirection )
{
        orderDirection=p_orderDirection;
}

//=============================================

void TCAbstractDataModel::setDoRealFetch(bool p_doRealFetch,TCAbstractDataModel::RealQueryRoles role) const
{
	//qDebug()<<"REAL Fetch value, role:"<<p_doRealFetch<<" / "<<role;
	doRealFetch=p_doRealFetch;
}

//=============================================

void TCAbstractDataModel::setDoRealCount(bool p_doRealCount,TCAbstractDataModel::RealQueryRoles role) const
{
	//qDebug()<<"REAL Count value, role:"<<p_doRealCount<<" / "<<role;
	doRealCount=p_doRealCount;
}

//=============================================

void TCAbstractDataModel::setCountPuffer(qint64 p_countPuffer) const
{
        countPuffer=p_countPuffer;
}

//=============================================

void TCAbstractDataModel::setShowAggregators(bool p_showAggregators) const
{
        showAggregators=p_showAggregators;
}

//=============================================

void TCAbstractDataModel::setShowInactives(bool p_showInactives) const
{
	showInactives=p_showInactives;
}


