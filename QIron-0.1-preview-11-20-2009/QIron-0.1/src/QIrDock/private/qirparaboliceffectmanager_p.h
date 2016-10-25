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
#ifndef QIRPARABOLICEFFECTMANAGER_P_H
#define QIRPARABOLICEFFECTMANAGER_P_H

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

#include <QPointF>
#include <QMap>
#include <QVector>

#include "../qirparaboliceffectmanager.h"
#include "qirdockeffectmanager_p.h"

class QParallelAnimationGroup;
class QPropertyAnimation;
class QRectF;

QIR_BEGIN_NAMESPACE

class QIrParabolicEffectManagerPrivate : public QIrDockEffectManagerPrivate
{
	QIR_DECLARE_OBJECT(QIrParabolicEffectManager)

public:
        QIrParabolicEffectManagerPrivate() : coverage(2), isAnimationInterrupted(false),
        isTransiting(false), isExtending(false), mustStartAnimation(false),
        isHoverMode(false), restoring(false), doingLayout(false), hover(0), dockletClicked(0)
	{
	}	   
	~QIrParabolicEffectManagerPrivate()
	{ 
	}
	void animate();
	qreal calculateCoverage() const;
	void evaluatePos( const QPoint & );
	void restoreSizes();
	void updateMinDockletRects();
	void updateDockletRects();
	void updateAnimationValues( bool updateStartValue );
	void updateDockletIconSizes( const QPointF & p = QPointF() );
	void updateHoverDocklet( const QPointF & );
	
	int lastUnchangedLeftIndex;
	int firstUnchangedRightIndex;
	int coverage;

	bool isAnimationInterrupted,
		isTransiting,
		isExtending,
		mustStartAnimation,
		isHoverMode,
		restoring,
		doingLayout;
	QPointer< QIrDockletBundle > hover;
	QVector< QRectF > dockletRects, 
		minDockletRects;
	QParallelAnimationGroup * bundleAnimationGroup;
	bool clicked;
	QPointer< QIrDockletBundle > dockletClicked;
	QMap< QIrDockletBundle *, QPropertyAnimation * > geomAnimations;
	QMap< QIrDockletBundle *, QPropertyAnimation * > offsetAnimations;


protected:
	void init();
};

QIR_END_NAMESPACE

#endif // QIRPARABOLICEFFECTMANAGER_P_H

