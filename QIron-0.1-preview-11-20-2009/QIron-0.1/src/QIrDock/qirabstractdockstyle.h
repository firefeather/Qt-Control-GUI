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
#ifndef QIRABSTRACTDOCKSTYLE_H
#define QIRABSTRACTDOCKSTYLE_H

#include "../Common/qirobject.h"
#include "qirstyleoptiondock.h"
#include "../Common/qirsubstyle.h"

QIR_BEGIN_NAMESPACE

class QIrAbstractDockStylePrivate;
class QIrCommonDockStylePrivate;

class QIRONSHARED_EXPORT QIrAbstractDockStyle : public QIrSubStyle, public QIrObject
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrAbstractDockStyle)

public:
	enum ContentsType {
		CT_Dock,
		CT_DockDragGuide,
		CT_Docklet,
		CT_DockTipLabel
	};
	enum ControlElement {
		CE_DockGuideIndicator,
		CE_Docklet,
		CE_DockTipLabel
	};
	enum PixelMetric {
		PM_DockContentMargin,
		PM_DockDragGuideIndicatorWidth,
		PM_DockDragGuideSpacing,
		PM_DockLayoutBottomMargin,
		PM_DockLayoutLeftMargin,
		PM_DockLayoutRightMargin,
		PM_DockLayoutTopMargin,
		PM_DockletIndicatorHeight,
		PM_DockletSpacing,
		PM_DockTipLabelBottomMargin,
		PM_DockTipLabelLeftMargin,
		PM_DockTipLabelRightMargin,
		PM_DockTipLabelTopMargin
	};
	enum PrimitiveElement {
		PE_DockDragGuideIndicator,
		PE_DockletIndicator,
		PE_FrameDockDragGuide,
		PE_FrameDockDragOutline,
		PE_PanelDock,
		PE_PanelDockTipLabel
	};
	enum StandardPixmap {
		SP_UnknownDocklet
	};
	enum StyleHint {
		SH_DockTipFont
	};
	enum SubElement {
		SE_DockLabel,
		SE_DockDragBar,
		SE_DockContent,
		SE_DockletBundleContentsRect,
		SE_DockletBundleIndicator,
		SE_DockPanelRect,
		SE_DockTipLabelContents
	};
	QIrAbstractDockStyle();
	virtual ~QIrAbstractDockStyle();

protected:
	QIrAbstractDockStyle( QIrAbstractDockStylePrivate & p );

private:
	Q_DISABLE_COPY(QIrAbstractDockStyle)
};


class QIRONSHARED_EXPORT QIrCommonDockStyle : public QIrAbstractDockStyle
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrCommonDockStyle)

public:
	QIrCommonDockStyle();
	virtual ~QIrCommonDockStyle();

	virtual void drawPrimitive(QStyle::PrimitiveElement , const QStyleOption * , QPainter * , const QWidget * w = 0) const;
	virtual QSize sizeFromContents(QStyle::ContentsType , const QStyleOption * , const QSize & , const QWidget * w = 0) const;
	virtual int pixelMetric(QStyle::PixelMetric pm, const QStyleOption * option , const QWidget * w = 0) const;

protected:
	QIrCommonDockStyle( QIrCommonDockStylePrivate & p );

private:
	Q_DISABLE_COPY(QIrCommonDockStyle)
};

QIR_END_NAMESPACE

#endif // QIRABSTRACTDOCKSTYLE_H
