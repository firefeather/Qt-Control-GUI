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
#include "qirdock.h"
#include "qirdocklet.h"
#include "qirdockletbundle.h"
#include "qirdockview.h"
#include "qirdockstyle.h"
#include "../Common/qirboolblocker.h"
#include "private/qirdockeffectmanager_p.h"

QIR_BEGIN_NAMESPACE

//////////////////////////////
//QIrDockEffectManagerPrivate
//////////////////////////////
QIrDockEffectManagerPrivate::QIrDockEffectManagerPrivate() : view(0), useAnimation(true)
{
}
QIrDockEffectManagerPrivate::~QIrDockEffectManagerPrivate()
{
}
void QIrDockEffectManagerPrivate::setDockView( QIrDockView * v )
{
	if ( view == v )
		return;
	if ( view )
		o_func()->unpolish();
	view = v;
	if ( view )
		o_func()->polish();
}

///////////////////////
//QIrDockEffectManager
///////////////////////
QIrDockEffectManager::QIrDockEffectManager( QObject * parent )
: QObject(parent), QIrObject(* new QIrDockEffectManagerPrivate)
{
	QIR_P(QIrDockEffectManager);

	p->setup();
}
QIrDockEffectManager::QIrDockEffectManager(QObject * parent, QIrDockEffectManagerPrivate & p)
: QObject(parent), QIrObject(p)
{
}
QIrDockEffectManager::~QIrDockEffectManager()
{
}
QIrDockView * QIrDockEffectManager::view() const
{
	QIR_P(const QIrDockEffectManager);

	return p->view;
}
void QIrDockEffectManager::hoverDockletChanged(QIrDocklet * current, QIrDocklet * previous)
{
	QIR_P(QIrDockEffectManager);

	p->view->hoverDockletChanged(current,previous);	
}
void QIrDockEffectManager::polish()
{
	QIR_P(QIrDockEffectManager);

	foreach ( QIrDockletBundle * bundle, p->view->bundles(false) ) {
		polish(bundle);
	}
}
void QIrDockEffectManager::unpolish()
{
	QIR_P(QIrDockEffectManager);

	foreach ( QIrDockletBundle * bundle, p->view->bundles(false) ) {
		unpolish(bundle);
	}
}
void QIrDockEffectManager::polish( QIrDockletBundle * )
{
}
void QIrDockEffectManager::unpolish( QIrDockletBundle * )
{
}
QSize QIrDockEffectManager::sizeForIconSize(QIrDockStyle * style, int spacing, int minimumIconSize, int iconSize, int maximumIconSize, bool isSeparator, Qt::Orientation orientation)
{
	QSize sz = style->sizeForIconSize(minimumIconSize,iconSize,maximumIconSize,isSeparator,orientation);

	if ( orientation == Qt::Horizontal )
		sz.rwidth() += spacing;
	else
		sz.rheight() += spacing;
	return sz;
}
bool QIrDockEffectManager::useAnimation() const
{
	return p_func()->useAnimation;
}
void QIrDockEffectManager::setUseAnimation( bool use )
{
	p_func()->useAnimation = use;
}

/////////////////////////////////
//QIrDefaultEffectManagerPrivate
////////////////////////////////
QIrDefaultEffectManagerPrivate::QIrDefaultEffectManagerPrivate() :  restoring(false), doingLayout(false), hover(0)
{
}
QIrDefaultEffectManagerPrivate::~QIrDefaultEffectManagerPrivate()
{
}
void QIrDefaultEffectManagerPrivate::evaluatePos( const QPoint & pos )
{
	QIR_O(QIrDefaultEffectManager);

	updateHoverDocklet(view->mapToScene(pos));	
	o->doLayout();
}
void QIrDefaultEffectManagerPrivate::updateDockletRects()
{
	QIrDock * dock = view->dock();
	QList< QIrDockletBundle * > list = view->bundles();
	QIrDockletBundle * bundle;
	QPointF startPoint(0,0);
	int count = list.count();
	QSize dockletRect, minSepSize;
	QIrDockStyle * style = dock->dockStyle();
	int spacing = view->dockletSpacing();

	
	dockletRects.clear();
	dockletRects.resize(count);
	if ( !style )
		return;

	dockletRect = QIrDockEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),
		dock->effectiveMinimumIconSize(),dock->maximumIconSize(),false,dock->orientation());
	minSepSize = QIrDockEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),
		dock->effectiveMinimumIconSize(),
		dock->maximumIconSize(),true,dock->orientation());
	
	switch ( dock->dockArea() ) {
		case QIrAbstractDock::DA_BottomDockArea:
			startPoint.ry() = dockletRect.height();
			break;
		case QIrAbstractDock::DA_RightDockArea:
			startPoint.rx() = dockletRect.width();
			break;
		default:
			break;
	}
	for (int i = 0; i < count; i++) {
		QRectF geom;
		
		bundle = list[i];
		geom.setSize(bundle->docklet()->isSeparator() ? minSepSize : dockletRect);
		switch ( dock->dockArea() ) {
			case QIrAbstractDock::DA_BottomDockArea:
				geom.moveBottomLeft(startPoint);
				startPoint.rx() = geom.right() + 1;
				break;
			case QIrAbstractDock::DA_TopDockArea:
				geom.moveTopLeft(startPoint);
				startPoint.rx() = geom.right() + 1;
				break; 			
			case QIrAbstractDock::DA_LeftDockArea:			
				geom.moveTopLeft(startPoint);
				startPoint.ry() = geom.bottom() + 1;
				break; 			
			default:
				geom.moveTopRight(startPoint);
				startPoint.ry() = geom.bottom() + 1;
				break;
		}
		dockletRects[i] = geom;
	}
}
void QIrDefaultEffectManagerPrivate::updateHoverDocklet( const QPointF & pos )
{
	QIR_O(QIrDefaultEffectManager);
	QIrDock * dock = view->dock();
	QIrDockletBundle * oldHover = hover, * bundle;
	QList< QIrDockletBundle * > list = view->bundles();
	QRectF rect;

	for(int i = 0; i < dockletRects.count(); i++) {
		bundle = list[i];
		rect = dockletRects[i];
		if ( dock->orientation() == Qt::Horizontal ) {
			if (rect.x() <= pos.x() && (rect.x() + rect.width()) >= pos.x()) {
					if ( hover != bundle ) {
						hover = bundle;
						QIrDocklet * old = oldHover ? oldHover->docklet() : 0;

						o->changeHoverDocklet(hover->docklet(),old);
					}					
					return;
			}
		} else {
			if (rect.y() <= pos.y() && (rect.y() + rect.height()) >= pos.y()) {
				if ( hover != bundle ) {
					hover = bundle;
					QIrDocklet * old = oldHover ? oldHover->docklet() : 0;

					o->changeHoverDocklet(hover->docklet(),old);
				}
				return;
			}		
		}
	}
	bundle = hover;
	hover = 0;
	if ( bundle != hover )
		o->changeHoverDocklet(0,bundle->docklet());
}


//////////////////////////
//QIrDefaultEffectManager
//////////////////////////
QIrDefaultEffectManager::QIrDefaultEffectManager( QObject * parent ) : QIrDockEffectManager(parent,* new QIrDefaultEffectManagerPrivate)
{
	QIR_P(QIrDefaultEffectManager);

	p->setup();
}
QIrDefaultEffectManager::QIrDefaultEffectManager(QObject * parent, QIrDefaultEffectManagerPrivate & p) : QIrDockEffectManager(parent,p)
{
}
QIrDefaultEffectManager::~QIrDefaultEffectManager()
{
}
void QIrDefaultEffectManager::changeHoverDocklet(QIrDocklet * current, QIrDocklet * previous)
{
	QIR_P(QIrDefaultEffectManager);

	if ( p->view->dock()->showTipMode() != QIrDock::SpontaneousShowTip )
		p->view->dock()->setShowTipMode(QIrDock::SpontaneousShowTip);
	hoverDockletChanged(current,previous);	
}
QRectF QIrDefaultEffectManager::contentRect() const
{
	QIR_P(const QIrDefaultEffectManager);
	QRectF rect;

	foreach (QIrDockletBundle * docklet, p->view->bundles()) {		
		rect |= docklet->geometry();
	}
	return rect;
}
void QIrDefaultEffectManager::clearHover()
{
	QIR_P(QIrDefaultEffectManager);

	if ( p->hover ) {
		changeHoverDocklet(0,p->hover->docklet());
		p->hover = 0;
	}
}
QRectF QIrDefaultEffectManager::normalContentRect() const
{
	QIR_P(const QIrDefaultEffectManager);
	QRectF rect;

	const_cast< QIrDefaultEffectManagerPrivate * >(p)->updateDockletRects();

	foreach (QRectF r, p->dockletRects)
		rect |= r;
	return rect;
}
QRectF QIrDefaultEffectManager::extendContentRect() const
{
	QIR_P(const QIrDefaultEffectManager);
	QIrDock * dock = p->view->dock();
	QIrDockStyle * style = dock->dockStyle();
	int spacing = p->view->dockletSpacing();
	QSize sz;
	int ret;

	if ( !style )
		return QRectF();

	sz = sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->effectiveMinimumIconSize(),dock->maximumIconSize(),false,dock->orientation());
	ret = (dock->orientation() == Qt::Horizontal ? sz.height() : sz.width());
	
	return QRectF(QPoint(),QSize(ret,ret));
}

void QIrDefaultEffectManager::invalidate()
{
	QIR_P(QIrDefaultEffectManager);

	p->updateDockletRects();
	doLayout();	
}
QSize QIrDefaultEffectManager::sizeHint() const
{
	QIR_P(const QIrDefaultEffectManager);
	QIrDockView * view = p->view;
	QList< QIrDockletBundle * > list = view->bundles();
	QIrDock * dock = view->dock();
	QRectF bounding;
	QRectF geom;
	int sepCount = 0, count = list.count();
	QIrDockStyle * style = dock->dockStyle();
	int spacing = p->view->dockletSpacing();

	if ( !style )
		return QSize();

	bounding = contentRect();
	for (int i = 0; i < count; i++) {
		if ( list[i]->docklet()->isSeparator() )
			sepCount++;
	}
	QSize s = bounding.size().toSize();
	
	if ( !dock->isExtended() ) {
		QSize dockletRects = sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->minimumIconSize(),dock->maximumIconSize(),false,dock->orientation());
		QSize minSepSize = sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->minimumIconSize(),
			dock->maximumIconSize(),true,dock->orientation());

		if ( dock->orientation() == Qt::Horizontal ) {
			s.rwidth() = (count - sepCount) * (dockletRects.width() + 1) + sepCount * (minSepSize.width() + 1) - 1;
		} else {
			s.rheight() = (count - sepCount) * (dockletRects.height() + 1) + sepCount * (minSepSize.height() + 1) - 1;
		}
	}	
	return s;
}
void QIrDefaultEffectManager::enterEvent( QEvent * )
{
}
void QIrDefaultEffectManager::leaveEvent( QEvent * )
{
	QIR_P(QIrDefaultEffectManager);

	p->evaluatePos(QPoint(-10000,-10000));
}
void QIrDefaultEffectManager::mouseMoveEvent( QMouseEvent * evt )
{
	QIR_P(QIrDefaultEffectManager);

	p->evaluatePos(evt->pos());
}
void QIrDefaultEffectManager::mousePressEvent( QMouseEvent * )
{
}
void QIrDefaultEffectManager::mouseReleaseEvent( QMouseEvent * )
{
}
void QIrDefaultEffectManager::reset()
{
	QIR_P(QIrDefaultEffectManager);
	QIR_NO_RECURSIVE(p->restoring,(void)0);

	if ( p->hover && p->hover->docklet() ) {
		changeHoverDocklet(0,p->hover->docklet());
		p->hover = 0;
	}
	invalidate();
}
void QIrDefaultEffectManager::doLayout()
{
	QIR_P(QIrDefaultEffectManager);
	QList< QIrDockletBundle * > list = view()->bundles();
	int count = list.count();
	
	for (int i = 0; i < count; i++) {
		list[i]->setGeometry(p->dockletRects[i]);
	}
	emit updateGeometryRequested();
	p->view->adjustLayers();
}
int QIrDefaultEffectManager::closestLegalIconSize( const QSize & s ) const
{
	QIR_P(const QIrDefaultEffectManager);
	int spacing = p->view->dockletSpacing();
	int c = p->view->bundles().count();
	QIrDock * dock = p->view->dock();
	QIrDockStyle * style = dock->dockStyle();

	if ( !style || c == 0 )
		return 8;
	if ( dock->orientation() == Qt::Horizontal )
		return style->iconSizeForSize(QSize(s.width() / c - spacing,s.height()),false,Qt::Horizontal);
	else
		return style->iconSizeForSize(QSize(s.width(), s.height() / c - spacing),false,Qt::Vertical);
}
QIR_END_NAMESPACE
