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
#ifndef QIRDOCKLET_H
#define QIRDOCKLET_H

#include <QGraphicsWidget>
#include "../Common/qirobject.h"

class QIcon;
class QString;

QIR_BEGIN_NAMESPACE

class QIrAbstractDock;
class QIrDockletPrivate;
class QIrStyleOptionDocklet;

class QIRONSHARED_EXPORT QIrDocklet : public QGraphicsWidget, public QIrObject
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDocklet)
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry)
	friend class QIrAbstractDock;
	friend class QIrDock;

public:
	explicit QIrDocklet(QIrAbstractDock * dock = 0, Qt::WindowFlags wFlags = 0);
	explicit QIrDocklet(const QString & text, QIrAbstractDock * dock = 0);
	explicit QIrDocklet(const QIcon & icon, const QString & text, QIrAbstractDock * dock = 0);

	~QIrDocklet();

	bool isActive() const;
	void setActive( bool );

	void setEnabled( bool enabled );

	void setVisible( bool visible );

	QIrAbstractDock * dock() const;

	virtual void setGeometry( const QRectF & );

	QIcon icon() const;
	void setIcon( const QIcon & );

	int iconSize() const;
	void setIconSize( int );

	bool destroyed() const;

	bool isLauncher() const;
	void setLauncher( bool );

	bool isSeparator() const;
	void setSeparator( bool );

	QString text() const;
	void setText( const QString & );	
	
public slots:
	void slotUpdate();
	void slotTrigger();
	
signals:
	void activated( bool );
	void changed();
	void geometryChanged();
	void iconChanged( const QIcon & );
	void iconSizeChanged( int );
	void moved( const QPointF & );
	void resized( const QSizeF & );
	void textChanged( const QString & );
	void triggered();	
	
protected:
	explicit QIrDocklet(QIrAbstractDock * dock, QIrDockletPrivate & p);

	virtual void initStyleOption( QIrStyleOptionDocklet * option ) const;

	virtual void moveEvent( QGraphicsSceneMoveEvent * );
	virtual void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	virtual void resizeEvent( QGraphicsSceneResizeEvent * );
		
private:
	void setDock( QIrAbstractDock * );
};

QIR_END_NAMESPACE

#endif // QIRDOCKWIDGETITEM_H 
