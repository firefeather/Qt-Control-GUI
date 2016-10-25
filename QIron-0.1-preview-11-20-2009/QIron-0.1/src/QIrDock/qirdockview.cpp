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
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QTimer>
#include "private/qirdockeffectmanager_p.h"
#include "qirparaboliceffectmanager.h"
#include "qirstyleoptiondock.h"
#include "qirdockstyle.h"
#include "qirdocklet.h"
#include "qirdockletbundle.h"
#include "qirdock.h"
#include "ui/qirdockview_ui.h"
#include "../Common/qirboolblocker.h"

QIR_BEGIN_NAMESPACE

///////////////
//QIrDockViewUi
///////////////
QIrDockViewUi::QIrDockViewUi() : dock(0), effectManager(0), blockEvents(false), entered(false), called(false)
{
}
QIrDockViewUi::~QIrDockViewUi()
{
}
void QIrDockViewUi::init()
{
	QIR_W(QIrDockView);

	w->setScene(new QGraphicsScene(w));
	w->setMouseTracking(true);
	w->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	w->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	w->setEffectManager(new QIrDefaultEffectManager(w));//QIrParabolicEffectManager(w));
	w->setFrameShape(QFrame::NoFrame);
	w->setStyleSheet("background: transparent");
	delayTimer = new QTimer(w);
	delayTimer->setSingleShot(true);

}
void QIrDockViewUi::setupConnections()
{
	QIR_W(QIrDockView);
	
	w->connect(delayTimer,SIGNAL(timeout()),w,SLOT(slotHandleDiferredPosEvaluation()));
}

//////////////
//QIrDockView
//////////////
QIrDockView::QIrDockView( QIrDock * parent ) : QGraphicsView(parent), QIrWidget(* new QIrDockViewUi)
{	
	QIR_UI(QIrDockView);

	ui->dock = parent;
	ui->setupUi();
}
QIrDockView::~QIrDockView()
{	
}
QIrDock * QIrDockView::dock() const
{
	QIR_UI(const QIrDockView);

	return ui->dock;
}
int QIrDockView::dockletSpacing() const
{
	QIR_UI(const QIrDockView);
	static QIrSubStyle * lastPolisher = 0;
	static int spacing = 0;
	QIrSubStyle * style = ui->dock->subStyle();

	if ( lastPolisher != style ) {
		lastPolisher = style;
		QIrStyleOptionDock option;
		
		ui->dock->initStyleOption(&option);
		spacing = qMax(0,style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockletSpacing,&option));
	}
	return spacing;
}
int QIrDockView::normalSize() const
{
	return calculateSize(NormalSize);
}
int QIrDockView::extendSize() const
{
	return calculateSize(ExtendSize);
}
int QIrDockView::calculateSize( QIrDockView::SizeType st ) const
{
	QIR_UI(const QIrDockView);
	bool horizontal = (ui->dock->orientation() == Qt::Horizontal);
	QIrSubStyle * style = ui->dock->subStyle();
	QRectF contentRect;
	QSize sz;
	QStyleOption option;

	if (!style || !ui->effectManager || (!ui->dock->isExtended() && st == ExtendSize))
		return -1;
	option.initFrom(this);
	contentRect = (st == NormalSize ? ui->effectManager->normalContentRect() : ui->effectManager->extendContentRect());
	sz = contentRect.size().toSize();

	return (horizontal ? sz.height(): sz.width());
}
int QIrDockView::extendOffset() const
{
	QIR_UI(const QIrDockView);

	if ( ui->dock->orientation() == Qt::Horizontal ) {
                return (int)ui->effectManager->contentRect().x();
	} else {
                return (int)ui->effectManager->contentRect().y();
	}
}
bool QIrDockView::areEventsToEffectBlocked() const
{
	QIR_UI(const QIrDockView);

	return ui->blockEvents;
}
void QIrDockView::blockEventsToEffect( bool block )
{
	QIR_UI(QIrDockView);

	if ( ui->blockEvents == block )
		return;
	ui->blockEvents = block;
	if ( !block )
		deferPosEvaluation(200);
}
void QIrDockView::deferPosEvaluation( int delay )
{
	QIR_UI(QIrDockView);

	ui->delayTimer->start(delay);
}
void QIrDockView::slotHandleDiferredPosEvaluation()
{
	QPoint pos = mapFromGlobal(QCursor::pos());

	if ( rect().contains(pos) && QApplication::activeWindow() == window() ) {
		evaluatePos(pos);
	} else {
			evaluatePos(QIR_POINT_AWAY);
	}
}
QList< QIrDockletBundle * > QIrDockView::bundles( bool visibleOnly ) const
{
	QIR_UI(const QIrDockView);

	if ( !visibleOnly ) {
		return ui->bundles;
	} else {
		QList< QIrDockletBundle * > onlyVisibles;

		foreach (QIrDockletBundle * bundle, ui->bundles) {

			if (bundle && bundle->isInternallyVisible()) {
				onlyVisibles << bundle;
			}
		}
		return onlyVisibles;
	}
}
QIrDockEffectManager * QIrDockView::effectManager() const
{
	QIR_UI(const QIrDockView);

	return ui->effectManager;
}
void QIrDockView::setEffectManager( QIrDockEffectManager * manager )
{
	QIR_UI(QIrDockView);

	if ( !manager ) {
		if ( ui->effectManager ) {
                        if ( strcmp(ui->effectManager->metaObject()->className(),"QIrDefaultEffectManager"))
				return;
			ui->effectManager->p_func()->setDockView(0);
			QObject::disconnect(ui->effectManager,SIGNAL(extendRequested()),ui->dock,SLOT(slotHandleExtendRequest()));
			QObject::disconnect(ui->effectManager,SIGNAL(normalizeRequested()),ui->dock,SLOT(slotHandleNormalizeRequest()));
			if ( ui->effectManager->parent() == this )
				delete ui->effectManager;
		}
		ui->effectManager = new QIrDefaultEffectManager(this);
		ui->effectManager->p_func()->setDockView(this);
	} else {
		if ( ui->effectManager == manager )
			return;
		if ( ui->effectManager ) {
			ui->effectManager->p_func()->setDockView(0);

			QObject::disconnect(ui->effectManager,SIGNAL(extendRequested()),ui->dock,SLOT(slotHandleExtendRequest()));
			QObject::disconnect(ui->effectManager,SIGNAL(normalizeRequested()),ui->dock,SLOT(slotHandleNormalizeRequest()));
			if ( ui->effectManager->parent() == this )
				delete ui->effectManager;
		}
		ui->effectManager = manager;
		ui->effectManager->p_func()->setDockView(this);
	}
	QObject::connect(ui->effectManager,SIGNAL(updateGeometryRequested()),ui->dock,SLOT(slotUpdateGeometry()),Qt::DirectConnection);
	QObject::connect(ui->effectManager,SIGNAL(extendRequested()),ui->dock,SLOT(slotHandleExtendRequest()));
	QObject::connect(ui->effectManager,SIGNAL(normalizeRequested()),ui->dock,SLOT(slotHandleNormalizeRequest()));
	ui->effectManager->invalidate();
}
void QIrDockView::hoverDockletChanged(QIrDocklet * current, QIrDocklet * previous)
{
	QIR_UI(QIrDockView);

	ui->dock->handleHoverDockletChanged(current,previous);
}

QSize QIrDockView::boundingSize() const
{
	QIR_UI(const QIrDockView);
	bool horizontal = (ui->dock->orientation() == Qt::Horizontal);
	QSize sz =  ui->effectManager->contentRect().size().toSize();

	//I may have forgotten use of sz.rheight() = ui->effectManager->sizeHint().height() ...

	if ( horizontal )
		sz.rheight() = qMax(sz.height(),extendSize());
	else
		sz.rwidth() = qMax(sz.width(),extendSize());
	return sz;
}
QIrDockletBundle * QIrDockView::bundle( QIrDocklet * docklet ) const
{
	QIR_UI(const QIrDockView);

	if ( !docklet )
		return 0;

	foreach (QIrDockletBundle * bundle, ui->bundles) {
		if ( !bundle )
			continue;
		if ( bundle->docklet() == docklet )
			return bundle;
	}
	return 0;
}
QSize QIrDockView::sizeHint() const
{
	QIR_UI(const QIrDockView);
	bool horizontal = (ui->dock->orientation() == Qt::Horizontal);
	QSize sz = ui->effectManager->sizeHint();

	if ( horizontal )
		sz.rheight() = qMax(normalSize(),extendSize());
	else
		sz.rwidth() = qMax(normalSize(),extendSize());
	return sz;
}
void QIrDockView::contextMenuEvent( QContextMenuEvent * )
{
	QIR_UI(QIrDockView);

	ui->dock->slotPopupMenu();
}
void QIrDockView::enterEvent( QEvent * evt )
{
	QIR_UI(QIrDockView);

	ui->entered = true;
	if ( !ui->blockEvents )
		ui->effectManager->enterEvent(evt);
	QGraphicsView::enterEvent(evt);
}
void QIrDockView::leaveEvent( QEvent * evt )
{
	QIR_UI(QIrDockView);

	ui->entered = false;
	if ( !ui->blockEvents )
		ui->effectManager->leaveEvent(evt);
	QGraphicsView::leaveEvent(evt);
}
void QIrDockView::mousePressEvent( QMouseEvent * evt )
{
	QIR_UI(QIrDockView);

	if ( ui->blockEvents )
		return;
	if ( ui->delayTimer->isActive() )
		ui->delayTimer->stop();
	ui->effectManager->mouseMoveEvent(evt);
	ui->effectManager->mousePressEvent(evt);
	QGraphicsView::mousePressEvent(evt);
}
void QIrDockView::mouseMoveEvent( QMouseEvent * evt )
{
	QIR_UI(QIrDockView);
	
	if ( ui->blockEvents )
		return;
	ui->effectManager->mouseMoveEvent(evt);
	QGraphicsView::mouseMoveEvent(evt);
}
void QIrDockView::mouseReleaseEvent( QMouseEvent * evt )
{
	QIR_UI(QIrDockView);

	if ( ui->blockEvents )
		return;
	ui->effectManager->mouseReleaseEvent(evt);
	QGraphicsView::mouseReleaseEvent(evt);
}
void QIrDockView::adjustLayers()
{
	QIR_UI(QIrDockView);
	QRectF bR;
	QStyleOption option;
	QIrSubStyle * style = ui->dock->subStyle();
	QRectF minRect = ui->effectManager->normalContentRect();

	QIR_NO_RECURSIVE(ui->called,(void)0);

	if (!style && !minRect.isValid())
		return;

	option.initFrom(this);
	if ( ui->dock->isExtended() ) {
		bR = ui->effectManager->contentRect().toRect();
		
		switch ( ui->dock->dockArea() ) {
			case QIrAbstractDock::DA_BottomDockArea :
				bR.setHeight(height());
				bR.moveTop(-(height() - minRect.height() + 1));
				break;
			case QIrAbstractDock::DA_TopDockArea:
				bR.setHeight(height());
				break;
			case QIrAbstractDock::DA_LeftDockArea:
				bR.setWidth(width());
				break;
			default:
				bR.setWidth(width());
				bR.moveLeft(-(width() - minRect.width() + 1));
				break;
		}
	} else {
		QRectF layer = ui->effectManager->contentRect();

		bR = rect();
		switch ( ui->dock->dockArea() ) {
			case QIrAbstractDock::DA_BottomDockArea :
				bR.moveBottom(layer.bottom());
				break;
			case QIrAbstractDock::DA_TopDockArea:
				bR.moveTop(layer.top());
				break;
			case QIrAbstractDock::DA_LeftDockArea:
				bR.moveLeft(layer.left());
				break;
			default:
				bR.moveRight(layer.right());
				break;
		}
	}
	scene()->setSceneRect(bR);
	ui->effectManager->invalidate();	
}
void QIrDockView::resizeEvent( QResizeEvent * )
{
	adjustLayers();
}
void QIrDockView::slotHandleMinimumIconSizeChanged( int )
{
	QIR_UI(QIrDockView);

	if ( !ui->effectManager )
		return;
	ui->effectManager->reset();
}
void QIrDockView::slotHandleMaximumIconSizeChanged( int )
{
	QIR_UI(QIrDockView);

	if ( !ui->effectManager )
		return;
	ui_func()->effectManager->reset();
}
void QIrDockView::insertBundle(int index, QIrDockletBundle * bundle)
{
	QIR_UI(QIrDockView);

	scene()->addItem(bundle);
	ui->bundles.insert(index,bundle);
	//bundle->installEventFilter(this);
	
	if ( !ui->effectManager )
		return;
	ui->effectManager->polish(bundle);
	ui->effectManager->invalidate();
}
void QIrDockView::removeBundle( QIrDockletBundle * bundle )
{
	QIR_UI(QIrDockView);
	QGraphicsScene * sc = scene();

	bundle->hide();
	sc->removeItem(bundle);
	ui->bundles.removeAll(bundle);
	ui->effectManager->unpolish(bundle);
	delete bundle;
}
void QIrDockView::evaluatePos( const QPoint & pos )
{
	QIR_UI(QIrDockView);

	if ( !ui->effectManager )
		return;
	QMouseEvent tempEvt(QEvent::MouseMove,pos,Qt::NoButton,0,0);
	ui->effectManager->mouseMoveEvent(&tempEvt);
}

QIR_END_NAMESPACE
