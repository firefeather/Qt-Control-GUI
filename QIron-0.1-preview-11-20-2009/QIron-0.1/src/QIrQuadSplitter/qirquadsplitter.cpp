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
#include <QStyle>
#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QChildEvent>
#include <QLayout>
#include "qirquadsplitterlayout.h"
#include "ui/qirquadsplitter_ui.h"

#define INDEX(i,j) i * (CAPACITY / 2) + j
#define MAX_HANDLE_WIDTH 9

extern QSize qSmartMinSize(const QWidgetItem * i );


QIR_BEGIN_NAMESPACE

/////////////////////
//QIrQuadSplitterUi
/////////////////////
void QIrQuadSplitterUi::init()
{
	QIR_W(QIrQuadSplitter);
	QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	w->setLayout(m_layout = new QIrQuadSplitterLayout);
	w->setSizePolicy(sp);
    w->setAttribute(Qt::WA_WState_OwnSizePolicy, false);
	
	m_ratio.horizontal = m_ratio.vertical = 1/2.0;
	m_newCenter = m_center = QPoint(w->width() / 2,w->height() / 2); 
	
	w->setMouseTracking(true);
	w->setFocusPolicy(Qt::StrongFocus);
	w->setAutoFillBackground(false);
}
void QIrQuadSplitterUi::doMove( const QPoint & pos )
{
	moveCenter(closestLegalPosition(pos));
}
void QIrQuadSplitterUi::setRubberBands( const QPoint & pos )
{
	QIR_W(QIrQuadSplitter);
	int hW = w->handleWidth(), sHW = qRound((double)hW / 2);
	QPoint tempPos = m_newCenter;

	showRubberBands();
	
	if ( m_orientation == QIrQuadSplitterUi::Vertical ) 
		tempPos.setY(pos.y());
	else if ( m_orientation == QIrQuadSplitterUi::Horizontal )
		tempPos.setX(pos.x());
	else if ( m_orientation == QIrQuadSplitterUi::All ) {
		tempPos = pos;
	}
	m_newCenter = tempPos;

	if ( !m_horizontalBand->isHidden() ) {
		QRect r(w->mapToGlobal(QPoint(0,m_newCenter.y() - sHW)),QSize(w->width(),hW));
		
		m_horizontalBand->setGeometry(r);
	} 
	if ( !m_verticalBand->isHidden() ) {
		QRect r(w->mapToGlobal(QPoint(m_newCenter.x() - sHW,0)),QSize(hW,w->height()));
		
		m_verticalBand->setGeometry(r);
	} 		
}
void QIrQuadSplitterUi::showRubberBands()
{
	switch ( m_orientation ) {
		case QIrQuadSplitterUi::Vertical:
			if ( m_horizontalBand->isHidden() )
				m_horizontalBand->show();
			break;
		case QIrQuadSplitterUi::Horizontal:
			if ( m_verticalBand->isHidden() )
				m_verticalBand->show();
			break;
		case QIrQuadSplitterUi::All:
			if ( m_horizontalBand->isHidden() )
				m_horizontalBand->show();
			if ( m_verticalBand->isHidden() )
				m_verticalBand->show();
			break;
		default:
			hideRubberBands();
	}
}
QPoint QIrQuadSplitterUi::closestLegalPosition( const QPoint & pos )
{
	QIR_W(QIrQuadSplitter);

	int hW = w->handleWidth(), sHW = qRound((double)hW / 2), top = sHW, bottom = sHW, left = sHW, right = sHW, i;
	QRect sRect(pos - QPoint(sHW, sHW),QSize(hW,hW));
	QRect validRect = w->rect();
	QPoint legalPos = pos;

	if (m_orientation & QIrQuadSplitterUi::Vertical) {
		for (i = 0; i < CAPACITY / 2 ; i++)
			if ( m_layout->hasVisibleItemAt(i) )
				top = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).height() + sHW,top);
		for (i = CAPACITY / 2; i < CAPACITY ; i++) 
			if ( m_layout->hasVisibleItemAt(i) )
				bottom = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).height() + sHW,bottom);
	}
	if (m_orientation & QIrQuadSplitterUi::Horizontal) {
		for (i = 0; i < CAPACITY; i += 2)
			if ( m_layout->hasVisibleItemAt(i) )
				left = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).width() + sHW,left);
		for (i = 1; i < CAPACITY ; i += 2) 
			if ( m_layout->hasVisibleItemAt(i) )
				right = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).width() + sHW,right);
	}
	validRect.adjust(left,top,-right,-bottom);
	if ( pos.x() < validRect.left() )
		legalPos.setX(validRect.left());
	else if ( pos.x() > validRect.right() )
		legalPos.setX(validRect.right());
	if ( pos.y() < validRect.top() )
		legalPos.setY(validRect.top());
	else if ( pos.y() > validRect.bottom() )
		legalPos.setY(validRect.bottom());

	return legalPos;
}
QPoint QIrQuadSplitterUi::adjustedPosition( const QPoint & pos )
{
	QIR_W(QIrQuadSplitter);
	int hW = w->handleWidth(), sHW = hW / 2, top = sHW, bottom = sHW, left = sHW, right = sHW, i;
	QRect sRect(pos - QPoint(sHW, sHW),QSize(hW,hW));
	QRect validRect = w->rect();
	QPoint adjustedPos = pos;
	bool firstRow = false, secRow = false, firstCol = false, secCol = false;

	for (i = 0; i < CAPACITY / 2 ; i++) 
		if ( m_layout->hasVisibleItemAt(i) ) {
			firstRow = true;
			top = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).height() + sHW,top);
		}
	for (i = CAPACITY / 2; i < CAPACITY ; i++) 
		if ( m_layout->hasVisibleItemAt(i) ) {
			secRow = true;
			bottom = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).height() + sHW,bottom);
		}
	for (i = 0; i < CAPACITY; i += 2) 
		if ( m_layout->hasVisibleItemAt(i) ) {
			firstCol = true;
			left = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).width() + sHW,left);
		}
	for (i = 1; i < CAPACITY ; i += 2)
		if ( m_layout->hasVisibleItemAt(i) ) {
			secCol = true;
			right = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).width() + sHW,right);
		}
	validRect.adjust(left,top,-right,-bottom);

	if (firstCol && secCol) {
		if ( pos.x() < validRect.left() )  {
			
			adjustedPos.setX(validRect.left());
		}
		else if ( pos.x() > validRect.right() )
			adjustedPos.setX(validRect.right());
	}
	if (firstRow && secRow) {
		if ( pos.y() < validRect.top() )
			adjustedPos.setY(validRect.top());
		else if ( pos.y() > validRect.bottom() )
			adjustedPos.setY(validRect.bottom());
	}
	return adjustedPos;
}
void QIrQuadSplitterUi::adjustCenter()
{
	QIR_W(QIrQuadSplitter);
        QPoint pos((int)(w->width() * m_ratio.horizontal),(int)(w->height() * m_ratio.vertical));
	QPoint legalPos = adjustedPosition(pos);

	setCenter(legalPos,pos != legalPos);
}
void QIrQuadSplitterUi::hideRubberBands()
{
	m_horizontalBand->hide();
	m_verticalBand->hide();
}
void QIrQuadSplitterUi::setCenter()
{
	if ( m_newCenter == m_center )
		return;
	m_center = m_newCenter;
	updateRatios();
	m_layout->invalidate();
}
void QIrQuadSplitterUi::setCenter( const QPoint & pos, bool update)
{
	m_center = m_newCenter = pos;
	if ( update )
		updateRatios();
	m_layout->update();
}
void QIrQuadSplitterUi::moveCenter( const QPoint & pos )
{
	QPoint tempPos = m_center;

	if ( m_center == pos )
		return;
	if ( m_orientation == QIrQuadSplitterUi::Vertical ) 
		tempPos.setY(pos.y());
	else if ( m_orientation == QIrQuadSplitterUi::Horizontal )
		tempPos.setX(pos.x());
	else if ( m_orientation == QIrQuadSplitterUi::All )
		tempPos = pos;
	m_center = m_newCenter = tempPos;
	updateRatios();
	m_layout->invalidate();
}

void QIrQuadSplitterUi::resetCenter()
{
	m_newCenter = m_center;
}
void QIrQuadSplitterUi::updateRatios()
{
	QIR_W(QIrQuadSplitter);

	if ( w->width() != 0 )
		m_ratio.horizontal = m_center.x() / (double) w->width();
	if ( w->height() != 0 )
		m_ratio.vertical = m_center.y() / (double) w->height();
	recalc();
}
void QIrQuadSplitterUi::setOrientation( QIrQuadSplitterUi::Orientation o )
{
	if ( m_orientation == o )
		return;
	m_orientation = o;
	updateCursor();
}
void QIrQuadSplitterUi::updateCursor()
{
	QIR_W(QIrQuadSplitter);

	switch ( m_orientation )
	{
		case None:
			w->setCursor(Qt::ArrowCursor);
			break;
		case All:
			w->setCursor(Qt::SizeAllCursor);
			break;
		case Horizontal:
			w->setCursor(Qt::SplitHCursor);
			break;
		case Vertical:
			w->setCursor(Qt::SplitVCursor);
			break;
		default:
			w->setCursor(Qt::ArrowCursor);
			break;
	}	
}
QIrQuadSplitterUi::Orientation QIrQuadSplitterUi::getOrientation( const QPoint & pos )
{
	QIR_W(QIrQuadSplitter);

	bool horizontal = false, vertical = false;
	int x = pos.x(), y = pos.y(), handleWidth = w->handleWidth();

	if(x > (m_center.x() - handleWidth / 2) && x < (m_center.x() + handleWidth / 2))
		if ((m_layout->hasVisibleItemAt(0) && m_layout->hasVisibleItemAt(1)) 
			|| (m_layout->hasVisibleItemAt(2) && m_layout->hasVisibleItemAt(3)))
			horizontal = true;
	if(y > (m_center.y() - handleWidth / 2) && y < (m_center.y() + handleWidth / 2))
		vertical = true;
	if(horizontal && !vertical)
		return Horizontal;
	if(!horizontal && vertical)
		return Vertical;
	if(horizontal && vertical)
		return All;
	return None;
}
void QIrQuadSplitterUi::queueChild( QWidget * widget )
{
	if ( !m_queuedChildren.contains(widget) ) {
		m_queuedChildren.prepend(widget);
		widget->hide();
	}
}
void QIrQuadSplitterUi::dequeueChild( QWidget * widget )
{
	int index;

	if ((index = m_queuedChildren.indexOf(widget)) != -1)
		m_queuedChildren.removeAt(index);
}
void QIrQuadSplitterUi::popLastChild()
{
	QIR_W(QIrQuadSplitter);
	int index = m_layout->nextFreeCell(), i, j;

	if ( index == -1 || m_queuedChildren.count() == 0)
		return;
	QWidget * widget = m_queuedChildren.takeLast();
	i = index / (CAPACITY / 2);
	j = index % (CAPACITY / 2);
	w->setWidget(widget,i,j);	
}
void QIrQuadSplitterUi::recalc()
{
	QIR_W(QIrQuadSplitter);

	w->setMinimumSize(calcMinSize());
	m_layout->invalidate();
}
QSize QIrQuadSplitterUi::calcMinSize() const
{
	QIR_W(const QIrQuadSplitter);
	int hW = w->handleWidth(), sHW = qRound((double)hW / 2), minH1 = 0, minH2 = 0, minW1 = 0, minW2 = 0, i;
	bool firstRow = false , secRow = false, firstCol = false, secCol = false;

	//Determine the minimum height
	for (i = 0; i < CAPACITY / 2 ; i++) 
		if ( m_layout->hasVisibleItemAt(i) ) {
			firstRow = true;
			minH1 = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).height(),minH1);
		}
	for (i = CAPACITY / 2; i < CAPACITY ; i++)
		if ( m_layout->hasVisibleItemAt(i) ) {
			secRow = true;
			minH2 = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).height(),minH2);
		}
	if ( !m_keepRatios )
		minH1 += minH2 + (firstRow && secRow ? hW : 0);
	else {
		if (firstRow && secRow) {
			double p = (1 - m_ratio.vertical) / m_ratio.vertical;

			if ( (minH1 + sHW) * p >  (minH2 + sHW) )
                                minH1 = (int)((minH1 + sHW) / m_ratio.vertical);
			else
                                minH1 = (int)((minH2 + sHW) / (1 - m_ratio.vertical));
		} else if (secRow)
			minH1 = minH2;
	}

	//Determine the minimum width
	for (i = 0; i < CAPACITY; i += 2) 
		if ( m_layout->hasVisibleItemAt(i) ) {
			firstCol = true;
			minW1 = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).width(),minW1);
		}
	for (i = 1; i < CAPACITY ; i +=2 )
		if ( m_layout->hasVisibleItemAt(i) ) {
			secCol = true;
			minW2 = qMax(qSmartMinSize(m_layout->splitterItemAt(i)).width(),minW2);
		}
	
	if ( !m_keepRatios )
		minW1 += minW2 + (firstCol && secCol ? hW : 0);
	else {
		if (firstCol && secCol) {
			double p = (1 - m_ratio.horizontal) / m_ratio.horizontal;
			
			if ( (minW1 + sHW) * p >  (minW2 + sHW) ) 
                                minW1 = (int)((minW1 + sHW) / m_ratio.horizontal);
			else
                                minW1 = (int)((minW2 + sHW) / (1 - m_ratio.horizontal));
		} else if (secCol)
			minW1 = minW2;
	}	
	
	return QSize(minW1,minH1);
}
/////////////////////
//QIrQuadSplitter
////////////////////
QIrQuadSplitter::QIrQuadSplitter( QWidget* parent ) 
: QFrame(parent), QIrWidget(* new QIrQuadSplitterUi)
{
	QIR_UI(QIrQuadSplitter);

	ui->setupUi();
}
QIrQuadSplitter::~QIrQuadSplitter()
{
	QIR_UI(QIrQuadSplitter);

	if ( ui->m_horizontalBand )
		delete ui->m_horizontalBand;
	if ( ui->m_verticalBand )
		delete ui->m_verticalBand;
}
int QIrQuadSplitter::count() const
{
	QIR_UI(const QIrQuadSplitter);
	
	return ui->m_layout->count();
}
int QIrQuadSplitter::handleWidth() const
{
	QIR_UI(const QIrQuadSplitter);

    if (ui->m_handleWidth > 0) {
        return ui->m_handleWidth;
    } else {
        return style()->pixelMetric(QStyle::PM_SplitterWidth, 0, this);
    }
}
void QIrQuadSplitter::setHandleWidth( int width )
{
	QIR_UI(QIrQuadSplitter);

	if (handleWidth() == width || (width < 0 && width > MAX_HANDLE_WIDTH))
		return;
	ui->m_handleWidth = width;
}
bool QIrQuadSplitter::opaqueResize() const
{
	return ui_func()->m_opaque;
}
void QIrQuadSplitter::setOpaqueResize( bool o )
{
	QIR_UI(QIrQuadSplitter);

	if ( ui->m_opaque == o ) 
		return;
	ui->m_opaque = o;
	if ( !ui->m_opaque ) 
		if ( !ui->m_horizontalBand ) {
			ui->m_horizontalBand = new QRubberBand(QRubberBand::Rectangle);
			ui->m_verticalBand = new QRubberBand(QRubberBand::Rectangle);
		}
}
QWidget * QIrQuadSplitter::widgetAt(int i, int j)
{
	QIR_UI(const QIrQuadSplitter);
	int index = INDEX(i,j);

	if (index < 0 && index >= CAPACITY) {
		qWarning("QIrQuadSplitter::widgetAt(): Invalid indexes.");
		return 0;
	}
	return ui->m_layout->m_structList[index]->widget();
}
int QIrQuadSplitter::indexOf( QWidget * widget ) const
{
	QIR_UI(const QIrQuadSplitter);

	for (int i = 0; i < CAPACITY; i++)
		if ( ui->m_layout->m_structList[i]->widget() == widget )
			return i;
	return -1;
}
void QIrQuadSplitter::setWidget(QWidget * widget,int i, int j)
{
	QIR_UI(QIrQuadSplitter);
	int index = INDEX(i,j);

	if (index < 0 && index >= CAPACITY) {
		qWarning("QIrQuadSplitter::setWidget(): Invalid indexes.");
		return;
	}
	ui->m_layout->m_structList[index]->setWidget(widget);
}
void QIrQuadSplitter::removeWidget( QWidget * widget )
{
	QIR_UI(QIrQuadSplitter);

	int index = indexOf(widget);

	if ( index != -1) {
		ui->m_blockAddRemoveChild = true;
		ui->m_layout->m_structList[index]->deleteWidget();
		ui->m_blockAddRemoveChild = false;
	}
}
void QIrQuadSplitter::removeWidgetAt(int i, int j)
{
	QIR_UI(QIrQuadSplitter);

	int index = INDEX(i,j);

	if (index < 0 && index >= CAPACITY) {
		qWarning("QIrQuadSplitter::removeWidgetAt(): Invalid indexes.");
		return;
	}
	ui->m_blockAddRemoveChild = true;
	ui->m_layout->m_structList[index]->deleteWidget();
	ui->m_blockAddRemoveChild = false;
}
bool QIrQuadSplitter::event( QEvent * evt )
{
	QIR_UI(QIrQuadSplitter);

	switch ( evt->type() ) {
		case QEvent::HoverLeave:
			ui->setOrientation(QIrQuadSplitterUi::None);
			update();
			break;
		case QEvent::ShowToParent:			
		case QEvent::HideToParent:
			ui->recalc();
			ui->m_layout->invalidate();
			break;
		default:
			break;
	}
	return  QFrame::event(evt);
}
void QIrQuadSplitter::childEvent( QChildEvent * evt )
{
	QIR_UI(QIrQuadSplitter);
	int freeCell;

    if ( !evt->child()->isWidgetType() )
        return;
    QWidget * w = static_cast< QWidget * >(evt->child());

	if ( w->isWindow() )
		return;
	
	if ( evt->added() ) {
		w->installEventFilter(this);
		if ( !ui->m_blockAddRemoveChild ) {			
			if ( (freeCell = ui->m_layout->nextFreeCell()) != -1 )
				ui->m_layout->m_structList[freeCell]->setWidget(w);
			else 
				ui->queueChild(w);
		}
    } else if ( evt->polished() && !ui->m_blockAddRemoveChild ) {
        if (isVisible() && indexOf(w) != -1 && !(w->isHidden() && w->testAttribute(Qt::WA_WState_ExplicitShowHide)))
            w->show();
    } else if ( evt->type() == QEvent::ChildRemoved ) {
		w->removeEventFilter(this);
		if ( !ui->m_blockAddRemoveChild ) {
			int index;
			if ( (index = indexOf(w)) != -1 )
				ui->m_layout->m_structList[index]->setWidget(0,false);
			else
				ui->dequeueChild(w);
		}
		ui->recalc();
		ui->m_layout->invalidate();
	}
}
bool QIrQuadSplitter::eventFilter(QObject * obj, QEvent * evt)
{
	QIR_UI(QIrQuadSplitter);

	switch ( evt->type() ) {
		case QEvent::Enter :
			ui->setOrientation(QIrQuadSplitterUi::None);
			break;
		case QEvent::Show:
			if ( obj->isWidgetType() ) {
				QWidget * widget = qobject_cast< QWidget * >(obj);
				if ( ui->m_queuedChildren.contains(widget) ) {
					widget->hide();
					return true;
				} 
			}
		case QEvent::Hide:
			ui->recalc();
			ui->adjustCenter();
			ui->m_layout->invalidate();
			break;
		default:
			break;
	}
	return QFrame::eventFilter(obj,evt);		
}
void QIrQuadSplitter::mousePressEvent( QMouseEvent * evt )
{
	QIR_UI(QIrQuadSplitter);

	if ( evt->button() == Qt::LeftButton ) {
		if ( ui->m_orientation != QIrQuadSplitterUi::None )
			grabMouse();
	}
}
void QIrQuadSplitter::mouseMoveEvent( QMouseEvent * evt )
{
	QIR_UI(QIrQuadSplitter);
	QPoint pos = evt->pos(), legalPoint;

	if ( evt->buttons() & Qt::LeftButton ) {
		if (ui->m_orientation != QIrQuadSplitterUi::None && this == QWidget::mouseGrabber()) {
			if ( ui->m_opaque )  
				ui->doMove(pos);
			else
				ui->setRubberBands(ui->closestLegalPosition(pos));
		}
	} else
		ui->setOrientation(ui->getOrientation(pos));
}
void QIrQuadSplitter::mouseReleaseEvent( QMouseEvent * evt)
{
	QIR_UI(QIrQuadSplitter);
	QPoint pos = evt->pos();

	if ( evt->button() == Qt::LeftButton ) {
		if ( ui->m_orientation != QIrQuadSplitterUi::None) {
			ui->setOrientation(QIrQuadSplitterUi::None);
			if ( !ui->m_opaque )
				ui->hideRubberBands();
			if ( ui->m_moveCancelled ) {
				ui->resetCenter();
				ui->m_moveCancelled = false;
			} else {
				ui->setCenter();
				ui->m_layout->invalidate();
				ui->setOrientation(ui->getOrientation(pos));
			}
		}
		releaseMouse();
	}
}
void QIrQuadSplitter::changeEvent( QEvent * evt )
{
    QIR_UI(QIrQuadSplitter);

	if( evt->type() == QEvent::StyleChange ) {
		ui->recalc();
		ui->m_layout->invalidate();
	}
    QFrame::changeEvent(evt);
}
void QIrQuadSplitter::keyPressEvent( QKeyEvent * evt )
{
	QIR_UI(QIrQuadSplitter);

	if ( evt->key() == Qt::Key_Escape ) {
		if ( !ui->m_opaque ) {
			QMouseEvent e(QEvent::MouseButtonRelease,QPoint(),Qt::LeftButton,0,Qt::NoModifier);

			ui->m_moveCancelled = true;
			mouseReleaseEvent(&e);
		}
	}
}
QSize QIrQuadSplitter::sizeHint() const
{
	QSize s(200,200);

    return s;
}
QSize QIrQuadSplitter::minimumSizeHint() const
{
	QIR_UI(const QIrQuadSplitter);

	return ui->calcMinSize();
}
bool QIrQuadSplitter::keepRatios() const
{
	return ui_func()->m_keepRatios;
}
void QIrQuadSplitter::setKeepRatios( bool keep )
{
	QIR_UI(QIrQuadSplitter);
	
	if ( ui->m_keepRatios == keep )
		return;
	ui->m_keepRatios = keep;
	ui->recalc();
	ui->m_layout->invalidate();
}
QWidgetList QIrQuadSplitter::queuedWidgets() const
{
	return ui_func()->m_queuedChildren;
}
void QIrQuadSplitter::resizeEvent( QResizeEvent * )
{
	QIR_UI(QIrQuadSplitter);
	
	if ( !ui->m_keepRatios )
		ui->adjustCenter();
	else {
                QPoint pos((int)(width() * ui->m_ratio.horizontal),(int)(height() * ui->m_ratio.vertical));
		ui->setCenter(pos, false);
	}
}
double QIrQuadSplitter::verticalRatio() const
{
	return ui_func()->m_ratio.vertical;
}
void QIrQuadSplitter::setVerticalRatio( double ratio )
{
	QIR_UI(QIrQuadSplitter);
	double minRatio = width() != 0 ? (double) handleWidth() / width() : 0.1, maxRatio = 1 - minRatio;

	if ( ratio < minRatio || ratio > maxRatio ) {
		qWarning("QIrQuadSplitter::setVerticalRatio(): Invalid ratio. It should be between %f and %f.",minRatio,maxRatio);
		return;
	}
	ui->m_ratio.vertical = ratio;
	ui->recalc();
	ui->adjustCenter();
}
double QIrQuadSplitter::horizontalRatio() const
{
	return ui_func()->m_ratio.horizontal;
}
void QIrQuadSplitter::setHorizontalRatio( double ratio )
{
	QIR_UI(QIrQuadSplitter);
	double minRatio = width() != 0 ? (double) handleWidth() / width() : 0.1, maxRatio = 1 - minRatio;

	if ( ratio < minRatio || ratio > maxRatio ) {
		qWarning("QIrQuadSplitter::setHorizontalRatio(): Invalid ratio. It should be between 0.0 and 1.0.");
		return;
	}
	ui->m_ratio.horizontal = ratio;
	ui->recalc();
	ui->adjustCenter();
}

QIR_END_NAMESPACE
