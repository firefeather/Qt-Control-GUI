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

#include "tcabstractzoomparams.h"

TCAbstractZoomParams::TCAbstractZoomParams(QObject *parent) : QObject( parent)
{}

TCAbstractZoomParams::~TCAbstractZoomParams()
{}

//================================

void TCAbstractZoomParams::setId(qint64 p_id)
{
        id=p_id;
}

//================================

void TCAbstractZoomParams::setName(const QString &p_name)
{
        name=p_name;
}

//================================

void TCAbstractZoomParams::setTitle(const QString &p_title)
{
        title=p_title;
}

//================================

void TCAbstractZoomParams::setDataTable(const QString &p_dataTable)
{
        dataTable=p_dataTable;
}
//================================

void TCAbstractZoomParams::setActiveColumn(const QString &p_activeColumn)
{
        activeColumn=p_activeColumn;
}


//================================

void TCAbstractZoomParams::setSelectionMode(QAbstractItemView::SelectionMode p_selectionMode)
{
        selectionMode=p_selectionMode;
}

//================================

void TCAbstractZoomParams::setActive(bool p_active )
{
        active=p_active;
}

//================================

void TCAbstractZoomParams::setColumns(QStringList p_columns)
{
        columns=p_columns;
}

//================================

void TCAbstractZoomParams::setColumnTypes(QStringList p_columnTypes)
{
        columnTypes=p_columnTypes;
}

//================================

void TCAbstractZoomParams::setHeaderNames(QStringList p_headerNames)
{
        headerNames=p_headerNames;
}

//================================

void TCAbstractZoomParams::setFilterZoomNames(QStringList p_filterZoomNames)
{
        filterZoomNames=p_filterZoomNames;
}

//================================

void TCAbstractZoomParams::setFilterZoomColumns(QStringList p_filterZoomColumns)
{
        filterZoomColumns=p_filterZoomColumns;
}

//================================

void TCAbstractZoomParams::setEditZoomNames(QStringList p_editZoomNames)
{
        editZoomNames=p_editZoomNames;
}

//================================

void TCAbstractZoomParams::setRightGroups( QList<qint64> p_rightGroups)
{
	rightGroups=p_rightGroups;
}

//================================

void TCAbstractZoomParams::setFGColorColumn( QString p_fgColorColumn )
{
	fgColorColumn=p_fgColorColumn;
}

//================================

void TCAbstractZoomParams::setBGColorColumn( QString p_bgColorColumn )
{
	bgColorColumn=p_bgColorColumn;
}

//================================

QStringList TCAbstractZoomParams::getHeaderNamesFilteredByType()
{
	QStringList ret;
	if (columnTypes.size() != headerNames.size()) {
			//error
		qDebug()<<"Column types: " <<columnTypes;
		qDebug()<<"Header names:" << headerNames;
		qDebug()<<"TCAbstractZoomParams::getHeaderNamesFilteredByType() columnType and headerNames  size difference !";
		return ret;
	}
	for (int i = 0; i < headerNames.size(); ++i) {
		QString ct=columnTypes.at(i);
		if ( ct=="QString" || ct=="int" || ct=="qlonglong" || ct=="QDateTime" || ct=="QDate" || ct=="QTime" || ct=="double") {
			ret.append(headerNames[i]);
		}
	}
	return ret;
}
