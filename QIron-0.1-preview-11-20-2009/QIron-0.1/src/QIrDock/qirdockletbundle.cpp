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
#include <QGraphicsLayout>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "qirdock.h"
#include "qirdocklet.h"
#include "qirdockview.h"
#include "qirdockletbundle.h"
#include "../Common/private/qirobject_p.h"
#include "qirdockstyle.h"
#include <QtDebug>

QIR_BEGIN_NAMESPACE

//////////////////////////
//QIrDockletBundlePrivate
//////////////////////////
class QIrDockletBundlePrivate : public QIrObjectPrivate
{
	QIR_DECLARE_OBJECT(QIrDockletBundle)

public:
        QIrDockletBundlePrivate() : docklet(0), down(false), blockRefresh(false), visible(false)
		{ }
	~QIrDockletBundlePrivate() {}

	QPointer< QIrDocklet > docklet;
	QIrDockView * view;
	QRectF dockletRect,
		indicatorRect;
	QPointF dockletOffet;

	uint down : 1;
	uint blockRefresh : 1;
	bool visible;

	void click();

	void emitPressed();
    void emitReleased();
    void emitClicked();

	void refresh();

protected:
	void init();
	void setupConnections();
};

void QIrDockletBundlePrivate::init()
{
	QIR_O(QIrDockletBundle);

	o->setEnabled(docklet->isEnabled());
    o->setVisible(docklet->isVisible());
	o->setAcceptHoverEvents(true);
}
void QIrDockletBundlePrivate::setupConnections()
{
	QIR_O(QIrDockletBundle);

	o->connect(docklet,SIGNAL(activated(bool)),o,SLOT(slotUpdate()));	
}
void QIrDockletBundlePrivate::click()
{
    QIR_O(QIrDockletBundle);
	QPointer< QIrDockletBundle > guard(o);

    down = false;
	o->update();
	if ( guard )
        emitReleased();
    if (guard)
        emitClicked();
	if (docklet)
		docklet->slotTrigger();
}
void QIrDockletBundlePrivate::emitClicked()
{
    QIR_O(QIrDockletBundle);
    
    emit o->clicked();
}
void QIrDockletBundlePrivate::emitPressed()
{
    QIR_O(QIrDockletBundle);
 
    emit o->pressed();
}

void QIrDockletBundlePrivate::emitReleased()
{
     QIR_O(QIrDockletBundle);
   
    emit o->released();
}
void QIrDockletBundlePrivate::refresh()
{
	QIR_O(QIrDockletBundle);

	if ( blockRefresh )
        return;
	o->update();
}
///////////////////
//QIrDockletBundle
//////////////////
QIrDockletBundle::QIrDockletBundle(QIrDockView * view, QIrDocklet * docklet ) : QGraphicsWidget(0,0), QIrObject(* new QIrDockletBundlePrivate)
{
	QIR_P(QIrDockletBundle);

	p->view = view;
	p->docklet = docklet;
	p->docklet->setParentItem(this);
    p->setup();
	p->docklet->installEventFilter(this);	
}
QIrDockletBundle::~QIrDockletBundle()
{
	releaseDocklet();
}
void QIrDockletBundle::releaseDocklet()
{
	QIR_P(QIrDockletBundle);

	if ( p->docklet ) {
		p->docklet->removeEventFilter(this);
		if ( p->docklet->parentItem() == this ) {
			p->docklet->setParentItem(0);
		}
		p->docklet = 0;
	}
}

void QIrDockletBundle::setGeometry( const QRectF & r )
{
	if ( geometry() == r )
		return;
	QGraphicsWidget::setGeometry(r);
	emit geometryChanged();
	invalidate();
}
QIrDocklet * QIrDockletBundle::docklet() const
{
	QIR_P(const QIrDockletBundle);

	return p->docklet;
}
QPointF QIrDockletBundle::dockletOffset() const
{
	QIR_P(const QIrDockletBundle);
	
	return p->dockletOffet;
}
void QIrDockletBundle::setDockletOffset( const QPointF & offset )
{
	QIR_P(QIrDockletBundle);
		
	if ( p->dockletOffet == offset )
		return;
	p->dockletOffet = offset;
	p->docklet->setGeometry(QRectF(p->dockletRect.topLeft() + p->dockletOffet,p->dockletRect.size()));
}
bool QIrDockletBundle::isInternallyVisible() const
{
	return p_func()->visible;
}
bool QIrDockletBundle::event( QEvent * evt )
{
	switch ( evt->type() ) {
		case QEvent::Show:
		case QEvent::Hide: {
				QIR_P(QIrDockletBundle);

				p->visible = (evt->type() == QEvent::Show);
				if ( p->view ) {
					p->view->emitResetViewRequested();
				}
			}
			break;
		default:
			break;
	}
	return QGraphicsWidget::event(evt);
}
bool QIrDockletBundle::isDown() const
{
	QIR_P(const QIrDockletBundle);

	return p->down;
}
void QIrDockletBundle::setDown( bool down )
{
	QIR_P(QIrDockletBundle);

    if (p->down == down)
        return;
    p->down = down;
	if ( down )
    p->refresh();
}
void QIrDockletBundle::invalidate()
{
	QIR_P(QIrDockletBundle);
	QIrStyleOptionDockletBundle option;
	QIrSubStyle * style;
	int spacing = p->view->dockletSpacing() / 2;
	QIrDock * dock;

	if (!p->view || !p->docklet)
		return;
	dock = p->view->dock();
	style = dock->subStyle();
	initStyleOption(&option);
	if ( dock->orientation() == Qt::Horizontal )
		option.rect.adjust(spacing,0,-spacing,0);
	else
		option.rect.adjust(0,spacing,0,-spacing);
	p->dockletRect =  style->subElementRect((QStyle::SubElement)QIrDockStyle::SE_DockletBundleContentsRect,&option);
	p->indicatorRect = style->subElementRect((QStyle::SubElement)QIrDockStyle::SE_DockletBundleIndicator,&option);
	if ( p->docklet->isSeparator() )
		p->dockletRect |= p->indicatorRect;
	p->docklet->setGeometry(QRectF(p->dockletRect.topLeft() + p->dockletOffet,p->dockletRect.size()));
}
void QIrDockletBundle::initStyleOption( QIrStyleOptionDockletBundle * option ) const
{
	QIR_P(const QIrDockletBundle);
	QIrDock * dock = p->view->dock();
	QGraphicsWidget::initStyleOption(option);

	dock->initStyleOptionDocklet(p->docklet,option);
}
void QIrDockletBundle::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget * widget)
{
	QIR_P(const QIrDockletBundle);
	QIrStyleOptionDockletBundle opt;
	QIrSubStyle * style;

	if (!p->view || !p->docklet || !(p->docklet && !p->docklet->isSeparator() && p->docklet->isActive()))
		return;
	style = p->view->dock()->subStyle();
	initStyleOption(&opt);
	opt.rect = p->indicatorRect.toRect();
	style->drawPrimitive((QStyle::PrimitiveElement)QIrDockStyle::PE_DockletIndicator,&opt,painter,widget);
	
}
bool QIrDockletBundle::hitBundle( const QPointF & pos ) const
{
	return boundingRect().contains(pos);
}
void QIrDockletBundle::hoverEnterEvent( QGraphicsSceneHoverEvent * evt )
{
	QGraphicsWidget::hoverEnterEvent(evt);
	emit entered();
}
void QIrDockletBundle::hoverLeaveEvent( QGraphicsSceneHoverEvent * evt )
{
	QIR_P(QIrDockletBundle);

	QGraphicsWidget::hoverLeaveEvent(evt);
	emit left();
	if ( p->down ) {
		setDown(false);
		emit released();
	}
}
void QIrDockletBundle::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * evt )
{
	QGraphicsWidget::mouseDoubleClickEvent(evt);
	emit doubleClicked();
}
void QIrDockletBundle::mousePressEvent( QGraphicsSceneMouseEvent * evt )
{
	QIR_P(QIrDockletBundle);

	QGraphicsWidget::mousePressEvent(evt);

	if ( !p->docklet || (p->docklet && p->docklet->isSeparator()) )
			return;
	if ( evt->button() != Qt::LeftButton ) {
		evt->ignore();
        return;
    }
    if ( hitBundle(evt->pos()) ) {
        setDown(true);
		update(); 
        p->emitPressed();
        evt->accept();
    } else {
        evt->ignore();
    }
}
void QIrDockletBundle::mouseReleaseEvent( QGraphicsSceneMouseEvent * evt )
{
	QIR_P(QIrDockletBundle);

	QGraphicsWidget::mouseReleaseEvent(evt);

    if ( evt->button() != Qt::LeftButton ) {
        evt->ignore();
        return;
    }
    if ( !p->down ) {
        evt->ignore();
        return;
    }
    if ( hitBundle(evt->pos()) ) {
        p->click();
        evt->accept();
    } else {
        setDown(false);
        evt->ignore();
    }
}
void QIrDockletBundle::mouseMoveEvent( QGraphicsSceneMouseEvent * evt )
{
   QIR_P(QIrDockletBundle);

   QGraphicsWidget::mouseMoveEvent(evt);

    if (!(evt->buttons() & Qt::LeftButton)) {
        evt->ignore();
        return;
    }
    if ( hitBundle(evt->pos()) != p->down ) {
        setDown(!p->down);
        if ( p->down )
            p->emitPressed();
        else
            p->emitReleased();
        evt->accept();
    } else if ( !hitBundle(evt->pos()) ) {
        evt->ignore();
    }
}
QIR_END_NAMESPACE
