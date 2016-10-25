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
#ifndef QIRABSTRACTDOCK_UI_H
#define QIRABSTRACTDOCK_UI_H

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

#include <QHash>
#include "../qirabstractdock.h"
#include "../../Common/ui/qirwidget_ui.h"

QIR_BEGIN_NAMESPACE

class QIrDockManager;

class QIrAbstractDockUi : public QIrWidgetUi
{
	QIR_DECLARE_WIDGET(QIrAbstractDock)

public:
	QIrAbstractDockUi() 
                : state(QIrAbstractDock::DS_Shown),
                autoHide(false),
                extended(false),
                lockSignal(false),
		movable(true),
                useAnimation(true),
                firstTimeShown(true),
                maxDockWidth(1.0),
                autoHideDelay(1000),
                quietDelay(1000),
                dockOffset(0),
                offset(0),
		overrideMinIconSize(-1),
		minIconSize(44),
                maxIconSize(64),
                manager(0),
                wid(0)
	{ }
	virtual ~QIrAbstractDockUi() { }

	bool adjustDockAlignment();
	bool adjustDockArea();

	QIrAbstractDock::DockArea area;
	QIrAbstractDock::DockAlignment alignment;
	QIrAbstractDock::DockAreas allowedAreas;
	QHash< QIrAbstractDock::DockArea, QIrAbstractDock::DockAlignments > allowedAlignments;
	QIrAbstractDock::DockState state;
	QRect normalGeom;
	bool autoHide,
		extended,
		lockSignal,
		movable,
                useAnimation,
		firstTimeShown;
        double maxDockWidth;
	int autoHideDelay,
		quietDelay,
		dockOffset,
		offset,
		overrideMinIconSize,
		minIconSize,
		maxIconSize;
	QIrDockManager * manager;
	QWidget * wid;
	QIcon icon;
	QString title;
	
protected:
	virtual void init();
	virtual void setupConnections();
};

QIR_END_NAMESPACE

#endif // QIRABSTRACTDOCK_UI_H
