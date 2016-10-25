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
#include <QAction>
#include <QMenu>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QMessageBox>
#include "qirdockeffectmanager.h"
#include "qirdocklet.h"
#include "qirdockletbundle.h"
#include "qirdockview.h"
#include "ui/qirdock_ui.h"
#include "qirdockstyle.h"
#include "qirdockmanager.h"
#include "qirstyleoptiondock.h"
#include "qirdocktiplabel.h"
#include "ui/qirdockview_ui.h"
#include "private/qirdocklet_p.h"
#include <QContextMenuEvent>

QIR_BEGIN_NAMESPACE

//////////////////
//QIrDockUi
//////////////////
QIrDockUi::QIrDockUi() : isDirty(false), marginsDirty(true), tipToShow(false), tipLabel(0), hoverDocklet(0),
lastNonNullHoverDocklet(0), spacing(-1), showTipMode(QIrDock::SpontaneousShowTip), defaultItems(QIrDock::MI_AllMenuEntries)
{
}
void QIrDockUi::createActions()
{
	QIR_W(QIrDock);

	QIrAbstractDockUi::createActions();
	
	actAutoHide = new QAction(QApplication::tr("Auto-hide"),w);
	actAutoHide->setCheckable(true);
	
	actLockDragging = new QAction(QApplication::tr("Lock Dragging"),w);
	actLockDragging->setCheckable(true);
	
	actAreaGroup = new QActionGroup(w);
	
	actTopDockArea = new QAction(QApplication::tr("Top"),w);
	actTopDockArea->setCheckable(true);
	
	actBottomDockArea = new QAction(QApplication::tr("Bottom"),w);
	actBottomDockArea->setCheckable(true);
	
	actRightDockArea = new QAction(QApplication::tr("Right"),w);
	actRightDockArea->setCheckable(true);

	actLeftDockArea = new QAction(QApplication::tr("Left"),w);
	actLeftDockArea->setCheckable(true);
	
	actAreaGroup->addAction(actTopDockArea);
	actAreaGroup->addAction(actBottomDockArea);
	actAreaGroup->addAction(actLeftDockArea);
	actAreaGroup->addAction(actRightDockArea);
	actAreaGroup->setExclusive(true);


	actAlignGroup = new QActionGroup(w);
	actDockLeftAlign = new QAction(QApplication::tr("Align Left"),w);
	actDockLeftAlign->setCheckable(true);
	
	actDockCenterAlign = new QAction(QApplication::tr("Align Center"),w);
	actDockCenterAlign->setCheckable(true);

	actDockRightAlign = new QAction(QApplication::tr("Align Right"),w);
	actDockRightAlign->setCheckable(true);
	
	actAlignGroup->addAction(actDockLeftAlign);
	actAlignGroup->addAction(actDockCenterAlign);
	actAlignGroup->addAction(actDockRightAlign);
	actAlignGroup->setExclusive(true);

	actRemoveEntry = new QAction(QApplication::tr("Remove Entry"),w);
	actEntryProperty = new QAction(QApplication::tr("Entry Property"),w);

	actDockSettings = new QAction(QApplication::tr("Dock Settings..."),w);
}
void QIrDockUi::createMenus()
{	
	QIR_W(QIrDock);

	QIrAbstractDockUi::createMenus();
	menu = new QMenu(w);
	dockMenu = new QMenu(QApplication::tr("This Dock"),w);
	updateDockMenu();
}
void QIrDockUi::init()
{
	QIR_W(QIrDock);

	QIrAbstractDockUi::init();
	QFont font = w->font();
	font.setBold(true);
	
	view = new QIrDockView(w);
	tipLabel = new QIrDockTipLabel(w,w->parentWidget());
	tipLabel->setFont(font);
	tipLabel->hide();
	fadeAnimation = new QPropertyAnimation(tipLabel,"opacity",w);
	fadeAnimation->setDuration(250);
	fadeAnimation->setStartValue(0.0);
	fadeAnimation->setEndValue(1.0);
	fadeAnimation->setEasingCurve(QEasingCurve::Linear);

	updateActionsStates();
}
void QIrDockUi::setupConnections()
{
	QIR_W(QIrDock);

	QIrAbstractDockUi::setupConnections();

	w->connect(actAutoHide,SIGNAL(triggered(bool)),w,SLOT(slotChangeAutoHide(bool)));
	w->connect(actLockDragging,SIGNAL(triggered(bool)),w,SLOT(slotChangeMovable(bool)));
	w->connect(actTopDockArea,SIGNAL(triggered()),w,SLOT(slotChangeArea()));
	w->connect(actBottomDockArea,SIGNAL(triggered()),w,SLOT(slotChangeArea()));
	w->connect(actRightDockArea,SIGNAL(triggered()),w,SLOT(slotChangeArea()));
	w->connect(actLeftDockArea,SIGNAL(triggered()),w,SLOT(slotChangeArea()));
	w->connect(actDockLeftAlign,SIGNAL(triggered()),w,SLOT(slotChangeAlignment()));
	w->connect(actDockCenterAlign,SIGNAL(triggered()),w,SLOT(slotChangeAlignment()));
	w->connect(actDockRightAlign,SIGNAL(triggered()),w,SLOT(slotChangeAlignment()));
	w->connect(actRemoveEntry,SIGNAL(triggered()),w,SLOT(slotRemoveEntry()));
	w->connect(actEntryProperty,SIGNAL(triggered()),w,SLOT(slotEntryProperty()));
	w->connect(actDockSettings,SIGNAL(triggered()),w,SLOT(slotDockSettings()));
	w->connect(fadeAnimation,SIGNAL(finished()),w,SLOT(slotFadeFinished()));
	w->connect(w,SIGNAL(minimumIconSizeChanged( int )),view,SLOT(slotHandleMinimumIconSizeChanged( int )));
	w->connect(w,SIGNAL(maximumIconSizeChanged( int )),view,SLOT(slotHandleMaximumIconSizeChanged( int )));
	w->connect(view,SIGNAL(resetViewRequested()),w,SLOT(slotResetView()));
}
void QIrDockUi::updateActionsStates()
{
	QIR_W(QIrDock);
	
	actAutoHide->setChecked(autoHide);
	actLockDragging->setChecked(!movable);

	actTopDockArea->setChecked(w->dockArea() == QIrDock::DA_TopDockArea);
	actTopDockArea->setEnabled(w->allowedDockAreas() & QIrDock::DA_TopDockArea);

	actBottomDockArea->setChecked(w->dockArea() == QIrDock::DA_BottomDockArea);
	actBottomDockArea->setEnabled(w->allowedDockAreas() & QIrDock::DA_BottomDockArea);

	actRightDockArea->setChecked(w->dockArea() == QIrDock::DA_RightDockArea);
	actRightDockArea->setEnabled(w->allowedDockAreas() & QIrDock::DA_RightDockArea);

	actLeftDockArea->setChecked(w->dockArea() == QIrDock::DA_LeftDockArea);
	actLeftDockArea->setEnabled(w->allowedDockAreas() & QIrDock::DA_LeftDockArea);

	actDockLeftAlign->setChecked(w->dockAlignment() == QIrDock::DAL_DockAlignLeft);
	actDockLeftAlign->setEnabled(w->allowedDockAlignments(w->dockArea()) & QIrDock::DAL_DockAlignLeft);

	actDockCenterAlign->setChecked(w->dockAlignment() == QIrDock::DAL_DockAlignCenter);
	actDockCenterAlign->setEnabled(w->allowedDockAlignments(w->dockArea()) & QIrDock::DAL_DockAlignLeft);

	actDockRightAlign->setChecked(w->dockAlignment() == QIrDock::DAL_DockAlignRight);
	actDockRightAlign->setEnabled(w->allowedDockAlignments(w->dockArea()) & QIrDock::DAL_DockAlignLeft);
}
QMenu * QIrDockUi::getActiveMenu() 
{
	if ( hoverDocklet ) {
		setupMenu();
		if ( !menu->isEmpty() )
			return menu;
	}
	return dockMenu;
}
void QIrDockUi::setupMenu()
{
	QList< QAction * > actions;
	bool noSeparator = true, useDockMenu = true ;
		
	if ( hoverDocklet->isEnabled() ) 
		actions = hoverDocklet->actions();

	menu->clear();
	if ( !actions.isEmpty() ) {
		menu->addActions(actions);
		noSeparator = false;
		useDockMenu = false;
	}
	if ( !dockMenu->isEmpty() ) {
		if ( !noSeparator ) {
			noSeparator = true;
			menu->addSeparator();
		}
		menu->addMenu(dockMenu);
	}
	if ( defaultItems & QIrDock::MI_RemoveEntry ) {
		if ( !noSeparator ) {
			noSeparator = true;
			menu->addSeparator();
		}
		menu->addAction(actRemoveEntry);
		useDockMenu = false;
	}
	if ( defaultItems & QIrDock::MI_EntryProperty ) {
		if ( !noSeparator )
			menu->addSeparator();
		menu->addAction(actEntryProperty);
		useDockMenu = false;
	}
	if ( useDockMenu )
		menu->clear();
}
bool QIrDockUi::shouldRecalculateClosestLegalSize( const QSize & s ) const
{
	if ( !isDirty && lastSizeToAdjust == s ) {
		return false;
	}
	isDirty = false;
	lastSizeToAdjust = s;
	return true;
}
void QIrDockUi::updateDockMenu()
{
	bool noSeparator = true;

	dockMenu->clear();
	if ( defaultItems & QIrDock::MI_AutoHide ) {
		noSeparator = false;
		dockMenu->addAction(actAutoHide);
	}
	if ( defaultItems & QIrDock::MI_LockDragging ) {
		noSeparator = false;
		dockMenu->addAction(actLockDragging);
	}
	if ( defaultItems & QIrDock::MI_DockPosition ) {
		QMenu * positionMenu = dockMenu->addMenu(QApplication::tr("Position"));
		
		positionMenu->addAction(actTopDockArea);
		positionMenu->addAction(actBottomDockArea);
		positionMenu->addAction(actLeftDockArea);
		positionMenu->addAction(actRightDockArea);
		positionMenu->addSeparator();
		positionMenu->addAction(actDockLeftAlign);
		positionMenu->addAction(actDockCenterAlign);
		positionMenu->addAction(actDockRightAlign);
		noSeparator = false;
	}
	if ( defaultItems & QIrDock::MI_DockSettings ) {
		if ( !noSeparator )
			dockMenu->addSeparator();
		dockMenu->addAction(actDockSettings);
	}
}
//////////
//QIrDock
//////////
QIrDock::QIrDock( QWidget * parent ) 
: QIrAbstractDock(parent, * new QIrDockUi)
{	
	QIR_UI(QIrDock);
	ui->setupUi();
	connect(this,SIGNAL(dataChanged(QVariant,QIrAbstractDock::DataRole)),this,SLOT(slotHandleDataChanged(QVariant,QIrAbstractDock::DataRole)),Qt::DirectConnection);
}
QIrDock::~QIrDock()
{	
	QIR_UI(QIrDock);

	subStyle()->unpolish(this);
	foreach (QIrDockletBundle * bundle,ui->view->ui_func()->bundles) {
		if ( bundle ) {
			QIrDocklet * docklet = bundle->docklet();
			if ( docklet ) {
				docklet->p_func()->removing = true;
				delete docklet;
			}
			ui->view->ui_func()->bundles.removeAll(bundle);
			delete bundle;
		}
	}
}
QIrDock::DefaultMenuItems QIrDock::defaultMenuItems() const
{
	QIR_UI(const QIrDock);

	return ui->defaultItems;
}
void QIrDock::setDefaultMenuItems( QIrDock::DefaultMenuItems items )
{
	QIR_UI(QIrDock);

	if ( ui->defaultItems == items )
		return;
	ui->defaultItems = items;
	ui->updateDockMenu();
}
QIrDockEffectManager * QIrDock::effectManager() const
{
	QIR_UI(const QIrDock);

	return ui->view->effectManager();
}
void QIrDock::setEffectManager( QIrDockEffectManager * effect )
{
	QIR_UI(QIrDock);

	ui->view->setEffectManager(effect);
}
QIrDockStyle * QIrDock::dockStyle() const
{
	return qobject_cast< QIrDockStyle * >(subStyle());
}
void QIrDock::evaluateCursorPos()
{
	QIR_UI(QIrDock);
	
	QPoint pos = mapFromGlobal(QCursor::pos());

	if ( ui->view->geometry().contains(pos) ) {
		ui->view->evaluatePos(ui->view->mapFromParent(pos));
	}
}
QIrDockManager * QIrDock::manager() const
{
	return ui_func()->manager;
}
void QIrDock::restore()
{
	QIR_UI(QIrDock);

	ui->view->effectManager()->reset();
}
QIrDockView * QIrDock::view() const 
{
	return ui_func()->view;
}
QSize QIrDock::normalSize() const
{
	return smartDockSize(Normal);
}
QSize QIrDock::sizeHint() const
{
	return smartDockSize(Preferred);
}
QMargins QIrDock::getContentsMargins() const
{
	QIR_UI(const QIrDock);
	static QMargins margins;
	static QIrSubStyle * lastStyle = 0;
	QIrSubStyle * style = subStyle();

	if ( lastStyle != style || ui->marginsDirty ) {
		QIrStyleOptionDock option;

		lastStyle = style;
		ui->marginsDirty = false;
		initStyleOption(&option);
		margins.setTop(style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockLayoutTopMargin,&option));
		margins.setBottom(style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockLayoutBottomMargin,&option));
		margins.setLeft(style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockLayoutLeftMargin,&option));
		margins.setRight(style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockLayoutRightMargin,&option));
	}
	return margins;
}
QSize QIrDock::smartDockSize( QIrDock::SizeType st ) const
{
	int topMargin,
		leftMargin,
		bottomMargin,
		rightMargin;
	QSize s(st == Normal ? view()->effectManager()->normalContentRect().size().toSize():st == Preferred ? view()->sizeHint() : view()->boundingSize());
	QMargins margins = getContentsMargins();

	topMargin = margins.top();
	leftMargin = margins.left();
	bottomMargin = margins.bottom();
	rightMargin = margins.right();

	s.rwidth() += leftMargin + rightMargin;
	s.rheight() += topMargin + bottomMargin;
	
	return s;
}
QSize QIrDock::closestAcceptableSize( const QSize & size )
{
	QIR_UI(QIrDock);
	
	int viewWidth;
	int topMargin,
		leftMargin,
		bottomMargin,
		rightMargin;
	QMargins margins = getContentsMargins();
	
	topMargin = margins.top();
	leftMargin = margins.left();
	bottomMargin = margins.bottom();
	rightMargin = margins.right();
	
	
	if ( orientation() == Qt::Horizontal ) 
		viewWidth = size.width() - leftMargin - rightMargin;
	else
		viewWidth = size.height() - topMargin - bottomMargin;
			
	if ( viewWidth <= 0 )
		return size;
	if ( orientation() == Qt::Horizontal ) {
		QSize sh = sizeHint();
		if ( sh.width() <= size.width() ) {
			overrideMinimumIconSize(-1);
			ui->lastSizeAdjusted = size;
		} else {
			overrideMinimumIconSize(ui->view->effectManager()->closestLegalIconSize(size - QSize(leftMargin + rightMargin,0)));
			ui->lastSizeAdjusted = smartDockSize(Bounding);
		}
	} else {
		if ( sizeHint().height() <= size.height() ) {
			overrideMinimumIconSize(-1);
			ui->lastSizeAdjusted = size;
		} else {
			overrideMinimumIconSize(ui->view->effectManager()->closestLegalIconSize(size - QSize(0,topMargin + bottomMargin)));
			ui->lastSizeAdjusted = smartDockSize(Bounding);
		}
	}

	return ui->lastSizeAdjusted;
}
void QIrDock::parentChanged()
{
	QIR_UI(QIrDock);
	
	if ( ui->tipLabel ) {
		QWidget * parent = parentWidget();

		if ( ui->tipLabel->parentWidget() != parent ) {
			ui->tipLabel->setParent(parent);
		}
	}

}
void QIrDock::slotPopupMenu( bool fromView )
{
	QIR_UI(QIrDock);

	setDockState(QIrDock::DS_MenuPoppedUp);
	QMenu * menu;
	
	if ( fromView )
		menu = ui->getActiveMenu();
	else
		menu = ui->dockMenu;
	if ( !menu->isEmpty() ) {
		connect(menu,SIGNAL(aboutToShow()),this,SLOT(slotHandleMenuAboutToShow()));
		connect(menu,SIGNAL(aboutToHide()),this,SLOT(slotHandleMenuAboutToHide()));
		menu->exec(QCursor::pos());
		disconnect(menu,SIGNAL(aboutToShow()),this,SLOT(slotHandleMenuAboutToShow()));
		disconnect(menu,SIGNAL(aboutToHide()),this,SLOT(slotHandleMenuAboutToHide()));
	}
	if ( dockState() == DS_MenuPoppedUp )
		setDockState(DS_Shown);
}
void QIrDock::insertDocklet(QIrDocklet * before, QIrDocklet * docklet)
{ 
	QIR_UI(QIrDock);
	QIrDockletBundle * bundle;
	QIrDockStyle * style = dockStyle();
	int index = -1;

	if ( !docklet )
		return;
	if ( indexOf(docklet) != -1 || (before && ((index = indexOf(before)) == -1)))
		return;
	index++;
	ownDocklet(docklet);
	
	bundle = new QIrDockletBundle(ui->view,docklet);
	ui->view->insertBundle(index,bundle);
	if ( style )
		style->polish(bundle);
	ui->isDirty = true;
	//resetView();	
}
void QIrDock::resetView()
{
	QIR_UI(QIrDock);
	bool oldState = ui->view->effectManager()->useAnimation();

	if ( isHidden() || ui->firstTimeShown )
		return;
	//ui->view->blockEventsToEffect(true);
	ui->view->effectManager()->setUseAnimation(false);
	ui->view->effectManager()->clearHover();
	ui->manager->updateNormalDockGeometry();
	setExtended(false);
	slotUpdateGeometry();
	//ui->view->blockEventsToEffect(false);
	ui->view->update();//effectManager()->invalidate();
	evaluateCursorPos();
	view()->effectManager()->invalidate();
	

	ui->view->effectManager()->setUseAnimation(oldState);	
}
QIrDock::ShowTipMode QIrDock::showTipMode() const
{
	return ui_func()->showTipMode;
}
void QIrDock::setShowTipMode( QIrDock::ShowTipMode mode )
{
	ui_func()->showTipMode = mode;
}
int QIrDock::count() const
{
	QIR_UI(const QIrDock);

	return ui->view->bundles(false).count();
}
QIrDocklet * QIrDock::dockletAt( int index ) const
{
	QIR_UI(const QIrDock);
	QList< QIrDockletBundle * > bundles = ui->view->bundles(false);
	QIrDockletBundle * bundle;
	int c = bundles.count();

	if (index < 0 || index >= c)
		return 0;
	bundle = bundles[index];
	if ( bundle )
		return bundle->docklet();
	return 0;
}
QIrDocklet * QIrDock::takeAt( int index )
{
	QIR_UI(QIrDock);
	QList< QIrDockletBundle * > bundles = ui->view->bundles(false);
	QIrDockletBundle * bundle;
	QIrDockStyle * style = dockStyle();
	int c = bundles.count();

	if (index < 0 || index >= c)
		return 0;
	bundle = bundles[index];
	if ( bundle ) {
		QIrDocklet * docklet = bundle->docklet();

		if ( style ) {
			style->unpolish(bundle);			
		}
		ui->view->removeBundle(bundle);
		slotUpdateTip();
		releaseDocklet(docklet);
		return docklet;
	}
	return 0;
}
void QIrDock::subStyleChanged()
{
	QIR_UI(QIrDock);
	QStyleHintReturnVariant tipFont;

	subStyle()->styleHint((QStyle::StyleHint)QIrDockStyle::SH_DockTipFont,0,0,&tipFont);
	ui->tipLabel->setFont(tipFont.variant.value< QFont >());
	ui->view->effectManager()->invalidate();//evaluateCursorPos();
	slotUpdateGeometry();
}

QIrSubStyle * QIrDock::defaultSubStyle() const
{
	return new QIrDefaultDockStyle;
}
void QIrDock::contextMenuEvent( QContextMenuEvent * )
{
	if (dockState() == QIrDock::DS_Dragging || dockState() == QIrDock::DS_Dropping)
		return;
	slotPopupMenu(false);
}
void QIrDock::mousePressEvent( QMouseEvent * evt )
{
	QIR_UI(QIrDock);

	if ( evt->button() == Qt::LeftButton ) {
		ui->lastCursPos = evt->globalPos();
	}
}
void QIrDock::mouseMoveEvent( QMouseEvent * evt )
{
	QIR_UI(QIrDock);
	QPoint offset = ui->lastCursPos;
	QIrAbstractDock::DockState state = dockState();
	 
	if ( evt->buttons() & Qt::LeftButton ) {
		 offset = evt->globalPos() - offset;
		 if (isMovable() && !isExtended()) {
			 if ( state != QIrAbstractDock::DS_Dragging ) {
				 if ( offset.manhattanLength() > QApplication::startDragDistance() ) {
					 startDrag();
					 move(pos() + offset);
				 }
			 } else {
				 move(pos() + offset);
				 ui->lastCursPos = evt->globalPos();
			 }
		 }
	}	
}
void QIrDock::mouseReleaseEvent( QMouseEvent * evt )
{
	if ( evt->button() == Qt::LeftButton )
		if ( dockState() == QIrAbstractDock::DS_Dragging )
			endDrag();
}
void QIrDock::slotHandleDataChanged(const QVariant &, QIrAbstractDock::DataRole role)
{
	QIR_UI(QIrDock);
	bool oldState;

	ui->isDirty = true;
	switch ( role ) {
		case QIrAbstractDock::DR_Area :
		case QIrAbstractDock::DR_Alignment :
			setDockState(DS_Hidden);
			ui->marginsDirty = true;
			oldState = ui->view->effectManager()->useAnimation();
			ui->view->effectManager()->setUseAnimation(false);
			ui->view->effectManager()->reset();
			ui->view->effectManager()->setUseAnimation(oldState);
			foreach (QIrDockletBundle * bundle, ui->view->bundles(false)) {
				bundle->invalidate();
			}
			ui->view->adjustLayers();		
		case QIrAbstractDock::DR_AutoHide :
		case QIrAbstractDock::DR_AllowedAlignments :
		case QIrAbstractDock::DR_AllowedAreas :
		case QIrAbstractDock::DR_Movable :
			ui->updateActionsStates();
			break;
		default:
			break;
	}
}
void QIrDock::handleHoverDockletChanged(QIrDocklet * current, QIrDocklet * previous)
{
	Q_UNUSED(previous);
	QIR_UI(QIrDock);

	if ( ui->hoverDocklet ) {
		disconnect(ui->hoverDocklet,SIGNAL(textChanged( QString )),this,SLOT(slotUpdateTip()));
		disconnect(ui->hoverDocklet,SIGNAL(moved( QPointF )),this,SLOT(slotUpdateTip()));
		disconnect(ui->view->bundle(ui->hoverDocklet),SIGNAL(geometryChanged()),this,SLOT(slotUpdateTip()));
	}
	ui->hoverDocklet = current;
	if ( ui->hoverDocklet ) {
		ui->lastNonNullHoverDocklet = ui->hoverDocklet;
		if ( ui->hoverDocklet->isSeparator() )
			hideTip();
		else {
			ui->tipToShow = true;
			if ( showTipMode() == QIrDock::SpontaneousShowTip ) {
				slotUpdateTip();
			}
		}
		connect(ui->hoverDocklet,SIGNAL(textChanged( QString )),this,SLOT(slotUpdateTip()));
		connect(ui->hoverDocklet,SIGNAL(moved( QPointF )),this,SLOT(slotUpdateTip()));
		connect(ui->view->bundle(ui->hoverDocklet),SIGNAL(geometryChanged()),this,SLOT(slotUpdateTip()));
		emit hovered(ui->hoverDocklet);
	} else {
		hideTip();
	}
}
void QIrDock::slotHandleExtendRequest()
{
	QIR_UI(QIrDock);

	setExtended(true);
	slotUpdateGeometry();
	if ( ui->hoverDocklet )
		showTip();
}
void QIrDock::slotHandleNormalizeRequest()
{
	QIR_UI(QIrDock);

	setExtended(false);
	slotUpdateGeometry();
	if ( !ui->hoverDocklet )
		hideTip();
}
void QIrDock::resizeEvent( QResizeEvent * )
{
	doLayout();
}
void QIrDock::initStyleOption( QIrStyleOptionDock * option ) const
{
	int w = normalDockGeometry().width();
	int h = normalDockGeometry().height();
	QRect r = rect();

	option->initFrom(this);
	
	switch ( dockArea() ) {
		case QIrAbstractDock::DA_BottomDockArea :
			r.setHeight(h);
			r.moveBottom(rect().bottom());
			break;
		case QIrAbstractDock::DA_TopDockArea :
			r.setHeight(h);
			r.moveTop(rect().top());
			break;
		case QIrAbstractDock::DA_LeftDockArea :
			r.setWidth(w);
			r.moveLeft(rect().left());
			break;
		default:
			r.setWidth(w);
			r.moveRight(rect().right());
			break;
	}
	option->area = dockArea();
	option->alignment = dockAlignment();
	option->icon = icon();
	option->title = title();
	option->autoHide = autoHide();
	option->orientation = orientation();
	option->frameRect = r;
}
void QIrDock::paintEvent( QPaintEvent * )
{
	QPainter painter(this);
	QIrStyleOptionDock option;
	QIrSubStyle * style = subStyle();

	if ( !style )
		return;
	initStyleOption(&option);
	style->drawPrimitive((QStyle::PrimitiveElement)QIrDockStyle::PE_PanelDock,&option,&painter,this);
}
void QIrDock::doLayout()
{
	QIR_UI(QIrDock);
	int topMargin,
		leftMargin,
		bottomMargin,
		rightMargin,
		w = width();
	QMargins margins = getContentsMargins();
	QRect normalGeom(QPoint(),
		(orientation() == Qt::Horizontal ? QSize(w,ui->view->normalSize()) : QSize(ui->view->normalSize(),w)));
	QRect extendGeom = rect();
	
	topMargin = margins.top();
	leftMargin = margins.left();
	bottomMargin = margins.bottom();
	rightMargin = margins.right();


	normalGeom = rect();
	if ( orientation() == Qt::Horizontal )
		normalGeom.setHeight(ui->view->normalSize() + topMargin + bottomMargin);
	else
		normalGeom.setWidth(ui->view->normalSize() + leftMargin + rightMargin);

	normalGeom.adjust(leftMargin,topMargin,-rightMargin,-bottomMargin);
	if ( isExtended() ) {
		extendGeom.adjust(leftMargin,topMargin,-rightMargin,-bottomMargin);
	} else 
		extendGeom = normalGeom;
	view()->setGeometry(extendGeom);
}
QPoint QIrDock::extendOffset() const
{
	QIR_UI(const QIrDock);

	if ( orientation() == Qt::Horizontal )
		return QPoint(ui->view->extendOffset(),0);
	else
		return QPoint(0,ui->view->extendOffset());

}
void QIrDock::showTip()
{
	QIR_UI(QIrDock);

	if (!ui->hoverDocklet || (ui->hoverDocklet && ui->hoverDocklet->isSeparator()))
		return;
	ui->tipToShow = true;
}
void QIrDock::hideTip()
{
	fadeTipOut();
}
QRect QIrDock::visualRect( QIrDocklet * docklet ) const
{
	QIR_UI(const QIrDock);

	if ( indexOf(docklet) == -1 )
		return QRect();
	if ( !docklet->isVisible() )
		return QRect();
	QRectF geom = docklet->geometry();
	QRect ret(QPoint(),geom.size().toSize());

	ret.moveTo(mapToParent(ui->view->mapToParent(ui->view->mapFromScene(docklet->parentItem()->mapToScene(geom.topLeft())))));

	return ret;
}
void QIrDock::slotHandleMenuAboutToShow()
{
	QIR_UI(QIrDock);

	ui->view->blockEventsToEffect(true);
}
void QIrDock::slotHandleMenuAboutToHide()
{
	QIR_UI(QIrDock);

	ui->view->blockEventsToEffect(false);
	if ( dockState() == DS_MenuPoppedUp )
		setDockState(DS_Shown);
}
void QIrDock::slotFadeFinished()
{
	QIR_UI(QIrDock);

	if ( !ui->fadeIn ) {
		ui->tipLabel->hide();
	} else {
		ui->tipToShow = false;
	}
}
void QIrDock::fadeTipIn()
{	
	QIR_UI(QIrDock);
	bool pause = false;
	
	if ( ui->fadeAnimation->state() == QPropertyAnimation::Running ) {
		if ( ui->fadeIn )
			return;
		ui->fadeAnimation->pause();
		pause = true;
	} else {
		if ( !ui->tipLabel->isHidden() ) {
			ui->tipToShow = false;
			return;
		}
	}
	ui->fadeIn = true;
	ui->tipLabel->show();
	ui->tipLabel->raise();
	ui->fadeAnimation->setDirection(QPropertyAnimation::Forward);
	if ( pause )
		ui->fadeAnimation->resume();
	else
		ui->fadeAnimation->start();
}
void QIrDock::fadeTipOut()
{	
	QIR_UI(QIrDock);
	bool pause = false;

	ui->tipToShow = false;
	if ( ui->fadeAnimation->state() == QPropertyAnimation::Running ) {
		if ( !ui->fadeIn )
			return;
		pause = true;
		ui->fadeAnimation->pause();
	} 
	ui->fadeIn = false;
	ui->tipLabel->raise();
	ui->fadeAnimation->setDirection(QPropertyAnimation::Backward);
	if ( pause )
		ui->fadeAnimation->resume();
	else
		ui->fadeAnimation->start();
}
void QIrDock::slotUpdateTip()
{
	QIR_UI(QIrDock);

	QSize sz;
	QRect geom = geometry(),
		parentRect = parentWidget()->rect(),
		tipGeom;
	QRect dockletVisualRect = visualRect(ui->hoverDocklet);

	//try {
		if (!ui->hoverDocklet || (ui->hoverDocklet && ui->hoverDocklet->isSeparator())) {
			fadeTipOut();
			return;
		}
		if ( ui->tipToShow ) {
			fadeTipIn();
		}
		if (ui->tipLabel->isHidden() || !dockletVisualRect.isValid())
			return;
		if ( ui->hoverDocklet )
			ui->tipLabel->setText(ui->hoverDocklet->text());
		else {
			fadeTipOut();
			return;
		}
		
		ui->tipLabel->setTargetVisualRect(dockletVisualRect);
		tipGeom.setSize(ui->tipLabel->sizeHint());	
		
		tipGeom.moveCenter(dockletVisualRect.center());

		switch ( dockArea() ) {
			case QIrAbstractDock::DA_BottomDockArea :
                                tipGeom.moveBottom((int)(geom.top() + ui->hoverDocklet->pos().y()));
				tipGeom.moveLeft(qMax(tipGeom.left() ,parentRect.left()));
				tipGeom.moveRight(qMin(tipGeom.right(),parentRect.right()));
				break;
			case QIrAbstractDock::DA_TopDockArea :
                                tipGeom.moveTop(int(geom.bottom() + ui->hoverDocklet->pos().y()));
				tipGeom.moveLeft(qMax(tipGeom.left(),parentRect.left()));
				tipGeom.moveRight(qMin(tipGeom.right(),parentRect.right()));
				break;
			case QIrAbstractDock::DA_LeftDockArea :
                                tipGeom.moveLeft((int)(geom.right() + ui->hoverDocklet->pos().x()));
				break;
			default :
                                tipGeom.moveRight((int)(geom.left() + ui->hoverDocklet->pos().x()));
				break;
		}
		ui->tipLabel->setGeometry(tipGeom);
		ui->tipLabel->update();
/*	} catch(...) {
		fadeTipIn();
	}*/

}
void QIrDock::slotChangeAutoHide( bool autoHide )
{
	setAutoHide(autoHide);
}
void QIrDock::slotChangeMovable( bool locked )
{
	setMovable(!locked);
}
void QIrDock::slotChangeArea()
{
	QIR_UI(QIrDock);

	QAction * action = qobject_cast< QAction * >(sender());

	if ( !action )
		return;

	if ( action == ui->actBottomDockArea ) {
		setDockArea(QIrDock::DA_BottomDockArea);
	} else if ( action == ui->actTopDockArea ) {
		setDockArea(QIrDock::DA_TopDockArea);
	} else if ( action == ui->actLeftDockArea ) {
		setDockArea(QIrDock::DA_LeftDockArea);
	} else if ( action == ui->actRightDockArea ) {
		setDockArea(QIrDock::DA_RightDockArea);
	}
}
void QIrDock::slotChangeAlignment()
{
	QIR_UI(QIrDock);

	QAction * action = qobject_cast< QAction * >(sender());

	if ( !action )
		return;

	if ( action == ui->actDockLeftAlign ) {
		setDockAlignment(QIrDock::DAL_DockAlignLeft);
	} else if ( action == ui->actDockCenterAlign ) {
		setDockAlignment(QIrDock::DAL_DockAlignCenter);
	} else if ( action == ui->actDockRightAlign) {
		setDockAlignment(QIrDock::DAL_DockAlignRight);
	}
}
void QIrDock::slotDockSettings()
{
	QMessageBox::information(0,"Dock Settings","Not available yet. Soon!!!");
}
void QIrDock::slotRemoveEntry()
{
	QIR_UI(QIrDock);

	if ( ui->lastNonNullHoverDocklet ) {
		removeDocklet(ui->lastNonNullHoverDocklet);
	} 
}
void QIrDock::slotEntryProperty()
{
	QMessageBox::information(0,"Entry Property","Not available yet. Soon!!!");
}

QIR_END_NAMESPACE
