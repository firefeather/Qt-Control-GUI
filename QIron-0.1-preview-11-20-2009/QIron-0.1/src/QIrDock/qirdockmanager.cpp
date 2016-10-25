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
#include <QApplication>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QFontMetrics>
#include <QHoverEvent>
#include "qirdockdragguide.h"
#include "private/qirdockmanager_p.h"
#include "qirdockstyle.h"
#include "../Common/qirboolblocker.h"

QIR_BEGIN_NAMESPACE

#define SHOW_ANIMATION_DURATION 350
#define HIDE_ANIMATION_DURATION 500

/////////////////////
//QIrDockDragOutline
/////////////////////
QIrDockDragOutline::QIrDockDragOutline(QIrDockManager * manager, QWidget * parent) : QWidget(parent), m_manager(manager)

{
	setAttribute(Qt::WA_NoChildEventsForParent);
}
QIrDockDragOutline::~QIrDockDragOutline()
{
}
QString QIrDockDragOutline::widgetGroupKey() const 
{
	return qiron_dock_drag_elements;
}
void QIrDockDragOutline::paintEvent( QPaintEvent * )
{
	QIrSubStyle * style = m_manager->dock()->subStyle();
	QStyleOption option;
	QPainter painter(this);

	option.initFrom(this);
	style->drawPrimitive((QStyle::PrimitiveElement)QIrDockStyle::PE_FrameDockDragOutline,&option,&painter,this);
}


///////////////////////
//QIrDockManagerPrivate
///////////////////////
QIrDockManagerPrivate::QIrDockManagerPrivate() : alreadyShown(false), isInSensitiveRegion(false),
 managedWindowIsActive(false), quiet(false),toShow(false),  called(false),
 showBlocked(false), hideBlocked(false), callShow(false),
 animOffset(-1), dock(0), managed(0),
dragOutline(0), guideManager(0),
 showAnimation(0), showTimer(0), quietTimer(0),
lockMouseEvent(false), lockUpdateGeometry(false)
{	
}
QIrDockManagerPrivate::~QIrDockManagerPrivate()
{
}
void QIrDockManagerPrivate::init()
{
	QIR_O(QIrDockManager);

	guideManager = new QIrDockDragGuideManager(o);
	dragOutline = new QIrDockDragOutline(o);
	dragOutline->hide();
	
	quietTimer = new QTimer(o);
	quietTimer->setSingleShot(true);
	quietTimer->setInterval(o->dock()->quietDelay());

	showTimer = new QTimer(o);
	showTimer->setSingleShot(true);
	showTimer->setInterval(o->dock()->autoHideDelay());

	showAnimation = new QPropertyAnimation(o,"animatedOffset",o);
	showAnimation->setEasingCurve(QEasingCurve::OutCubic);
	
	qApp->installEventFilter(o);
	
	if ( dock->autoHide() )
		dock->hide();
}
void QIrDockManagerPrivate::setupConnections()
{
	QIR_O(QIrDockManager);

	dock->installEventFilter(guideManager);
	o->connect(showTimer,SIGNAL(timeout()),o,SLOT(slotHandleShowTimeout()));
	o->connect(quietTimer,SIGNAL(timeout()),o,SLOT(slotLeaveQuietTime()));
	o->connect(showAnimation,SIGNAL(finished()),o,SLOT(slotHandleAnimationFinished()));
}
void QIrDockManagerPrivate::evaluatePos( const QPoint & pos )
{
	QIR_O(QIrDockManager);
	QRect sr = o->sensitiveRegion();

	if ( !sr.isValid() )
		return;

	sr.moveTo(managed->mapToGlobal(sr.topLeft()));

	if (!dock->autoHide() || dock->dockState() == QIrAbstractDock::DS_MenuPoppedUp)
		return;
	if (sr.contains(pos) && !isInSensitiveRegion) {
		
		if (dock->dockState() == QIrAbstractDock::DS_Hidden && !quiet && !showBlocked) {
			toShow = true;
			showTimer->start();
		} else if ( dock->dockState() == QIrAbstractDock::DS_Hiding ) {
			o->slotShowDock(true);			
		} else {
			if ( showTimer->isActive() )
				showTimer->stop();
		}
		isInSensitiveRegion = true;
	} else if (!sr.contains(pos) && isInSensitiveRegion) {
		if ( showTimer->isActive() )
			showTimer->stop();
		if ( !(dock->dockState() == QIrAbstractDock::DS_Hidden || 
			dock->dockState() == QIrAbstractDock::DS_Dragging || 
			dock->dockState() == QIrAbstractDock::DS_Dropping) && !hideBlocked) {
				toShow = false;
				showTimer->start();
		}
		isInSensitiveRegion = false;
	}
}
QSize QIrDockManagerPrivate::smartDockSize( bool ignore ) const
{
	if ( !managed )
		return QSize();

	QSize realSize, sizeHint, maxSize;
	QRect r = managed->rect();
	QIrAbstractDock::DockArea area = dock->dockArea();
	int hOffset = 0, wOffset = 0, offset = dock->dockOffset();
	bool extended = dock->isExtended();

	sizeHint = ignore ? dock->normalSize() : dock->sizeHint();
	
	
	switch ( area ) {
		case QIrAbstractDock::DA_TopDockArea:
		case QIrAbstractDock::DA_BottomDockArea:
			maxSize.rwidth() = extended  ? QWIDGETSIZE_MAX : dock->maximumDockWidth();
			maxSize.rheight() = QWIDGETSIZE_MAX;
			hOffset = 2 * offset;
			wOffset = offset;
			break;
		default:
                        maxSize.rheight() = extended  ? QWIDGETSIZE_MAX : (int)(dock->maximumDockWidthF() * r.height());
			maxSize.rwidth() = QWIDGETSIZE_MAX;
			hOffset = offset;
			wOffset = 2 * offset;
			break;
	}	
	realSize.rwidth() = qMin(sizeHint.width(),maxSize.width() + wOffset); 
	realSize.rheight() = qMin(sizeHint.height(),maxSize.height() + hOffset);
	
	if ( dock->isExtended() )
		return realSize;
	else 
		return dock->closestAcceptableSize(realSize);
}
QRect QIrDockManagerPrivate::calculateDockGeometry( bool ignoreState ) const
{
	if ( !managed )
		return QRect();

	QRect r = managed->rect(), geom;
	QIrAbstractDock::DockArea area = dock->dockArea();
	QIrAbstractDock::DockAlignment alignment = dock->dockAlignment();
	int offset = dock->dockOffset();
	QSize size = smartDockSize(ignoreState);

	
	geom.setSize(size);
	if ( dock->isExtended() && !ignoreState ) {
		switch ( dock->dockArea() ) {
			case QIrAbstractDock::DA_BottomDockArea :
				geom.moveLeft(dock->normalDockGeometry().left() + dock->extendOffset().x());
				geom.moveBottom(dock->normalDockGeometry().bottom());
				break;
			case QIrAbstractDock::DA_TopDockArea :
				geom.moveLeft(dock->normalDockGeometry().left() + dock->extendOffset().x());
				geom.moveTop(dock->normalDockGeometry().top());
				break;
			case QIrAbstractDock::DA_LeftDockArea :
				geom.moveTop(dock->normalDockGeometry().top() + dock->extendOffset().y());
				geom.moveLeft(dock->normalDockGeometry().left());
				break;
			default :
				geom.moveTop(dock->normalDockGeometry().top() + dock->extendOffset().y());
				geom.moveRight(dock->normalDockGeometry().right());
				break;
		}
	} else {
		switch ( alignment ) {
			case QIrAbstractDock::DAL_DockAlignLeft:
				if (area == QIrAbstractDock::DA_TopDockArea || area == QIrAbstractDock::DA_LeftDockArea)
					geom.moveTopLeft(r.topLeft() + QPoint(offset,offset));
				else if ( area == QIrAbstractDock::DA_BottomDockArea ) 
					geom.moveBottomLeft(r.bottomLeft() + QPoint(offset,-offset));
				else 
					geom.moveTopRight(r.topRight() + QPoint(-offset,offset));
				break;
			case QIrAbstractDock::DAL_DockAlignCenter:
				geom.moveCenter(r.center());
				if ( area == QIrAbstractDock::DA_TopDockArea )
					geom.moveTop(r.top() + offset);
				else if ( area == QIrAbstractDock::DA_LeftDockArea )
					geom.moveLeft(r.left() + offset);
				else if ( area == QIrAbstractDock::DA_BottomDockArea )
					geom.moveBottom(r.bottom() - offset);
				else
					geom.moveRight(r.right() - offset);
				break;
			default:
				if (area == QIrAbstractDock::DA_BottomDockArea || area == QIrAbstractDock::DA_RightDockArea)
					geom.moveBottomRight(r.bottomRight() + QPoint(-offset,-offset));
				else if ( area == QIrAbstractDock::DA_TopDockArea )
					geom.moveTopRight(r.topRight() + QPoint(-offset,offset));
				else 
					geom.moveBottomLeft(r.bottomLeft() + QPoint(offset,-offset));
				break;
		}
	}
	return geom;
}
void QIrDockManagerPrivate::getAnimationOffsets(const QRect & newGeom, 
										   int & startOffset, 
										   int & endOffset, 
										   bool interrupted) const
{
	QRect rect = managed->rect(),
		dockRect = newGeom,
		geom = dock->geometry();
	
	switch ( dock->dockArea() ) {
		case QIrAbstractDock::DA_TopDockArea:
			startOffset = rect.top() - dockRect.bottom();
			if ( interrupted) {
				endOffset = geom.bottom() - dockRect.bottom();
			} else { 
				endOffset = 0;
			}
			break;
		case QIrAbstractDock::DA_LeftDockArea:
			startOffset = rect.left() - dockRect.right();
			if ( interrupted) {
				endOffset = geom.right() - dockRect.right();
			} else { 
				endOffset = 0;
			}
			break;
		case QIrAbstractDock::DA_BottomDockArea:
			startOffset = rect.bottom() - dockRect.top();
			if ( interrupted) {
				endOffset = geom.top() - dockRect.top();
			} else { 
				endOffset = 0;
			}
			break;
		default:
			startOffset = rect.right() - dockRect.left();
			if ( interrupted) {
				endOffset = geom.left() - dockRect.left();
			} else { 
				endOffset = 0;
			}
			break;
	}
}
QRect QIrDockManagerPrivate::getDragOutlineGeometry() const
{
	QRect geom, r = managed->rect();
	QSize size = dragInfo.geometry.size();
	bool horizontal = (dock->dockArea() == QIrAbstractDock::DA_TopDockArea 
		|| dock->dockArea() == QIrAbstractDock::DA_BottomDockArea);
	bool dragHorizontal = (dragInfo.area == QIrAbstractDock::DA_TopDockArea 
		|| dragInfo.area == QIrAbstractDock::DA_BottomDockArea);
	int dockOffset = dock->dockOffset();

	if ( horizontal != dragHorizontal )
		qSwap< int >(size.rheight(),size.rwidth());
	geom.setSize(size);
	switch ( dragInfo.alignment ) {
		case QIrAbstractDock::DAL_DockAlignLeft:
			if ( dragInfo.area == QIrAbstractDock::DA_TopDockArea || 
				dragInfo.area == QIrAbstractDock::DA_LeftDockArea)
				geom.moveTopLeft(r.topLeft() + QPoint(dockOffset,dockOffset));
			else if ( dragInfo.area == QIrAbstractDock::DA_BottomDockArea ) 
				geom.moveBottomLeft(r.bottomLeft() + QPoint(dockOffset,-dockOffset));
			else 
				geom.moveTopRight(r.topRight() + QPoint(-dockOffset,dockOffset));
			break;
		case QIrAbstractDock::DAL_DockAlignCenter:
			geom.moveCenter(r.center());
			if ( dragInfo.area == QIrAbstractDock::DA_TopDockArea )
				geom.moveTop(r.top() + dockOffset);
			else if ( dragInfo.area == QIrAbstractDock::DA_LeftDockArea )
				geom.moveLeft(r.left() + dockOffset);
			else if ( dragInfo.area == QIrAbstractDock::DA_BottomDockArea )
				geom.moveBottom(r.bottom() - dockOffset);
			else
				geom.moveRight(r.right() - dockOffset);
			break;
		default:
			if ( dragInfo.area == QIrAbstractDock::DA_BottomDockArea 
				|| dragInfo.area == QIrAbstractDock::DA_RightDockArea)
				geom.moveBottomRight(r.bottomRight() + QPoint(-dockOffset,-dockOffset));
			else if ( dragInfo.area == QIrAbstractDock::DA_TopDockArea )
				geom.moveTopRight(r.topRight() + QPoint(-dockOffset,dockOffset));
			else 
				geom.moveBottomLeft(r.bottomLeft() + QPoint(dockOffset,-dockOffset));
			break;
	}
	return geom;

}
void QIrDockManagerPrivate::dropDock()
{
	QIR_O(QIrDockManager);
	QRect startGeom = dock->geometry(), endGeom;
	
	lockMouseEvent = true;
	if ( !(dragInfo.area == QIrAbstractDock::DA_NoDockArea 
		|| dragInfo.alignment == QIrAbstractDock::DAL_NoDockAlignment || 
		(dragInfo.area == dock->dockArea() && dragInfo.alignment == dock->dockAlignment())) ) {
			dock->setDockArea(dragInfo.area);
			dock->setDockAlignment(dragInfo.alignment);
	}
	QRect geom = calculateDockGeometry();
	o->setDockGeometry(geom);
	lockMouseEvent = false;
	dock->setDockState(QIrAbstractDock::DS_Shown);
	

	
	/*endGeom = calculateDockGeometry();
	dropAnimation->setStartValue(startGeom);
	dropAnimation->setEndValue(endGeom);
	dropAnimation->start();*/
}

/////////////////
//QIrDockManager
/////////////////
QIrDockManager::QIrDockManager( QIrAbstractDock * parent ) : QObject(parent), QIrObject(* new QIrDockManagerPrivate)
{
	QIR_P(QIrDockManager);

	p->dock = parent;
	p->dock->installEventFilter(this);
	p->setup();	
}
QIrDockManager::~QIrDockManager()
{
	QIR_P(QIrDockManager);

	if ( p->dragOutline && !p->dragOutline->parent() ) {
		p->dragOutline->hide();
		delete p->dragOutline;
	}
}
bool QIrDockManager::isShowBlocked() const
{
	QIR_P(const QIrDockManager);

	return p->showBlocked;
}
void QIrDockManager::blockShow( bool block )
{
	QIR_P(QIrDockManager);

	if ( p->showBlocked == block )
		return;
	p->showBlocked = block;
	if ( block ) {
		if (p->showTimer->isActive() && p->dock->dockState() != QIrAbstractDock::DS_Hidden)
			p->showTimer->stop();
	} else {
		p->evaluatePos();
	}
}
bool QIrDockManager::isHideBlocked() const
{
	QIR_P(const QIrDockManager);

	return p->hideBlocked;
}
void QIrDockManager::blockHide( bool block )
{
	QIR_P(QIrDockManager);

	if ( p->hideBlocked == block )
		return;
	p->hideBlocked = block;
	if ( block ) {
		if (p->showTimer->isActive() && p->dock->dockState() == QIrAbstractDock::DS_Hidden)
			p->showTimer->stop();
	} else {
		p->evaluatePos();
	}
}
QWidget * QIrDockManager::managed() const
{
	QIR_P(const QIrDockManager);

	return p->managed;
}
void QIrDockManager::setManaged( QWidget * w )
{
	QIR_P(QIrDockManager);
	bool toShow = false, oldState;

	if ( p->managed == w )
		return;
	p->lockMouseEvent = false;
	if ( p->managed ) {
		p->managed->removeEventFilter(this);

		if ( p->showAnimation->state() == QPropertyAnimation::Running )
			p->showAnimation->stop();

		oldState = p->guideManager->useAnimation();
		p->guideManager->setUseAnimation(false);
		p->guideManager->undeploy();
		p->guideManager->setUseAnimation(oldState);
		p->dragOutline->hide();
		toShow = !p->dock->autoHide();
	}
	p->managed = w;
	p->guideManager->updateManaged(w);
	p->dragOutline->setParent(w);
	if ( w )
		w->installEventFilter(this);
	else 
		toShow = false;
	if ( toShow && !p->showBlocked ) {
		slotShowDock(true);
	}
}
QIrAbstractDock * QIrDockManager::dock() const
{
	QIR_P(const QIrDockManager);

	return p->dock;
}
QRect QIrDockManager::sensitiveRegion() const
{
	QIR_P(const QIrDockManager);
	if ( !p->managed )
		return QRect();

	QRect r = p->managed->rect(), sr;
	QRect geom = p->calculateDockGeometry();
	QIrAbstractDock::DockArea area = p->dock->dockArea();
	int dockOffset = p->dock->dockOffset();

	switch ( area ) {
		case QIrAbstractDock::DA_TopDockArea:
			sr = QRect(0,0,r.width(),qMin(r.height(),geom.height() + dockOffset));
			break;
		case QIrAbstractDock::DA_BottomDockArea:
			sr = QRect(0,0,r.width(),qMin(r.height(),geom.height() + dockOffset));
			sr.moveBottom(r.bottom());
			break;
		case QIrAbstractDock::DA_LeftDockArea:
			sr = QRect(0,0,qMin(r.width(),geom.width() + dockOffset),r.height());
			break;
		case QIrAbstractDock::DA_RightDockArea:
			sr = QRect(0,0,qMin(r.width(),geom.width() + dockOffset),r.height());
			sr.moveRight(r.right());
			break;
		default:
			break;
	}
	return sr;
}
void QIrDockManager::slotHandleAnimationFinished()
{
	QIR_P(QIrDockManager);
	
	if ( p->dock->dockState() == QIrAbstractDock::DS_Hiding ) {
		p->lockMouseEvent = false;
		p->dock->setDockState(QIrAbstractDock::DS_Hidden);
		slotEnterQuietTime();
		return;
	} else if ( p->dock->dockState() == QIrAbstractDock::DS_Dropping ) {
		p->dock->setDockState(QIrAbstractDock::DS_Shown);
		p->isInSensitiveRegion = true;
		p->evaluatePos();
		p->dock->setNormalDockGeometry(p->dock->geometry());
		p->lockMouseEvent = false;
		p->dock->evaluateCursorPos();
	} else if ( p->dock->dockState() == QIrAbstractDock::DS_Showing ) {
	
		p->dock->setDockState(QIrAbstractDock::DS_Shown);
		p->lockMouseEvent = false;
		p->isInSensitiveRegion = true;
		p->evaluatePos();
		p->dock->evaluateCursorPos();
	} else {
		p->lockMouseEvent = false;
		p->dock->setDockState(QIrAbstractDock::DS_Shown);	
	}
}
void QIrDockManager::resize( QResizeEvent * )
{
	QIR_P(QIrDockManager);

	if ( !(p->dock->dockState() == QIrAbstractDock::DS_Hidden || 
		p->dock->dockState() == QIrAbstractDock::DS_Dragging) ) {
		QRect geom = p->calculateDockGeometry();
		
		
		if (p->dock->dockState() == QIrAbstractDock::DS_Hiding || 
			p->dock->dockState() == QIrAbstractDock::DS_Showing) {
			switch ( p->dock->dockArea() ) {
				case QIrAbstractDock::DA_BottomDockArea:
					int endOffset,startOffset;

					//Update animation
					p->getAnimationOffsets(geom,endOffset,startOffset,false);
					p->showAnimation->setStartValue(endOffset);
					geom.moveTop(p->managed->rect().bottom() - p->showAnimation->currentValue().toInt());
					break;
				default:
					break;
			}
		}
		setDockGeometry(geom);
	}
}
void QIrDockManager::setManagedWindowActive( bool active )
{
	QIR_P(QIrDockManager);

	if ( p->managedWindowIsActive == active )
		return;
	p->managedWindowIsActive = active;
	if ( active ) {
		p->evaluatePos();
	} else {
		p->dock->restore();
		p->evaluatePos(QIR_POINT_AWAY);
	}	
}
bool QIrDockManager::eventFilter(QObject * obj, QEvent * evt)
{
	QIR_P(QIrDockManager);

	if ( !p->managed )
		return false;
	switch ( evt->type() ) {
		case QEvent::ActivationChange:
			if ( p->managed->window()->isActiveWindow() )
				setManagedWindowActive(true);
			else
				setManagedWindowActive(false);
			break;
		case QEvent::HoverMove: {
			if ( !p->managed->window()->isActiveWindow() )
				return QObject::eventFilter(obj,evt);
			p->evaluatePos();
		}
		case QEvent::MouseMove:
		case QEvent::MouseButtonPress:
			if ( p->lockMouseEvent )
				if (p->dock == obj || qFindChildren< QObject * >(p->dock).contains(obj) )
					return true;
			break;
		case QEvent::Resize:
			if ( p->managed == obj )
				resize(static_cast< QResizeEvent * >(evt));
			break;
		case QEvent::Leave:
			if ( p->managed == obj )
				p->evaluatePos();
			break;
		case QEvent::Show:
			if ( p->dock == obj ) {
				if ( !p->callShow )
					this->slotShowDock(true);
			}
		default:
			break;
	}
	return QObject::eventFilter(obj,evt);
}
void QIrDockManager::slotShowDock( bool show )
{
	QIR_P(QIrDockManager);

	QRect geom, realGeom;
	bool interrupted;
	int startOffset = 0, endOffset = 0;

	if ( !p->managed )
		return;

	if ( show ) {
		if ( !(p->dock->dockState() == QIrAbstractDock::DS_Hidden ||  
			p->dock->dockState() == QIrAbstractDock::DS_Hiding) )
			return;
		geom = p->calculateDockGeometry();
	
		if ( !p->dock->useAnimation() ) {
			setDockGeometry(geom);
			p->dock->setDockState(QIrAbstractDock::DS_Shown);
		} else {
			p->lockMouseEvent = true;
			interrupted = (p->dock->dockState() == QIrAbstractDock::DS_Hiding);
			p->getAnimationOffsets(geom,endOffset,startOffset,interrupted);
			if ( p->showAnimation->state() == QPropertyAnimation::Running )
				p->showAnimation->stop();
			p->dock->setDockState(QIrAbstractDock::DS_Showing);
			p->callShow = true;
			p->dock->show();
			p->callShow = false;
			setDockGeometry(geom);
			p->lastArea = p->dock->dockArea();
			p->showAnimation->setDuration(SHOW_ANIMATION_DURATION);
			p->showAnimation->setStartValue(startOffset);
			p->showAnimation->setEndValue(endOffset);
			p->showAnimation->start();
		}
	}
	else {
		if ( p->dock->isHidden() )
			return;
		if ( !p->dock->useAnimation() ) {
			p->dock->setDockState(QIrAbstractDock::DS_Hidden);
			slotEnterQuietTime();
		} else {
			p->lockMouseEvent = true;
			interrupted = (p->dock->dockState() == QIrAbstractDock::DS_Showing);
			p->getAnimationOffsets(p->dock->geometry(),startOffset,endOffset,interrupted);
			if ( p->showAnimation->state() == QPropertyAnimation::Running )
				p->showAnimation->stop();
			p->dock->setDockState(QIrAbstractDock::DS_Hiding);
			p->lastArea = p->dock->dockArea();
			p->showAnimation->setDuration(HIDE_ANIMATION_DURATION);
			p->showAnimation->setStartValue(startOffset);
			p->showAnimation->setEndValue(endOffset);
			p->showAnimation->start();
		}
	}
}
void QIrDockManager::slotUpdateDockGeometry()
{
	QIR_P(QIrDockManager);
	QIR_NO_RECURSIVE(p->called,void(0));

	if (!(p->dock->dockState() == QIrAbstractDock::DS_Dropping || 
		p->dock->dockState() == QIrAbstractDock::DS_Showing ||
		p->dock->dockState() == QIrAbstractDock::DS_Hiding)) {
			QRect geom = p->calculateDockGeometry();
			setDockGeometry(geom);
	}
}
void QIrDockManager::slotHandleDataChanged(const QVariant & data, QIrAbstractDock::DataRole role )
{
	QIR_P(QIrDockManager);

	switch ( role ) {
		case QIrAbstractDock::DR_AutoHide:
			if ( !data.toBool() ) {
				if ( p->dock->dockState() == QIrAbstractDock::DS_Hidden )
					slotShowDock(true);
			} else {
				p->isInSensitiveRegion = true;
				p->evaluatePos();
			}
			break;
		case QIrAbstractDock::DR_Area:
		case QIrAbstractDock::DR_Alignment:
			slotHandleDockPositionChanged();
			break;
		case QIrAbstractDock::DR_AutoHideDelay:
			p->showTimer->setInterval(data.toInt());
			break;
		case QIrAbstractDock::DR_QuietDelay:
			p->quietTimer->setInterval(data.toInt());
			break;
		case QIrAbstractDock::DR_Offset:
			slotUpdateDockGeometry();
			break;
		default:
			break;
	}
}
void QIrDockManager::slotHandleDockPositionChanged()
{
	QIR_P(QIrDockManager);

	p->dock->setDockState(QIrAbstractDock::DS_Shown);
	if ( p->dock->dockState() == QIrAbstractDock::DS_Shown ) {
	
		if ( p->showTimer->isActive() )
			p->showTimer->stop();
		QRect geom = p->calculateDockGeometry();
		
		setDockGeometry(geom);
		p->dock->restore();
		p->isInSensitiveRegion = true;
		p->evaluatePos();
	} 
}
void QIrDockManager::slotHandleDragAlignChanged(QIrAbstractDock::DockAlignment newAlign, QIrAbstractDock::DockAlignment previousAlign)
{
	QIR_P(QIrDockManager);
	QIrDockDragGuide * guide = qobject_cast< QIrDockDragGuide * >(sender());
	Q_UNUSED(previousAlign);

	if (!guide || p->dock->dockState() != QIrAbstractDock::DS_Dragging)
		return;

	if ( newAlign == QIrAbstractDock::DAL_NoDockAlignment ) {
		p->dragInfo.area = QIrAbstractDock::DA_NoDockArea;
		p->dragInfo.alignment = QIrAbstractDock::DAL_NoDockAlignment;
		if ( !p->dragOutline->isHidden() )
			p->dragOutline->hide();	
	} else {
		p->dragInfo.area = guide->dockArea();
		p->dragInfo.alignment = newAlign;
		QRect geom = p->getDragOutlineGeometry();
		
		p->dragOutline->setGeometry(geom);
		if ( p->dragOutline->isHidden() ) {
			p->dragOutline->show();
			p->dragOutline->stackUnder(p->dock);
		}
	}
}
int QIrDockManager::animatedOffset() const
{
	QIR_P(const QIrDockManager);

	return p->animOffset;
}
void QIrDockManager::setAnimatedOffset( int offset )
{
	QIR_P(QIrDockManager);
	QRect geom = p->dock->geometry(), rect = p->managed->rect();
	
	p->animOffset = offset;
	switch ( p->lastArea ) {
		case QIrAbstractDock::DA_TopDockArea:
			geom.moveBottom(rect.top() - offset);
			break;
		case QIrAbstractDock::DA_LeftDockArea:
			geom.moveRight(rect.left() - offset);
			break;
		case QIrAbstractDock::DA_BottomDockArea:
			geom.moveTop(rect.bottom() - offset);
			break;
		case QIrAbstractDock::DA_RightDockArea:
			geom.moveLeft(rect.right() - offset);
			break;
                default:
                        break;
	}
	setDockGeometry(geom);
}
void QIrDockManager::plugDock()
{
	QIR_P(QIrDockManager);

	p->dock->setDockState(QIrAbstractDock::DS_Dropping);
	p->dragOutline->hide();
	p->guideManager->undeploy();
	p->dropDock();
	p->dock->evaluateCursorPos();
}
void QIrDockManager::unplugDock()
{
	QIR_P(QIrDockManager);

	p->dragInfo.geometry = p->dock->geometry();
	p->dragInfo.area = QIrAbstractDock::DA_NoDockArea;
	p->dragInfo.alignment = QIrAbstractDock::DAL_NoDockAlignment;
	p->guideManager->deploy();
}
void QIrDockManager::slotHandleShowTimeout()
{
	QIR_P(QIrDockManager);

	slotShowDock(p->toShow);
}
void QIrDockManager::slotEnterQuietTime()
{
	QIR_P(QIrDockManager);

	p->quiet = true;
	p->quietTimer->start();
}
void QIrDockManager::slotLeaveQuietTime()
{
	QIR_P(QIrDockManager);

	p->quiet = false;
}
void QIrDockManager::setDockGeometry( const QRect & geom )
{
	QIR_P(QIrDockManager);

	p->dock->setGeometry(geom);
	if ( !p->dock->isExtended() ) {
		p->dock->setNormalDockGeometry(geom);
	}
}
void QIrDockManager::updateNormalDockGeometry()
{
	QIR_P(QIrDockManager);

	QRect normalDock = p->calculateDockGeometry(true);

	p->dock->setNormalDockGeometry(normalDock);

}


QIR_END_NAMESPACE
