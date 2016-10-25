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
#include <QPainter>
#include "private/qirabstractdockstyle_p.h"

QIR_BEGIN_NAMESPACE

static const char * const qir_left_corner_indicator[] = {
    "9 9 2 1",
    "   c None",
    "*  c #BFBFBF",
    " ***     ",
    "  ***    ",
    "   ***   ",
	"    ***  ",
	"     *** ",
	"    ***  ",
	"   ***   ",
	"  ***    ",
    " ***     "};
static const char * const qir_down_corner_indicator[] = {
    "9 9 2 1",
    "   c None",
    "*  c #BFBFBF",
    "         ",
	"*       *",
    "**     **",
    "***   ***",
	" *** *** ",
	"  *****  ",
	"   ***   ",
	"    *    ",
	"         "};

	
static const char * const qir_middle_corner_indicator[] = {
    "9 9 2 1",
    "   c None",
    "*  c #BFBFBF",
    "         ",
    "    *    ",
    "   ***   ",
	"  *****  ",
	" ******* ",
	"  *****  ",
	"   ***   ",
	"    *    ",
    "         "};

///////////////////////////////
//QIrAbstractDockStylePrivate
///////////////////////////////
QIrAbstractDockStylePrivate::QIrAbstractDockStylePrivate()
{
}
QIrAbstractDockStylePrivate::~QIrAbstractDockStylePrivate()
{
}

////////////////////////////
//QIrCommonDockStylePrivate
////////////////////////////
QIrCommonDockStylePrivate::QIrCommonDockStylePrivate()
{
}
QIrCommonDockStylePrivate::~QIrCommonDockStylePrivate()
{
}

////////////////////////
//QIrAbstractDockStyle
///////////////////////
QIrAbstractDockStyle::QIrAbstractDockStyle() : QIrObject(* new QIrAbstractDockStylePrivate)
{
	QIR_P(QIrAbstractDockStyle);

	p->setup();
}
QIrAbstractDockStyle::QIrAbstractDockStyle( QIrAbstractDockStylePrivate & p ) : QIrObject(p)
{
}
QIrAbstractDockStyle::~QIrAbstractDockStyle()
{
}

///////////////////////
//QIrCommonDockStyle
///////////////////////
QIrCommonDockStyle::QIrCommonDockStyle() : QIrAbstractDockStyle(* new QIrCommonDockStylePrivate)
{
	QIR_P(QIrCommonDockStyle);

	p->setup();
}
QIrCommonDockStyle::QIrCommonDockStyle( QIrCommonDockStylePrivate & p ) : QIrAbstractDockStyle(p)
{
}
QIrCommonDockStyle::~QIrCommonDockStyle()
{
}
void QIrCommonDockStyle::drawPrimitive(QStyle::PrimitiveElement pe , const QStyleOption * option, QPainter * painter, const QWidget *) const
{
	switch ( pe ) {
		case PE_FrameDockDragGuide:
			if ( const QIrStyleOptionDockDragGuide * guide = qstyleoption_cast< const QIrStyleOptionDockDragGuide * >(option) ) {
				bool enabled = (guide->state & QStyle::State_Enabled);

				QPoint P1, P2;
				QColor color = option->palette.highlight().color();
				QColor startGradColor = color.light(), 
					endGradColor = color, 
					outlineColor = endGradColor;
				bool horizontal = (guide->area == QIrAbstractDock::DA_BottomDockArea || guide->area == QIrAbstractDock::DA_TopDockArea);
				int alpha = enabled ? 150 : 50;
				QRect r = option->rect.adjusted(0,0,-1,-1);

				startGradColor.setAlpha(alpha);
				endGradColor.setAlpha(alpha);
				outlineColor.setAlpha(alpha);
				if ( horizontal ) {
					P1 = r.topLeft();
					P2 = r.bottomLeft();
				} else {
					P1 = r.topLeft();
					P2 = r.topRight();
				}
				QLinearGradient gr(P1,P2);
				
				gr.setColorAt(0,startGradColor);
				gr.setColorAt(0.495,endGradColor.light(125));
				gr.setColorAt(0.5,endGradColor.light(105));
				gr.setColorAt(1,endGradColor.dark(120));
				
				painter->setPen(outlineColor);
				painter->setBrush(QBrush(gr));
				painter->drawRoundedRect(r,4,4);
				painter->setPen(QColor(250,250,255,80));
				painter->setBrush(Qt::NoBrush);
				painter->drawRoundedRect(r.adjusted(1,1,-1,-1),4,4);
			}
			return;
		case PE_DockDragGuideIndicator:
			if ( const QIrStyleOptionDockDragGuideIndicator * gi = qstyleoption_cast< const QIrStyleOptionDockDragGuideIndicator * >(option) ) {
				bool enabled = (gi->state & QStyle::State_Enabled);

				QRect r = option->rect.adjusted(0,0,-1,-1);
				QColor color = option->palette.color(QPalette::Normal,QPalette::Highlight);
				QColor imgColor = option->palette.highlightedText().color();
				QColor startGradColor = color.light(), 
					endGradColor = color, 
					outlineColor = endGradColor;
				bool horizontal = (gi->area == QIrAbstractDock::DA_BottomDockArea || gi->area == QIrAbstractDock::DA_TopDockArea);
				bool hover = gi->state & QStyle::State_MouseOver;
				QLinearGradient gr(r.topLeft(),r.bottomLeft());
				QImage img(gi->alignment != QIrAbstractDock::DAL_DockAlignCenter ? horizontal 
					? qir_left_corner_indicator : qir_down_corner_indicator : qir_middle_corner_indicator);
				QRect imgR(QPoint(),img.size());
				int alpha = hover ? 200 : 150;
				
				imgColor.setAlpha(enabled ? 255 : 200);
				if ( gi->alignment == QIrAbstractDock::DAL_DockAlignLeft )
					img = img.mirrored(horizontal,!horizontal);
				startGradColor.setAlpha(alpha);
				endGradColor.setAlpha(alpha);

				if ( hover ) {
					gr.setColorAt(0,QColor(255,255,255,150));
					gr.setColorAt(1,endGradColor.dark(120));
					painter->setPen(QColor(255,255,255,100));
					painter->setBrush(Qt::NoBrush);
					painter->drawEllipse(r.adjusted(0,1,0,1));
					painter->setPen(outlineColor);
					painter->setBrush(QBrush(gr));
					painter->drawEllipse(r);
				}
				if ( !img.isNull() ) {
					imgR.moveCenter(r.center());
					img.setColor(1,QColor(0,0,0,50).rgba());
					painter->drawImage(imgR.adjusted(0,1,0,1),img);
					img.setColor(1,imgColor.rgba());
					painter->drawImage(imgR,img);
				}
			}
			return;
		case PE_FrameDockDragOutline: {
			QColor color = option->palette.highlight().color();
			
			painter->setPen(color);
			color.setAlpha(100);
			painter->setBrush(color);
			painter->drawRect(option->rect.adjusted(0,0,-1,-1));
			}
			return;
		default:
			break;
	}
}

int QIrCommonDockStyle::pixelMetric(QStyle::PixelMetric pm, const QStyleOption * option , const QWidget * widget) const
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
	int px = 0;

	switch ( pm ) {
		case PM_DockDragGuideIndicatorWidth:
			return 20;
		case PM_DockDragGuideSpacing :
			return 10;
		case PM_DockletIndicatorHeight :
			return 10;
		case PM_DockLayoutTopMargin :
		case PM_DockLayoutBottomMargin :
		case PM_DockLayoutLeftMargin :
		case PM_DockLayoutRightMargin :
                        return 9;
		case PM_DockTipLabelTopMargin :
		case PM_DockTipLabelBottomMargin :
		case PM_DockTipLabelRightMargin :
		case PM_DockTipLabelLeftMargin :
                        return 6;
		default:
			break;
	}
	return px;
}
QSize QIrCommonDockStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *, const QSize & contentsSize, const QWidget *) const
{
	QSize sz;

	switch ( type ) {
		case CT_DockDragGuide:
			return contentsSize + QSize(6,6);
		default:
			break;
	}
	return sz;
}

QIR_END_NAMESPACE
