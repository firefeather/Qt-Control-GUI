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
#ifndef QIREXPANDER_UI_H
#define QIREXPANDER_UI_H

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

#include <QAbstractButton>
#include <QIcon>
#include <QLayout>
#include <QRect>
#include <QWidgetItem>
#include "../../Common/ui/qirwidget_ui.h"
#include "../qirexpander.h"

QIR_BEGIN_NAMESPACE

class QIrExpanderUi;

class QIrExpanderHeaderButton : public QAbstractButton
{
	Q_OBJECT

public:
	explicit QIrExpanderHeaderButton( QIrExpander * );
	~QIrExpanderHeaderButton();

	void initStyleOption( QStyleOptionButton * ) const;
		
protected:	
	void paintEvent( QPaintEvent * );

private:
	QIrExpander * m_expander;	
};

class QIrExpanderLayout : public QLayout
{
public:
	enum Position {North, West, South, East};
	enum ItemRole {Header, Central, RoleCount};

	QIrExpanderLayout(QWidget * parent);
	~QIrExpanderLayout();
	
	void addItem( QLayoutItem * item );
	void addWidget(QWidget * widget, ItemRole role);
	void setHeaderPosition(Position position);
	Qt::Orientations expandingDirections() const;
	bool hasHeightForWidth() const;
	int count() const;
	QLayoutItem *itemAt( int index ) const;
	QSize minimumSize() const;
	void setGeometry( const QRect & rect );
	QSize sizeHint() const;
	QLayoutItem * takeAt( int index );
	QSize minimumHeaderSize() const;
	void add(QLayoutItem * item, ItemRole);
	QSize sizeOnFirstShow() const;

private:
    friend class QIrExpander;
	friend class QIrExpanderUi;
	bool m_handleResize;
	Position m_position;
	enum SizeType { MinimumSize, SizeHint};
     QSize calculateSize(SizeType sizeType) const;
     QVector< QLayoutItem * > m_items;
	
 };

class QIrExpanderUi : public QIrWidgetUi
{
	QIR_DECLARE_WIDGET(QIrExpander);
	
public:
	QIrExpanderUi() : icon(QIcon()), title(QString::null), widget(0), 
                headerH(0),
                button(0),
                expanded(false),
                updateOldSize(true),
                blockAddChild(true),
                initializing(true),
                position(QIrExpander::North),
                layout(0),
                firstShow(true),
                addingWidget(false)
        { }
	~QIrExpanderUi() { }
	
	void adjustGeometry();
	int minimumHeaderWidth() const;
	void polish();

	QIcon icon;
	QString title;
	QWidget * widget;
	int headerH;
	QIrExpanderHeaderButton * button;
	bool expanded,
		opaque;
	bool updateOldSize;
	bool blockAddChild;
	bool initializing;
	QIrExpander::HeaderPosition position;
	QIrExpanderLayout * layout;
	QSize oldSize;
	bool firstShow;
	bool addingWidget;

protected:
	void init();
	void setupConnections();
};

QIR_END_NAMESPACE

#endif //QIREXPANDER_UI_H
