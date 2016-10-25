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

#include <QtGui/QMessageBox>
#include <QtSql/QSqlError>
#include <QtCore/QHash>
#include <QtSql/QSqlRecord>
#include "dialog.h"
#include "example.h"
#include "tcpsqlzoomparams.h"
#include "tcpsqlfilters.h"
#include "tcpsqldatamodel.h"
#include "twdatazoom.h"

example::example( QWidget* parent)
{
        ui.setupUi(this);

        (void) statusBar();

        ws= new QWorkspace(this);
        ws->setScrollBarsEnabled(true);
        setCentralWidget(ws);
}

//========================================

example::~example()
{
        //No more message: connection ...is still in use
        zoomDB = QSqlDatabase();
        QSqlDatabase::removeDatabase(uniqueDBKey);
}

//========================================

void example::on_actionEmbedded_in_dialog_triggered()
{
        statusBar()->showMessage("SQL Dialog");

	QHash<int, QSqlRecord> result;
        Dialog *dg = new Dialog(&result,this);
	dg->setAttribute(Qt::WA_DeleteOnClose);
        dg->exec();
	qDebug()<<result;
}

