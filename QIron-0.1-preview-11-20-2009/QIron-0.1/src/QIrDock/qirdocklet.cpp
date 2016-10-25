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
#include <QPainter>
#include "private/qirdocklet_p.h"
#include "qirdockstyle.h"
#include "qirstyleoptiondock.h"
#include "qirabstractdock.h"
#include "../Common/private/qirobject_p.h"
#include <QGraphicsLinearLayout>

QIR_BEGIN_NAMESPACE

////////////////////
//QIrDockletPrivate
////////////////////
QIrDockletPrivate::QIrDockletPrivate() : iconSize(-1), isSeparator(false), removing(false),
destroying(false), active(false), isLauncher(false), dock(0)
{
}
void QIrDockletPrivate::init()
{
	QIR_O(QIrDocklet);

	o->setAcceptHoverEvents(false);
	o->setFlag(QGraphicsItem::ItemStacksBehindParent);
}


/////////////
//QIrDocklet
/////////////
QIrDocklet::QIrDocklet(QIrAbstractDock * dock, Qt::WindowFlags wFlags) 
: QGraphicsWidget(0,wFlags), QIrObject(* new QIrDockletPrivate)
{
	QIR_P(QIrDocklet);
	p->dock = dock;
	p->setup();
}
QIrDocklet::QIrDocklet(const QString & text, QIrAbstractDock * dock) 
: QGraphicsWidget(0,0), QIrObject(* new QIrDockletPrivate)
{
	QIR_P(QIrDocklet);
	p->dock = dock;
	p->text = text;
	p->setup();
}
QIrDocklet::QIrDocklet(const QIcon & icon, const QString & text, QIrAbstractDock * dock)
: QGraphicsWidget(0,0), QIrObject(* new QIrDockletPrivate)
{
	QIR_P(QIrDocklet);
	p->dock = dock;
	p->text = text;
	p->icon = icon;
	p->setup();
}
QIrDocklet::QIrDocklet(QIrAbstractDock * dock, QIrDockletPrivate & pr) : QGraphicsWidget(0,0), 
QIrObject(pr)
{
	QIR_P(QIrDocklet);
	p->dock = dock;
}
QIrDocklet::~QIrDocklet()
{
	QIR_P(QIrDocklet);

	p->destroying = true;
	if (!p->removing && p->dock)
		p->dock->removeDocklet(this);
}
bool QIrDocklet::isActive() const
{
	QIR_P(const QIrDocklet);

	return p->active;
}
void QIrDocklet::setActive( bool a )
{
	QIR_P(QIrDocklet);
	
	if ( p->active == a )
		return;
	p->active = a;
	emit activated( a );
}
QIrAbstractDock * QIrDocklet::dock() const
{
	QIR_P(const QIrDocklet);

	return p->dock;
}
void QIrDocklet::setDock( QIrAbstractDock * dock )
{
	QIR_P(QIrDocklet);

	if ( !dock || p->dock )
		return;
	p->dock = dock;
}
void QIrDocklet::setEnabled( bool enabled )
{
	if ( QGraphicsItem * parent = parentItem() )
		parent->setEnabled(enabled);
}
void QIrDocklet::setVisible( bool visible )
{
	if ( QGraphicsItem * parent = parentItem() )
		parent->setVisible(visible);
}
void QIrDocklet::setGeometry( const QRectF & r )
{
	if ( geometry() == r )
		return;
	QGraphicsWidget::setGeometry(r);
	emit geometryChanged();	
}
QIcon QIrDocklet::icon() const
{
	QIR_P(const QIrDocklet);

	return p->icon;
}
void QIrDocklet::setIcon( const QIcon & icon )
{
	QIR_P(QIrDocklet);

	p->icon = icon;
	emit iconChanged(p->icon);
}
int QIrDocklet::iconSize() const
{	
	QIR_P(const QIrDocklet);
	int s = qMax(4,p->iconSize);
	
	if ( !p->dock )
		return s;
	if ( s < p->dock->effectiveMinimumIconSize() )
		return p->dock->effectiveMinimumIconSize();
	if ( s > p->dock->maximumIconSize() )
		return p->dock->maximumIconSize();
	return s;
}
void QIrDocklet::setIconSize( int s )
{
	QIR_P(QIrDocklet);
	int old = iconSize(), current;

	p->iconSize = s;
	if ( (current = iconSize()) != old ) {
		emit iconSizeChanged(current);
	}
}
bool QIrDocklet::isLauncher() const
{
	QIR_P(const QIrDocklet);

	return p->isLauncher;
}
void QIrDocklet::setLauncher( bool launcher )
{
	QIR_P(QIrDocklet);

	if ( p->isLauncher == launcher )
		return;
	p->isLauncher = launcher;
	emit changed();
}
bool QIrDocklet::isSeparator() const
{
	QIR_P(const QIrDocklet);

	return p->isSeparator;
}
void QIrDocklet::setSeparator( bool sep )
{
	QIR_P(QIrDocklet);

	if ( p->isSeparator == sep )
		return;
	p->isSeparator = sep;
	if ( p->dock )
		update();
}
QString QIrDocklet::text() const
{
	QIR_P(const QIrDocklet);

	return p->text;
}
void QIrDocklet::setText( const QString & text )
{
	QIR_P(QIrDocklet);

	if ( p->text == text )
		return;
	p->text == text;
	emit textChanged(p->text);
}
void QIrDocklet::initStyleOption( QIrStyleOptionDocklet * option ) const
{
	QIR_P(const QIrDocklet);
	QGraphicsWidget::initStyleOption(option);

	p->dock->initStyleOptionDocklet(this,option);
}
void QIrDocklet::slotUpdate()
{
	update();
}
void QIrDocklet::slotTrigger()
{
	if ( isEnabled() )
		emit triggered();
}
void QIrDocklet::moveEvent( QGraphicsSceneMoveEvent * )
{
	emit moved(pos());
}
void QIrDocklet::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QIR_P(QIrDocklet);
        QIrSubStyle * style = 0;
	QIrStyleOptionDocklet dockletOption;

	if (!p->dock || (p && !(style = p->dock->subStyle())) || !option->rect.isValid())
		return;
	initStyleOption(&dockletOption);
	style->drawControl((QStyle::ControlElement)QIrDockStyle::CE_Docklet,&dockletOption,painter,widget);
}
void QIrDocklet::resizeEvent( QGraphicsSceneResizeEvent * )
{
	emit resized(size());
}
QIR_END_NAMESPACE
