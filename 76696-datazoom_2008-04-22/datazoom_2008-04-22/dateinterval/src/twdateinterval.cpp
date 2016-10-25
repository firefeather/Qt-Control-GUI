/*
 * Date Interval wdget
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

#include "twdateinterval.h"
#include <QtCore/QDateTime>
#include <QtGui/QComboBox>
#include <QtCore/QDebug>

TWDateInterval::TWDateInterval( QWidget* parent ) : QWidget( parent )
{
    ui.setupUi(this);
    ui.fromDateEdit->setDate(QDate::currentDate());
    ui.toDateEdit->setDate(QDate::currentDate());

    ui.toDateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
    ui.fromDateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
    
    ui.fromDateEdit->adjustSize();
    ui.toDateEdit->adjustSize();
}

//=============================================================

TWDateInterval::~TWDateInterval()
{
}

//=============================================================

void TWDateInterval::on_comboBox_activated()
{
	
	if (ui.comboBox->currentIndex()==0)
	{
		ui.fromDateEdit->setEnabled(TRUE);
		ui.toDateEdit->setEnabled(TRUE);
	} else 
	{
		ui.fromDateEdit->setEnabled(FALSE);
		ui.toDateEdit->setEnabled(FALSE);
	
		QList<QDate> dt = getDates(ui.comboBox->currentIndex());
	
    		ui.fromDateEdit->setDate(dt.first());
		ui.toDateEdit->setDate(dt.last());
		ui.fromDateEdit->adjustSize();
		ui.toDateEdit->adjustSize();
	}
}

//=============================================================

QDate TWDateInterval::getFromDate()
{
	return ui.fromDateEdit->date();
}

//=============================================================

QDate TWDateInterval::getToDate()
{
	return ui.toDateEdit->date();
}

//=============================================================

QList<QDate> TWDateInterval::getDates(int code)
{
	QList<QDate> result;
	QDate fromDate;
	QDate toDate;
	int qrtmonth=QDate::currentDate().month()-((QDate::currentDate().month()-1) % 3);

	switch (code)
	{
		//User defined
		case 0 :
			break;
		//Today
		case 1:
			fromDate=QDate::currentDate();
			toDate=QDate::currentDate();
			break;
		//Yesterday
		case 2:
			fromDate=QDate::currentDate().addDays(-1);
			toDate=QDate::currentDate().addDays(-1);
			break;
		//To Today
		case 3:
			fromDate=QDate(2000,1,1);
			toDate=QDate::currentDate();
			break;
		//This Month
		case 4:
			fromDate=QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
			toDate=QDate::currentDate();
			break;
		//previous Month
		case 5:
			fromDate=QDate(QDate::currentDate().year(),QDate::currentDate().month(),1).addMonths(-1);
			toDate=QDate(QDate::currentDate().year(),QDate::currentDate().month(),1).addDays(-1);
			break;
		//To This Month
		case 6:
			fromDate=QDate(2000,1,1);
			toDate=QDate(QDate::currentDate().year(),QDate::currentDate().month(),1).addDays(-1);
			break;
		//This Quarter
		case 7:
			fromDate=QDate(QDate::currentDate().year(),qrtmonth,1);
			toDate=QDate::currentDate();
			break;
		//Previous Quarter
		case 8:
			fromDate=QDate(QDate::currentDate().year(),qrtmonth,1).addMonths(-3);
			toDate=QDate(QDate::currentDate().year(),qrtmonth,1).addDays(-1);
			break;
		//To This Quarter
		case 9:
			fromDate=QDate(2000,1,1);
			toDate=QDate(QDate::currentDate().year(),qrtmonth,1).addDays(-1);
			break;
		//This Year
		case 10:
			fromDate=QDate(QDate::currentDate().year(),1,1);
			toDate=QDate::currentDate();
			break;
		//Previous Year
		case 11:
			fromDate=QDate(QDate::currentDate().year(),1,1).addYears(-1);
			toDate=QDate(QDate::currentDate().year(),1,1).addDays(-1);
			break;
		//To This Year
		case 12:
			fromDate=QDate(2000,1,1);
			toDate=QDate(QDate::currentDate().year(),1,1).addDays(-1);
			break;
		default:
		{
		}
	}
	result.append(fromDate);
	result.append(toDate);
	return result;
}

//=============================================================

int TWDateInterval::getCurrentCode()
{
	return ui.comboBox->currentIndex();
}

//=============================================================

