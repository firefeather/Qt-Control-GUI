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
#ifndef QIRQUADSPLITTER_H
#define QIRQUADSPLITTER_H

#include <QFrame>
#include "../Common/qirwidget.h"

QIR_BEGIN_NAMESPACE

class QIrQuadSplitterUi;
class QIrQuadSplitterLayoutStruct;
class QIrQuadSplitterLayout;

class QIRONSHARED_EXPORT QIrQuadSplitter : public QFrame, public QIrWidget
{
	Q_OBJECT
	QIR_DECLARE_UI(QIrQuadSplitter)
	Q_PROPERTY(int handleWidth READ handleWidth WRITE setHandleWidth DESIGNABLE true);
	Q_PROPERTY(bool opaqueResize READ opaqueResize WRITE setOpaqueResize DESIGNABLE true);
	Q_PROPERTY(bool keepRatios READ keepRatios WRITE setKeepRatios DESIGNABLE true);
	Q_PROPERTY(double verticalRatio READ verticalRatio WRITE setVerticalRatio DESIGNABLE true);
	Q_PROPERTY(double horizontalRatio READ horizontalRatio WRITE setHorizontalRatio DESIGNABLE true);
	Q_PROPERTY(int count READ count);
	friend class QIrQuadSplitterLayoutStruct;	
	friend class QIrQuadSplitterLayout;

public:
	QIR_WIDGET_CAST

	QIrQuadSplitter(QWidget * parent = 0);
	~QIrQuadSplitter();

	int count() const;

	int handleWidth() const;
	void setHandleWidth( int );

	bool opaqueResize() const;
	void setOpaqueResize( bool );
	
	bool keepRatios() const;
	void setKeepRatios( bool );

	double verticalRatio() const;
	void setVerticalRatio( double );
	double horizontalRatio() const;
	void setHorizontalRatio( double );

	QWidget * widgetAt(int i, int j);
	QWidgetList queuedWidgets() const;

	void setWidget(QWidget * , int i, int j); 
	void removeWidget( QWidget * );
	void removeWidgetAt(int i, int j);

	int indexOf( QWidget * ) const;

    QSize sizeHint() const;
	QSize minimumSizeHint() const;
 
protected:
	bool event( QEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void keyPressEvent( QKeyEvent * );
	void resizeEvent( QResizeEvent * );
	bool eventFilter(QObject *, QEvent *);
	void childEvent( QChildEvent * );
	void changeEvent( QEvent * );
};

QIR_END_NAMESPACE

#endif // QIRQUADSPLITTER_H
