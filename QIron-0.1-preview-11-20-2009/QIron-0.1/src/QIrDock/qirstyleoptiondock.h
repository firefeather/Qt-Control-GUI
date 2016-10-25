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
#ifndef QSTYLEOPTIONDOCK_H
#define QSTYLEOPTIONDOCK_H

#include <QStyleOption>
#include "qirabstractdock.h"
#include "qirdockdragguide.h"

QIR_BEGIN_NAMESPACE

class QIRONSHARED_EXPORT QIrStyleOptionDock : public QStyleOption
{
public:
	enum StyleOptionType { Type = QStyleOption::SO_CustomBase + 5 };
	enum StyleOptionVersion { Version = 1 };

	QIrStyleOptionDock();
	QIrStyleOptionDock( const QIrStyleOptionDock & other ) : QStyleOption(Version, Type) 
	{ 
		*this = other; 
	}
	QIrAbstractDock::DockArea area;
	QIrAbstractDock::DockAlignment alignment;
	QIrAbstractDock::DockSizePolicy policy;
	bool autoHide;
	QIcon icon;
	QRect frameRect;
	QString title;
	Qt::Orientation orientation;
	
protected:
	QIrStyleOptionDock( int version );
};

class QIRONSHARED_EXPORT QIrStyleOptionDocklet : public QStyleOption
{
public:
	enum StyleOptionType { Type = QStyleOption::SO_CustomBase + 1 };
	enum StyleVersion { Version = 1 };

	QIrStyleOptionDocklet();
	QIrStyleOptionDocklet(int version, int type);
	QIrStyleOptionDocklet( const QIrStyleOptionDocklet & other ) : QStyleOption(Version, Type) 
	{ 
		*this = other; 
	}
	bool separator,
		launcher,
		active;
	int iconSize;
	QIcon icon;
	QString text;
	Qt::Orientation orientation;
	QIrAbstractDock::DockArea area;
	
protected:
	QIrStyleOptionDocklet( int version );
};

class QIRONSHARED_EXPORT QIrStyleOptionDockletBundle : public QIrStyleOptionDocklet
{
public:
	enum StyleOptionType { Type = QStyleOption::SO_CustomBase + 8 };
	enum StyleVersion { Version = 1 };

	QIrStyleOptionDockletBundle();
	QIrStyleOptionDockletBundle( const QIrStyleOptionDockletBundle & other ) : QIrStyleOptionDocklet(Version, Type) 
	{ 
		*this = other; 
	}
	
protected:
	QIrStyleOptionDockletBundle( int version );
};


class QIRONSHARED_EXPORT QIrStyleOptionDockDragGuide : public QStyleOption
{
public:
	enum StyleOptionType { Type = QStyleOption::SO_CustomBase + 2 };
	enum StyleVersion { Version = 1 };

	QIrStyleOptionDockDragGuide();
	QIrStyleOptionDockDragGuide( const QIrStyleOptionDockDragGuide & other ) : QStyleOption(Version, Type) 
	{ *this = other; 
	}
	QIrAbstractDock::DockArea area;

protected:
	QIrStyleOptionDockDragGuide( int version );
};


class QIRONSHARED_EXPORT QIrStyleOptionDockDragGuideIndicator : public QStyleOption
{
public:
	enum StyleOptionType { Type = QStyleOption::SO_CustomBase + 3 };
	enum StyleVersion { Version = 1 };

	QIrStyleOptionDockDragGuideIndicator();
	QIrStyleOptionDockDragGuideIndicator( const QIrStyleOptionDockDragGuideIndicator & other ) : QStyleOption(Version, Type) 
	{ *this = other; 
	}
	QIrAbstractDock::DockArea area;
	QIrAbstractDock::DockAlignment alignment;

protected:
	QIrStyleOptionDockDragGuideIndicator( int version );
};

class QIRONSHARED_EXPORT QIrStyleOptionDockTipLabel : public QStyleOption
{
public:
	enum StyleOptionType { Type = QStyleOption::SO_CustomBase + 7 };
	enum StyleVersion { Version = 1 };

	QIrStyleOptionDockTipLabel();
	QIrStyleOptionDockTipLabel( const QIrStyleOptionDockTipLabel & other ) : QStyleOption(Version, Type) 
	{ 
		*this = other; 
	}
	QFont font;
	QString text;
	QRect targetRect;
	QIrAbstractDock::DockArea area;
	
protected:
	QIrStyleOptionDockTipLabel( int version );
};

QIR_END_NAMESPACE

#endif // QSTYLEOPTIONDOCK_H
