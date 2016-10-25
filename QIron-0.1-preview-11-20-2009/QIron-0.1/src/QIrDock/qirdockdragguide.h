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
#ifndef QIRDOCKDRAGGUIDE_H
#define QIRDOCKDRAGGUIDE_H

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

#include "qirabstractdock.h"
#include "../Common/qirobject.h"

QIR_BEGIN_NAMESPACE

class QIrDockDragGuideUi;
class QIrDockDragGuideManager;
class QIrDockDragGuideManagerPrivate;
class QIrDockManager;
class QIrStyleOptionDockDragGuide;
class QIrStyleOptionDockDragGuideIndicator;

class QIrDockDragGuide : public QWidget, public QIrWidget
{
	Q_OBJECT
	QIR_DECLARE_UI(QIrDockDragGuide)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	friend class QIrDockDragGuideManager;

	enum IndicatorRegion {
		IR_LeftIndicator, 
		IR_CenterIndicator, 
		IR_RightIndicator,
		IR_None
	};

public:
	QIR_WIDGET_CAST

	explicit QIrDockDragGuide(QIrAbstractDock::DockArea area, QIrDockDragGuideManager * manager, QWidget * parent = 0);
	~QIrDockDragGuide() { } 

	QIrAbstractDock::DockArea dockArea() const;
	QIrAbstractDock::DockAlignment dockAlignment() const;

	QIrSubStyle * dockSubStyle() const;

	QIrDockDragGuideManager * manager() const;

	qreal opacity() const;
	void setOpacity( qreal );

	QSize sizeHint() const;

	QString widgetGroupKey() const;

	void init();

signals:
	void alignmentChanged(QIrAbstractDock::DockAlignment newAligment, QIrAbstractDock::DockAlignment previousAligment);

protected:
	void enterEvent ( QEvent * );
	void leaveEvent( QEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void paintEvent( QPaintEvent * );	
	void showEvent( QShowEvent * );
	
	void initStyleOption( QIrStyleOptionDockDragGuide * ) const;
	void initStyleOption(QIrStyleOptionDockDragGuideIndicator *,IndicatorRegion reg) const; 

private slots:
	void handleAnimationFinished();

private:
	void deploy();
	void undeploy();
	void setAnimationGeometry(const QRect & start, const QRect & end);
};

class QIrDockDragGuideManager : public QObject, public QIrObject
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDockDragGuideManager)
	Q_PROPERTY(bool useAnimation READ useAnimation WRITE setUseAnimation);

public:
	QIrDockDragGuideManager( QIrDockManager * manager );
	~QIrDockDragGuideManager();
	
	void deploy();
	void undeploy();

	QIrDockManager * dockManager() const;
	
	void updateManaged( QWidget * );

	bool useAnimation() const;
	void setUseAnimation( bool use );

protected:
	bool eventFilter(QObject * , QEvent *);

private:
	QIrDockDragGuide * guideAt( const QPoint & ) const;
	QRect getGuideGeometry( QIrDockDragGuide * ) const;
	
	void enter( QEvent * );
	void leave( QEvent * );
	void mouseMove( QMouseEvent * );
	void mouseRelease( QMouseEvent * );
};

QIR_END_NAMESPACE

#endif // QIRDOCKDRAGGUIDE_H
