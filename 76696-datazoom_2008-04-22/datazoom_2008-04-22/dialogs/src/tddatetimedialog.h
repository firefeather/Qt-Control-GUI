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

#ifndef TDDATETIMEDIALOG_H
#define TDDATETIMEDIALOG_H
#include <QtCore/QDebug>
#include <QtGui/QDialog>
#include <ui_tddatetimedialog.h>

/*! \class TDDateTimeDialog
 *  \brief date dialog example
 *  \author Kund Gordos
 *  \version 4.0
 *  \date     2004-2007
 */

class TDDateTimeDialog;
class TDDateTimeDialog : public QDialog
{
        Q_OBJECT

public:
        TDDateTimeDialog( QWidget* parent = 0);
        ~TDDateTimeDialog();
        enum Mode {
                Date = 0,
                Time = 1,
                DateTime = 2,
        };
	virtual void setMode(TDDateTimeDialog::Mode md);

private:
        Ui_TDDateTimeDialog ui;
	Mode mode;

private slots:
};
#endif
