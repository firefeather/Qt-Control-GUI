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

#include "tcsqlconvertutils.h"
#include "tcpsqlzoomparams.h"

TCPSqlZoomParams::TCPSqlZoomParams(QString &conn, const QString & name, QObject *parent) : TCAbstractZoomParams( parent)
{
        lastErrors.clear();
        pgDB=QSqlDatabase::database (conn);
        zoomParamsModel = new QSqlQueryModel(this);
        setName(name);
        loadZoomParameter(name);
}

//================================

TCPSqlZoomParams::~TCPSqlZoomParams()
{}

//================================

bool TCPSqlZoomParams::loadZoomParameter( const QString & name )
{
        QString sqlQuery="SELECT * FROM zoom.params WHERE params_uname='"+name+"' AND params_active=TRUE limit 1";

        zoomParamsModel->setQuery(sqlQuery,pgDB);
        if (zoomParamsModel->lastError().isValid()) {
                lastErrors.append(zoomParamsModel->lastError().text());
                return false;
        }

	if (zoomParamsModel->rowCount()==0)
	{
                lastErrors.append("TCPSqlZoomParams::loadZoomParameter()::\n\n Can't find zoom: "+name);
                return false;
	}

        QSqlRecord record=zoomParamsModel->record(0);
        setName(record.value("params_uname").toString());
        setTitle(record.value("params_title").toString());
        setDataTable(record.value("params_datatable").toString());
        setSelectionMode((QAbstractItemView::SelectionMode)record.value("params_selectionmode").toInt());
        setActiveColumn(record.value("params_activecolumn").toString());
        setColumns(TCSqlConvertUtils(record.value("params_columns")).toStringList());
        setHeaderNames(TCSqlConvertUtils(record.value("params_headernames")).toStringList());
        setFilterZoomNames(TCSqlConvertUtils(record.value("params_filterzoomnames")).toStringList());
        setFilterZoomColumns(TCSqlConvertUtils(record.value("params_filterzoomcolumns")).toStringList());
        setEditZoomNames(TCSqlConvertUtils(record.value("params_editzoomnames")).toStringList());
        setRightGroups(TCSqlConvertUtils(record.value("params_rightgroups_p")).toQListBigInt());
        setFGColorColumn(record.value("params_fgcolorcolumn").toString());
        setBGColorColumn(record.value("params_bgcolorcolumn").toString());
        return true;
}

//================================

QStringList TCPSqlZoomParams::listZoomParameters()
{
        QStringList ret;
        return ret;
}

//================================

QStringList TCPSqlZoomParams::getErrors()
{
        QStringList ret=lastErrors;
        lastErrors.clear();
        return ret;
}
