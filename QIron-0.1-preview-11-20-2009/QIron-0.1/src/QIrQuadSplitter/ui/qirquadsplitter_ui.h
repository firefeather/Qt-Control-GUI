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
#ifndef QIRQUADSPLITTER_UI_H
#define QIRQUADSPLITTER_UI_H

#include <QPointer>
#include <QRubberBand>
#include <QPainter>
#include <QList>
#include "../qirquadsplitter.h"
#include "../../Common/ui/qirwidget_ui.h"

#define CAPACITY 4

class QIrQuadSplitterLayout;

class QIrQuadSplitterUi : public QIrWidgetUi
{
	QIR_DECLARE_WIDGET(QIrQuadSplitter)

public:
	enum Orientation {
		None = 0,
		Horizontal = 1,
		Vertical = 2,
		All = 3	
	};	
	struct Ratio {
		double vertical;
		double horizontal; 
	} m_ratio;

        QIrQuadSplitterUi() : m_opaque(true), m_handleWidth(0), m_orientation(None), m_verticalBand(0), m_horizontalBand(0)
                , m_moveCancelled(false), m_keepRatios(false), m_blockAddRemoveChild(false),  m_blockResize(true)
	{ }

	void showRubberBands();
	void hideRubberBands();
	void setRubberBands( const QPoint & );

	void doMove( const QPoint & );

	void updateRatios();	
	void updateCursor();
	
	QIrQuadSplitterUi::Orientation getOrientation( const QPoint & );
	void setOrientation( QIrQuadSplitterUi::Orientation );

	QPoint closestLegalPosition( const QPoint & );
	QPoint adjustedPosition( const QPoint & );
	void adjustCenter();

	void setCenter();
	void setCenter( const QPoint &, bool update);
	void moveCenter( const QPoint & );
	void resetCenter();

	void recalc(); 
	QSize calcMinSize() const;

	void queueChild( QWidget * );
	void dequeueChild( QWidget * );
	void popLastChild();
			
	bool m_opaque;
	QPoint m_center;
	QPoint m_newCenter;
	int m_handleWidth;
	Orientation  m_orientation;
	QPointer< QRubberBand > m_verticalBand;
	QPointer< QRubberBand > m_horizontalBand;
	bool m_moveCancelled;
	bool m_keepRatios;
	bool m_blockAddRemoveChild;
	bool m_blockResize;
	QIrQuadSplitterLayout * m_layout;
	QList< QWidget * > m_queuedChildren;

protected:
	void init();
};

#endif // QIRQUADSPLITTER_UI_H

