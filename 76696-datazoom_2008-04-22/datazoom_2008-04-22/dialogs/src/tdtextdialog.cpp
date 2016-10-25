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

#include "tdtextdialog.h"

TDTextDialog::TDTextDialog( QWidget* parent, const QString & title, const QString & label, const QString & text) : QDialog( parent)
{
        ui.setupUi(this);
	setWindowTitle(title);
	ui.label->setText(label);
        ui.lineEdit->setText(text);
        ui.lineEdit->selectAll();
	if (label=="")
	{
        	ui.label->hide();
	}
}

//========================================

TDTextDialog::~TDTextDialog()
{}

//========================================

QString TDTextDialog::getText()
{
    return ui.lineEdit->text();
}

//========================================

void TDTextDialog::setValidator( const QValidator *validator)
{
    ui.lineEdit->setValidator(validator);
}

//========================================

void TDTextDialog::setInputMask( const QString &mask)
{
    ui.lineEdit->setInputMask(mask);
}
