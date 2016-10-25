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
#ifndef QIRQUADSPLITTERLAYOUT_H
#define QIRQUADSPLITTERLAYOUT_H

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

#include <QLayout>
#include "../Common/qirglobal.h"

QIR_BEGIN_NAMESPACE

class QIrQuadSplitter;
class QIrQuadSplitterUi;
class QIrQuadSplitterLayout;

class QIrQuadSplitterLayoutStruct
{
public:
        QIrQuadSplitterLayoutStruct( QIrQuadSplitterLayout * layout, int index ) : m_widget(0), m_layout(layout), m_index(index)
	{
	}
	inline QWidget * widget() const { return m_widget; }
	
	void setWidget( QWidget *, bool queue = true, bool popLast = true);
	void deleteWidget();

	inline int index() { return m_index; }

private:
	QWidget * m_widget;
	QIrQuadSplitterLayout * m_layout;
	int m_index;
};

class QIrQuadSplitterLayoutItem : public QWidgetItem
{
public:
	QIrQuadSplitterLayoutItem(QWidget * widget, int index);

	inline int index() const { return m_index; }

	int m_index;
};

class QIrQuadSplitterLayout : public QLayout
{
	Q_OBJECT
	friend class QIrQuadSplitter;
	friend class QIrQuadSplitterUi;
	friend class QIrQuadSplitterLayoutStruct;
	
public:
	QIrQuadSplitterLayout( QWidget * parent = 0 );
    ~QIrQuadSplitterLayout();

     void addItem( QLayoutItem * item );
     Qt::Orientations expandingDirections() const;
     bool hasHeightForWidth() const;
     int count() const;
     QLayoutItem * itemAt( int index ) const;
	 QIrQuadSplitterLayoutItem * splitterItemAt( int index ) const;
     QSize minimumSize() const;
     void setGeometry(const QRect & rect);
     QSize sizeHint() const;
     QLayoutItem * takeAt( int index );
	 bool hasVisibleItemAt( int index ) const;
	 QIrQuadSplitter * splitter() const;
	 int nextFreeCell() const;
	 int splitterIndexOf( QWidget * ) const;

private:	
	void refresh();
	QList< QIrQuadSplitterLayoutItem * > m_itemList;
	QList< QIrQuadSplitterLayoutStruct * > m_structList;
};

QIR_END_NAMESPACE

#endif // QIRQUADSPLITTERLAYOUT_H
