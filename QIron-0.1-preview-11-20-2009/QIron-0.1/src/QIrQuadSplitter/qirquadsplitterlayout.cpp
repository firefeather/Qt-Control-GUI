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
#include "qirquadsplitterlayout.h"
#include "ui/qirquadsplitter_ui.h"

QIR_BEGIN_NAMESPACE

//////////////////////////////
//QIrQuadSplitterLayoutStruct
//////////////////////////////
void QIrQuadSplitterLayoutStruct::setWidget(QWidget * widget, bool queue, bool popLast)
{
        int i = 0, swap = false;
	QIrQuadSplitter * s = m_layout->splitter();
	QIrQuadSplitterUi * ui = s->ui_func();
	QWidget * tempWidget;

	if ( m_widget == widget )
		return;
	ui->m_blockAddRemoveChild = true;
        if ( widget ) {
		if ( (i = m_layout->splitterIndexOf(widget)) != -1 ) { 
			m_layout->m_structList[i]->setWidget(0,false,false);
			ui->m_blockAddRemoveChild = true;
			swap = true;
		} else if ( (i = ui->m_queuedChildren.indexOf(widget)) != -1 )
			ui->dequeueChild(widget);
        }
	if ( m_widget ) {
		m_layout->removeWidget(m_widget);
		if ((m_widget->parentWidget() == s) && queue)
			ui->queueChild(m_widget);
	}
	
	tempWidget = m_widget; 
	m_widget = widget;

	if ( !m_widget ) {
		ui->m_blockAddRemoveChild = false;
		if ( popLast )
			ui->popLastChild();
		return;
	}
    if ( widget->parentWidget() != s )
        widget->setParent(s);
	if ( tempWidget && swap ) {
		m_layout->m_structList[i]->setWidget(tempWidget);
		ui->m_blockAddRemoveChild = true;
	}  
	m_layout->addItem(new QIrQuadSplitterLayoutItem(widget,m_index));
	m_layout->invalidate();
	ui->m_blockAddRemoveChild = false;
}
void QIrQuadSplitterLayoutStruct::deleteWidget()
{
	QIrQuadSplitter * s = m_layout->splitter();
	QIrQuadSplitterUi * ui = s->ui_func();

	if ( m_widget ) {
		m_layout->removeWidget(m_widget);
		m_widget->hide();
		m_widget->deleteLater();
		m_widget = 0;
		ui->popLastChild();		
	}
}


/////////////////////////////
//QIrQuadSplitterLayoutItem
/////////////////////////////
QIrQuadSplitterLayoutItem::QIrQuadSplitterLayoutItem(QWidget * widget, int index) : QWidgetItem(widget), m_index(index)
{	
}


////////////////////////
//QIrQuadSplitterLayout
////////////////////////
QIrQuadSplitterLayout::QIrQuadSplitterLayout( QWidget * parent ) : QLayout(parent)
{
	for(int i = 0; i < CAPACITY; i++)
		m_structList.append(new QIrQuadSplitterLayoutStruct(this,i));
}
QIrQuadSplitterLayout::~QIrQuadSplitterLayout()
{
	QLayoutItem * item;
	
	while ((item = takeAt(0)))
         delete item;

	while ( m_structList.isEmpty() )
		delete m_structList.takeFirst();

}
QIrQuadSplitter * QIrQuadSplitterLayout::splitter() const
{
	return static_cast< QIrQuadSplitter * >(parentWidget());
}
int QIrQuadSplitterLayout::splitterIndexOf( QWidget * w ) const
{
	int i = 0;

	if ( !w )
		return -1;

	foreach (QIrQuadSplitterLayoutStruct * s, m_structList) {
		if ( s->widget() == w )
			return i;
		i++;
	}
	return -1;
}
void QIrQuadSplitterLayout::addItem( QLayoutItem * item )
{
	if ( !splitter()->ui_func()->m_blockAddRemoveChild ) {
		qWarning("QIrQuadSplitterLayout::addItem(): Please use QIrQuadSplitter::setWidget()");
		return;
	}
	if ( indexOf(item->widget()) != -1 )
			return;
	m_itemList.append(static_cast< QIrQuadSplitterLayoutItem * >(item));
}
Qt::Orientations QIrQuadSplitterLayout::expandingDirections() const
{
	return Qt::Horizontal | Qt::Vertical;
}
bool QIrQuadSplitterLayout::hasHeightForWidth() const
{
	return false;
}
int QIrQuadSplitterLayout::nextFreeCell() const
{
	int i = 0;
	foreach (QIrQuadSplitterLayoutStruct * s, m_structList) {
		if ( !s->widget() )
			return i;
		i++;
	}
	return -1;
}
int QIrQuadSplitterLayout::count() const
{
	return m_itemList.size();
}
QLayoutItem * QIrQuadSplitterLayout::itemAt( int index ) const
{
	return m_itemList.value(index);
}
QIrQuadSplitterLayoutItem * QIrQuadSplitterLayout::splitterItemAt( int index ) const
{
	foreach (QIrQuadSplitterLayoutItem * item, m_itemList)
		if ( item->index() == index )
			return item;
	return 0;
}
QSize QIrQuadSplitterLayout::minimumSize() const
{
	return parentWidget()->minimumSizeHint();
}
void QIrQuadSplitterLayout::setGeometry( const QRect & )
{
	refresh();
}
QSize QIrQuadSplitterLayout::sizeHint() const
{
	return parentWidget()->sizeHint();
}
QLayoutItem * QIrQuadSplitterLayout::takeAt( int index )
{
	if (index >= 0 && index < m_itemList.size()) 
         return m_itemList.takeAt(index);
     else
         return 0;
}
bool QIrQuadSplitterLayout::hasVisibleItemAt( int index ) const
{
	QIrQuadSplitterLayoutItem * item = splitterItemAt(index);

	return item && item->widget()->isVisible();	
}
void QIrQuadSplitterLayout::refresh()
{
	QRect geom;
	QIrQuadSplitter * s = splitter(); 
	QIrQuadSplitterUi * ui = s->ui_func();
	int width = s->width(), height = s->height(), hW = s->handleWidth(), sHW = hW / 2;
	QPoint center  = ui->m_center;

	ui->m_blockResize = false;
	for(int i = 0; i < CAPACITY; i++) {
		if ( hasVisibleItemAt(i) ) {	
			geom = QRect(QPoint(),QSize(width,height));
			switch (i) {
			case 0:
				if ( hasVisibleItemAt(1) )
					geom.setRight(center.x() - sHW);
				if (hasVisibleItemAt(2) || hasVisibleItemAt(3))
					geom.setBottom(center.y() - sHW);
				break;		

			case 1:
				if ( hasVisibleItemAt(0) )
					geom.setLeft(center.x() + sHW);
				if (hasVisibleItemAt(2) || hasVisibleItemAt(3))
					geom.setBottom(center.y() - sHW);
				break;

			case 2:
				if (hasVisibleItemAt(0) || hasVisibleItemAt(1)) {
					geom.setTop(center.y() + sHW);
				}
				if( hasVisibleItemAt(3) )
					geom.setRight(center.x() - sHW);
				break;

			case 3:
				if ( hasVisibleItemAt(2) )
					geom.setLeft(center.x() + sHW);
				if (hasVisibleItemAt(0) || hasVisibleItemAt(1))
					geom.setTop(center.y() + sHW);
			}
			splitterItemAt(i)->setGeometry(geom);
		}
	}
	ui->m_blockResize = true;
}

QIR_END_NAMESPACE
