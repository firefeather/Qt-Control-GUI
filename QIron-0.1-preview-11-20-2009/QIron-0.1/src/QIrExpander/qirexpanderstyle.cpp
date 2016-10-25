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
#include <QFontMetrics>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include "qirexpander.h"
#include "qirexpanderstyle.h"


QIR_BEGIN_NAMESPACE

static const char * const arrow_down_img[] = {
    "9 6 2 1",
    "   c None",
    "*  c #BFBFBF",
    "         ",
	" **   ** ",
    " *** *** ",
    "  *****  ",
	"   ***   ",
	"    *    "
};
static const char * const arrow_up_img[] = {
    "9 6 2 1",
    "   c None",
    "*  c #BFBFBF",
    "    *    ",
	"   ***   ",
	"  *****  ",
	" *** *** ",
	" **   ** ",
    "         "
};

void QIrDefaultExpanderStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption * option, QPainter * painter, const QWidget * widget) const
{
    switch ( pe ) {
    case QIrDefaultExpanderStyle::PE_FrameExpanderHeader :
		painter->save();
		painter->setPen(option->palette.window().color().dark(120));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(option->rect.adjusted(0,0,-1,-1));
		painter->restore();
		return;
    default:
                break;
            }
    QIrExpanderStyle::drawPrimitive(pe,option,painter,widget);
}
void QIrDefaultExpanderStyle::drawControl(QStyle::ControlElement ce, const QStyleOption * option, QPainter * painter, const QWidget * widget) const
{
	switch (ce) {
		case QIrDefaultExpanderStyle::CE_ExpanderButton: {
			const QStyleOptionButton * btn = qstyleoption_cast< const QStyleOptionButton * >(option);
			const QIrExpander * expander = qobject_cast< const QIrExpander * >(widget);

			if ( btn && expander ) {
				QColor color;
				
				bool checked = btn->state & QStyle::State_On,
					down = btn->state & QStyle::State_Sunken,
					hover = btn->state & QStyle::State_MouseOver;
				QRect r = option->rect;
				QImage img(checked ? arrow_down_img : arrow_up_img);
				QRect imgRect(QPoint(),img.size());
				QRect tempRect, iconRect, labelRect, indicatorRect;
				QFont font = widget->font();
				QString text, elidedText;
				QIcon icon;
				int s;
				QSize iconSize;
				
				painter->save();
				if (expander->headerPosition() == QIrExpander::West || 
					expander->headerPosition() == QIrExpander::East) {
						int tempW = r.width();
						r.setWidth(r.height());
						r.setHeight(tempW);
						
						painter->translate(r.height(),0);
						painter->rotate(90);
				}
				if (expander->headerPosition() == QIrExpander::South || 
					expander->headerPosition() == QIrExpander::West)
						img = img.mirrored(false,true);
				//Setup regions

				//Indicator
				s = qMin(20,qMin(r.width(),r.height()));
				indicatorRect = QRect(0,0,s,s);
				indicatorRect.moveCenter(r.center());
				indicatorRect.moveRight(r.right() - 5);

				//Icon
				iconSize = btn->iconSize;
				int offset = 2;
				QPixmap pixmap = btn->icon.pixmap(iconSize);
				if ( !pixmap.size().isEmpty() ) {
					iconSize = pixmap.size();
					iconRect = QRect(QPoint(),iconSize);
					iconRect.moveCenter(r.center());
					iconRect.moveLeft(5);
					offset = iconRect.right() + 4;
				}
				
				//Text
				text = btn->text;
				if ( !text.isEmpty() ) {
					int availableWidth = r.width() - indicatorRect.width() - 5 - offset; 
					int w = (!pixmap.isNull() ? iconRect.right() + 4 : 5);
							
					elidedText = option->fontMetrics.elidedText(text,Qt::ElideRight,availableWidth);
					labelRect = option->fontMetrics.boundingRect(elidedText);
					labelRect.moveCenter(r.center());
					labelRect.moveLeft(w);
				}						
				

					painter->setRenderHint(QPainter::Antialiasing);
					color = down ? btn->palette.color(QPalette::Normal,QPalette::Highlight).dark(120) 
					: hover ? btn->palette.color(QPalette::Normal,QPalette::Highlight).light(120) : btn->palette.window().color().dark(120);
					color.setAlpha(200);
				
					//Draw the indicator
					painter->setPen(color);
					painter->drawEllipse(indicatorRect);
					color = btn->palette.foreground().color();
					img.setColor(1,color.rgba());
					imgRect.moveCenter(indicatorRect.center());
					painter->drawImage(imgRect,img);

					//Draw the icon				
					if ( !pixmap.size().isEmpty() ) {
						if ( expander->headerPosition() == QIrExpander::West ) {
							painter->save();
							painter->translate(iconRect.center());
							painter->rotate(180);
							painter->translate(-iconRect.center());
						}
						painter->drawPixmap(iconRect,pixmap);					
						if ( expander->headerPosition() == QIrExpander::West )
							painter->restore();
					}
					
					//Draw the text
					if ( !text.isEmpty() ){
						if ( expander->headerPosition() == QIrExpander::West ) {
							painter->save();
							painter->translate(labelRect.center());
							painter->rotate(180);
							painter->translate(-labelRect.center());
						}
						painter->setFont(font);
						painter->setPen(option->palette.buttonText().color());
						painter->drawText(labelRect,Qt::AlignHCenter|Qt::AlignLeft,elidedText);
						if ( expander->headerPosition() == QIrExpander::West )
							painter->restore();
				}
				painter->restore();
				return;
			}
			}
			break;

		default:
			break;
	}
	QIrExpanderStyle::drawControl(ce,option,painter,widget);
}
int QIrDefaultExpanderStyle::pixelMetric(QStyle::PixelMetric pm, const QStyleOption * option, const QWidget * widget) const
{
	switch (pm) {
		case PM_ExpanderHeaderHeight:
			return qMax(pixelMetric((QStyle::PixelMetric)PM_ExpanderIconSize,option,widget),option->fontMetrics.height()) + 10;
		case PM_ExpanderIconSize:
			return 16;
		case PM_ExpanderHeaderMinimumWidth: 
			if ( const QStyleOptionButton * btn = qstyleoption_cast< const QStyleOptionButton * >(option) ) {
				QRect r = option->rect;
				int s = qMin(20,qMin(r.width(),r.height())) + 5;
				QSize iconSize = btn->iconSize;
				QPixmap pixmap = btn->icon.pixmap(iconSize);
				QString text = btn->text;

				if ( !pixmap.size().isEmpty() ) {
					s += btn->iconSize.width() + 5 + 10;
				}
				if ( !text.isEmpty() )
					s += option->fontMetrics.boundingRect(text).width() + 20;
				return s;
			}
			break;
		default:
			break;
		}
	return QIrExpanderStyle::pixelMetric(pm,option,widget);
}

QIR_END_NAMESPACE
