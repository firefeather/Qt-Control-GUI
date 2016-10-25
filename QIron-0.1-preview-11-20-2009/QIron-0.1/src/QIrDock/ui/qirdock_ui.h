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
#ifndef QIrDock_UI_H
#define QIrDock_UI_H

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

#include "../qirdock.h"
#include "qirabstractdock_ui.h"

class QActionGroup;
class QMenu;
class QPropertyAnimation;

QIR_BEGIN_NAMESPACE

class QIrDockTipLabel;
class QIrDockView;

class QIrDockUi : public QIrAbstractDockUi
{
	QIR_DECLARE_WIDGET(QIrDock)

public:
	QIrDockUi();
	~QIrDockUi() { }

	QMenu * getActiveMenu();
	void setupMenu();
	bool shouldRecalculateClosestLegalSize( const QSize & size ) const;
	void updateActionsStates();
	void updateDockMenu();


	//cache
	mutable bool isDirty;
	mutable bool marginsDirty;
	mutable QSize lastSizeToAdjust;
	bool tipToShow;
	QSize lastSizeAdjusted;

	//Menu action
	QAction * actAutoHide,
		* actLockDragging,
		* actTopDockArea,
		* actBottomDockArea,
		* actRightDockArea,
		* actLeftDockArea,
		* actDockLeftAlign,
		* actDockCenterAlign,
		* actDockRightAlign,
		* actDockSettings,
		* actRemoveEntry,
		* actEntryProperty;
	QActionGroup * actAreaGroup;
	QActionGroup * actAlignGroup;

	QPoint lastCursPos;
	QIrDockView * view;
	QMenu * menu,
		* dockMenu,
		* positionMenu;

	QIrDockTipLabel * tipLabel;
	QPointer< QIrDocklet > hoverDocklet;
	QPointer< QIrDocklet > lastNonNullHoverDocklet;

	int spacing;
	bool fadeIn;
	QPropertyAnimation * fadeAnimation;

	QIrDock::ShowTipMode showTipMode;
	QIrDock::DefaultMenuItems defaultItems;

protected:
	virtual void createActions();
	virtual void createMenus();
	virtual void init();	
	virtual void setupConnections();
};

QIR_END_NAMESPACE

#endif // QIrDock_UI_H
