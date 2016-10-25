/********************************************************************** 
**
** This file is part of QIron Toolkit.
**
** Copyright (C) 2009-2020 Dzimi Mve Alexandre <dzimiwine@gmail.com>
** 
** Contact: dzimiwine@gmail.com
** 
** QIron is a free toolkit developed in C++/Qt by Dzimi Mve Alexandre; you can redistribute 
** sources and libraries of this library and/or modify them under the terms of 
** the GNU Library General Public License version 3.0 as published by the 
** Free Software Foundation and appearing in the file LICENSE.txt included in 
** the packaging of this file.
** Please review the following information to ensure the GNU General 
** Public License version 3.0 requirements will be met: 
** http://www.gnu.org/copyleft/gpl.html.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
**********************************************************************/
/*!
  \brief This file contains the declaration of QIrAbstractDock class.
  \file qirabstractdock.h
  \author Dzimi Mve Alexandre
  \date 10/12/2009
*/
#ifndef QIRABSTRACTDOCK_H
#define QIRABSTRACTDOCK_H

#include <QWidget>
#include "../Common/qirwidget.h"

//Used to track memory leaks using Microsoft Visual Studio.
#if defined(QIR_VS_MEMORY_LEAKS_CHECK)
	#if defined(Q_CC_MSVC) && defined(WIN32) && defined(_DEBUG)
     #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
     #define new DEBUG_NEW
  #endif
#endif

QIR_BEGIN_NAMESPACE

class QIrAbstractDockUi;
class QIrDocklet;
class QIrDockManager;
class QIrStyleOptionDocklet;
const char qiron_dock_drag_elements[] = "qiron_dock_drag_elements";

class QIRONSHARED_EXPORT QIrAbstractDock : public QWidget, public QIrWidget
{
	Q_OBJECT
	QIR_DECLARE_UI(QIrAbstractDock)
	Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide)
	Q_PROPERTY(int autoHideDelay READ autoHideDelay WRITE setAutoHideDelay DESIGNABLE true)
	Q_PROPERTY(DockAlignment dockAlignment READ dockAlignment WRITE setDockAlignment)
	Q_PROPERTY(DockArea dockArea READ dockArea WRITE setDockArea)
	Q_PROPERTY(int dockOffset READ dockOffset WRITE setDockOffset DESIGNABLE true)
	Q_PROPERTY(bool isMovable READ isMovable WRITE setMovable)
	Q_PROPERTY(int maximumDockWidth READ maximumDockWidth DESIGNABLE true)
	Q_PROPERTY(double maximumDockWidthF READ maximumDockWidthF WRITE setMaximumDockWidthF DESIGNABLE true)
	Q_PROPERTY(int maximumIconSize READ maximumIconSize WRITE setMaximumIconSize DESIGNABLE true)
	Q_PROPERTY(int minimumIconSize READ minimumIconSize WRITE setMinimumIconSize DESIGNABLE true)
	Q_PROPERTY(int quietDelay READ quietDelay WRITE setQuietDelay DESIGNABLE true)
	Q_PROPERTY(bool useAnimation READ useAnimation WRITE setUseAnimation DESIGNABLE true)
	Q_DISABLE_COPY(QIrAbstractDock)
	friend class QIrDockManager;
	Q_ENUMS(DockState)

public:
	QIR_WIDGET_CAST

	enum DataRole {
		DR_Alignment,
		DR_AllowedAlignments,
		DR_AllowedAreas,
		DR_Area,		
		DR_AutoHide,
		DR_AutoHideDelay,
		DR_Offset,
		DR_Other,
		DR_Icon,
		DR_Movable,
		DR_QuietDelay,
		DR_Title
	};
	enum DockState {
		DS_Dragging,
		DS_Dropping,
		DS_Hidden, 
		DS_Hiding,
		DS_MenuPoppedUp,
		DS_Showing,
		DS_Shown
	};
	enum DockArea {
		DA_BottomDockArea = 1,
		DA_LeftDockArea = 2,
		DA_NoDockArea = 0,
		DA_RightDockArea = 4,
		DA_TopDockArea = 8,
		DA_AllDockAreas = DA_TopDockArea | DA_LeftDockArea | DA_BottomDockArea | DA_RightDockArea
	};
	enum DockAlignment {
		DAL_DockAlignCenter = 1,
		DAL_DockAlignLeft = 2,
		DAL_DockAlignRight = 4, 
		DAL_NoDockAlignment = 0,
		DAL_AllDockAlignments = DAL_DockAlignLeft | DAL_DockAlignCenter | DAL_DockAlignRight
	};
	enum DockSizePolicy {
		DP_Ignored,
		DP_Maximum, 
		DP_Preferred
	};
	Q_DECLARE_FLAGS(DockAreas,DockArea);
	Q_DECLARE_FLAGS(DockAlignments,DockAlignment);

	//Contructor and Destructor
	explicit QIrAbstractDock(QWidget * parent = 0, DockArea area = DA_BottomDockArea, DockAlignment alignment = DAL_DockAlignCenter);
	~QIrAbstractDock();

	//Add and remove docklets
	QIrDocklet * addDocklet( const QString & text );
	QIrDocklet * addDocklet(const QIcon & icon, const QString & text);
	QIrDocklet * addDocklet(const QString & text, const QObject * receiver, const char * member);
	QIrDocklet * addDocklet(const QIcon & icon, const QString & text, const QObject * receiver, const char * member);
	void addDocklet( QIrDocklet * );
	void addDocklets( const QList< QIrDocklet * > & docklets );

	QIrDocklet * addSeparator();
	QIrDocklet * insertSeparator( QIrDocklet * before );

	virtual void insertDocklet(QIrDocklet * before, QIrDocklet * docklet) = 0;
	void insertDocklets(QIrDocklet * before, QList< QIrDocklet * > docklet);	
	
	void removeDocklet( QIrDocklet *  docklet );

	virtual int count() const = 0;

	virtual QIrDocklet * dockletAt( int ) const = 0;	
	
	inline virtual QSize closestAcceptableSize( const QSize & size ) { return size; } 
	
	void clear();

	virtual int indexOf ( QIrDocklet * ) const;

	QRect normalDockGeometry() const;

	virtual QSize normalSize() const { return QSize(); }

	inline virtual void restore() {};

	virtual QIrDocklet * takeAt( int ) = 0;

	virtual QRect visualRect( QIrDocklet * docklet ) const = 0;

	//Properties
	DockAreas allowedDockAreas() const;
	void setAllowedDockAreas( DockAreas );

	DockAlignments allowedDockAlignments( DockArea ) const;
	void setAllowedDockAlignments(DockArea, DockAlignments);

	bool autoHide() const;
	void setAutoHide( bool );

	int autoHideDelay() const;
	void setAutoHideDelay( int );

	DockAlignment dockAlignment() const;
	void setDockAlignment( DockAlignment );

	DockArea dockArea() const;
	void setDockArea( DockArea );

	int dockOffset() const;
	void setDockOffset( int );

	DockState dockState() const;
	void setDockState( DockState );

	int effectiveMinimumIconSize() const;

	inline virtual QPoint extendOffset() const { return QPoint(); };

	QIcon icon() const;
	void setIcon( const QIcon & icon );

	bool isExtended() const;

	bool isMovable() const;
	void setMovable( bool );

	double maximumDockWidthF() const;
	void setMaximumDockWidthF( double );

	int maximumDockWidth() const;

	int maximumIconSize() const;
	void setMaximumIconSize( int );

	int minimumIconSize() const;
	void setMinimumIconSize( int );

	Qt::Orientation orientation() const;

	void overrideMinimumIconSize( int );
	
	int quietDelay() const;
	void setQuietDelay( int );

	QString title() const;
	void setTitle( const QString & );

	bool useAnimation() const;
	void setUseAnimation( bool );

public slots:
	void slotUpdateGeometry();
	
protected:
	friend class QIrDocklet;
	
	QIrAbstractDock(QWidget * parent, QIrAbstractDockUi &);

	virtual bool event( QEvent * );

	virtual void initStyleOptionDocklet(const QIrDocklet *, QIrStyleOptionDocklet * ) const;

	virtual void parentChanged();
	
	void ownDocklet( QIrDocklet * );
	void releaseDocklet( QIrDocklet * );

	void setExtended( bool );
	void startDrag();
	void endDrag();

	inline virtual void evaluateCursorPos() { };

signals:
	void autoHideChanged( bool );
	void dataChanged(const QVariant & data, QIrAbstractDock::DataRole);
	void dockAlignmentChanged( QIrAbstractDock::DockAlignment );
	void dockAreaChanged( QIrAbstractDock::DockArea );
	void dockStateChanged( QIrAbstractDock::DockState );
	void hovered( QIrDocklet * );
	void minimumIconSizeChanged( int );
	void maximumIconSizeChanged( int );
	void triggered( QIrDocklet * );
	void updateGeometryRequested();
	
private:
	void setNormalDockGeometry( const QRect & geometry );

private slots:
	void slotHandleTriggered();
};

QIR_END_NAMESPACE

#endif // QIRABSTRACTDOCK_H
