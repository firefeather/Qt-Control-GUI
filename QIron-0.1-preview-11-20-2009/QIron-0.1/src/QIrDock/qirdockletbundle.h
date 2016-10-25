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
#ifndef QIRDOCKLETBUNDLE_H
#define QIRDOCKLETBUNDLE_H

#include <QGraphicsWidget>
#include "../Common/qirobject.h"

QIR_BEGIN_NAMESPACE

class QIrDocklet;
class QIrDockletBundlePrivate;
class QIrDockView;
class QIrStyleOptionDockletBundle;

class QIRONSHARED_EXPORT QIrDockletBundle : public QGraphicsWidget, public QIrObject
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDockletBundle)
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(QPointF dockletOffset READ dockletOffset WRITE setDockletOffset)
	Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry)
	friend class QIrDock;
	friend class QIrDockView;

	
public:
	explicit QIrDockletBundle(QIrDockView * view, QIrDocklet * docklet);
	~QIrDockletBundle();

	QIrDocklet * docklet() const;

	QPointF dockletOffset() const;
	void setDockletOffset( const QPointF & offset );
	
	bool isDown() const;
	void setDown( bool down );

	void paint(QPainter *, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

	virtual void setGeometry( const QRectF & geom );

	void invalidate();

public slots:
	inline void slotUpdate() { update(); }

signals:
	void entered();
	void left();
	
	void doubleClicked();
	void clicked();
	void pressed();
	void released();
		
	void geometryChanged();

protected:
	virtual bool event( QEvent * );

	virtual void initStyleOption( QIrStyleOptionDockletBundle * option ) const;

	virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * evt );
	virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * evt );

	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * evt );
	virtual void mousePressEvent( QGraphicsSceneMouseEvent * evt );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * evt );
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * evt );

private:
	bool isInternallyVisible() const;

	void releaseDocklet();
	bool hitBundle( const QPointF & ) const;
};

QIR_END_NAMESPACE

#endif // QIRDOCKLETBUNDLE_H
