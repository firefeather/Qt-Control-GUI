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
#include <QChildEvent>
#include <QSplitter>
#include <QList>
#include "qirexpanderbox.h"
#include "qirexpander.h"
#include "../Common/ui/qirwidget_ui.h"

#define DISABLED_HANDLE_SIZE 1

QIR_BEGIN_NAMESPACE

class QIrExpanderBoxUi : public QIrWidgetUi
{
	QIR_DECLARE_WIDGET(QIrExpanderBox)

public:
        QIrExpanderBoxUi() : splitter(0), movable(true), initializing(true),
        headerHeight(0), blockAddChild(true) { }
	~QIrExpanderBoxUi() { }

	void _w_updateSplitter();
	void setOrientation( Qt::Orientation );
	void setMovable( bool );
	void setHeaderHeight( int );
	void enableHandle(int index, bool enabled = true);
	int findWidget( QWidget * ) const;
	int findExpander( QIrExpander * ) const;
	QIrExpander * findNextFreeExpander() const;

	QSplitter * splitter;
	bool movable;
	bool initializing;
	int headerHeight;
	bool blockAddChild;
	
protected:
	void init();
};


static QIrExpander::HeaderPosition headerPositionForOrientation( Qt::Orientation o )
{
	if ( o == Qt::Vertical )
		return QIrExpander::North;
	return QIrExpander::West;
}


///////////////////
//QIrExpanderBoxUi
///////////////////
void QIrExpanderBoxUi::init()
{
	QIR_W(QIrExpanderBox);

	splitter = new QSplitter(w);
	w->setWidget(splitter);
	w->setWidgetResizable(true);
	w->setFrameShape(QFrame::NoFrame);
	initializing = false;
}
void QIrExpanderBoxUi::enableHandle(int index, bool enabled)
{
	int handleIndex = index + 1;
	
	if ( handleIndex > splitter->count() - 1 )
		return;
	if ( enabled ) {
		if ( splitter->handle(handleIndex)->isEnabled() )
			return;
		splitter->handle(handleIndex)->setDisabled(false);
	} else {
		if ( !splitter->handle(handleIndex)->isEnabled() )
			return;
		splitter->handle(handleIndex)->setDisabled(true);
	}
}
void QIrExpanderBoxUi::_w_updateSplitter()
{
	QIR_W(QIrExpanderBox);
	int i, lastExpandedItem = -1;
	QIrExpander * exp;
	QList< QIrExpander * > list = w->expanders();

	//Disable all handles
	for (i = 0; i < list.count(); i++) 
		enableHandle(i,false);
	
	if ( !movable )
		return;

	for (i = list.count() - 1; i > 0 ; i--) {
		exp = list[i];
		if ( exp->isExpanded() ) {
			lastExpandedItem = i;
			break;
		}
	}

	//Enable only handles that can be moved
	for (i = 0; i < lastExpandedItem; i++) {
		exp = list[i];
		if ( exp->isExpanded() ) 
			enableHandle(i,true);
	}
}
QIrExpander * QIrExpanderBoxUi::findNextFreeExpander() const
{
	QIR_W(const QIrExpanderBox);

	foreach (QIrExpander * exp, w->expanders()) {
		if ( !exp->widget() )
			return exp;
	}
	return 0;
}
void QIrExpanderBoxUi::setOrientation( Qt::Orientation o )
{
	QIR_W(QIrExpanderBox);

	foreach (QIrExpander * e, w->expanders()) {
		if ( e )
			e->setHeaderPosition(headerPositionForOrientation(o));
	}
	splitter->setOrientation(o);
}
void QIrExpanderBoxUi::setMovable( bool movable )
{
	movable = movable;
	_w_updateSplitter();
}
void QIrExpanderBoxUi::setHeaderHeight( int h )
{
	QIR_W(QIrExpanderBox);

	headerHeight = h;
	foreach (QIrExpander * exp, w->expanders()) {
		exp->setHeaderHeight(h);
	}
}
int QIrExpanderBoxUi::findWidget( QWidget * wid ) const
{
	QIR_W(const QIrExpanderBox);
	int i = 0;
	
	if ( !wid )
		return -1;

	foreach (QIrExpander * exp, w->expanders()) {
		if ( exp && exp->widget() == wid )
			return i;
		i++;
	}
	return -1;
}
int QIrExpanderBoxUi::findExpander( QIrExpander * e ) const
{
	QIR_W(const QIrExpanderBox);
	int i = 0;

	if ( !e )
		return -1;

	foreach (QIrExpander * exp, w->expanders()) {
		if ( exp == e )
			return i;
		i++;
	}
	return -1;
}


/////////////////
//QIrExpanderBox
/////////////////
QIrExpanderBox::QIrExpanderBox(QWidget * parent ) : QScrollArea(parent), QIrWidget(* new QIrExpanderBoxUi)
{
	QIR_UI(QIrExpanderBox);

	ui->setupUi();
	ui->splitter->setOrientation(Qt::Vertical);
}
QIrExpanderBox::QIrExpanderBox(Qt::Orientation o, QWidget * parent) : QScrollArea(parent), QIrWidget(* new QIrExpanderBoxUi)
{
	QIR_UI(QIrExpanderBox);

	ui->setupUi();
	ui->splitter->setOrientation(o);
}
QIrExpanderBox::~QIrExpanderBox()
{
}
bool QIrExpanderBox::opaqueResize() const
{
	return ui_func()->splitter->opaqueResize();
}
void QIrExpanderBox::setOpaqueResize( bool o )
{
	ui_func()->splitter->setOpaqueResize( o );
}
Qt::Orientation QIrExpanderBox::orientation()
{
	return ui_func()->splitter->orientation();
}
void QIrExpanderBox::setOrientation( Qt::Orientation o )
{
	if ( orientation() == o )
		return;
	ui_func()->setOrientation(o);	
}
int QIrExpanderBox::indexOfWidget( QWidget * w ) const
{
	QIR_UI(const QIrExpanderBox);

	return ui->findWidget(w);
}
int QIrExpanderBox::indexOfExpander( QIrExpander * e ) const
{
	QIR_UI(const QIrExpanderBox);

	return ui->findExpander(e);
}
QIrExpander * QIrExpanderBox::addWidget( QWidget * w )
{
	return addWidget(QIcon(),QString(),w);
}
QIrExpander * QIrExpanderBox::addWidget(const QString & title, QWidget * w)
{
	return addWidget(QIcon(),title,w);
}
QIrExpander * QIrExpanderBox::addWidget(const QIcon & icon, const QString & title, QWidget * w)
{
	if ( indexOfWidget(w) != -1 )
		return expander(w);
	QIrExpander * expander = new QIrExpander(icon,title,0,headerPositionForOrientation(orientation()));

	expander->setWidget(w);
	addExpander(expander);
	return expander;
}
void QIrExpanderBox::addExpander( QIrExpander * e )
{
	QIR_UI(QIrExpanderBox);

	if ( indexOfExpander(e) != -1 )
		return;
	ui->blockAddChild = false;
	e->setHeaderPosition(headerPositionForOrientation(orientation()));
	e->setHeaderHeight(uniformHeaderHeight());
	ui->splitter->addWidget(e);
	ui->blockAddChild = true;
	QObject::connect(e,SIGNAL(expanded( bool )),this,SLOT(_w_updateSplitter()));
}
void QIrExpanderBox::_w_updateSplitter()
{
	ui_func()->_w_updateSplitter();
}
QWidget * QIrExpanderBox::widgetAt( int i ) const
{
	QList< QIrExpander * > list = expanders();
	QIrExpander * exp;

	if ( i < 0 || i >= list.count() )
		return 0;
        if ( (exp = list[i]) )
		return exp->widget();
	return 0;
}
QIrExpander * QIrExpanderBox::expander( QWidget * w ) const
{
	QList< QIrExpander * > list = expanders();
	int index;

	if ( (index = indexOfWidget(w)) == -1 )
		return 0;
	return list[index];
}
QIrExpander * QIrExpanderBox::expanderAt( int i ) const
{
	QList< QIrExpander * > list = expanders();

	if ( i < 0 || i >= list.count() )
		return 0;
	return list[i];
}
int QIrExpanderBox::count()
{
	return ui_func()->splitter->count();
}
QList< QWidget * > QIrExpanderBox::widgets() const
{
	QList< QWidget * > list;

	foreach (QIrExpander * e , expanders()) {
		if ( e && e->widget())
			list << e->widget();
	}
	return list;
}
QList< QIrExpander * > QIrExpanderBox::expanders() const
{
	QIR_UI(const QIrExpanderBox);
	QList< QIrExpander * > list;
	QIrExpander * exp;

	for (int i = 0; i < ui->splitter->count(); i++) {
                if ( (exp = qobject_cast< QIrExpander * >(ui->splitter->widget(i))) !=  0 )
			list << exp;
	}
	return list;
}
bool QIrExpanderBox::isMovable() const
{
	return ui_func()->movable;
}
void QIrExpanderBox::setMovable( bool m )
{
	QIR_UI(QIrExpanderBox);

	if ( ui->movable == m )
		return;
	ui->setMovable(m);
}

int QIrExpanderBox::uniformHeaderHeight() const
{
	return ui_func()->headerHeight;
}
void QIrExpanderBox::setUniformHeaderHeight( int h )
{
	QIR_UI(QIrExpanderBox);

	if ( ui->headerHeight == h )
		return;
	ui->setHeaderHeight(h);
}
QList< int > QIrExpanderBox::sizes() const
{
	return ui_func()->splitter->sizes();
}
void QIrExpanderBox::setSizes( const QList< int > & sizes)
{
	return ui_func()->splitter->setSizes(sizes);
}
void QIrExpanderBox::childEvent( QChildEvent * evt )
{
	QIR_UI(QIrExpanderBox);
	QWidget * w;
	
	QWidget::childEvent(evt);

	if ( !evt->child()->isWidgetType() || ui->initializing)
		return;
	w = static_cast< QWidget * >(evt->child());
	if (evt->added() && ui->blockAddChild) {
		QIrExpander * exp = ui->findNextFreeExpander();
		
		if ( exp )
			w->setParent(exp);
		else {
			QIrExpander * expander = new QIrExpander(w->objectName(),0);
			
			addExpander(expander);
			w->setParent(expander);
		}
	}
}

QIR_END_NAMESPACE
