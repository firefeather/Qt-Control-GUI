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
#ifndef QIRDOCKSTYLE_P_H
#define QIRDOCKSTYLE_P_H

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

#include "../qirdockstyle.h"
#include "qirabstractdockstyle_p.h"
#include "../qirdropshadow.h"

class QPixmapBlurFilter;

QIR_BEGIN_NAMESPACE

class QIrDockStylePrivate : public QIrCommonDockStylePrivate
{
	QIR_DECLARE_OBJECT(QIrDockStyle)

public:
	QIrDockStylePrivate();
	virtual ~QIrDockStylePrivate();
};

class QIrDefaultDockStylePrivate : public QIrDockStylePrivate
{
	QIR_DECLARE_OBJECT(QIrDefaultDockStyle)

public:
	QIrDefaultDockStylePrivate();
	virtual ~QIrDefaultDockStylePrivate();
};

class QIrStyledDockStylePrivate : public QIrDefaultDockStylePrivate
{
	QIR_DECLARE_OBJECT(QIrStyledDockStyle)

public:
	QIrStyledDockStylePrivate ();
	virtual ~QIrStyledDockStylePrivate ();

	QPixmapBlurFilter * blurFilter;
	mutable QIrDropShadow shadow;

protected:
	virtual void init();
};

QIR_END_NAMESPACE

#endif // QIRDOCKSTYLE_P_H
