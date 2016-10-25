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
#ifndef QIRDOCKSTYLE_H
#define QIRDOCKSTYLE_H

#include "qirabstractdockstyle.h"

QIR_BEGIN_NAMESPACE

class QIrDefaultDockStylePrivate;
class QIrDock;
class QIrDockletBundle;
class QIrDockStylePrivate;
class QIrStyledDockStylePrivate;

class QIRONSHARED_EXPORT QIrDockStyle : public QIrCommonDockStyle
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDockStyle)

public:	
	QIrDockStyle();
	~QIrDockStyle();
	
	virtual int iconSizeForSize(const QSize & size, bool separator, Qt::Orientation orientation) const = 0;
	virtual QSize sizeForIconSize(int minimumIconSize, int iconSize, int maximumIconSize, bool separator, Qt::Orientation) const = 0;

	virtual void polish( QIrDockletBundle * ) { }
	virtual void unpolish( QIrDockletBundle * ) { }	

protected:
	QIrDockStyle( QIrDockStylePrivate & p );

	void initStyleOption(const QIrDock *, QIrStyleOptionDock * option) const;

protected slots:
	QIcon standardIconImplementation(QStyle::StandardPixmap standardIcon, const QStyleOption * option = 0, const QWidget * widget = 0 ) const; 

private:
	Q_DISABLE_COPY(QIrDockStyle)
};

class QIRONSHARED_EXPORT QIrDefaultDockStyle : public QIrDockStyle
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrDefaultDockStyle)

public:
	QIrDefaultDockStyle();
	~QIrDefaultDockStyle();

	virtual int iconSizeForSize(const QSize & size, bool separator, Qt::Orientation orientation) const;
	virtual QSize sizeForIconSize(int minimumIconSize, int iconSize, int maximumIconSize, bool separator, Qt::Orientation) const;


	virtual void drawPrimitive(QStyle::PrimitiveElement , const QStyleOption * , QPainter * , const QWidget * w = 0) const;
	virtual void drawControl(QStyle::ControlElement , const QStyleOption * , QPainter * , const QWidget * w = 0) const;
	virtual int pixelMetric(QStyle::PixelMetric , const QStyleOption * ,const QWidget * w = 0) const;
	virtual QRect subElementRect(QStyle::SubElement, const QStyleOption * option, const QWidget * widget = 0) const;
	virtual QSize sizeFromContents(QStyle::ContentsType type, const QStyleOption * option, const QSize & contentsSize, const QWidget * w = 0) const;
	virtual int styleHint(QStyle::StyleHint hint, const QStyleOption * option = 0, const QWidget * widget = 0, QStyleHintReturn * returnData = 0) const;
		
	virtual void polish( QWidget * );
	virtual void unpolish( QWidget * );
	
	virtual void polish( QIrDockletBundle * );
	virtual void unpolish( QIrDockletBundle * );

protected slots:
	inline void slotHandleBundleEntered() { handleBundleEnteredImpl(); }
	inline void slotHandleBundleLeft() { handleBundleLeftImpl(); }
	inline void slotHandleBundleMousePressed() { handleBundleMousePressedImpl(); }
	inline void slotHandleBundleMouseReleased()  { handleBundleMouseReleasedImpl(); }
	inline void slotHandleDockAreaChanged() { handleDockAreaChangedImpl(); }

protected:
	QIrDefaultDockStyle( QIrDefaultDockStylePrivate & p );

	virtual void setEffects( QIrDockletBundle * );
	virtual void unsetEffects( QIrDockletBundle * );

	virtual void handleBundleEnteredImpl();
	virtual void handleBundleLeftImpl();
	virtual void handleBundleMousePressedImpl();
	virtual void handleBundleMouseReleasedImpl();
	virtual void handleDockAreaChangedImpl();

private:
	Q_DISABLE_COPY(QIrDefaultDockStyle)
};

class QIRONSHARED_EXPORT QIrStyledDockStyle : public QIrDefaultDockStyle
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrStyledDockStyle)

public:
	QIrStyledDockStyle();
	~QIrStyledDockStyle();

	virtual void drawPrimitive(QStyle::PrimitiveElement , const QStyleOption * , QPainter * , const QWidget * w = 0) const;
	virtual void drawControl(QStyle::ControlElement , const QStyleOption * , QPainter * , const QWidget * w = 0) const;
	virtual int pixelMetric(QStyle::PixelMetric , const QStyleOption * ,const QWidget * w = 0) const;
	virtual QRect subElementRect(QStyle::SubElement, const QStyleOption * option, const QWidget * widget = 0) const;
	virtual int styleHint(QStyle::StyleHint hint, const QStyleOption * option = 0, const QWidget * widget = 0, QStyleHintReturn * returnData = 0) const;
		
protected:
	QIrStyledDockStyle( QIrStyledDockStylePrivate & p );

	
	virtual void setEffects( QIrDockletBundle * );

	virtual void handleBundleEnteredImpl();
	virtual void handleBundleLeftImpl();
	virtual void handleBundleMousePressedImpl();
	virtual void handleBundleMouseReleasedImpl();
	virtual void handleDockAreaChangedImpl();

private:
	Q_DISABLE_COPY(QIrStyledDockStyle)
};

QIR_END_NAMESPACE

#endif // QIRDOCKSTYLE_H
