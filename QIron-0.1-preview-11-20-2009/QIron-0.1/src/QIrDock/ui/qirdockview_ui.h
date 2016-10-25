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
#ifndef QIRDOCKVIEW_UI_H
#define QIRDOCKVIEW_UI_H

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

#include "../qirdocklet.h"
#include "../qirdockview.h"
#include "../../Common/ui/qirwidget_ui.h"

QIR_BEGIN_NAMESPACE


class QIrDockViewUi : public QIrWidgetUi
{
	QIR_DECLARE_WIDGET(QIrDockView)

public:
	QIrDockViewUi();
	~QIrDockViewUi();

	QIrDock * dock;
	QIrDockEffectManager * effectManager;
	QList< QIrDockletBundle * > bundles;
	bool blockEvents,
		entered,
		called;
	QTimer * delayTimer;
	
protected:
	void init();
	void setupConnections();
};

QIR_END_NAMESPACE

#endif // QIRDOCKVIEW_UI_H
