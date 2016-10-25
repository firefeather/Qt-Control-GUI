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

#include "dialog.h"
#include "tcpsqlzoomparams.h"
#include "tcpsqlfilters.h"
#include "tcpsqldatamodel.h"
#include <QtSql/QSqlError>
#include <QtGui/QMessageBox>

Dialog::Dialog(  QHash <int, QSqlRecord>* result, QWidget* parent) : QDialog( parent)
{
        zoomResult=result;
	ui.setupUi(this);

        QDateTime now = QDateTime::currentDateTime();
        uniqueDBKey=now.toString("yy-MM-dd-hh-mm-ss-zzz");
        uniqueDBKey="dialog-"+uniqueDBKey;

        zoomDB = QSqlDatabase::addDatabase( "QPSQL" , uniqueDBKey);

	//CHANGE !!
        zoomDB.setHostName( "localhost" );
	//CHANGE !!
        zoomDB.setDatabaseName( "test" );
	//CHANGE !!
        zoomDB.setUserName( "admin" );
	//CHANGE !!
        zoomDB.setPort( 5432 );
	//CHANGE !!
        zoomDB.setPassword( "" );


	if (! zoomDB.open())
	{
	    QString err= zoomDB.lastError().text();
    	    QMessageBox::warning(this,trUtf8("Dialog"), err , QMessageBox::Ok);
	    QCoreApplication::exit(0);
	}

        //load zoom parameters for gui
        TCPSqlZoomParams *zoomParams = new TCPSqlZoomParams(uniqueDBKey,"zoomname",this);
	QString err=zoomParams->getErrors().join("\n");
	if (err!="")
	{            
                QMessageBox::warning(this,trUtf8("example: "), err , QMessageBox::Ok);
		return;
        }

        //load last used filter
	TCPSqlFilters *filters = new TCPSqlFilters(uniqueDBKey,"zoomname","usrname",this);
	
	TCPSqlDataModel *sqlModel = new TCPSqlDataModel(uniqueDBKey,zoomParams,filters,this);
	ui.dataZoom->setHandlers(sqlModel,result);	
	ui.dataZoom->setShowAggregators(true);

        //data handling with limits & affosets & special queries
        //sql Model use filter
        //sqlModel->setFilter(filters);
}

//========================================

Dialog::~Dialog()
{}

//========================================

void Dialog::on_cancelPushButton_clicked( )
{}

//========================================

void Dialog::on_okPushButton_clicked( )
{}
