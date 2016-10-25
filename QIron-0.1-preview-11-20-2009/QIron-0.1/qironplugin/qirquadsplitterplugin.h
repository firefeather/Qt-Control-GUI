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
#ifndef QIRQUADSPLITTERPLUGIN_H
#define QIRQUADSPLITTERPLUGIN_H

#include "qironwidgetplugin.h"
#include <QObject>

class QIrQuadSplitterPlugin : public QObject, public QIronWidgetPlugin
{
	Q_OBJECT
	
public:
	QIrQuadSplitterPlugin( QObject * parent = 0 );

	QString domXml() const;
	QWidget * createWidget( QWidget * parent );
};

#endif // QIRQUADSPLITTERPLUGIN_H
