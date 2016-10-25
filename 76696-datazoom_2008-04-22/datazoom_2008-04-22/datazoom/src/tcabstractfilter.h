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

#ifndef TCABSTRACTFILTER_H
#define TCABSTRACTFILTER_H

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtCore/QRectF>
#include <QtCore/QRect>

/* TODO too old but similar :)
Filters columns:
CSV:						SQL:
-						-- (bserial) filters_id
(String) pointer to zoom name			-- (int8) filters_zoom_p
(String) user name				-- (int8) filters_user_p
(String) name					-- (text) filters_name 			// Unique with zoom_p & user_p
(StringList) filters				-- (text[]) filters_filters		// Example: "*","Ut*", "K*a" etc.
(String) complex filter				-- (text) filters_complexfilter		// Example: "column > '10' AND date < '2001-11-21'"
(String) last used current row			-- (int8) filters_selectedid
(String) last used sort column			-- (int4) filters_sortcolumn		// sorted column (No sorting=-1)
(String) last used sort column order		-- (int4) filters_sortcolumnorder	// order by. Asc, Desc
(StringList) last used window size: X,Y,W,H	-- (int4[4]) filters_window
(StringList) last used column widths		-- (int4[]) filters_columnwidths
(String) last used master_filter		-- (text) filters_masterfilter		//example: "DAT*"
(String) timestamp of last using		-- (timestamp) filters_savetime
 
Every zoom must have one DEFAULT named filter. This is read only from zoom. Zoom generate this by first call.
 
*/

class TCAbstractFilter;
class TCAbstractFilter : public QObject
{
        Q_OBJECT
public:

        explicit TCAbstractFilter(QObject *parent = 0);
        virtual ~TCAbstractFilter();

        //Must be implemented for TCHandler & TWDataZoom
        virtual QStringList getFilterList() = 0; //get Filter list for combobox
        virtual bool newFilter(const QString &name, int numCols) = 0;
        virtual bool activateFilter(const QString &name) = 0;
        virtual bool activateFilter( int cidx) = 0;
        virtual int changeCurrentIndex ( int cidx ) = 0;
        virtual bool loadAllFilters() = 0;
        virtual bool saveFilter() = 0;
        virtual bool deleteFilter(int idx) = 0;
        virtual bool insertDefaultFilter(int colNum) = 0;
        virtual int  numRows() = 0;
        virtual QStringList getErrors() = 0;

        virtual void setId(qint64 id);
        virtual void setZoomKey(const QString &zoomKey);
        virtual void setUserKey(const QString &userKey);
        virtual void setName(const QString &name);
        virtual void setHeaderFilters(QStringList filters);
	virtual void setHeaderFilter(int column,QString filter);
	virtual void setHeaderAggregators( QStringList aggregators );
	virtual void setHeaderAggregator(int column,QString aggregator);
	virtual void setComplexFilter(QString cfilter);
        virtual void setSelectedRow(qint64 selectedRow);
        virtual void setState(QByteArray state);
        virtual void setWindow(int x, int y, int w, int h);
        virtual void setWindow(QRect win);
        virtual void setMasterFilterText(QString masterFilter);
        virtual void setMasterFilterColumn(QString masterFilterColumn);
        virtual void setReadOnly(bool ro);
        virtual void setSaveTime(QDateTime saveTime);

        //inline implementations
        inline qint64 getId() const
        {
                return id;
        }
        inline QString getZoomKey() const
        {
                return zoomKey;
        }
        inline QString getUserKey() const
        {
                return userKey;
        }
        inline QString getName() const
        {
                return name;
        }
        inline QStringList getHeaderFilters() const
        {
                return headerFilters;
        }
        inline QString getHeaderFilter (int idx) const
        {
		//if row numbers in params was changed
		if (headerFilters.count()<=0 || idx>=headerFilters.count() || idx <0)
		{
			return "*";
		}
               return headerFilters.value(idx);
        }
	inline QStringList getHeaderAggregators() const
	{
		return aggregators;
	}
	inline QString getHeaderAggregator (int idx) const
	{
		if (aggregators.count()<=0 || idx>=aggregators.count() || idx<0)
		{
			return "";
		}
		return aggregators.value(idx);
	}
        inline QString getComplexFilter() const
        {
                return complexFilter;
        }
        inline qint64 getSelectedRow() const
        {
                return selectedRow;
        }
        inline QByteArray getState() const
        {
                return state;
        }
        inline QRect getWindow() const
        {
                return window;
        }
        inline QString getMasterFilterText() const
        {
                return masterFilterText;
        }
        inline QString getMasterFilterColumn() const
        {
                return masterFilterColumn;
        }
        inline bool getReadOnly() const
        {
                return readOnly;
        }
        inline QDateTime getSaveTime() const
        {
                return saveTime;
        }

	virtual void startLogTimer(const QString & name);
	virtual int endLogTimer(const QString & name);
	
protected:

private:
        QString zoomKey;		 	// Zoom ID foreign Key
        QString userKey; 			// User ID foreign Key
        QString name; 				// Name of this filter
        QStringList headerFilters;		// FIlters example: *,Kc*...etc.
	QStringList aggregators;		// SUM, COUNT NONE...
	QString complexFilter;			// 
        qint64 selectedRow;			// Last selected row ID (first item of QSelection)
        QRect window;
        QString masterFilterText;		// Master filter value
        QString masterFilterColumn;		// Master filter value
        bool readOnly;				// Read only filter
        QDateTime saveTime;		// Filter save timestamp
        QByteArray state;			// Zoom horizontal header sizes & orders

	//Do not save:
        qint64 id; 					// Unique id for current data (for sql)
	
	//for timer utility
	QHash<QString, QTime> logTimer;

};

#endif // TCABSTRACTFILTER_H
