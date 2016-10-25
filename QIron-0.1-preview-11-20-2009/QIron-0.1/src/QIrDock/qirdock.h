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
#ifndef QIRDOCK_H
#define QIRDOCK_H

#include "qirabstractdock.h"

QIR_BEGIN_NAMESPACE

class QIrDockStyle;
class QIrDockUi;
class QIrDockView;
class QIrStyleOptionDock;
class QIrDockManager;
class QIrDockEffectManager;

class QIRONSHARED_EXPORT QIrDock : public QIrAbstractDock
{	
	Q_OBJECT
	QIR_DECLARE_UI(QIrDock)
	Q_PROPERTY(DefaultMenuItems defaultMenuItems READ defaultMenuItems WRITE setDefaultMenuItems)
	Q_DISABLE_COPY(QIrDock)
	friend class QIrDockView;

public:
	enum ShowTipMode {
		SpontaneousShowTip,
		NonSpontaneousShowTip
	};
	enum DefaultMenuItem {
		MI_NoMenuEntry = 0,
		MI_AutoHide = 1,
		MI_DockSettings = 2,
		MI_DockPosition = 4,
		MI_EntryProperty = 8,
		MI_LockDragging = 16,		
		MI_RemoveEntry = 32,
		MI_AllMenuEntries = MI_AutoHide | MI_DockSettings | MI_DockPosition | MI_LockDragging | MI_EntryProperty | MI_RemoveEntry
	};
	Q_DECLARE_FLAGS(DefaultMenuItems,DefaultMenuItem);

	explicit QIrDock( QWidget * parent = 0 );
	~QIrDock();

	using QIrAbstractDock::addDocklet;
	void insertDocklet(QIrDocklet * before, QIrDocklet * docklet);
	
	int count() const;

	DefaultMenuItems defaultMenuItems() const;
	void setDefaultMenuItems( DefaultMenuItems );

	QIrDocklet * dockletAt( int index ) const;	

	QSize closestAcceptableSize( const QSize & size );

	QIrDockEffectManager * effectManager() const;
	void setEffectManager( QIrDockEffectManager * effect );

	QIrDockStyle * dockStyle() const;

	QPoint extendOffset() const;

	QSize normalSize() const;

	ShowTipMode showTipMode() const;
	void setShowTipMode( ShowTipMode mode );

	QSize sizeHint() const;

	QIrDockView * view() const;

	QIrDockManager * manager() const;
	
	void restore();

	QIrDocklet * takeAt( int index );

	QRect visualRect( QIrDocklet * docklet ) const;

	inline QString widgetGroupKey() const { return "qiron_dock"; }
	
protected:
	friend class QIrDockletBundle;
	friend class QIrDockStyle;
	
	QIrSubStyle * defaultSubStyle() const;

	void evaluateCursorPos();

	void initStyleOption( QIrStyleOptionDock * ) const;

	void parentChanged();

	void contextMenuEvent( QContextMenuEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void paintEvent( QPaintEvent * );
	void resizeEvent( QResizeEvent * );

	virtual void subStyleChanged();	

protected slots:
	void slotChangeAutoHide( bool );
	void slotChangeMovable( bool );
	void slotChangeArea();
	void slotChangeAlignment();
	void slotDockSettings();
	void slotRemoveEntry();
	void slotEntryProperty();

private slots:
	inline void slotResetView() { resetView(); }
	void slotHandleMenuAboutToShow();
	void slotHandleMenuAboutToHide();
	void slotHandleDataChanged(const QVariant & data, QIrAbstractDock::DataRole);
	void slotUpdateTip();
	void slotPopupMenu( bool fromView = true );
	void slotHandleExtendRequest();
	void slotHandleNormalizeRequest();
	void slotFadeFinished();
	
private:
	void fadeTipIn();
	void fadeTipOut();
	void resetView();

	void doLayout();
	void showTip();
	void hideTip();
	void handleHoverDockletChanged(QIrDocklet * current, QIrDocklet * previous);

	QMargins getContentsMargins() const;

	enum SizeType {Normal, Preferred, Bounding };

	QSize smartDockSize( SizeType ) const;
};
QIR_END_NAMESPACE
#endif // QIRDOCK_H
