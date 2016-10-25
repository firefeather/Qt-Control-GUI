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
#ifndef QIRDOCKTIPLABEL_H
#define QIRDOCKTIPLABEL_H

#include <QWidget>
#include "../Common/qirwidget.h"


QIR_BEGIN_NAMESPACE

class QIrDock;
class QIrDockTipLabelUi;
class QIrStyleOptionDockTipLabel;

class QIRONSHARED_EXPORT QIrDockTipLabel : public QWidget, public QIrWidget
{
	Q_OBJECT
	QIR_DECLARE_UI(QIrDockTipLabel)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
	QIR_WIDGET_CAST

	explicit QIrDockTipLabel(QIrDock * dock, QWidget * parent = 0 );
	~QIrDockTipLabel();

	QSize sizeHint() const;

	qreal opacity() const;
	void setOpacity( qreal opacity );

	QString text() const;
	void setText( const QString & );

	QRect targetVisualRect() const;
	void setTargetVisualRect( const QRect & rect );
		
protected:
	void paintEvent( QPaintEvent * );
	void initStyleOption( QIrStyleOptionDockTipLabel * option ) const;
};

QIR_END_NAMESPACE

#endif // QIRDOCKTIPLABEL_H
