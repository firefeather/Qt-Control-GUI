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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QIron API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
#include <QVector>
#include <QMap>
#include <QPropertyAnimation>
#include <QtDebug>
#include <QParallelAnimationGroup>
#include "qirparaboliceffectmanager_p.h"
#include "../qirdocklet.h"
#include "../qirdockview.h"
#include "../qirdock.h"
#include "../qirdockstyle.h"
#include "../qirdockletbundle.h"

QIR_BEGIN_NAMESPACE


void QIrParabolicEffectManagerPrivate::init()
{
	QIR_O(QIrParabolicEffectManager);

	bundleAnimationGroup = new QParallelAnimationGroup(o);
	o->connect(bundleAnimationGroup,SIGNAL(finished()),o,SLOT(slotHandleAnimationFinished()));
	o->connect(bundleAnimationGroup,SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)),o,SLOT(slotHandleAnimationStateChanged()));
}
void QIrParabolicEffectManagerPrivate::animate()
{	
	if ( bundleAnimationGroup->state() == QAbstractAnimation::Running ) {
		isAnimationInterrupted = true;
		bundleAnimationGroup->stop();
	}
	isTransiting = true;
	updateAnimationValues(true);
	mustStartAnimation = false;
	if ( hover )
		isExtending = true;
	else
		isExtending = false;
	bundleAnimationGroup->start();	
}
qreal QIrParabolicEffectManagerPrivate::calculateCoverage() const
{
	int spacing = view->dockletSpacing();

	return (((2 * coverage  + 1) / 2.0) * (view->dock()->effectiveMinimumIconSize() + spacing + 1));
}
void QIrParabolicEffectManagerPrivate::evaluatePos( const QPoint & pos )
{
	updateHoverDocklet(view->mapToScene(pos));
	updateDockletIconSizes(view->mapToScene(pos));
}
void QIrParabolicEffectManagerPrivate::restoreSizes()
{
	int min = view->dock()->effectiveMinimumIconSize();

	foreach (QIrDockletBundle * bundle, view->bundles()) {
		bundle->docklet()->setIconSize(min);
	}
}
void QIrParabolicEffectManagerPrivate::updateMinDockletRects()
{
	QIrDock * dock = view->dock();
	QList< QIrDockletBundle * > list = view->bundles();
	QIrDockletBundle * docklet;
	QPointF startPoint(0,0);
	int count = list.count();
	QSize minDockletSize, minSepSize;
	QIrDockStyle * style = dock->dockStyle();
	int spacing = view->dockletSpacing();

	minDockletRects.clear();
	minDockletRects.resize(count);
	if ( !style )
		return;

	minDockletSize = QIrParabolicEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),
		dock->effectiveMinimumIconSize(),dock->maximumIconSize(),false,dock->orientation());
	minSepSize = QIrParabolicEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),
		dock->effectiveMinimumIconSize(),
		dock->maximumIconSize(),true,dock->orientation());
	
	switch ( dock->dockArea() ) {
		case QIrAbstractDock::DA_BottomDockArea:
			startPoint.ry() = minDockletSize.height();
			break;
		case QIrAbstractDock::DA_RightDockArea:
			startPoint.rx() = minDockletSize.width();
			break;
		default:
			break;
	}
	for (int i = 0; i < count; i++) {
		QRectF geom;
		
		docklet = list[i];
		geom.setSize(docklet->docklet()->isSeparator() ? minSepSize : minDockletSize);
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
		minDockletRects[i] = geom;
	}
}
void QIrParabolicEffectManagerPrivate::updateDockletRects()
{
	QIR_O(QIrParabolicEffectManager);
	QIrDock * dock = view->dock();
	QList< QIrDockletBundle * > list = view->bundles();
	QIrDockletBundle * docklet;
	QPointF startPoint(0,0);
	int count = list.count();
	QSize minDockletSize, minSepSize;
	QIrDockStyle * style = dock->dockStyle();
	int spacing = view->dockletSpacing();
	bool noEffect = (dock->effectiveMinimumIconSize() == dock->maximumIconSize());

	if ( !style ) {
		dockletRects.clear();
		dockletRects.resize(count);
		return;
	}
	if ( count == 0 )
		return;
	minDockletSize = QIrParabolicEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->effectiveMinimumIconSize(),dock->maximumIconSize(),false,dock->orientation());
	minSepSize = QIrParabolicEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),dock->effectiveMinimumIconSize(),
		dock->maximumIconSize(),true,dock->orientation());

	if (!isHoverMode || !hover || noEffect) {
		dockletRects.clear();
		dockletRects.resize(count);
		for (int i = 0; i < count; i++) {
			dockletRects[i] = minDockletRects[i];
		}
	} else {
		QRectF geom;
		int hoverIndex = list.indexOf(hover);
		int reach = o->calculateReach();
		QRectF hoverRect = dockletRects[hoverIndex],
			minHoverRect = minDockletRects[hoverIndex];
		//if ( hoverRect.x() < -200 )

		dockletRects.clear();
		dockletRects.resize(count);

		dockletRects[hoverIndex] = hoverRect; 
		//Set docklet sizes.
		for (int i = 0; i < count; i++) {
			docklet = list[i];
			if ( i != hoverIndex )
				dockletRects[i].setSize(QIrParabolicEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),docklet->docklet()->iconSize(),dock->maximumIconSize(),docklet->docklet()->isSeparator(),dock->orientation()));
			switch ( dock->dockArea() ) {
				case QIrAbstractDock::DA_BottomDockArea:
					dockletRects[i].moveBottom(minDockletRects[i].bottom());
				break;
			case QIrAbstractDock::DA_TopDockArea:
				dockletRects[i].moveTop(minDockletRects[i].top());
				break; 			
			case QIrAbstractDock::DA_LeftDockArea:			
				dockletRects[i].moveLeft(minDockletRects[i].left());
				break; 			
			default:
				dockletRects[i].moveRight(minDockletRects[i].right());
				break;
			}
		}
		// Set docklet positions.
		if ( dock->orientation() == Qt::Horizontal ) {
			qreal right = hoverRect.left() - 1;
			///Start with the left changed docklets.
			for (int i = hoverIndex - 1; i > lastUnchangedLeftIndex && i >= 0; i--) {
				dockletRects[i].moveRight(right);
				right = dockletRects[i].left() - 1;
			}
			if ( lastUnchangedLeftIndex >= 0 ) {
				//Move the last left unchanged docklet's position a reach away from the hover docklet's position.
				dockletRects[lastUnchangedLeftIndex].moveRight(minHoverRect.center().x() - reach);
				right = dockletRects[lastUnchangedLeftIndex].left() - 1;
				
				//Move the other unchanged docklet.
				for (int i = lastUnchangedLeftIndex - 1; i >= 0; i--) {
					dockletRects[i].moveRight(right);
					right = dockletRects[i].left() - 1;
				}
			}
			
			qreal left = hoverRect.right() + 1;
			///Update the left changed docklets.
			for (int i = hoverIndex + 1; i < firstUnchangedRightIndex && i < count; i++) {
				dockletRects[i].moveLeft(left);
				left = dockletRects[i].right() + 1;
			}
			if ( firstUnchangedRightIndex < count ) {
				dockletRects[firstUnchangedRightIndex].moveLeft(minHoverRect.center().x() + reach);
				left = dockletRects[firstUnchangedRightIndex].right() + 1;
				for (int i = firstUnchangedRightIndex + 1; i < count; i++) {
					dockletRects[i].moveLeft(left);
					left = dockletRects[i].right() + 1;
				}
			}
		} else {
			qreal bottom = hoverRect.top() - 1;
			///Start with the top changed docklets.
			for (int i = hoverIndex - 1; i > lastUnchangedLeftIndex && i >= 0; i--) {
				dockletRects[i].moveBottom(bottom);
				bottom = dockletRects[i].top() - 1;
			}
			if ( lastUnchangedLeftIndex >= 0 ) {
				//Move the last top unchanged docklet's position a reach away from the hover docklet's position.
				dockletRects[lastUnchangedLeftIndex].moveBottom(minHoverRect.center().y() - reach);
				bottom = dockletRects[lastUnchangedLeftIndex].top() - 1;
				
				//Move the other unchanged docklet.
				for (int i = lastUnchangedLeftIndex - 1; i >= 0; i--) {
					dockletRects[i].moveBottom(bottom);
					bottom = dockletRects[i].top() - 1;
				}
			}
			qreal top = hoverRect.bottom() + 1;
			///Update the bottom changed docklets.
			for (int i = hoverIndex + 1; i < firstUnchangedRightIndex && i < count; i++) {
				dockletRects[i].moveTop(top);
				top = dockletRects[i].bottom() + 1;
			}
			if ( firstUnchangedRightIndex < count ) {
				dockletRects[firstUnchangedRightIndex].moveTop(minHoverRect.center().y() + reach);
				top = dockletRects[firstUnchangedRightIndex].bottom() + 1;
				for (int i = firstUnchangedRightIndex + 1; i < count; i++) {
					dockletRects[i].moveTop(top);
					top = dockletRects[i].bottom() + 1;
				}
			}
		}
	}
	if ( mustStartAnimation )
		animate();		
}
void QIrParabolicEffectManagerPrivate::updateAnimationValues( bool updateStartValue )
{
	QRectF geom;
	QIrDockletBundle * bundle;
	QPropertyAnimation * anim;
	QList< QIrDockletBundle * > list = view->bundles();

	for (int i = 0; i < list.count(); i++) {
		bundle = list[i];
		anim = geomAnimations[bundle];
		if ( updateStartValue )
			anim->setStartValue(bundle->geometry());
		anim->setEndValue(dockletRects[i]);
	}
}
void QIrParabolicEffectManagerPrivate::updateDockletIconSizes( const QPointF & p )
{
	QIR_O(QIrParabolicEffectManager);
	QIrDock * dock = view->dock();
	QIrDockletBundle * bundle;
	qreal x = p.x(), y = p.y();
	QList< QIrDockletBundle * > list = view->bundles();
	int i, count = list.count();
	qreal deltaX, deltaY;
	qreal coverage = calculateCoverage();
	QIrDockStyle * style = dock->dockStyle();
	int spacing = view->dockletSpacing();
	bool noEffect = (dock->effectiveMinimumIconSize() == dock->maximumIconSize()); 


	firstUnchangedRightIndex = count;
	lastUnchangedLeftIndex = -1;

	if ( !style )
		return;
	updateMinDockletRects();
	dockletRects.clear();
	dockletRects.resize(count);
	
	if ( dock->orientation() == Qt::Horizontal ) {
        //Determine the size of each docklets
        for (i = 0; i < count; i++) {
			QPointF center = minDockletRects[i].center();
			bundle = list[i];
			deltaX = qAbs(center.x() - x);
			if (deltaX <= coverage && hover) {
                                bundle->docklet()->setIconSize(o->calculateIconSize((int)deltaX));
			} else {
				bundle->docklet()->setIconSize(dock->effectiveMinimumIconSize());
				if (center.x() > x && firstUnchangedRightIndex == count)
					firstUnchangedRightIndex = i;
				else if ( center.x() < x )
						lastUnchangedLeftIndex = i;
			}
			//Move the hover docklet.
			if ( bundle == hover && !noEffect ) {
				qreal alpha;
				QPointF origin;
				QRectF hoverRect, minRect = minDockletRects[i];
	
				hoverRect.setSize(QIrParabolicEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),bundle->docklet()->iconSize(),
					dock->maximumIconSize(),bundle->docklet()->isSeparator(),dock->orientation()));


				alpha = (hoverRect.width() - minRect.width()) / minRect.width();
				origin = minRect.center() + QPointF(minRect.width() / 2,0);
				hoverRect.moveCenter(minRect.center());
				hoverRect.moveRight((int)(origin.x() +  alpha * (origin.x() - x)));
				dockletRects[i] = hoverRect;
			}
		}
	} else {
		//Determine the size of each docklets
		for (i = 0; i < count; i++) {
			QPointF center = minDockletRects[i].center();
			bundle = list[i];
			deltaY = qAbs(center.y() - y);
			if (deltaY <= coverage && hover) {
                                bundle->docklet()->setIconSize(o->calculateIconSize((int)deltaY));
			} else {
				bundle->docklet()->setIconSize(dock->effectiveMinimumIconSize());
				if (center.y() > y && firstUnchangedRightIndex == count)
					firstUnchangedRightIndex = i;
				else if ( center.y() < y )
						lastUnchangedLeftIndex = i;
			}
			//Move the hover docklet.
			if ( bundle == hover && !noEffect ) {
				qreal alpha;
				QPointF origin;
				QRectF hoverRect, minRect = minDockletRects[i];
	
				hoverRect.setSize(QIrParabolicEffectManager::sizeForIconSize(style,spacing,dock->effectiveMinimumIconSize(),bundle->docklet()->iconSize(),
					dock->maximumIconSize(),bundle->docklet()->isSeparator(),dock->orientation()));
				alpha = (hoverRect.height() - minRect.height()) / minRect.height();
				origin = minRect.center() + QPointF(0,minRect.height() / 2);
				hoverRect.moveCenter(minRect.center());
				hoverRect.moveBottom((int)(origin.y() + alpha * (origin.y() - y)));
				dockletRects[i] = hoverRect;
			}
		
		}
	}
	updateDockletRects();
	o->doLayout();
}
void QIrParabolicEffectManagerPrivate::updateHoverDocklet( const QPointF & pos )
{
	QIR_O(QIrParabolicEffectManager);
	QIrDock * dock = view->dock();
	QIrDockletBundle * oldHover = hover, * bundle;
	QList< QIrDockletBundle * > list = view->bundles();
	QRectF minRect;

	for(int i = 0; i < minDockletRects.count(); i++) {
		bundle = list[i];
		minRect = minDockletRects[i];
		if ( dock->orientation() == Qt::Horizontal ) {
			if (minRect.x() <= pos.x() && (minRect.x() + minRect.width()) >= pos.x()) {
					if ( hover != bundle ) {
						hover = bundle;
						QIrDocklet * old = oldHover ? oldHover->docklet() : 0;

						o->changeHoverDocklet(hover->docklet(),old);
					}
					//this->useAnimation = false;
					isHoverMode = true;
					return;
			}
		} else {
			if (minRect.y() <= pos.y() && (minRect.y() + minRect.height()) >= pos.y()) {
				if ( hover != bundle ) {
					hover = bundle;
					QIrDocklet * old = oldHover ? oldHover->docklet() : 0;

					o->changeHoverDocklet(hover->docklet(),old);
				}
				isHoverMode = true;
				return;
			}		
		}
	}
	bundle = hover;
	isHoverMode = false;
	hover = 0;
	if ( bundle != hover )
		o->changeHoverDocklet(0,bundle->docklet());
}

QIR_END_NAMESPACE
