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


#ifndef TCABSTRACTDATAMODEL_H
#define TCABSTRACTDATAMODEL_H


#include <QtCore/QAbstractTableModel>
#include <QtCore/QDebug>
#include <QtGui/QPixmap>

#include "tcabstractfilter.h"
#include "tcabstractzoomparams.h"

/*! \class TCAbsractDataModel
 *  \brief Data Model. Transfer data  to widget.
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */

class TCAbstractDataModel : public QAbstractTableModel
{
        Q_OBJECT
        Q_ENUMS( order RealCountRoles)
public:
        enum order {
                Ascending,
                Descending,
                None
        };

        enum RealQueryRoles {
                UndefinedRole=0,
                ConstructorRole=1,
                RowCountRole=2,
                FetchRole=3,
                RealRowCountRole=4,
                ConstructFieldsRole=5,
                ConstructWhereRole=6,
                ConstructFromRole=7,
                SetOrderRole=8, 			//TWDataZoom
                HeaderClickedRole=9,			//TWDataZoom
		RefreshDataRole=10,			//TWDataZoom
  		UpdateWidgetsFromCurrentFilterRole=11,	//TWDataZoom
    		ShowEventRole=12			//TWDataZoom

        };

        TCAbstractDataModel(TCAbstractZoomParams *params,TCAbstractFilter *filters, QObject *parent = 0);
        virtual QStringList getErrors() = 0;
        virtual void closeConnections() = 0;
        virtual QSqlRecord getRecord(int row)=0;
        virtual bool testActiveColumnExists() = 0;
	virtual int fieldIndex(const QString & name) = 0;
        virtual void setSelectedColumn(int selectedColumn);
        virtual void setOrderDirection(order orderDirection);
        virtual void setDoRealFetch(bool flag, TCAbstractDataModel::RealQueryRoles role) const;
        virtual void setDoRealCount(bool flag, TCAbstractDataModel::RealQueryRoles role) const;
        virtual void setCountPuffer(qint64 cnt) const;
        virtual void setShowAggregators(bool flag) const;
        virtual void setShowInactives(bool flag) const;
        virtual void emitRefreshHorizontalHeader(int lastPos) const;
        virtual void emitRefreshRows(qint64 oldRowCount, qint64 newRowCount) const;
        virtual void emitRefreshData(qint64 newRowCount) const;
        virtual void emitRefreshAll(qint64 oldRowCount, qint64 newRowCount) const;
        virtual void emitLayoutChanged() const;

        inline TCAbstractZoomParams * getZoomParamsObject() const {
                return params;
        }
        inline TCAbstractFilter * getFilterObject() const {
                return filters;
        }

        inline QPixmap getRedPixmap() const {
                return redPixmap;
        }
        inline QIcon getFilterIcon() const {
                return QIcon(":/16x16/icons/16x16/actions/filter.png");
        }
        inline int getSelectedColumn() const {
                return selectedColumn;
        }
        inline order getOrderDirection() const {
                return orderDirection;
        }
        inline bool getDoRealFetch() const {
                return doRealFetch;
        }
        inline bool getDoRealCount() const {
                return doRealCount;
        }
        inline qint64 getCountPuffer() const {
                return countPuffer;
        }
        inline bool getShowAggregators() const {
                return showAggregators;
        }
        inline bool getShowInactives() const {
                return showInactives;
        }



private:
        QPixmap redPixmap;			// initialized by by constructor
        int selectedColumn;			// hold zoom active column for pattern drawing by dataModel
        order orderDirection;			// hold zoom active column order direction
        bool rowNumChange;
        bool visbleChange;
        mutable bool doRealCount;
        mutable bool doRealFetch;
        mutable qint64 countPuffer;
        mutable bool showAggregators;
        mutable bool showInactives;
        mutable QString masterFilterText;
        mutable QString masterFilterColumn;
        mutable QStringList aggregators;
        mutable QStringList headerFilters;
protected:
        TCAbstractZoomParams *params;
        TCAbstractFilter *filters;
public slots:
        virtual QModelIndexList match ( const QModelIndex & start, int role, const QVariant & value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags( Qt::MatchStartsWith | Qt::MatchWrap ) ) const;
};
#endif
