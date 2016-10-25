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
#ifndef QIREXPANDER_H
#define QIREXPANDER_H

#include <QIcon>
#include <QWidget>
#include "../Common/qirwidget.h"


#define qir_expander_group_key "qiron_expander"

template< typename T >
class QList;
class QStyleOptionButton;

QIR_BEGIN_NAMESPACE

class QIrExpander;
class QIrExpanderLayout;
class QIrExpanderUi;

class QIRONSHARED_EXPORT QIrExpander : public QWidget, public QIrWidget
{	
	Q_OBJECT
	Q_ENUMS(HeaderPosition)
	QIR_DECLARE_UI(QIrExpander)
	Q_PROPERTY(QString title READ title WRITE setTitle DESIGNABLE true)
	Q_PROPERTY(QIcon icon READ icon WRITE setIcon DESIGNABLE true)
	Q_PROPERTY(bool expanded READ isExpanded WRITE setExpanded DESIGNABLE true)
	Q_PROPERTY(int headerHeight READ headerHeight WRITE setHeaderHeight DESIGNABLE true)
	Q_PROPERTY(HeaderPosition headerPosition READ headerPosition WRITE setHeaderPosition DESIGNABLE true)
	friend class QIrExpanderLayout;
		
public:
	QIR_WIDGET_CAST

	enum HeaderPosition{North, West, South, East};
	explicit QIrExpander(QWidget * parent = 0, HeaderPosition position = North);
	explicit QIrExpander(const QString & title, QWidget * parent = 0, HeaderPosition position = North);
	explicit QIrExpander(const QIcon &  icon, const QString & title, QWidget * parent = 0, HeaderPosition position = North);
	
	~QIrExpander();

	QWidget * widget() const;
	void setWidget( QWidget * );

	int headerHeight() const;
	void setHeaderHeight( int );

	HeaderPosition headerPosition() const;
	void setHeaderPosition( HeaderPosition );

	QIcon icon() const;
	void setIcon(const QIcon & );
	
	QString title() const;
	void setTitle( const QString & );

	bool isExpanded() const;
	void setExpanded( bool );

	inline QString widgetGroupKey() const { return qir_expander_group_key; }
	
protected:
	QIrSubStyle * defaultSubStyle() const;
	
	virtual void subStyleChanged();

	void resizeEvent( QResizeEvent * evt );
	void paintEvent( QPaintEvent * );
	bool eventFilter(QObject *, QEvent *);
	void childEvent( QChildEvent * );
	
signals:
	void expanded( bool );
	
public slots:
	void expand();
	void collapse();

private slots:
	void _w_expand(bool);
	void _w_expand() { _w_expand(!isExpanded()); }

private:
	Q_DISABLE_COPY(QIrExpander)
};

QIR_END_NAMESPACE

#endif //QIREXPANDER_H
