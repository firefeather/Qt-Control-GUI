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
/*!
  \brief This file contains the definition of QIrExpander class.
  \file qirexpander.cpp
  \author Dzimi Mve Alexandre
  \date 07/28/2009
*/
#include <QChildEvent>
#include <QCursor>
#include <QEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionButton>
#include <QVBoxLayout>
#include "ui/qirexpander_ui.h"
#include "qirexpanderstyle.h"

extern QSize qSmartMinSize( const QWidgetItem * );

QIR_BEGIN_NAMESPACE

///////////////////////////
//QIrExpanderHeaderButton
///////////////////////////
QIrExpanderHeaderButton::QIrExpanderHeaderButton( QIrExpander * parent ) : QAbstractButton(parent), m_expander(parent)
{
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	setFocusPolicy(Qt::NoFocus);
}
QIrExpanderHeaderButton::~QIrExpanderHeaderButton()
{
}
void QIrExpanderHeaderButton::initStyleOption( QStyleOptionButton * option ) const
{
    if ( !option )
        return;
    option->initFrom(this);
    option->features = QStyleOptionButton::None;
	if ( isDown() ) {
        option->state |= QStyle::State_Sunken;
		

	}
	QRect r(mapToGlobal(rect().topLeft()),rect().size());
	if ( !r.contains(QCursor::pos()) )
		option->state &= ~QStyle::State_MouseOver;

	if ( m_expander->isExpanded() )
        option->state |= QStyle::State_On;

    option->text = text();
    option->icon = icon();
    option->iconSize = iconSize();
}
void QIrExpanderHeaderButton::paintEvent( QPaintEvent * )
{
	QIrSubStyle * style = m_expander->subStyle();
	QStyleOptionButton option;
	QPainter painter(this);
	
	initStyleOption(&option);
	style->drawPrimitive((QStyle::PrimitiveElement)QIrExpanderStyle::PE_FrameExpanderHeader,&option,&painter,m_expander);
	style->drawControl((QStyle::ControlElement)QIrExpanderStyle::CE_ExpanderButton,&option,&painter,m_expander);
}

////////////////////
//QIrExpanderLayout
////////////////////
QIrExpanderLayout::QIrExpanderLayout( QWidget * parent ) 
: QLayout(parent), m_handleResize(false), m_position(North), m_items(RoleCount,0)
{    
}
QIrExpanderLayout::~QIrExpanderLayout()
{
    QLayoutItem * l;

    while ((l = takeAt(0)))
        delete l;
}
void QIrExpanderLayout::addItem( QLayoutItem * )
{
	qWarning("QIrExpanderLayout::addItem(): Please use QIrExpanderLayout::setWidget()");
	return;
}
void QIrExpanderLayout::addWidget(QWidget * widget, ItemRole role)
{
	widget->setParent(parentWidget());
    add(new QWidgetItem(widget), role);
}
void QIrExpanderLayout::setHeaderPosition( Position position )
{
	if ( m_position == position )
		return;
	m_position = position;
	invalidate();
}
Qt::Orientations QIrExpanderLayout::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}
bool QIrExpanderLayout::hasHeightForWidth() const
{
	return false;
}
int QIrExpanderLayout::count() const
{
	int c = 0;

	foreach (QLayoutItem * i, m_items ) {
		if ( i )
			c++;
	}
	return c;
}
QLayoutItem *QIrExpanderLayout::itemAt( int index ) const
{
	return m_items.value(index);
}
QSize QIrExpanderLayout::minimumSize() const
{
	return calculateSize(MinimumSize);
}
void QIrExpanderLayout::setGeometry( const QRect & rect )
{
	QIrExpander * expander = qobject_cast< QIrExpander * >(parentWidget());
	int hH = expander->headerHeight();
	QRect headerRect, centralRect = rect;
	QLayoutItem * header = m_items[Header],
		* central = m_items[Central];
		

	QLayout::setGeometry(rect);
	if ( !header )
		return;

	m_handleResize = true;
	switch ( m_position ) {
		case North :
			headerRect = QRect(rect.topLeft(),QSize(rect.width(),hH));
			centralRect.adjust(0,hH,0,0);
			break;
		case South :
			headerRect = QRect(rect.bottomLeft() - QPoint(0,hH - 1),QSize(rect.width(),hH));
			centralRect.adjust(0,0,0,-hH);
			break;
		case West :
			headerRect = QRect(rect.topLeft(),QSize(hH,rect.height()));
			centralRect.adjust(hH,0,0,0);
			break;
		case East :
			headerRect = QRect(rect.topRight() - QPoint(hH - 1,0),QSize(hH,rect.height()));
			centralRect.adjust(0,0,-hH,0);
			break;
	}
	header->setGeometry(headerRect);
	if ( central && !central->widget()->isHidden())
		central->setGeometry(centralRect);
	m_handleResize = false;
}

 QSize QIrExpanderLayout::sizeHint() const
 {
     return calculateSize(SizeHint);
 }
 QLayoutItem * QIrExpanderLayout::takeAt( int index )
 {
     if (index >= 0 && index < m_items.size()) {
         QLayoutItem * item = m_items[index];
		 m_items[index] = 0;
         return item;
     }
     return 0;
 }
 void QIrExpanderLayout::add(QLayoutItem * item, ItemRole role)
 {
	 if ( m_items[role] ) {
		 delete item;
		 return;
	 }
	 m_items[role] = item;
 }
 QSize QIrExpanderLayout::minimumHeaderSize() const
 {
	 QIrExpander * e = qobject_cast< QIrExpander * >(parentWidget());

	 return QSize(e->ui_func()->minimumHeaderWidth(),e->headerHeight());
 }
 QSize QIrExpanderLayout::sizeOnFirstShow() const
 {
	QLayoutItem * header = m_items[Header],
		* central = m_items[Central];
	QSize totalSize, headerSize, centralSize(0,0);
		
	if ( !header )
		return QSize(0,0);
	headerSize = header->geometry().size();
	if ( central ) {
		if ( central->geometry().isValid() ) 
			centralSize = central->geometry().size();
		else
			centralSize = qSmartMinSize(static_cast<QWidgetItem *>(central));
	}
	switch ( m_position ) {
		case North :
		case South :
			totalSize.rwidth() = qMax(headerSize.width(),centralSize.width());
			totalSize.rheight() = headerSize.height() + centralSize.height();		
			break;
		case West :
		case East :
			totalSize.rwidth() = headerSize.width() + centralSize.width();
			totalSize.rheight() = qMax(headerSize.height(),centralSize.height());
			break;
	}
	return totalSize;
 }

 QSize QIrExpanderLayout::calculateSize( SizeType sizeType ) const
 {
    QLayoutItem * header = m_items[Header],
		* central = m_items[Central];
	QSize totalSize, headerSize, centralSize(0,0);
		
	if ( !header )
		return QSize(0,0);
	headerSize = minimumHeaderSize();
	if ( sizeType == MinimumSize ) {		
		if ( central )
			centralSize = central->minimumSize();
	} else {
		if ( central )
			centralSize = central->sizeHint();
	}
	

	switch ( m_position ) {
		case North :
		case South :
			totalSize.rwidth() = qMax(headerSize.width(),centralSize.width());
			totalSize.rheight() = headerSize.height() + centralSize.height();		
			break;
		case West :
		case East :
			totalSize.rwidth() = headerSize.height() + centralSize.width();
			totalSize.rheight() = qMax(headerSize.width(),centralSize.height());
			break;
	}
	return totalSize;
 }

/////////////////////
//QIrExpanderUi
/////////////////////
void QIrExpanderUi::init()
{
	QIR_W(QIrExpander);
	
	layout = new QIrExpanderLayout(w);
	button = new QIrExpanderHeaderButton(w);
	layout->addWidget(button,QIrExpanderLayout::Header);
}
void QIrExpanderUi::setupConnections()
{
	QIR_W(QIrExpander);

	QObject::connect(button,SIGNAL(clicked()),w,SLOT(_w_expand()));
	initializing = false;
}
void QIrExpanderUi::adjustGeometry()
{
	QIR_W(QIrExpander);
	
	
	w->setMinimumSize(layout->minimumSize());
	w->setMaximumSize(QSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX));
	switch ( position ) {
		case QIrExpander::North :
		case QIrExpander::South :
			if (!widget || !expanded) {
							
				w->setFixedHeight(w->headerHeight());
				if ( oldSize.isValid() )
					w->resize(oldSize);
			}
			else {
				
				w->setMaximumHeight(qMin(QWIDGETSIZE_MAX,1000));
				if ( this->firstShow ) {
					w->resize(layout->sizeOnFirstShow());
					firstShow = false;
				} else {
				if ( oldSize.isValid() )
					w->resize(oldSize);
				}
			}
			break;
		case QIrExpander::West :
		case QIrExpander::East :
			if (!widget || !expanded) {
				
				w->setFixedWidth(w->headerHeight());
				if ( oldSize.isValid() )
					w->resize(oldSize);
			} else { 
				
				w->setMaximumWidth(qMin(QWIDGETSIZE_MAX,1000));
				if ( this->firstShow ) {
					w->resize(layout->sizeOnFirstShow());
					firstShow = false;
				} else {
					if ( oldSize.isValid() )
					w->resize(oldSize);
				}
			}
			break;
	}
	
	button->update();
}
int QIrExpanderUi::minimumHeaderWidth() const
{
	QIR_W(const QIrExpander);
	QStyleOptionButton option;
	
	button->initStyleOption(&option);
	return w->subStyle()->pixelMetric((QStyle::PixelMetric)QIrExpanderStyle::PM_ExpanderHeaderMinimumWidth,&option,w);
}
void QIrExpanderUi::polish()
{
	QIR_W(QIrExpander);
	QIrSubStyle * style = w->subStyle();
	QStyleOption opt;

	opt.initFrom(w);
	if ( button ) {
		if ( !button->iconSize().isValid() ) {
			int s = style->pixelMetric((QStyle::PixelMetric)QIrExpanderStyle::PM_ExpanderIconSize,&opt,w);
			button->setIconSize(QSize(s,s));
		}		
	}
	adjustGeometry();
}


//////////////
//QIrExpander
//////////////
QIrExpander::QIrExpander(QWidget * parent, HeaderPosition position) 
: QWidget(parent), QIrWidget(* new QIrExpanderUi)
{
	QIR_UI(QIrExpander);
	
	ui->setupUi();
	setHeaderPosition(position);
	setTitle("Expander");
	ui->polish();
}
QIrExpander::QIrExpander(const QString & title, QWidget * parent, HeaderPosition position) 
: QWidget(parent), QIrWidget(* new QIrExpanderUi)
{
	QIR_UI(QIrExpander);
	
	ui->setupUi();
	setHeaderPosition(position);
	if ( title.isEmpty() )
		setTitle("Expander");
	else
		setTitle(title);
	ui->polish();
}
QIrExpander::QIrExpander(const QIcon & icon, const QString & title, QWidget * parent, HeaderPosition position)
: QWidget(parent), QIrWidget(* new QIrExpanderUi)
{
	QIR_UI(QIrExpander);
	
	ui->setupUi();
	setHeaderPosition(position);
	setIcon(icon);
	if ( title.isEmpty() )
		setTitle("Expander");
	else
		setTitle(title);
	ui->polish();
}
	
QIrExpander::~QIrExpander()
{	
}
QWidget * QIrExpander::widget() const
{
	QIR_UI(const QIrExpander);

	return ui->widget;
}
void QIrExpander::setWidget( QWidget * w )
{
	QIR_UI(QIrExpander);

	if ( ui->widget == w  )
		return;
	if ( ui->widget && w ) {
                qWarning("QIrExpander::setWidget(): The expander already has a widget.");
		return;
	}
	if ( ui->widget ) {
		ui->widget->removeEventFilter(this);
		ui->widget->hide();
		ui->widget->deleteLater();
	}
	ui->widget = w;
	if ( w ) {
		ui->widget->installEventFilter(this);
		ui->adjustGeometry();
		ui->blockAddChild = false;
		ui->layout->addWidget(ui->widget,QIrExpanderLayout::Central);
		ui->blockAddChild = true;
		expand();
	} else {
		ui->adjustGeometry();
		collapse();
	}
}
QIcon QIrExpander::icon() const
{
	QIR_UI(const QIrExpander);

	return ui->icon;
}
void QIrExpander::setIcon( const QIcon & icon )
{
	QIR_UI(QIrExpander);

	ui->button->setIcon(ui->icon = icon);
	ui->adjustGeometry();
}
QString QIrExpander::title() const
{
	QIR_UI(const QIrExpander);
	
	return ui->title;
}
void QIrExpander::setTitle( const QString & title )
{
	QIR_UI(QIrExpander);

	if ( ui->title == title )
		return;

	ui->button->setText(ui->title = title);
	ui->adjustGeometry();
}
int QIrExpander::headerHeight() const
{
	QIR_UI(const QIrExpander);

	if ( ui->headerH < 1 ) {
		QIrSubStyle * s = subStyle();
		QStyleOption option;

		option.initFrom(this);
		return s->pixelMetric((QStyle::PixelMetric)QIrExpanderStyle::PM_ExpanderHeaderHeight,&option,this);
	}
	return ui->headerH;
}
void QIrExpander::setHeaderHeight( int h )
{
	QIR_UI(QIrExpander);
	
	if ( ui->headerH == h )
		return;
	ui->headerH = h;
	ui->adjustGeometry();
}
QIrExpander::HeaderPosition QIrExpander::headerPosition() const
{
	QIR_UI(const QIrExpander);

	return ui->position;
}
void QIrExpander::setHeaderPosition( HeaderPosition p )
{
	QIR_UI(QIrExpander);

	if ( ui->position == p )
		return;
	ui->updateOldSize = false;
	ui->position = p;
	if ( ui->layout )
		ui->layout->setHeaderPosition((QIrExpanderLayout::Position)p);
	if ( ui->button ) {
		ui->adjustGeometry();
	}
	ui->updateOldSize = true;
}
bool QIrExpander::isExpanded() const
{
	QIR_UI(const QIrExpander);

	return ui->expanded;
}
void QIrExpander::setExpanded( bool e )
{
	QIR_UI(QIrExpander);
	
	if ( ui->expanded == e )
		return;
	if ( e )
		expand();
	else
		collapse();
}
void QIrExpander::subStyleChanged()
{
	QIR_UI(QIrExpander);

	ui->polish();
}
void QIrExpander::resizeEvent( QResizeEvent * )
{
	QIR_UI(QIrExpander);

	if ( !this->isExpanded() && ui->updateOldSize ) {
		if ( ui->layout->m_position == QIrExpanderLayout::North || ui->layout->m_position == QIrExpanderLayout::South )
			ui->oldSize.rwidth() =  width();
		else
			ui->oldSize.rheight() =  height();
	} else if (this->isExpanded()){
		ui->oldSize = size();
	}
}
QIrSubStyle * QIrExpander::defaultSubStyle() const
{
	return new QIrDefaultExpanderStyle;
}
void QIrExpander::paintEvent( QPaintEvent * )
{
	QPainter p(this);
	QStyleOption option;

	option.initFrom(this);

	subStyle()->drawPrimitive((QStyle::PrimitiveElement)QIrExpanderStyle::PE_FrameExpander,&option,&p,this);
}
bool QIrExpander::eventFilter(QObject * obj, QEvent * evt)
{
	QIR_UI(QIrExpander);

	switch ( evt->type() ) {
		case QEvent::Show:
			if ( !ui->expanded ) {
				ui->widget->hide();
				return true;
			}
			break;
		case QEvent::Hide:
			if ( ui->expanded ) {
				ui->widget->show();
				return true;
			}
			break;
		case QEvent::Move:
		case QEvent::Resize:
			if ( !ui->layout->m_handleResize )
				ui->layout->invalidate();
			break;
		default:
			break;
	}
	return QWidget::eventFilter(obj,evt);
}
void QIrExpander::childEvent( QChildEvent * evt )
{
	QIR_UI(QIrExpander);
	QWidget * w;
	
	QWidget::childEvent(evt);

	if ( !evt->child()->isWidgetType() || ui->initializing)
		return;
	w = static_cast< QWidget * >(evt->child());
	if (evt->added() && ui->blockAddChild) {
		if ( !ui->widget ) {
			ui->addingWidget = true;
			setWidget(w);
			ui->addingWidget = false;
		}
	} else if ( evt->polished() && !ui->blockAddChild ) {
		if ( isVisible() && ui->widget == w && !(w->isHidden() && w->testAttribute(Qt::WA_WState_ExplicitShowHide))) {
			w->show();
		}
	}
	else if ( evt->removed() ) {
		if ( evt->child() == ui->widget ) {
			ui->widget = 0;
			collapse();
			foreach (QObject * child, children()) {
				if ( child->isWidgetType() && child != (QObject *)ui->button ) {
					setWidget(qobject_cast< QWidget * >(child));
					return;
				}
			}
		}	
	}
}
void QIrExpander::expand()
{
	QIR_UI(QIrExpander);

	ui->button->setChecked(true);
	_w_expand(true);

}
void QIrExpander::collapse()
{
	QIR_UI(QIrExpander);
	
	ui->button->setChecked(false);
	_w_expand(false);	
}
void QIrExpander::_w_expand( bool e )
{
	QIR_UI(QIrExpander);
	
	if ( (ui->expanded == e) || (e && !ui->widget) )
		return;
	
	ui->expanded = e;
	if ( e ) {
		ui->adjustGeometry();
		if ( !ui->addingWidget ) {
			ui->widget->setHidden(!ui->expanded);
		}
	} else {
		if ( !ui->widget ) {
			ui->adjustGeometry();	
			return;
		}
		ui->oldSize = size();
		ui->widget->setHidden(!ui->expanded);
		ui->adjustGeometry();	
	}
	emit expanded(ui->expanded);
}

QIR_END_NAMESPACE
