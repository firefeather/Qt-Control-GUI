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
#ifndef QIRPARABOLICEFFECTMANAGER_H
#define QIRPARABOLICEFFECTMANAGER_H

#include "qirdockeffectmanager.h"


QIR_BEGIN_NAMESPACE

class QIrParabolicEffectManagerPrivate;

class QIRONSHARED_EXPORT QIrParabolicEffectManager : public QIrDockEffectManager
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrParabolicEffectManager)
				
public:
	explicit QIrParabolicEffectManager( QObject * parent = 0 );
	~QIrParabolicEffectManager();

	QRectF contentRect() const;
	QRectF normalContentRect() const;
	QRectF extendContentRect() const;

	int dockletCoverage() const;
	void setDockletCoverage( int );

	QIrDocklet * hoverDocklet();

	void invalidate();

	virtual void reset();

	QSize sizeHint() const;

	void clearHover();
	
protected:
	virtual void doLayout();

	virtual int calculateIconSize( int deltaCursorPos ) const;
	virtual int calculateReach() const;
	virtual int closestLegalIconSize( const QSize & s ) const;



	virtual void polish( QIrDockletBundle * );
	virtual void unpolish( QIrDockletBundle * );

	void enterEvent( QEvent * );
	void leaveEvent( QEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );

private slots:
	void slotHandleDockletClicked();
	void slotHandleAnimationFinished();
	void slotHandleAnimationStateChanged();
	
private:
	void changeHoverDocklet(QIrDocklet *, QIrDocklet *);
};

QIR_END_NAMESPACE

#endif // QIRPARABOLICEFFECTMANAGER_H
