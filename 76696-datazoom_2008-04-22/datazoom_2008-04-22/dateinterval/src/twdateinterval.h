/*
 * Date Interval Widget
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

#ifndef TWDATEINTERVAL_H
#define TWDATEINTERVAL_H

#include "ui_twdateinterval.h"
#include <QtGui/QWidget>
#include <QtCore/QDate>

/*! \class TWDateInterval
 *  \brief TWDateInterval
 *  \author Kund Gordos
 *  \version 4.0
 *  \date     2008
 */

class TWDateInterval : public QWidget
{
    Q_OBJECT

public:
    TWDateInterval( QWidget* parent = 0 );
    virtual ~TWDateInterval();

    virtual QDate getFromDate();
    virtual QDate getToDate();
    virtual int getCurrentCode();
    virtual QList<QDate> getDates(int code);
    
public slots:
    virtual void on_comboBox_activated();

private:    
    Ui_TWDateInterval ui;
};

#endif // TWDATEINTERVAL_H
