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
#include <QActionEvent>
#include <QApplication>
#include <QMouseEvent>
#include "qirdockmanager.h"
#include "private/qirdocklet_p.h"
#include "ui/qirabstractdock_ui.h"
#include "qirstyleoptiondock.h"
#include <QMetaEnum>

QIR_BEGIN_NAMESPACE

////////////////////
//QIrAbstractDockUi
////////////////////
void QIrAbstractDockUi::init()
{
	QIR_W(QIrAbstractDock);
	
	state = QIrAbstractDock::DS_Shown;
	manager = new QIrDockManager(w);
	manager->setManaged(w->parentWidget());
#ifndef QT_NO_WHATSTHIS
    w->setAttribute(Qt::WA_CustomWhatsThis);
#endif
	w->setMouseTracking(true);
	w->setAttribute(Qt::WA_NoChildEventsForParent);
	
	allowedAreas = QIrAbstractDock::DA_AllDockAreas;
	allowedAlignments[QIrAbstractDock::DA_TopDockArea] = QIrAbstractDock::DAL_AllDockAlignments;
	allowedAlignments[QIrAbstractDock::DA_LeftDockArea] = QIrAbstractDock::DAL_AllDockAlignments;
	allowedAlignments[QIrAbstractDock::DA_BottomDockArea] = QIrAbstractDock::DAL_AllDockAlignments;
	allowedAlignments[QIrAbstractDock::DA_RightDockArea] = QIrAbstractDock::DAL_AllDockAlignments;
}
void QIrAbstractDockUi::setupConnections()
{
	QIR_W(QIrAbstractDock);
	
	w->connect(w,SIGNAL(dataChanged(QVariant,QIrAbstractDock::DataRole)),manager,SLOT(slotHandleDataChanged(QVariant,QIrAbstractDock::DataRole)));
	w->connect(w,SIGNAL(updateGeometryRequested()),manager,SLOT(slotUpdateDockGeometry()),Qt::DirectConnection);
}
bool QIrAbstractDockUi::adjustDockAlignment()
{
	QIR_W(QIrAbstractDock);

	if ( allowedAlignments[area] & alignment )
		return false;
	for (int i = QIrAbstractDock::DAL_DockAlignCenter; i <= QIrAbstractDock::DAL_DockAlignRight; i++ ) {
		if ( allowedAlignments[area] & i ) {
			w->setDockAlignment((QIrAbstractDock::DockAlignment)i);
			break;
		}
	}
	return true;
}
bool QIrAbstractDockUi::adjustDockArea()
{
	QIR_W(QIrAbstractDock);

	if ( allowedAreas & area )
		return false;
	for (int i = QIrAbstractDock::DA_BottomDockArea; i <= QIrAbstractDock::DA_TopDockArea; i++ ) {
		if ( allowedAreas & i ) {
			w->setDockArea((QIrAbstractDock::DockArea)i);
			break;
		}
	}
	return true;
}

///////////////////
//QIrAbstractDock
//////////////////
QIrAbstractDock::QIrAbstractDock(QWidget * parent , DockArea area, DockAlignment alignment) : QIrWidget(* new QIrAbstractDockUi), QWidget(parent)
{
	QIR_UI(QIrAbstractDock);
	
	ui->area = area;
	ui->alignment = alignment;
	ui->setupUi();
}
QIrAbstractDock::QIrAbstractDock(QWidget * parent, QIrAbstractDockUi & u ) : QIrWidget(u), QWidget(parent)
{
	QIR_UI(QIrAbstractDock);
	
	ui->area = QIrAbstractDock::DA_BottomDockArea;
	ui->alignment = QIrAbstractDock::DAL_DockAlignCenter;
}
QIrAbstractDock::~QIrAbstractDock()
{
}
QIrDocklet * QIrAbstractDock::addDocklet( const QString & text )
{
	return addDocklet(QIcon(),text);
}
QIrDocklet * QIrAbstractDock::addDocklet(const QIcon & icon, const QString & text)
{
	QIrDocklet * docklet = new QIrDocklet(icon,text);

	addDocklet(docklet);
	if ( docklet->dock() == 0 ) {
		delete docklet; 
		docklet = 0;
	} 
	return docklet;
}
QIrDocklet * QIrAbstractDock::addDocklet(const QString & text, const QObject * receiver, const char * member)
{
	return addDocklet(QIcon(),text,receiver,member);
}
QIrDocklet * QIrAbstractDock::addDocklet(const QIcon & icon, const QString & text, const QObject * receiver, const char * member)
{
	QIrDocklet * docklet = new QIrDocklet(icon,text);

    addDocklet(docklet);
	if ( docklet->dock() == 0 ) {
		delete docklet; 
		docklet = 0;
	} else 
		QObject::connect(docklet,SIGNAL(clicked(bool)),receiver,member);
	return docklet;
}
void QIrAbstractDock::addDocklet( QIrDocklet * docklet )
{
	if ( docklet )
		insertDocklet(dockletAt(qMax(0,count() - 1)),docklet);
}
QIrDocklet * QIrAbstractDock::addSeparator()
{
	return insertSeparator(dockletAt(qMax(0,count() - 1)));
}
QIrDocklet * QIrAbstractDock::insertSeparator( QIrDocklet * before )
{
	QIrDocklet * docklet = new QIrDocklet;

	docklet->setSeparator(true);
    insertDocklet(before,docklet);
	if ( docklet->dock() == 0 ) {
		delete docklet;
		docklet = 0;
	}
	return docklet;
}
void QIrAbstractDock::insertDocklets(QIrDocklet * before, QList< QIrDocklet * > docklets)
{
	foreach (QIrDocklet * docklet, docklets) {
		if ( docklet )
			insertDocklet(before,docklet);
	}
}
void QIrAbstractDock::removeDocklet( QIrDocklet *  docklet )
{
	int index;

	if ( !docklet )
		return;
	if ( (index = indexOf(docklet)) != -1 ) {
		docklet->p_func()->removing = true;
		takeAt(index);
		if ( !docklet->p_func()->destroying ) {
			delete docklet;
			docklet = 0;
		}
	}
}
void QIrAbstractDock::clear()
{
	/*QList< QAction * > acts = actions();
    
	for(int i = 0; i < acts.size(); i++) {
        QWidget::removeAction(acts[i]);
		if (acts[i]->parent() == this && acts[i]->associatedWidgets().isEmpty())
            delete acts[i];
    }*/
}
void QIrAbstractDock::parentChanged()
{
}
int QIrAbstractDock::indexOf( QIrDocklet * docklet ) const
{
	int c = count();

	if (!docklet || c == 0)
		return -1;
	for (int i = 0; i < c; i++) {
		if ( dockletAt(i) == docklet )
			return i;
	}
	return -1;
}
void QIrAbstractDock::setNormalDockGeometry( const QRect & geom )
{
	QIR_UI(QIrAbstractDock);

	ui->normalGeom = geom;	
}
//Properties
QIrAbstractDock::DockAlignments QIrAbstractDock::allowedDockAlignments( DockArea area ) const
{
	QIR_UI(const QIrAbstractDock);

	if (area == QIrAbstractDock::DA_AllDockAreas || area == QIrAbstractDock::DA_NoDockArea)
            return QIrAbstractDock::DAL_NoDockAlignment;
	return ui->allowedAlignments[area];
}
void QIrAbstractDock::setAllowedDockAlignments(DockArea area, DockAlignments alignments)
{
	QIR_UI(QIrAbstractDock);

	if (area == QIrAbstractDock::DA_AllDockAreas || area == QIrAbstractDock::DA_NoDockArea || 
		alignments == QIrAbstractDock::DAL_NoDockAlignment || ui->allowedAlignments[area] == alignments)
		return;
	ui->allowedAlignments[area] = alignments;
	ui->adjustDockAlignment();
	emit dataChanged(area,QIrAbstractDock::DR_AllowedAlignments);
}
QIrAbstractDock::DockAreas QIrAbstractDock::allowedDockAreas() const
{
	QIR_UI(const QIrAbstractDock);

	return ui->allowedAreas;
}
void QIrAbstractDock::setAllowedDockAreas( QIrAbstractDock::DockAreas areas )
{
	QIR_UI(QIrAbstractDock);

	if (ui->allowedAreas == areas || areas == QIrAbstractDock::DA_NoDockArea)
		return;
	ui->allowedAreas = areas;
	ui->adjustDockArea();
	emit dataChanged((int)areas,QIrAbstractDock::DR_AllowedAreas);
}
bool QIrAbstractDock::autoHide() const
{
	QIR_UI(const QIrAbstractDock);

	return ui->autoHide;
}
void QIrAbstractDock::setAutoHide( bool a )
{
	QIR_UI(QIrAbstractDock);

	if ( ui->autoHide == a )
		return;
	ui->autoHide = a;
	emit autoHideChanged( a );
	emit dataChanged(a,QIrAbstractDock::DR_AutoHide);
}
int QIrAbstractDock::autoHideDelay() const
{
	QIR_UI(const QIrAbstractDock);

	return (ui->autoHideDelay < 1000 ? 1000 : ui->autoHideDelay > 3000 ? 3000 : ui->autoHideDelay);
}
void QIrAbstractDock::setAutoHideDelay( int d )
{
	QIR_UI(QIrAbstractDock);
	int old = autoHideDelay();

	ui->autoHideDelay = d;
	if ( autoHideDelay() == old )
		return;
	ui->autoHideDelay = d;
	emit dataChanged(autoHideDelay(),QIrAbstractDock::DR_AutoHideDelay);	
}
QIrAbstractDock::DockAlignment QIrAbstractDock::dockAlignment() const
{
	QIR_UI(const QIrAbstractDock);

	return ui->alignment;
}
void QIrAbstractDock::setDockAlignment( QIrAbstractDock::DockAlignment alignment )
{
	QIR_UI(QIrAbstractDock);

	if ( !((allowedDockAlignments(dockArea()) & alignment) && ui->alignment != alignment) ) 
		return;
	ui->alignment = alignment;
	if ( !ui->lockSignal ) {
		emit dockAlignmentChanged(alignment);
		emit dataChanged(alignment,QIrAbstractDock::DR_Alignment);
	}
}
QIrAbstractDock::DockArea QIrAbstractDock::dockArea() const
{
	QIR_UI(const QIrAbstractDock);

	return ui->area;
}
void QIrAbstractDock::setDockArea( QIrAbstractDock::DockArea area )
{
	QIR_UI(QIrAbstractDock);

	if ( !((allowedDockAreas() & area) && ui->area != area) )
		return;
	ui->area = area;
	ui->lockSignal = true;
	ui->adjustDockAlignment();
	ui->lockSignal = false;
	emit dockAreaChanged(area);
	emit dataChanged(area,QIrAbstractDock::DR_Area);
}
int QIrAbstractDock::dockOffset() const
{
	QIR_UI(const QIrAbstractDock);

	return ui->dockOffset;
}
void QIrAbstractDock::setDockOffset( int o )
{
	QIR_UI(QIrAbstractDock);

	if ( ui->dockOffset == o )
		return;
	ui->dockOffset = o;
	emit dataChanged(o,QIrAbstractDock::DR_Offset);
	emit updateGeometryRequested();
}
QIrAbstractDock::DockState QIrAbstractDock::dockState() const
{
	QIR_UI(const QIrAbstractDock);

	return isHidden() ? DS_Hidden : ui->state;
}
void QIrAbstractDock::setDockState( QIrAbstractDock::DockState ds )
{
	QIR_UI(QIrAbstractDock);
	QIrAbstractDock::DockState old = dockState(), current;

	QMetaObject mt = QIrAbstractDock::staticMetaObject;
	QMetaEnum m = mt.enumerator(0);

	ui->state = ds;
	switch ( ui->state ) {
		case DS_Shown:
			show();
			raise();
			ui->manager->blockHide(false);
			break;
		case DS_Hidden:
			hide();
			ui->manager->blockHide(false);
			break;
		case QIrAbstractDock::DS_MenuPoppedUp:
			ui->manager->blockHide(true);
			break;
		default:
			ui->manager->blockHide(false);
			break;
	}
	current = dockState();
	if ( old != current )
		emit dockStateChanged(current);
}
int QIrAbstractDock::effectiveMinimumIconSize() const
{
	QIR_UI(const QIrAbstractDock);
	
	if ( ui->overrideMinIconSize != -1 )
		return ui->overrideMinIconSize;
	else
		return minimumIconSize();
}
int QIrAbstractDock::minimumIconSize() const
{
	QIR_UI(const QIrAbstractDock);
	int ret;

	ret  = ui->minIconSize;
	if ( ret < 8 )
		return 8;
	return ret;
}
void QIrAbstractDock::setMinimumIconSize( int s )
{
	QIR_UI(QIrAbstractDock);
	int oldMin = effectiveMinimumIconSize();
	int oldMax = maximumIconSize();
	int currentMin;
	int currentMax;

	if ( ui->minIconSize == s )
		return;
	ui->minIconSize = s;
	currentMin = effectiveMinimumIconSize();
	currentMax = maximumIconSize();
	
	if ( currentMin != oldMin  )
		emit minimumIconSizeChanged(currentMin);
	if ( currentMax != oldMax )
		emit maximumIconSizeChanged(currentMax);
}
void QIrAbstractDock::overrideMinimumIconSize( int i )
{
	int oldMin = effectiveMinimumIconSize();
	int oldMax = maximumIconSize();
	int currentMin;
	int currentMax;

	if ( ui_func()->overrideMinIconSize == i )
		return;
	ui_func()->overrideMinIconSize = i;

	currentMin = effectiveMinimumIconSize();
	currentMax = maximumIconSize();
	if ( currentMin != oldMin  )
		emit minimumIconSizeChanged(currentMin);
	if ( currentMax != oldMax )
		emit maximumIconSizeChanged(currentMax);
}
int QIrAbstractDock::maximumIconSize() const
{
	QIR_UI(const QIrAbstractDock);
	
	if ( ui->maxIconSize < effectiveMinimumIconSize() )
		return minimumIconSize();
	return ui->maxIconSize;

}
void QIrAbstractDock::setMaximumIconSize( int s )
{
	QIR_UI(QIrAbstractDock);
	int oldMax = maximumIconSize();
	int currentMax;

	if ( ui->maxIconSize == s )
		return;
	ui->maxIconSize = s;
	currentMax = maximumIconSize();
	if( currentMax != oldMax )
		emit maximumIconSizeChanged(currentMax);
}
QRect QIrAbstractDock::normalDockGeometry() const
{
	QIR_UI(const QIrAbstractDock);

	if ( isExtended() ) {
		return ui->normalGeom;
	} 
	return geometry();
}
QIcon QIrAbstractDock::icon() const
{
	QIR_UI(const QIrAbstractDock);
	
	return ui->icon;
}
void QIrAbstractDock::setIcon( const QIcon & icon )
{
	QIR_UI(QIrAbstractDock);

	ui->icon = icon;
	emit dataChanged(icon,QIrAbstractDock::DR_Icon);
}
bool QIrAbstractDock::isExtended() const
{
	QIR_UI(const QIrAbstractDock);
	
	return ui->extended;
}
void QIrAbstractDock::setExtended( bool e )
{
	QIR_UI(QIrAbstractDock);

	if ( ui->extended == e )
		return;
	ui->extended = e;
}
bool QIrAbstractDock::isMovable() const
{
	QIR_UI(const QIrAbstractDock);

	return ui->movable;
}
void QIrAbstractDock::setMovable( bool m )
{
	QIR_UI(QIrAbstractDock);

	if ( ui->movable == m )
		return;
	ui->movable = m;
	emit dataChanged(m,QIrAbstractDock::DR_Movable);
}
double QIrAbstractDock::maximumDockWidthF() const
{
	QIR_UI(const QIrAbstractDock);

	return (ui->maxDockWidth < 0.0 ? 0.0 : ui->maxDockWidth > 1.0 ? 1.0 : ui->maxDockWidth);
}
void QIrAbstractDock::setMaximumDockWidthF( double max )
{
	QIR_UI(QIrAbstractDock);
	double old = maximumDockWidthF(), current;

	ui->maxDockWidth = max;
	if (ui->maxDockWidth < 0.0 || ui->maxDockWidth > 1.0)
		qWarning("QIrAbstractDock::setMaximumDockWidthF: Maximum width should be between 0.0 and 1.0.");
	current = maximumDockWidthF();
	if ( !qFuzzyCompare(old,current) ) {
		emit dataChanged(max,QIrAbstractDock::DR_Other);
		emit updateGeometryRequested();
	}
}
int QIrAbstractDock::maximumDockWidth() const
{
	QWidget * parent = parentWidget();
	
	if ( !parent )
		return 0;
	QSize ps = parent->size();

        return (int)(maximumDockWidthF() * (orientation() == Qt::Horizontal ? ps.width() : ps.height()));
}
Qt::Orientation QIrAbstractDock::orientation() const
{
	QIR_UI(const QIrAbstractDock);

	return (ui->area == QIrAbstractDock::DA_BottomDockArea || 
		ui->area == QIrAbstractDock::DA_TopDockArea ? Qt::Horizontal : Qt::Vertical);
}
int QIrAbstractDock::quietDelay() const
{
	QIR_UI(const QIrAbstractDock);

	return (ui->quietDelay < 1000 ? 1000 : ui->quietDelay > 3000 ? 3000 : ui->quietDelay);
}
void QIrAbstractDock::setQuietDelay( int d )
{
	QIR_UI(QIrAbstractDock);
	int old = quietDelay(), currentDelay;

	ui->quietDelay = d;
	currentDelay = quietDelay();
	if ( currentDelay != old )
		emit dataChanged(currentDelay,QIrAbstractDock::DR_QuietDelay);
}
QString QIrAbstractDock::title() const
{
	QIR_UI(const QIrAbstractDock);

	return ui->title;
}
void QIrAbstractDock::setTitle( const QString & title )
{
	QIR_UI(QIrAbstractDock);

	if ( ui->title == title )
		return;
	ui->title = title;
	emit dataChanged(title,QIrAbstractDock::DR_Title);
}
bool QIrAbstractDock::useAnimation() const
{
	QIR_UI(const QIrAbstractDock);
	
	return ui->useAnimation;
}
bool QIrAbstractDock::event( QEvent * evt )
{
	QIR_UI(QIrAbstractDock);
	
	switch ( evt->type() ) {
		case QEvent::ParentChange :
			parentChanged();
			if ( ui->manager )
				ui->manager->setManaged(parentWidget());

			break;
		case QEvent::Show :
			if ( ui->firstTimeShown ) {
				ui->firstTimeShown = false;
				if ( autoHide() ) {
					hide();
					break;
				}
			} 
			raise();
			break;
		default :
			break;
	}
	return QWidget::event(evt);
}
void QIrAbstractDock::setUseAnimation( bool use )
{
	QIR_UI(QIrAbstractDock);

	ui->useAnimation = use;
}
void QIrAbstractDock::slotUpdateGeometry()
{
	emit updateGeometryRequested();
}
void QIrAbstractDock::startDrag()
{
	QIR_UI(QIrAbstractDock);

	if (!ui->movable || dockState() == QIrAbstractDock::DS_Dragging || dockState() == QIrAbstractDock::DS_Dropping || isExtended())
		return;
	setDockState(QIrAbstractDock::DS_Dragging);
	ui->manager->unplugDock();
}
void QIrAbstractDock::endDrag()
{
	QIR_UI(QIrAbstractDock);

	ui->manager->plugDock();
}
void QIrAbstractDock::ownDocklet( QIrDocklet * docklet )
{
	if ( !docklet || (docklet && docklet->dock() == this))
		return;
	docklet->setDock(this);
	connect(docklet,SIGNAL(triggered()),this,SLOT(slotHandleTriggered()));
}
void QIrAbstractDock::releaseDocklet( QIrDocklet * docklet )
{
	if ( !docklet || (docklet && docklet->dock() != this))
		return;
	disconnect(docklet,SIGNAL(triggered()),this,SLOT(slotHandleTriggered()));
	docklet->setDock(0);
}
void QIrAbstractDock::initStyleOptionDocklet(const QIrDocklet * docklet, QIrStyleOptionDocklet * option) const
{
	option->palette = palette();
	option->active = docklet->isActive();
	option->icon = docklet->icon();
	option->iconSize = docklet->iconSize();
	option->text = docklet->text();
	option->separator = docklet->isSeparator();
	option->launcher = docklet->isLauncher();
	option->orientation = orientation();
	option->area = dockArea();
}
void QIrAbstractDock::slotHandleTriggered()
{
	QIrDocklet * docklet = qobject_cast< QIrDocklet * >(sender());

	if ( docklet ) {
		emit triggered(docklet);
	}
}
QIR_END_NAMESPACE
