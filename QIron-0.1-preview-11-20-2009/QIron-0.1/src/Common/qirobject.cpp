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
#include <QList>
#include "../Common/private/qirobject_p.h"

QIR_BEGIN_NAMESPACE

QIrObjectList QIrObjectPrivate::allObjects;

///////////////////
//QIrObjectPrivate
///////////////////
QIrObjectPrivate::QIrObjectPrivate() : o_ptr(0) 
{ 
}
QIrObjectPrivate::~QIrObjectPrivate() 
{
}
void QIrObjectPrivate::setup()
{
	createActions();
	createMenus();
	init();
	setupConnections();
}

////////////
//QIrObject
////////////
QIrObject::QIrObject() : p_ptr( new QIrObjectPrivate ) 
{
	QIrObjectPrivate::allObjects.append(this);
	p_ptr->o_ptr = this;
}
QIrObject::~QIrObject() 
{ 
	QIrObjectPrivate::allObjects.removeAll(this);
	if ( p_ptr ) {
		delete p_ptr;
	}
}
QIrObject::QIrObject( QIrObjectPrivate & p ) : p_ptr(&p) 
{ 
	QIrObjectPrivate::allObjects.append(this);
	p_ptr->o_ptr = this; 
}
QIrObjectList QIrObject::allObjects( const QString & objectGroupKey )
{
	QString key = objectGroupKey.trimmed();
	QIrObjectList list;

	if ( key.isEmpty() )
		return QIrObjectPrivate::allObjects;
	foreach (QIrObject * o, QIrObjectPrivate::allObjects) {
		if ( o->objectGroupKey().trimmed() == key )
			list.append(list);
	}
	return list;
}

QIR_END_NAMESPACE
