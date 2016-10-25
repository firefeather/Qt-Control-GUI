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
#include "qirquadsplitterplugin.h"
#include <QIrQuadSplitter>

QIrQuadSplitterPlugin::QIrQuadSplitterPlugin( QObject * parent ) : QObject(parent), 
QIronWidgetPlugin("QIrQuadSplitter","QIrQuadSplitter",QIcon(":/qiron/designer/images/quadsplitter.png"),true)
{	
}
QWidget * QIrQuadSplitterPlugin::createWidget( QWidget * parent )
{
	return new QIrQuadSplitter(parent);
}
QString QIrQuadSplitterPlugin::domXml() const
{
	return "<widget class=\"QIrQuadSplitter\" name=\"quadSplitter\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>200</width>\n"
		"   <height>200</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}
