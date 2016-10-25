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
#ifndef QIREXPANDERBOX_H
#define QIREXPANDERBOX_H

#include <QScrollArea>
#include "../Common/qirwidget.h"

template < typename T >
class QList;

QIR_BEGIN_NAMESPACE 

class QIrExpander;
class QIrExpanderBoxUi;

class QIRONSHARED_EXPORT QIrExpanderBox : public QScrollArea, public QIrWidget
{
	Q_OBJECT
	QIR_DECLARE_UI(QIrExpanderBox)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation DESIGNABLE true)
	Q_PROPERTY(bool opaqueResize READ opaqueResize WRITE setOpaqueResize DESIGNABLE true)
	Q_PROPERTY(int count READ count)
	Q_PROPERTY(bool movable READ isMovable WRITE setMovable DESIGNABLE true)
	Q_PROPERTY(int uniformHeaderHeight READ uniformHeaderHeight WRITE setUniformHeaderHeight DESIGNABLE true)

public:
	QIR_WIDGET_CAST

	explicit QIrExpanderBox(QWidget * parent = 0 );
	explicit QIrExpanderBox(Qt::Orientation, QWidget * parent = 0);
	~QIrExpanderBox();

	Qt::Orientation orientation();
	void setOrientation( Qt::Orientation );

	bool opaqueResize() const;
	void setOpaqueResize( bool );

	QIrExpander * addWidget( QWidget * );
	QIrExpander * addWidget(const QString & title, QWidget *);
	QIrExpander * addWidget(const QIcon & icon, const QString & title, QWidget *);
	void addExpander( QIrExpander * expander );
	
	int indexOfWidget( QWidget * ) const;
	int indexOfExpander( QIrExpander * ) const;

	QWidget * widgetAt( int ) const;
	QIrExpander * expander( QWidget * ) const;
	QIrExpander * expanderAt( int ) const;	

	int count();

	QList< QWidget * > widgets() const;
	QList< QIrExpander * > expanders() const;

	bool isMovable() const;
	void setMovable( bool );
	
	int uniformHeaderHeight() const;
	void setUniformHeaderHeight( int );
	
	QList< int > sizes() const;
	void setSizes( const QList< int > & );

	QString widgetGroupKey() const { return "qiron_groubBox"; }

protected:
	void childEvent( QChildEvent * );

private slots:
	void _w_updateSplitter();

private:
	Q_DISABLE_COPY(QIrExpanderBox)
};

QIR_END_NAMESPACE

#endif // QIREXPANDERBOX_H
