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
#include <QList>
#include <QPainter>
#include "qirdropshadow.h"

QIR_BEGIN_NAMESPACE 

static QPixmap pixmapForRegion( int region )
{
	static QList< QPixmap > list;
	
	if ( list.empty() )
		list << QPixmap(":/qiron/styles/images/shadow_topleft.png")
		<< QPixmap(":/qiron/styles/images/shadow_left.png")
		<< QPixmap(":/qiron/styles/images/shadow_bottomleft.png")
		<< QPixmap(":/qiron/styles/images/shadow_bottom.png")
		<< QPixmap(":/qiron/styles/images/shadow_bottomright.png")
		<< QPixmap(":/qiron/styles/images/shadow_right.png")
		<< QPixmap(":/qiron/styles/images/shadow_topright.png")
		<< QPixmap(":/qiron/styles/images/shadow_top.png");
	return list[region];
}

static bool canPaintRegion(QIrDropShadow * drop, QIrDropShadow::DropShadowRegion reg)
{
	bool can = false;
	int edges = drop->edges();

	switch ( reg ) {
		case QIrDropShadow::Top: 
			can = (edges & QIrDropShadow::TopEdge);
			break;
		case QIrDropShadow::Left:
			can = (edges & QIrDropShadow::LeftEdge);
			break;
		case QIrDropShadow::Bottom:
			can = (edges & QIrDropShadow::BottomEdge);
			break;
		case QIrDropShadow::Right:
			can = (edges & QIrDropShadow::RightEdge);
			break;
		case QIrDropShadow::TopLeft: 
			can = (edges & QIrDropShadow::TopEdge) && (edges & QIrDropShadow::LeftEdge);
			break;
		case QIrDropShadow::BottomLeft: 
			can = (edges & QIrDropShadow::BottomEdge) && (edges & QIrDropShadow::LeftEdge);
			break;
		case QIrDropShadow::BottomRight: 
			can = (edges & QIrDropShadow::BottomEdge) && (edges & QIrDropShadow::RightEdge);
			break;
		case QIrDropShadow::TopRight: 
			can = (edges & QIrDropShadow::TopEdge) && (edges & QIrDropShadow::RightEdge);
			break;
	};
	return can;

}
static QList< QRect > getDropShadowRegions(QIrDropShadow * drop, const QRect & rect, int rad )
{
	int size = 14, x1, y1, x2, y2, diff = size - rad;
	QList< QRect > list;
	int diffTopLeft =  canPaintRegion(drop,QIrDropShadow::TopLeft) ? diff : 0,
		diffBottomLeft =  canPaintRegion(drop,QIrDropShadow::BottomLeft) ? diff : 0,
		diffBottomRight =  canPaintRegion(drop,QIrDropShadow::BottomRight) ? diff : 0,
		diffTopRight =  canPaintRegion(drop,QIrDropShadow::TopRight) ? diff : 0;



	rect.getCoords(&x1,&y1,&x2,&y2);
	return QList< QRect >() << QRect(x1 - rad,y1 - rad,size,size)
		<< QRect(x1 - rad ,y1 + diffTopLeft,size,y2 - y1 - (diffTopLeft + diffBottomLeft))
		<< QRect(x1 - rad,y2 - diff,size,size)
		<< QRect(x1 + diffBottomLeft,y2 - diff,x2 - x1 - (diffBottomLeft + diffBottomRight),size)
		<< QRect(x2 - diff,y2 - diff,size,size)
		<< QRect(x2 - diff,y1 + diffTopRight,size,y2 - y1 - (diffBottomRight + diffTopRight))
		<< QRect(x2 - diff,y1 - rad,size,size)
		<< QRect(x1 + diffTopLeft,y1 - rad,x2 - x1 - (diffTopLeft + diffTopRight),size);
}
QIrDropShadow::QIrDropShadow(const QRect & rect) : m_rect(rect), m_opacity(1.0), m_edges(QIrDropShadow::All), m_visible(true)
{
}
void QIrDropShadow::paint( QPainter * p )
{
	int reg = 0;
	QPainterPath mainPath;

	if (!m_visible || m_edges == QIrDropShadow::None || !m_rect.isValid())
		return;

	mainPath.addRect(m_rect);
	p->save();
	p->setRenderHint(QPainter::Antialiasing,false);
	p->setOpacity(m_opacity);

	if ( !m_path.isEmpty() )
		mainPath = m_path;

	foreach ( QRect rect, getDropShadowRegions(this,m_rect.adjusted(0,0,1,1),8)) {
		QPainterPath path;
		
		path.addRect(rect);
		p->setClipPath(path.subtracted(mainPath));
		if ( canPaintRegion(this,(QIrDropShadow::DropShadowRegion)reg)) {
			p->drawPixmap(rect,pixmapForRegion(reg));
		}
		reg++;
	}
	p->fillRect(m_rect,Qt::NoBrush);
	p->restore();
}

QIR_END_NAMESPACE
