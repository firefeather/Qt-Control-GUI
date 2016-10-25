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

#ifndef DIALOG_H
#define DIALOG_H
#include <QDebug>
#include <QDialog>
#include <QWidget>
#include <ui_dialog.h>

/*! \class example
 *  \brief Fast data Borwser example
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */

class Dialog;
class Dialog : public QDialog
{
        Q_OBJECT

public:
	Dialog(QHash <int, QSqlRecord> *result, QWidget* parent = 0);
        ~Dialog();
        QSqlDatabase zoomDB;

private:
        QString uniqueDBKey;
        Ui_Dialog ui;
	QHash <int,QSqlRecord> *zoomResult;

private slots:
        virtual void on_cancelPushButton_clicked();
        virtual void on_okPushButton_clicked();
};
#endif
