/********************************************************************** 
**
** This file is part of QIron Toolkit.
**
** Copyright (C) 2009-2020 Dzimi Mve Alexandre <dzimiwine@gmail.com>
** 
** Contact: dzimiwine@gmail.com
** 
** QIron is a free toolkit developed in Qt by Dzimi Mve A.; you can redistribute 
** sources and libraries of this library and/or modify them under the terms of 
** the GNU Library General Public License version 3.0 as published by the 
** Free Software Foundation and appearing in the file LICENSE.txt included in 
** the packaging of this file.
** Please review the following information to ensure the GNU General 
** Public License version 3.0 requirements will be met: 
** http://www.gnu.org/copyleft/gpl.html.
**
** This SDK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
**********************************************************************/
#include "qirbreadcrumbbarplugin.h"
#include <QIrBreadCrumbBar>

QIrBreadCrumbBarPlugin::QIrBreadCrumbBarPlugin( QObject * parent ) : QObject(parent), 
QIronWidgetPlugin("QIrBreadCrumbBar","QIrBreadCrumbBar",QIcon(":/qiron/designer/images/breadcrumbbar.png"),false)
{	
}
QWidget * QIrBreadCrumbBarPlugin::createWidget( QWidget * parent )
{
	return new QIrBreadCrumbBar(parent);
}
QString QIrBreadCrumbBarPlugin::domXml() const
{
	return "<widget class=\"QIrBreadCrumbBar\" name=\"breadCrumbBar\">\n"
		"</widget>\n";
}
