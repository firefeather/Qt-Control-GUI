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
#ifndef QIRDOCKEFFECTMANAGER_H
#define QIRDOCKEFFECTMANAGER_H

#include <QObject>
#include "../Common/qirobject.h"

class QMouseEvent;
class QRectF;
class QResizeEvent;
class QSize;

QIR_BEGIN_NAMESPACE

class QIrDock;
class QIrDockEffectManagerPrivate;
class QIrDefaultEffectManagerPrivate;
class QIrDocklet;
class QIrDockletBundle;
class QIrDockStyle;
class QIrDockView;

class QIRONSHARED_EXPORT QIrDockEffectManager : public QObject, public QIrObject
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDockEffectManager)
	friend class QIrDock;
	friend class QIrDockView;
	
public:
	explicit QIrDockEffectManager( QObject * parent = 0 );
	virtual ~QIrDockEffectManager();

	virtual QRectF contentRect() const = 0;

	virtual QRectF normalContentRect() const = 0;
	virtual QRectF extendContentRect() const = 0;

	virtual QSize sizeHint() const = 0;

	bool useAnimation() const;
	void setUseAnimation( bool );

	QIrDockView * view() const;
	
	virtual void clearHover() = 0;

	virtual void invalidate() = 0;
	
signals:
	void extendRequested();
	void normalizeRequested();
	void updateGeometryRequested();

protected:
	QIrDockEffectManager(QObject * parent, QIrDockEffectManagerPrivate &);

	static QSize sizeForIconSize(QIrDockStyle * style, int spacing, int minimumIconSize, int iconSize, int maximumIconSize, bool isSeparator, Qt::Orientation);
	
	virtual int closestLegalIconSize( const QSize & size ) const = 0;

	virtual void enterEvent( QEvent * evt ) = 0;
	virtual void leaveEvent( QEvent * evt ) = 0;	
	virtual void mouseMoveEvent( QMouseEvent * evt ) = 0;
	virtual void mousePressEvent( QMouseEvent * evt ) = 0;
	virtual void mouseReleaseEvent( QMouseEvent * ) = 0;
	virtual void reset() = 0;
	void hoverDockletChanged(QIrDocklet * , QIrDocklet * );

	virtual void polish();
	virtual void unpolish();
	virtual void polish( QIrDockletBundle * );
	virtual void unpolish( QIrDockletBundle * );
};

class QIRONSHARED_EXPORT QIrDefaultEffectManager : public QIrDockEffectManager
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDefaultEffectManager)

public:
	explicit QIrDefaultEffectManager( QObject * parent );
	~QIrDefaultEffectManager();

	virtual QRectF contentRect() const;

	virtual QRectF normalContentRect() const;
	virtual QRectF extendContentRect() const;

	virtual QSize sizeHint() const;

	virtual void invalidate();

	virtual void clearHover();
	
protected:
	QIrDefaultEffectManager(QObject * parent, QIrDefaultEffectManagerPrivate & p);

	virtual void doLayout();

	virtual int closestLegalIconSize( const QSize & size ) const;

	virtual void enterEvent( QEvent * evt );
	virtual void leaveEvent( QEvent * evt );	
	virtual void mouseMoveEvent( QMouseEvent * evt );
	virtual void mousePressEvent( QMouseEvent * evt );
	virtual void mouseReleaseEvent( QMouseEvent * );
	virtual void reset();

private:
	void changeHoverDocklet(QIrDocklet *, QIrDocklet *);
};

QIR_END_NAMESPACE

#endif // QIRDOCKEFFECTMANAGER_H
