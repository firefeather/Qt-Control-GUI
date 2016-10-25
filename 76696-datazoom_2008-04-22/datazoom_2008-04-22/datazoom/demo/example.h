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

#ifndef EXAMPLE_H
#define EXAMPLE_H
#include <QtGui/QMainWindow> 
#include <QtGui/QWorkspace>
#include <QtSql/QSqlDatabase>
#include <ui_example.h>

/*! \class example
 *  \brief Fast data Borwser example
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2004-2008
 */

class example;
class example : public QMainWindow
{
        Q_OBJECT

public:
        example( QWidget* parent = 0);
        ~example();
        QSqlDatabase zoomDB;

private:
        Ui_MainWindow ui;
        QString uniqueDBKey;
        QWorkspace *ws;

private slots:
        virtual void on_actionEmbedded_in_dialog_triggered();


};
#endif
