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
#ifndef QIRDOCKMANAGER_H
#define QIRDOCKMANAGER_H

#include "qirabstractdock.h"
#include "../Common/qirobject.h"

QIR_BEGIN_NAMESPACE

class QIrAbstractDock;
class QIrDockDragGuideManager;
class QIrDockManagerPrivate;

class QIRONSHARED_EXPORT QIrDockManager : public QObject, public QIrObject
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDockManager);
	Q_PROPERTY(int animatedOffset READ animatedOffset WRITE setAnimatedOffset DESIGNABLE false)
	friend class QIrAbstractDock;
	Q_DISABLE_COPY(QIrDockManager)

public:
	explicit QIrDockManager( QIrAbstractDock * parent );
	~QIrDockManager();

	QIrAbstractDock * dock() const;

	bool isShowBlocked() const;
	void blockShow( bool );

	bool isHideBlocked() const;
	void blockHide( bool );

	QWidget * managed() const;
	void setManaged( QWidget * );

	QRect sensitiveRegion() const;

	void updateNormalDockGeometry();

protected:
	bool eventFilter(QObject *, QEvent *);

public slots:
	void slotShowDock( bool );
	void slotUpdateDockGeometry();

private slots:
	void slotHandleDragAlignChanged(QIrAbstractDock::DockAlignment ,QIrAbstractDock::DockAlignment );
	void slotHandleAnimationFinished();
	void slotHandleDataChanged(const QVariant &, QIrAbstractDock::DataRole); 
	void slotHandleDockPositionChanged();
	void slotHandleShowTimeout();
	void slotEnterQuietTime();
	void slotLeaveQuietTime();

private:
	int animatedOffset() const;
	void setAnimatedOffset( int );
	void setManagedWindowActive( bool );
	void plugDock();
	void unplugDock();
	void resize( QResizeEvent * ); 
	void setDockGeometry( const QRect & );
};

QIR_END_NAMESPACE

#endif // QIRDOCKMANAGER_H
