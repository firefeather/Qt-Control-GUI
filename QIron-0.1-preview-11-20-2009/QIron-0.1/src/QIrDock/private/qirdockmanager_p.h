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
#ifndef QIRDOCKMANAGER_P_H
#define QIRDOCKMANAGER_P_H

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

#include <QCursor>
#include <QPropertyAnimation>
#include <QTimer>
#include "../../Common/private/qirobject_p.h"
#include "../qirdockmanager.h"
#include "../../Common/qirwidget.h"

QIR_BEGIN_NAMESPACE

//internal
class QIrDockDragOutline : public QWidget, public QIrWidget
{
public:
	QIR_WIDGET_CAST

	QIrDockDragOutline(QIrDockManager * manager, QWidget * parent = 0);
	~QIrDockDragOutline();

	QString widgetGroupKey() const;
	
protected:
	void paintEvent( QPaintEvent * );

private:
	QIrDockManager * m_manager;
};

class QIrDockManagerPrivate : public QIrObjectPrivate
{	
	QIR_DECLARE_OBJECT(QIrDockManager);

public:
	QIrDockManagerPrivate();
	~QIrDockManagerPrivate();

	QRect calculateDockGeometry( bool ignoreState = false ) const;
	void dropDock();
	void evaluatePos( const QPoint & pos = QCursor::pos() );
	void getAnimationOffsets(const QRect & destGeom, int & startOffset, int & endOffset, bool interrupted) const;
	QRect getDragOutlineGeometry() const;
	QSize smartDockSize( bool ignore = false ) const;
	
	struct DragInfo {
		QRect geometry;
		QIrAbstractDock::DockArea area;
		QIrAbstractDock::DockAlignment alignment;	
	} dragInfo;
	bool alreadyShown,
		isInSensitiveRegion,
		managedWindowIsActive,
		quiet,
		toShow,
		called,
		showBlocked,
		hideBlocked,
		callShow;
	int animOffset;
	QIrAbstractDock::DockArea lastArea;
	QIrAbstractDock * dock;
	QWidget * managed;
	QPointer< QIrDockDragOutline > dragOutline;
	QIrDockDragGuideManager * guideManager;
	QPropertyAnimation * showAnimation;
	QTimer * showTimer;
	QTimer * quietTimer;
	bool lockMouseEvent;
	bool lockUpdateGeometry;

protected:
	void init();	
	void setupConnections();
};

QIR_END_NAMESPACE

#endif // QIRDOCKMANAGER_P_H
