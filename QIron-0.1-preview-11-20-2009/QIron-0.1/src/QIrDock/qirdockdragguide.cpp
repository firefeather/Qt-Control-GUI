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
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPointer>
#include "qirdockstyle.h"
#include "private/qirdockmanager_p.h"
#include "qirdockdragguide.h"
#include "../Common/private/qirobject_p.h"
#include "../Common/ui/qirwidget_ui.h"

QIR_BEGIN_NAMESPACE

class QIrDockDragGuideUi : public QIrWidgetUi
{
	QIR_DECLARE_WIDGET(QIrDockDragGuide);

public:
        QIrDockDragGuideUi() : currentRegion(QIrDockDragGuide::IR_None),
        opacity(1.0),
        align(QIrAbstractDock::DAL_NoDockAlignment) { }
	~QIrDockDragGuideUi() { }

	QRect rectForRegion(QIrDockDragGuide::IndicatorRegion reg) const;
	QIrAbstractDock::DockAlignment alignForRegion(QIrDockDragGuide::IndicatorRegion reg) const;

	QIrDockDragGuide::IndicatorRegion currentRegion;
	qreal opacity;
	bool collapse,
		horizontal;
	QIrDockDragGuideManager * manager;
	QIrAbstractDock::DockArea area;
	QIrAbstractDock::DockAlignment align;
	QPropertyAnimation * geomAnimation,
		* opacityAnimation;

protected:
	void init();
	void setupConnections();
};

void QIrDockDragGuideUi::init()
{
	QIR_W(QIrDockDragGuide);

	w->setMouseTracking(true);
	geomAnimation = new QPropertyAnimation(w,"geometry",w);
	opacityAnimation = new QPropertyAnimation(w,"opacity",w);
	horizontal = (area == QIrAbstractDock::DA_BottomDockArea || area == QIrAbstractDock::DA_TopDockArea);
	geomAnimation->setDuration(200);
	opacityAnimation->setDuration(200);
	opacityAnimation->setStartValue(0.0);
	opacityAnimation->setEndValue(1.0);
}
void QIrDockDragGuideUi::setupConnections()
{
	QIR_W(QIrDockDragGuide);

	w->connect(opacityAnimation,SIGNAL(finished()),w,SLOT(handleAnimationFinished()));
}
QIrAbstractDock::DockAlignment QIrDockDragGuideUi::alignForRegion( QIrDockDragGuide::IndicatorRegion reg ) const
{
	switch ( reg ) {
		case QIrDockDragGuide::IR_LeftIndicator:
			return QIrAbstractDock::DAL_DockAlignLeft;
		case QIrDockDragGuide::IR_CenterIndicator:
			return QIrAbstractDock::DAL_DockAlignCenter;
		case QIrDockDragGuide::IR_RightIndicator:
			return QIrAbstractDock::DAL_DockAlignRight;
		default:
			return QIrAbstractDock::DAL_NoDockAlignment;
	}
}
QRect QIrDockDragGuideUi::rectForRegion(QIrDockDragGuide::IndicatorRegion reg) const
{
	QIR_W(const QIrDockDragGuide);
	QIrSubStyle * style = w->dockSubStyle();
	QIrStyleOptionDockDragGuide option;
	int wd, s;

	if ( !style )
		return QRect();
	w->initStyleOption(&option);
	wd = style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockDragGuideIndicatorWidth,&option,w);
	s =  style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockDragGuideSpacing,&option,w);
	QRect r(0,0,wd,wd);
	
	r.moveCenter(option.rect.center());
	switch ( reg ) {
		case QIrDockDragGuide::IR_LeftIndicator:
			if ( horizontal )
				r.moveLeft(r.left() - (s + wd));
			else
				r.moveTop(r.top() - (s + wd));
			break;
		case QIrDockDragGuide::IR_RightIndicator:
			if ( horizontal )
				r.moveRight(r.right() + (s + wd));
			else
				r.moveBottom(r.bottom() + (s + wd));
			break;
		default:
			break;
	}
	return r;
}


//////////////////////
//QIrDockDragGuide
//////////////////////
QIrDockDragGuide::QIrDockDragGuide(QIrAbstractDock::DockArea area, QIrDockDragGuideManager * manager, QWidget * parent)
: QWidget(parent), QIrWidget(* new QIrDockDragGuideUi)
{
	QIR_UI(QIrDockDragGuide);

	ui->manager = manager;
	ui->area = area;
	ui->setupUi();	
}
QIrAbstractDock::DockArea QIrDockDragGuide::dockArea() const
{
	QIR_UI(const QIrDockDragGuide);

	return ui->area;
}
QIrAbstractDock::DockAlignment QIrDockDragGuide::dockAlignment() const
{
	QIR_UI(const QIrDockDragGuide);

	return ui->align;
}
QIrSubStyle * QIrDockDragGuide::dockSubStyle() const
{
	QIR_UI(const QIrDockDragGuide);
	QIrAbstractDock * dock = ui->manager->dockManager()->dock();

	if ( !dock )
		return 0;
	return dock->subStyle();
}
QIrDockDragGuideManager * QIrDockDragGuide::manager() const
{
	QIR_UI(const QIrDockDragGuide);

	return ui->manager;
}
qreal QIrDockDragGuide::opacity() const
{
	QIR_UI(const QIrDockDragGuide);

	return ui->opacity;
}
void QIrDockDragGuide::setOpacity( qreal o )
{
	QIR_UI(QIrDockDragGuide);

	if ( qFuzzyCompare(ui->opacity,o) )
		return;
	ui->opacity = o;
	update();
}
QSize QIrDockDragGuide::sizeHint() const
{
	QIR_UI(const QIrDockDragGuide);
	QIrSubStyle * style = dockSubStyle();
	QIrStyleOptionDockDragGuide option;
	int w, h;

	if ( !style )
		return QSize();
	h = style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockDragGuideIndicatorWidth,&option,this);
	w = 3 * h;
	w +=  2 * style->pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockDragGuideSpacing,&option,this);
	if ( !ui->horizontal )
		qSwap(w,h);
	return style->sizeFromContents((QStyle::ContentsType)QIrDockStyle::CT_DockDragGuide,&option,QSize(w,h),this);
}
void QIrDockDragGuide::setAnimationGeometry(const QRect & start, const QRect & end)
{
	QIR_UI(QIrDockDragGuide);

	ui->geomAnimation->setStartValue(start);
	ui->geomAnimation->setEndValue(end);
}
void QIrDockDragGuide::deploy()
{	
	QIR_UI(QIrDockDragGuide);

	init();
	show();
	raise();
	ui->collapse = false;
	ui->geomAnimation->setDirection(QAbstractAnimation::Forward);
	ui->opacityAnimation->setDirection(QAbstractAnimation::Forward);
	if ( ui->manager->useAnimation() ) {
		ui->geomAnimation->start();
		ui->opacityAnimation->start();
	} else {
		ui->opacity = 1.0;
		setGeometry(ui->geomAnimation->endValue().toRect());
		update();
	}
}
void QIrDockDragGuide::undeploy()
{
	QIR_UI(QIrDockDragGuide);

	ui->collapse = true;
	ui->geomAnimation->setDirection(QAbstractAnimation::Backward);
	ui->opacityAnimation->setDirection(QAbstractAnimation::Backward);
	if ( ui->manager->useAnimation() ) {
		ui->geomAnimation->start();
		ui->opacityAnimation->start();
	} else {
		ui->opacity = 0.0;
		setGeometry(ui->geomAnimation->startValue().toRect());
		hide();
	}

}
void QIrDockDragGuide::handleAnimationFinished()
{
	QIR_UI(QIrDockDragGuide);

	if ( ui->collapse )
		hide();
}
QString QIrDockDragGuide::widgetGroupKey() const 
{
	return qiron_dock_drag_elements;
}
void QIrDockDragGuide::initStyleOption( QIrStyleOptionDockDragGuide * option ) const
{
	option->init(this);
	option->area = dockArea();
}
void QIrDockDragGuide::initStyleOption(QIrStyleOptionDockDragGuideIndicator * option,IndicatorRegion reg) const
{
	QIR_UI(const QIrDockDragGuide);

	QIrAbstractDock * dock = ui->manager->dockManager()->dock();
	QIrAbstractDock::DockAlignment align = ui->alignForRegion(reg);

	bool enabled = ((dock->allowedDockAreas() & ui->area) && (dock->allowedDockAlignments(ui->area) & align));

	option->rect = ui->rectForRegion(reg);
	option->state |= QStyle::State_Enabled;
	
	option->area = dockArea();
	option->alignment = ui->alignForRegion(reg);

	if ( enabled )
		option->state |= QStyle::State_Enabled;
	else
		option->state &= ~QStyle::State_Enabled;
			
	if ( (reg == ui->currentRegion) & enabled )
		option->state |= QStyle::State_MouseOver;
	else
		option->state &= ~QStyle::State_MouseOver;
}
void QIrDockDragGuide::enterEvent ( QEvent * )
{
	update();
}
void QIrDockDragGuide::leaveEvent( QEvent * )
{
	QIR_UI(QIrDockDragGuide);

	emit alignmentChanged(ui->alignForRegion(IR_None),ui->alignForRegion(ui->currentRegion));
	update();
}
void QIrDockDragGuide::mouseMoveEvent( QMouseEvent * evt )
{
	QIR_UI(QIrDockDragGuide);
	bool hover = false;
	QIrAbstractDock * dock = ui->manager->dockManager()->dock();
	
	for (int i = IR_LeftIndicator; i <= IR_RightIndicator; i++) {
		IndicatorRegion reg = (IndicatorRegion)i;
		if ( ui->rectForRegion(reg).contains(evt->pos()) && (dock->allowedDockAlignments(ui->area) & ui->alignForRegion(reg))) {
			if ( ui->currentRegion != reg ) {
				IndicatorRegion oldReg = ui->currentRegion;
				ui->currentRegion = reg;
				emit alignmentChanged(ui->alignForRegion(ui->currentRegion),ui->alignForRegion(oldReg));
				update();
			}
			hover = true;
			break;
		}
	}
	if ( !hover ) {
		if ( ui->currentRegion != IR_None ) {
			IndicatorRegion oldReg = ui->currentRegion;
			ui->currentRegion = IR_None;
			emit alignmentChanged(ui->alignForRegion(ui->currentRegion),ui->alignForRegion(oldReg));
		}
	}
}
void QIrDockDragGuide::mouseReleaseEvent( QMouseEvent * )
{
	QIR_UI(QIrDockDragGuide);

	ui->currentRegion = IR_None;
	update();
}
void QIrDockDragGuide::paintEvent( QPaintEvent * )
{
	QIrSubStyle * style = dockSubStyle();
	QIrStyleOptionDockDragGuide option;
	QIrStyleOptionDockDragGuideIndicator indicOption;	
	QPainter painter(this);

	if ( !style )
		return;
	initStyleOption(&option);
	style->drawPrimitive((QStyle::PrimitiveElement)QIrDockStyle::PE_FrameDockDragGuide,&option,&painter,this);
	for (int i = QIrDockDragGuide::IR_LeftIndicator; i <= QIrDockDragGuide::IR_RightIndicator; i++) {
		IndicatorRegion reg = (IndicatorRegion)i;
		initStyleOption(&indicOption,reg);
		style->drawPrimitive((QStyle::PrimitiveElement)QIrDockStyle::PE_DockDragGuideIndicator,&indicOption,&painter,this);
	}
}
void QIrDockDragGuide::showEvent( QShowEvent * )
{ 
	QIR_UI(QIrDockDragGuide);
	
	ui->currentRegion = IR_None; 
}
void QIrDockDragGuide::init()
{
	QIR_UI(QIrDockDragGuide);
	QIrAbstractDock * dock = ui->manager->dockManager()->dock();

	setEnabled(dock->allowedDockAreas() & ui->area);
}

/////////////////////////////////
//QIrDockDragGuideManagerPrivate
/////////////////////////////////
class QIrDockDragGuideManagerPrivate : public QIrObjectPrivate
{
	QIR_DECLARE_OBJECT(QIrDockDragGuideManager)

public:
	QIrDockDragGuideManagerPrivate();
	~QIrDockDragGuideManagerPrivate();
	
	QPointer< QIrDockDragGuide > currentGuide; 
	QList< QPointer< QIrDockDragGuide > > guides;
	bool useAnimation;

protected:
	void init();
};

QIrDockDragGuideManagerPrivate::QIrDockDragGuideManagerPrivate() : currentGuide(0), useAnimation(false)
{
}
QIrDockDragGuideManagerPrivate::~QIrDockDragGuideManagerPrivate()
{
}
void QIrDockDragGuideManagerPrivate::init()
{
	QIR_O(QIrDockDragGuideManager);
	QIrAbstractDock::DockArea areas[] = {QIrAbstractDock::DA_TopDockArea,QIrAbstractDock::DA_LeftDockArea,
		QIrAbstractDock::DA_BottomDockArea,QIrAbstractDock::DA_RightDockArea};
	QIrDockManager * dm = o->dockManager();

	for (int i = 0; i < 4; i++) {
		QIrDockDragGuide * guide = new QIrDockDragGuide(areas[i],o,dm->managed());
		
		guides.append(guide);
		QObject::connect(guide,SIGNAL(alignmentChanged(QIrAbstractDock::DockAlignment,QIrAbstractDock::DockAlignment)),
			dm,SLOT(slotHandleDragAlignChanged(QIrAbstractDock::DockAlignment,QIrAbstractDock::DockAlignment)));
		guide->hide();
	}
}

///////////////////////////
//QIrDockDragGuideManager
///////////////////////////
QIrDockDragGuideManager::QIrDockDragGuideManager( QIrDockManager * manager )
: QObject(manager), QIrObject(* new QIrDockDragGuideManagerPrivate)
{	
	QIR_P(QIrDockDragGuideManager);

	p->setup();
}
QIrDockDragGuideManager::~QIrDockDragGuideManager()
{
	QIR_P(QIrDockDragGuideManager);

	foreach (QIrDockDragGuide * guide, p->guides) {
		if ( guide ) {
			guide->hide();
			delete guide;
		}
	}
}
QIrDockManager * QIrDockDragGuideManager::dockManager() const
{
	return qobject_cast< QIrDockManager * >(parent());
}
void QIrDockDragGuideManager::updateManaged( QWidget * w )
{
	QIR_P(QIrDockDragGuideManager);

	foreach (QIrDockDragGuide * guide, p->guides) {
		if (guide && guide->parent() != w) {
			guide->setParent(w);
		}
	}
}
void QIrDockDragGuideManager::deploy()
{
	QIR_P(QIrDockDragGuideManager);
	QRect startGeom, endGeom, r;
	QWidget * managed = dockManager()->managed();

	try {

	if ( !managed )
		return;
	r = managed->rect();

	foreach (QIrDockDragGuide * guide, p->guides) {
		int s = qMin(guide->sizeHint().width(),guide->sizeHint().height());
		
		startGeom.setSize(QSize(s,s));
		startGeom.moveCenter(r.center());
		endGeom = getGuideGeometry(guide);
		guide->setAnimationGeometry(startGeom,endGeom);
		guide->deploy();
	}
	} catch(...) {
	}
}
void QIrDockDragGuideManager::undeploy()
{
	QIR_P(QIrDockDragGuideManager);

	foreach (QIrDockDragGuide * guide, p->guides) {
			guide->undeploy();
	}
}
QRect QIrDockDragGuideManager::getGuideGeometry( QIrDockDragGuide * guide ) const
{
	QRect re;
	QSize s = guide->sizeHint();
	QRect r(QPoint(),s);
	QWidget * managed = dockManager()->managed();

	if ( !managed )
		return QRect();
	re = managed->rect();
	r.moveCenter(re.center());

	switch ( guide->dockArea() ) {
		case QIrAbstractDock::DA_TopDockArea:
			r.moveTop(re.top() + 30);			
			break;
		case QIrAbstractDock::DA_LeftDockArea:
			r.moveLeft(re.left() + 30);
			break;
		case QIrAbstractDock::DA_RightDockArea:
			r.moveRight(re.right() - 30);
			break;
		case QIrAbstractDock::DA_BottomDockArea:
			r.moveBottom(re.bottom() - 30);
			break;
		default:
			break;
	}
	return r;
}
bool QIrDockDragGuideManager::eventFilter(QObject * obj, QEvent * evt)
{
	QIR_P(QIrDockDragGuideManager);
	QIrAbstractDock * dock = dockManager()->dock();

	if (dock != obj || dock->dockState() != QIrAbstractDock::DS_Dragging)
		return QObject::eventFilter(obj,evt);
	switch ( evt->type() ) {
		case QEvent::MouseMove:
			{
				QMouseEvent * mouseEvt = static_cast< QMouseEvent * >(evt);
				QIrDockDragGuide * guide = guideAt(dock->parentWidget()->mapFromGlobal(mouseEvt->globalPos()));

				if ( guide ) {
					if ( p->currentGuide ) {
						if ( p->currentGuide == guide )
							mouseMove(mouseEvt);
						else {
							leave(evt);
							p->currentGuide = guide;
							enter(evt);
						}
					} else {
						p->currentGuide = guide;
						enter(evt);
					}
				} else {
					if ( p->currentGuide ) {
						leave(evt);
						p->currentGuide = 0;
					}
				}
			}
			break;
		case QEvent::MouseButtonRelease: 
			{
				QMouseEvent * mouseEvt = static_cast< QMouseEvent * >(evt);

				if (p->currentGuide && mouseEvt->button() == Qt::LeftButton)
					mouseRelease(mouseEvt);
			}
			break;
		default:
			break;
	}
	return QObject::eventFilter(obj,evt);
}
QIrDockDragGuide * QIrDockDragGuideManager::guideAt( const QPoint & pos ) const
{
	QIR_P(const QIrDockDragGuideManager);

	foreach (QIrDockDragGuide * guide, p->guides) {
		if ( guide->geometry().contains(pos) )
			return guide;
	}
	return 0;
}
void QIrDockDragGuideManager::enter( QEvent * evt )
{
	QIR_P(QIrDockDragGuideManager);

	if ( p->currentGuide )
		p->currentGuide->enterEvent(evt);
}
void QIrDockDragGuideManager::leave( QEvent * evt )
{
	QIR_P(QIrDockDragGuideManager);

	if ( p->currentGuide )
		p->currentGuide->leaveEvent(evt);
}
void QIrDockDragGuideManager::mouseMove( QMouseEvent * evt )
{
	QIR_P(QIrDockDragGuideManager);

	if ( p->currentGuide && p->currentGuide->isEnabled() ) {
		QMouseEvent mouseEvent(evt->type(),p->currentGuide->mapFromGlobal(evt->globalPos()),evt->button(),evt->buttons(),
			evt->modifiers());
		p->currentGuide->mouseMoveEvent(&mouseEvent);
	}
}
void QIrDockDragGuideManager::mouseRelease( QMouseEvent * evt )
{
	QIR_P(QIrDockDragGuideManager);

	if ( p->currentGuide ) {
		QMouseEvent mouseEvent(evt->type(),p->currentGuide->mapFromGlobal(evt->globalPos()),evt->button(),evt->buttons(),
			evt->modifiers());
		p->currentGuide->mouseReleaseEvent(&mouseEvent);
	}
}
bool QIrDockDragGuideManager::useAnimation() const
{
	QIR_P(const QIrDockDragGuideManager);

	return p->useAnimation;
}
void QIrDockDragGuideManager::setUseAnimation( bool animation )
{
	QIR_P(QIrDockDragGuideManager);

	p->useAnimation = animation;
}


QIR_END_NAMESPACE
