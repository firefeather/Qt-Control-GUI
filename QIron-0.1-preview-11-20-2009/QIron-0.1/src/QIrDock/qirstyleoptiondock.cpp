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
#include "qirstyleoptiondock.h"

QIR_BEGIN_NAMESPACE

/////////////////////
//QIrStyleOptionDock
/////////////////////
QIrStyleOptionDock::QIrStyleOptionDock() : QStyleOption(Version,Type)
{
}
QIrStyleOptionDock::QIrStyleOptionDock( int version ) : QStyleOption(version,Type)
{
}

/////////////////////////
//QIrStyleOptionDocklet
/////////////////////////
QIrStyleOptionDocklet::QIrStyleOptionDocklet()
    : QStyleOption(Version,Type)
{
}
QIrStyleOptionDocklet::QIrStyleOptionDocklet(int version, int type)
	: QStyleOption(version,type)
{
}
QIrStyleOptionDocklet::QIrStyleOptionDocklet( int version )
    : QStyleOption(version,Type)
{
}

////////////////////////////////
//QIrStyleOptionDockletBundle
////////////////////////////////
QIrStyleOptionDockletBundle::QIrStyleOptionDockletBundle() 
	: QIrStyleOptionDocklet(Version,Type)
{
}
QIrStyleOptionDockletBundle::QIrStyleOptionDockletBundle( int version )
	: QIrStyleOptionDocklet(version,Type)
{
}


//////////////////////////////
//QIrStyleOptionDockDragGuide
//////////////////////////////
QIrStyleOptionDockDragGuide::QIrStyleOptionDockDragGuide()
: QStyleOption(Version,Type), area(QIrAbstractDock::DA_BottomDockArea)
{
}
QIrStyleOptionDockDragGuide::QIrStyleOptionDockDragGuide(int version)
    : QStyleOption(version,Type), area(QIrAbstractDock::DA_BottomDockArea)
{
}


///////////////////////////////////////
//QIrStyleOptionDockDragGuideIndicator
///////////////////////////////////////
QIrStyleOptionDockDragGuideIndicator::QIrStyleOptionDockDragGuideIndicator()
: QStyleOption(Version,Type), area(QIrAbstractDock::DA_BottomDockArea), alignment(QIrAbstractDock::DAL_DockAlignCenter)
{
}
QIrStyleOptionDockDragGuideIndicator::QIrStyleOptionDockDragGuideIndicator(int version)
    : QStyleOption(version,Type), area(QIrAbstractDock::DA_BottomDockArea), alignment(QIrAbstractDock::DAL_DockAlignCenter)
{
}

////////////////////////////
//QIrStyleOptionDockTipLabel
////////////////////////////
QIrStyleOptionDockTipLabel::QIrStyleOptionDockTipLabel()
: QStyleOption(Version,Type)
{
}
QIrStyleOptionDockTipLabel::QIrStyleOptionDockTipLabel(int version)
    : QStyleOption(version,Type)
{
}

QIR_END_NAMESPACE
