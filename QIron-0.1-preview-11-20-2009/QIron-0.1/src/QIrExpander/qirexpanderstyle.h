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
/*!
  \brief This file contains the declaration of QIrExpanderStyle class.
  \file qirexpanderstyle.h
  \author Dzimi Mve Alexandre
  \date 07/28/2009
*/
#ifndef QIREXPANDERSTYLE_H
#define QIREXPANDERSTYLE_H

#include <QRect>
#include "../Common/qirsubstyle.h"

class QPainter;
class QStyleOption;

QIR_BEGIN_NAMESPACE

class QIRONSHARED_EXPORT QIrExpanderStyle : public QIrSubStyle
{
	Q_OBJECT

public:
	enum PixelMetric{PM_ExpanderIconSize, PM_ExpanderHeaderMinimumWidth, PM_ExpanderHeaderHeight};
	enum PrimitiveElement{PE_FrameExpanderHeader, PE_FrameExpander};
	enum ControlElement{CE_ExpanderButton};

	QIrExpanderStyle() { }
	~QIrExpanderStyle() { }
};

class QIrDefaultExpanderStyle : public QIrExpanderStyle
{
	Q_OBJECT

public:
	QIrDefaultExpanderStyle() { }
	~QIrDefaultExpanderStyle() { }

	virtual void drawPrimitive(QStyle::PrimitiveElement , const QStyleOption *, QPainter *, const QWidget * widget = 0) const;
	virtual void drawControl(QStyle::ControlElement , const QStyleOption *, QPainter *, const QWidget * widget = 0) const;
	virtual int pixelMetric(QStyle::PixelMetric, const QStyleOption *, const QWidget * widget = 0) const;
};

QIR_END_NAMESPACE

#endif //QIREXPANDERSTYLE_H
