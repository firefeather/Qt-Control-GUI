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

#ifndef TCABSTRACTZOOMPARAMS_H
#define TCABSTRACTZOOMPARAMS_H

#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlField>
#include <QtSql/QSqlRecord>
#include <QtCore/QObject>
#include <QtGui/QAbstractItemView>

/*! \class TCAbstractZoomParams
 *  \brief Abstract class for zoom parameter handling
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */

class TCAbstractZoomParams;
class TCAbstractZoomParams : public QObject
{
        Q_OBJECT
public:
        explicit TCAbstractZoomParams( QObject *parent = 0);
        virtual ~TCAbstractZoomParams();

        //Must be implemented for TCHandler & TWDataZoom
        virtual bool loadZoomParameter(const QString &name) = 0;
        virtual QStringList listZoomParameters() = 0;
        virtual QStringList getErrors() = 0;

        virtual void setId(qint64 id);
        virtual void setName(const QString &name);
        virtual void setTitle(const QString &title);
        virtual void setDataTable(const QString &dataTable);
        virtual void setSelectionMode(QAbstractItemView::SelectionMode selectionMode);
        virtual void setActiveColumn(const QString &activeColumn);
        virtual void setActive(bool active );

        virtual void setColumns(QStringList columns);
        virtual void setColumnTypes(QStringList columns);
        virtual void setHeaderNames(QStringList headerNames);
        virtual void setFilterZoomNames(QStringList filterZoomNames);
        virtual void setFilterZoomColumns(QStringList filterZoomColumns);
        virtual void setEditZoomNames(QStringList editZoomNames);
        virtual void setRightGroups(QList<qint64> rightGroups);
        virtual void setFGColorColumn(QString fgColorColumn);
        virtual void setBGColorColumn(QString bgColorColumn);
	virtual  QStringList getHeaderNamesFilteredByType();	

        //inline implementations
        inline qint64 getId() const
        {
                return id;
        }
        inline QString getName() const
        {
                return name;
        }
        inline QString getTitle() const
        {
                return title;
        }
        inline QString getDataTable() const
        {
                return dataTable;
        }
        inline QString getActiveColumn() const
        {
                return activeColumn;
        }
        inline QAbstractItemView::SelectionMode getSelectionMode() const
        {
                return selectionMode;
        }
        inline bool getActive() const
        {
                return active;
        }
        inline QStringList getColumns() const
        {
                return columns;
        }
        inline QStringList getColumnTypes() const
        {
                return columnTypes;
        }
        inline QString getColumnType(int idx) const
        {
		if ( columnTypes.count()<=0 || idx>=columnTypes.count() || idx<0) 
		{
			return "NO_COLUMNTYPE";
		}
		return columnTypes.value(idx);
	}
        inline QString getColumnByName(const QString & colName) const
        {
		int col=headerNames.indexOf(colName);
                if (col<0 || col >= columns.count()) 
		{
			return "NO_COLUMNAME";
		}
		return columns.value(col);
	}

        inline QString getColumnName(int idx) const
        {
		if (columns.count()<=0 || idx>=columns.count() || idx<0 )
		{
			return "NO_COLUMNAME";
		}
		return columns.value(idx);
	}

        inline QStringList getHeaderNames() const
        {
                return headerNames;
        }

        inline QString getHeaderName(int idx) const
        {
		if (headerNames.count()<=0 || idx>=headerNames.count() || idx<0 )
		{
			return QString ("%1").arg(idx);
		}
		return headerNames.at(idx);
	}
        inline QStringList getFilterZoomNames() const
        {
                return filterZoomNames;
        }
        inline QStringList getFilterZoomColumns() const
        {
                return filterZoomColumns;
        }
	inline int transformHeaderIdxToFilteredIdx(int idx)
	{
		QStringList filteredColumns = getHeaderNamesFilteredByType();
		int filteredIdx=filteredColumns.indexOf(headerNames[idx]);
		if (filteredIdx==-1)
		{
			return -1;
		} 
		return idx;
	}
	
/*	inline int transformFilteredIdxToHeaderIdx(int idx)
	{
		QStringList filteredColumns = getColumnNamesFilteredByType();
		int ret=headerNames.indexOf(filteredColumns[idx]);
		return ret;
	}	
*/	
        inline QStringList getEditZoomNames() const
        {
                return editZoomNames;
        }
        inline QList<qint64> getRightGroups() const
        {
                return rightGroups;
        }
        inline QString getFGColorColumn() const
        {
                return fgColorColumn;
        }
        inline QString getBGColorColumn() const
        {
                return bgColorColumn;
        }
	
//protected:

private:
        qint64 id;
        QString name; 					// Unique zoom name
        QString title;					// Window title
        QString dataTable; 				// Sql data table
        QAbstractItemView::SelectionMode selectionMode;	// SelectionMode SingleRow or MultiRow
        QString activeColumn;				// active column in datatable
        bool active;

        QStringList columns; 				// Columns to show
        QStringList columnTypes; 			// Columns types. Do not save
        QStringList headerNames; 			// Column header names
        QStringList filterZoomNames; 		// Zoom names per column, to filter data
        QStringList filterZoomColumns; 		// column names to filter in "zoom_filterzoomnames" tables. if not set then the default is TABLE_name or TABLE_uname
        QStringList editZoomNames; 		// Zoom names per column, if user edit cell with right click
        QString fgColorColumn;
        QString bgColorColumn;
        QList<qint64> rightGroups;
};

#endif // TCABSTRACTZOOMPARAMS_H
