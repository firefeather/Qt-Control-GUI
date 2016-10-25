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
#include <QImage>
#include <QPainter>
#include <QLinearGradient>
#include <QtDebug>
#include "qirdocklet.h"
#include "qirgraphicsmirroreffect.h"
#include "private/qpixmapfilter_p.h"
#include "private/qirdockstyle_p.h"
#include "qirdockletbundle.h"
#include "qirdockview.h"
#include "qirdock.h"

QIR_BEGIN_NAMESPACE

static const char * const qir_docklet_separator_v[] = {
	"9 6 4 1",
    "   c None",
    "*  c #BFBFBF",
	"@  c #AAAAAA",
	"a  c #FFFFFF",
    "@@@@@@@@@",
    "*********",
    "*********",
	"aaaaaaaaa",
	"         ",
	"         "};

static const char * const qir_docklet_separator_h[] = {
	"6 8 4 1",
    "   c None",
    "*  c #BFBFBF",
	"@  c #AAAAAA",
	"a  c #FFFFFF",
    "@**a  ",
    "@**a  ",
    "@**a  ",
	"@**a  ",
	"@**a  ",
	"@**a  ",
    "@**a  ",
    "@**a  "};

static const char * const qir_docklet_indicator_up[] = {
	"9 5 2 1",
    "   c None",
    "*  c #BFBFBF",
    "    *    ",
    "   ***   ",
    "  *****  ",
	" ******* ",
	"*********"
};
static const char * const qir_docklet_indicator_down[] = {
	"9 5 2 1",
    "   c None",
    "*  c #BFBFBF",
    "*********",
    " ******* ",
    "  *****  ",
	"   ***   ",
	"    *    "};
static const char * const qir_docklet_indicator_right[] = {
	"5 9 2 1",
    "   c None",
    "*  c #BFBFBF",
    "*    ",
    "**   ",
    "***  ",
	"**** ",
	"*****",
	"**** ",
	"***  ",
	"**   ",
	"*    "};
static const char * const qir_docklet_indicator_left[] = {
	"5 9 2 1",
    "   c None",
    "*  c #BFBFBF",
    "    *",
    "   **",
    "  ***",
	" ****",
	"*****",
	" ****",
	"  ***",
	"   **",
	"    *"};


/////////////////
//Util functions
/////////////////
	
static QIrGraphicsMirrorEffect::MirrorPosition mirrorPositionForDockArea( QIrAbstractDock::DockArea area )
{
	switch ( area ) {
		case QIrAbstractDock::DA_BottomDockArea :
			return QIrGraphicsMirrorEffect::OnBottom;
		case QIrAbstractDock::DA_TopDockArea :
			return QIrGraphicsMirrorEffect::OnTop;
		case QIrAbstractDock::DA_LeftDockArea :
			return QIrGraphicsMirrorEffect::OnLeft;
		default :
			return QIrGraphicsMirrorEffect::OnRight;
	}
}

//////////////////////
//QIrDockStylePrivate
//////////////////////
QIrDockStylePrivate::QIrDockStylePrivate()
{
}
QIrDockStylePrivate::~QIrDockStylePrivate()
{
}


//////////////////////
//QIrDefaultDockStyle
//////////////////////
QIrDefaultDockStylePrivate::QIrDefaultDockStylePrivate()
{
}
QIrDefaultDockStylePrivate::~QIrDefaultDockStylePrivate()
{
}

////////////////////
//QIrStyleDockStyle
////////////////////
QIrStyledDockStylePrivate::QIrStyledDockStylePrivate()
{
}
QIrStyledDockStylePrivate::~QIrStyledDockStylePrivate()
{
}
void QIrStyledDockStylePrivate::init()
{
	QIR_O(QIrStyledDockStyle);

	blurFilter = new QPixmapBlurFilter(o);
}


////////////////
//QIrDockStyle
////////////////
QIrDockStyle::QIrDockStyle() : QIrCommonDockStyle(* new QIrDockStylePrivate)
{
	QIR_P(QIrDockStyle);

	p->setup();
}
QIrDockStyle::QIrDockStyle( QIrDockStylePrivate & p ) : QIrCommonDockStyle(p)
{
}
QIrDockStyle::~QIrDockStyle()
{
}
void QIrDockStyle::initStyleOption(const QIrDock * dock, QIrStyleOptionDock * option) const
{
	dock->initStyleOption(option);
}
QIcon QIrDockStyle::standardIconImplementation(QStyle::StandardPixmap standardIcon, const QStyleOption * option, const QWidget * widget) const
{
	switch ( standardIcon ) {
		case QIrDockStyle::SP_UnknownDocklet:
			return QIcon(":/qiron/styles/images/unknown_docklet.png");
		default:
			break;
	}
	return QIrCommonDockStyle::standardIconImplementation(standardIcon,option,widget);
}
//////////////////////
//QIrDefaultDockStyle
//////////////////////
QIrDefaultDockStyle::QIrDefaultDockStyle() : QIrDockStyle(* new QIrDefaultDockStylePrivate)
{
	QIR_P(QIrDefaultDockStyle);

	p->setup();	
}
QIrDefaultDockStyle::QIrDefaultDockStyle( QIrDefaultDockStylePrivate & p ) : QIrDockStyle(p)
{
}
QIrDefaultDockStyle::~QIrDefaultDockStyle()
{
}
int QIrDefaultDockStyle::iconSizeForSize(const QSize & size, bool separator, Qt::Orientation orientation) const
{
	Q_UNUSED(separator)

	return (orientation == Qt::Horizontal ? size.width() : size.height());
}
QSize QIrDefaultDockStyle::sizeForIconSize(int minimumIconSize, int iconSize, int maximumIconSize, bool separator,Qt::Orientation o) const
{
	Q_UNUSED(minimumIconSize)
	Q_UNUSED(maximumIconSize)
	int indicH =  pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockletIndicatorHeight,0,0);

	QSize sz = (separator ? o == Qt::Horizontal ? QSize(iconSize,minimumIconSize + indicH): QSize(minimumIconSize + indicH,iconSize) : o == Qt::Horizontal ?
		QSize(iconSize,iconSize + indicH) : QSize(iconSize + indicH,iconSize));

	return sz;
}
void QIrDefaultDockStyle::drawPrimitive(QStyle::PrimitiveElement pe , const QStyleOption * option, QPainter * painter, const QWidget * w) const
{
	switch ( pe ) {
		case PE_PanelDock:
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {
				QRect panelRect = subElementRect((QStyle::SubElement)QIrDockStyle::SE_DockPanelRect,dock);
				QPoint startGrad, endGrad;
				QColor bgColor = dock->palette.window().color().dark(120);
				QColor penColor = bgColor.dark();
				QColor startGradColor = bgColor.light(120),
					endGradColor = bgColor;

				if ( dock->orientation != Qt::Horizontal ) {
					startGrad = panelRect.topLeft();
					endGrad = panelRect.topRight();
				} else {
					startGrad = panelRect.topLeft();
					endGrad = panelRect.bottomLeft();
				}
				QLinearGradient VGrad(startGrad,endGrad);			
						
				VGrad.setColorAt(0,startGradColor);
				VGrad.setColorAt(1,endGradColor);
			
				painter->setPen(penColor);
				painter->setBrush(QBrush(VGrad));
				painter->drawRect(panelRect.adjusted(0,0,-1,-1));
			}
			return;
		case PE_DockletIndicator:
			if ( const QIrStyleOptionDockletBundle * di = qstyleoption_cast< const QIrStyleOptionDockletBundle * >(option) ) {
				QRect r = option->rect;
				QColor color = option->palette.foreground().color();//(QPalette::Normal,QPalette::Foreground);
				QImage img(di->area == QIrAbstractDock::DA_BottomDockArea ? qir_docklet_indicator_up : 
					di->area == QIrAbstractDock::DA_TopDockArea ? qir_docklet_indicator_down :
					di->area == QIrAbstractDock::DA_LeftDockArea ? qir_docklet_indicator_right : qir_docklet_indicator_left);
				QRect imgR(QPoint(),img.size());
				bool enabled = (di->state & QStyle::State_Enabled);
				
				if ( !img.isNull() ) {
					imgR.moveCenter(r.center());
					if ( !enabled ) {
						painter->translate(QPoint(1,1));
						img.setColor(1,QColor(255,255,255,80).rgba());
						painter->drawImage(imgR,img);
						painter->translate(QPoint(-1,-1));
					}						
					img.setColor(1,color.rgba());
					painter->drawImage(imgR,img);
				}				
			}
			return;
		default:
			QIrDockStyle::drawPrimitive(pe,option,painter,w);
			break;
	}
}
void QIrDefaultDockStyle::drawControl(QStyle::ControlElement ce, const QStyleOption * option, QPainter * painter, const QWidget * w) const
{
	switch ( ce ) {
		case CE_Docklet:
			if (const QIrStyleOptionDocklet * di = qstyleoption_cast< const QIrStyleOptionDocklet * >(option) ) {				
				QRect r = di->rect;
	
				if ( di->separator ) {			
					QPoint P1, P2, center = P1 = P2 = r.center(), P3, P4;
					QRect r = option->rect;
					QColor penColor = di->palette.window().color().dark(140);
										
					if ( di->orientation == Qt::Horizontal ) {
						r.adjust(0,2,0,2);
						P1.ry() = r.top();
						P2.ry() = r.bottom();
						P3 = P1 + QPoint(1,0);
						P4 = P2 + QPoint(1,0);
					}
					else {
						r.adjust(2,0,-2,0);
						P1.rx() = r.left();
						P2.rx() = r.right();
						P3 = P1 + QPoint(0,1);
						P4 = P2 + QPoint(0,1);
					}
					painter->setPen(QColor(255,255,255,50));
					painter->drawLine(P3,P4);
					painter->setPen(penColor);
					painter->drawLine(P1,P2);

				} else {
					int ics = qMin(r.width(),r.height());

					QIcon icon;
					
					if ( !di->icon.pixmap(ics,ics).isNull() ) 
						icon = di->icon;
					else
						icon = standardIconImplementation((QStyle::StandardPixmap)QIrDockStyle::SP_UnknownDocklet,0,0);
					
					QImage image = icon.pixmap(ics,ics,
						(option->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled)).toImage();

					if ( image.isNull() )
						return;
					QRect imgR = QRect(QPoint(0,0),image.size());
					QPixmap px(r.size());

					px.fill(Qt::transparent);
					QPainter pixp(&px);
					imgR.moveCenter(r.center());
					pixp.drawImage(imgR,image);
					pixp.end();
					painter->drawPixmap(r,px);							
				}
			}
			break;
			case CE_DockTipLabel :
				if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
					QRect contentRect = subElementRect((QStyle::SubElement)QIrDockStyle::SE_DockTipLabelContents,tip);
					QColor color = tip->palette.highlightedText().color();
					QPainterPath textPath;

					painter->save();
					painter->setRenderHint(QPainter::Antialiasing);
					drawPrimitive((QStyle::PrimitiveElement)QIrDockStyle::PE_PanelDockTipLabel,tip,painter);
					textPath.addText(contentRect.bottomLeft(),tip->font,tip->text);

					painter->translate(QPoint(1,1));
					painter->setPen(QColor(0,0,0,120));
					painter->setBrush(QColor(0,0,0,120));
					painter->drawPath(textPath);
					painter->translate(QPoint(-1,-1));

					painter->setPen(color.dark(130));
					painter->setBrush(color);				
					painter->drawPath(textPath);
					painter->restore();
				}
			break;
		default:
			QIrDockStyle::drawControl(ce,option,painter,w);
			break;
	}
}
int QIrDefaultDockStyle::pixelMetric(QStyle::PixelMetric pm, const QStyleOption * option , const QWidget * w) const
{
	switch ( pm ) {
		case PM_DockletSpacing:
			return 10;
		case PM_DockletIndicatorHeight :
			return 16;
		case PM_DockLayoutTopMargin :
		case PM_DockLayoutBottomMargin :
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {
				if ( dock->orientation == Qt::Horizontal )
					return 9;
				else
					return 40;			
			}
			break;
		case PM_DockLayoutLeftMargin :
		case PM_DockLayoutRightMargin :
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {
				if ( dock->orientation == Qt::Horizontal )
					return 40;
				else
					return 9;
			}
			break;
		case PM_DockTipLabelBottomMargin :
		case PM_DockTipLabelTopMargin :
		case PM_DockTipLabelRightMargin :
		case PM_DockTipLabelLeftMargin :
			if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
                            Q_UNUSED(tip)
                            return 6;
			}
			break;
		default:
			break;
	}
	return QIrDockStyle::pixelMetric(pm,option,w);
}
QSize QIrDefaultDockStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption * option, const QSize & contentsSize, const QWidget * w) const
{
	QSize sz;

	switch ( type ) {
		case CT_DockTipLabel:
			if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
				int topMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelTopMargin,tip),
					bottomMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelBottomMargin,tip),
					rightMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelRightMargin,tip),
					leftMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelLeftMargin,tip);
				sz.rwidth() = contentsSize.width() + leftMargin + rightMargin;
				sz.rheight() = contentsSize.height() + topMargin + bottomMargin;
				return sz;
			}
		default:
			break;
	}
	return QIrCommonDockStyle::sizeFromContents(type,option,contentsSize,w);
}
QRect QIrDefaultDockStyle::subElementRect(QStyle::SubElement se, const QStyleOption * option, const QWidget * widget) const
{
	QRect rect;

	switch ( se ) {
		case QIrDockStyle::SE_DockletBundleIndicator:
			if ( const QIrStyleOptionDockletBundle * bundle = qstyleoption_cast< const QIrStyleOptionDockletBundle * >(option) ) {
				QRect r = rect = bundle->rect;
				int height = pixelMetric((QStyle::PixelMetric)PM_DockletIndicatorHeight,option);
				
				switch ( bundle->area ) {
					case QIrAbstractDock::DA_BottomDockArea : 
						rect.setTop(r.bottom() - height);
						break;
					case QIrAbstractDock::DA_TopDockArea :
						rect.setBottom(r.top() + height);
						break;
					case QIrAbstractDock::DA_LeftDockArea :
						rect.setRight(r.left() + height);
						break;
					default:
						rect.setLeft(r.right() - height);
						break;
				}
				return rect;
			}
			break;
		case QIrDockStyle::SE_DockletBundleContentsRect:
			if ( const QIrStyleOptionDockletBundle * bundle = qstyleoption_cast< const QIrStyleOptionDockletBundle * >(option) ) {
				rect = bundle->rect;
				int height = pixelMetric((QStyle::PixelMetric)PM_DockletIndicatorHeight,option);

				switch ( bundle->area ) {
					case QIrAbstractDock::DA_BottomDockArea :
						return rect.adjusted(0,0,0,-height);
					case QIrAbstractDock::DA_TopDockArea :
						return rect.adjusted(0,height,0,0);
					case QIrAbstractDock::DA_LeftDockArea :
						return rect.adjusted(height,0,0,0);
					default :
						return rect.adjusted(0,0,-height,0);
				}			
			}
			break;
		case QIrDockStyle::SE_DockTipLabelContents :
			if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
				int topMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelTopMargin,tip),
					bottomMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelBottomMargin,tip),
					rightMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelRightMargin,tip),
					leftMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelLeftMargin,tip);
				return tip->rect.adjusted(leftMargin,topMargin,-rightMargin,-bottomMargin);
			}
			break;
		case QIrDockStyle::SE_DockPanelRect :
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {

				return dock->frameRect;
			}
			break;
		default:
			break;
	}
	return QIrCommonDockStyle::subElementRect(se,option,widget);
}
int QIrDefaultDockStyle::styleHint(QStyle::StyleHint hint, const QStyleOption * option, const QWidget * widget, QStyleHintReturn * returnData) const
{
	switch ( hint ) {
		case QIrDockStyle::SH_DockTipFont:
			if ( QStyleHintReturnVariant * ret = qstyleoption_cast< QStyleHintReturnVariant * >(returnData) ) {
				QFont font;

				font.setFamily("helvetica");
				font.setPointSize(16);
				font.setBold(true);				
				ret->variant = font;
				return 0;
			}
			break;
		default:
			break;
	}
	return QIrCommonDockStyle::styleHint(hint,option,widget,returnData);
}
void QIrDefaultDockStyle::polish( QWidget * widget )
{
	QIrDock * dock = qobject_cast< QIrDock * >(widget);
	
	if ( dock ) {
		connect(dock,SIGNAL(dockAreaChanged( QIrAbstractDock::DockArea )),this,SLOT(slotHandleDockAreaChanged()));
		foreach (QIrDockletBundle * bundle, dock->view()->bundles(false)) {
			polish(bundle);
			bundle->invalidate();
		}
	}
}
void QIrDefaultDockStyle::unpolish( QWidget * widget )
{
	QIrDock * dock = dynamic_cast< QIrDock * >(widget);
	
	if ( dock ) {
		disconnect(dock,SIGNAL(dockAreaChanged( QIrAbstractDock::DockArea )),this,SLOT(slotHandleDockAreaChanged()));
		foreach (QIrDockletBundle * bundle, dock->view()->bundles(false)) {
			unpolish(bundle);
		}
	}
}
void QIrDefaultDockStyle::setEffects( QIrDockletBundle * bundle )
{
	QIrDocklet * docklet = bundle->docklet();
        QGraphicsColorizeEffect * effect = new QGraphicsColorizeEffect(docklet);
	
        effect->setColor(Qt::black);
	docklet->setGraphicsEffect(effect);
	effect->setEnabled(false);
	bundle->invalidate();
}
void QIrDefaultDockStyle::unsetEffects( QIrDockletBundle * bundle )
{
	bundle->docklet()->setGraphicsEffect(0);
	bundle->invalidate();

}
void QIrDefaultDockStyle::polish( QIrDockletBundle * bundle )
{
	setEffects(bundle);
	connect(bundle,SIGNAL(entered()),this,SLOT(slotHandleBundleEntered()));
	connect(bundle,SIGNAL(left()),this,SLOT(slotHandleBundleLeft()));
	connect(bundle,SIGNAL(pressed()),this,SLOT(slotHandleBundleMousePressed()));
	connect(bundle,SIGNAL(released()),this,SLOT(slotHandleBundleMouseReleased()));
}
void QIrDefaultDockStyle::unpolish( QIrDockletBundle * bundle )
{
	unsetEffects(bundle);
	disconnect(bundle,SIGNAL(entered()),this,SLOT(slotHandleBundleEntered()));
	disconnect(bundle,SIGNAL(left()),this,SLOT(slotHandleBundleLeft()));
	disconnect(bundle,SIGNAL(pressed()),this,SLOT(slotHandleBundleMousePressed()));
	disconnect(bundle,SIGNAL(released()),this,SLOT(slotHandleBundleMouseReleased()));
}
void QIrDefaultDockStyle::handleBundleEnteredImpl()
{
}
void QIrDefaultDockStyle::handleBundleLeftImpl()
{
}
void QIrDefaultDockStyle::handleBundleMousePressedImpl()
{
	QIrDockletBundle * bundle = qobject_cast< QIrDockletBundle * >(sender());
	
	if ( bundle ) {
                QGraphicsColorizeEffect * effect = qobject_cast<QGraphicsColorizeEffect * >(bundle->docklet()->graphicsEffect());

		if ( effect )
			effect->setEnabled(true);
	}
}
void QIrDefaultDockStyle::handleBundleMouseReleasedImpl()
{
	QIrDockletBundle * bundle = qobject_cast< QIrDockletBundle * >(sender());

	if ( bundle ) {
                QGraphicsColorizeEffect * effect = qobject_cast<QGraphicsColorizeEffect * >(bundle->docklet()->graphicsEffect());

		if ( effect )
			effect->setEnabled(false);
	}
}
void QIrDefaultDockStyle::handleDockAreaChangedImpl()
{
}

/////////////////////
//QIrStyledDockStyle
/////////////////////
QIrStyledDockStyle::QIrStyledDockStyle() : QIrDefaultDockStyle(* new QIrStyledDockStylePrivate)
{
	QIR_P(QIrStyledDockStyle);

	p->setup();
}
QIrStyledDockStyle::QIrStyledDockStyle( QIrStyledDockStylePrivate & p) : QIrDefaultDockStyle(p)
{
}
QIrStyledDockStyle::~QIrStyledDockStyle()
{
}
void QIrStyledDockStyle::drawPrimitive(QStyle::PrimitiveElement pe , const QStyleOption * option, QPainter * painter, const QWidget * w) const
{
	QIR_P(const QIrStyledDockStyle);

	switch ( pe ) {
		case PE_PanelDock:
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {
				QRect panelRect = subElementRect((QStyle::SubElement)QIrDockStyle::SE_DockPanelRect,dock);
				QPoint startVGrad, endVGrad, startHGrad, endHGrad;
				QColor color = QColor(75,75,75);
				QColor startGradColor = color.light(), 
					endGradColor = color, 
					outlineColor = endGradColor;			
				QPoint P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12;
				QVector< QPoint	> points;
				int d = 10;
				
				QPainterPath cubicPath;
				QPoint center = panelRect.center();

				p->shadow.setOpacity(0.5);
				p->shadow.setRect(panelRect);

				switch ( dock->area ) {
					case QIrAbstractDock::DA_BottomDockArea :
						P11 = panelRect.bottomLeft() + QPoint(0,-1);
						P12 = panelRect.bottomRight() + QPoint(0,-1);

						P5 = QPoint(panelRect.left(),center.y() - d);
						P6 = QPoint(center.x(),panelRect.bottom());
						P7 = QPoint(center.x(),panelRect.top());
						P8 = QPoint(panelRect.right(),center.y() + d);
						P9 = panelRect.topRight();
						P10 = panelRect.topLeft();

						startVGrad = panelRect.topLeft();
						endVGrad = panelRect.bottomLeft();
						break;
					case QIrAbstractDock::DA_TopDockArea :

						P11 = panelRect.topLeft() + QPoint(0,1);
						P12 = panelRect.topRight() + QPoint(0,1);

						P5 = QPoint(panelRect.right(),center.y() + d);
						P6 = QPoint(center.x(),panelRect.top());
						P7 = QPoint(center.x(),panelRect.bottom());
						P8 = QPoint(panelRect.left(),center.y() - d);
						P9 = panelRect.bottomLeft();
						P10 = panelRect.bottomRight();

						startVGrad = panelRect.bottomLeft();
						endVGrad = panelRect.topLeft();
						break;
					case QIrAbstractDock::DA_LeftDockArea :
						
						P11 = panelRect.topLeft() + QPoint(1,0);
						P12 = panelRect.bottomLeft() + QPoint(1,0);

						P5 = QPoint(center.x() + d,panelRect.top());
						P6 = QPoint(panelRect.left(),center.y());
						P7 = QPoint(panelRect.right(),center.y());
						P8 = QPoint(center.x() - d,panelRect.bottom());
						P9 = panelRect.bottomRight();
						P10 = panelRect.topRight();

						startVGrad = panelRect.topRight();
						endVGrad = panelRect.topLeft();
						break;
					default:

						P11 = panelRect.topRight() + QPoint(-1,0);
						P12 = panelRect.bottomRight() + QPoint(-1,0);

						
						P5 = QPoint(center.x() - d,panelRect.top());
						P6 = QPoint(panelRect.right(),center.y());
						P7 = QPoint(panelRect.left(),center.y());
						P8 = QPoint(center.x() + d,panelRect.bottom());
						P9 = panelRect.bottomLeft();
						P10 = panelRect.topLeft();

						startVGrad = panelRect.topLeft();
						endVGrad = panelRect.topRight();
						break;
				}
				if ( dock->orientation == Qt::Horizontal ) {
					startHGrad = panelRect.topLeft();
					endHGrad = panelRect.topRight();
				} else {
					startHGrad = panelRect.topLeft();
					endHGrad = panelRect.bottomLeft();
				}
				QLinearGradient VGrad(startVGrad,endVGrad);
				QLinearGradient HGrad(startHGrad,endHGrad);
				
						
				startGradColor.setAlpha(200);
				endGradColor.setAlpha(200);
				VGrad.setColorAt(1,QColor(255,255,255,75));
				VGrad.setColorAt(0,QColor(255,255,255,10));
			
				HGrad.setColorAt(0,QColor(50,50,50,200));
				HGrad.setColorAt(0.2,QColor(50,50,50,100));
				HGrad.setColorAt(0.8,QColor(50,50,50,100));
				HGrad.setColorAt(1,QColor(50,50,50,200));

				//Draw shadow
				p->shadow.paint(painter);


				painter->setPen(Qt::NoPen);
				painter->setBrush(QBrush(HGrad));
				painter->drawRect(panelRect);

				painter->setPen(outlineColor);
				painter->setBrush(QBrush(startGradColor));
				painter->drawRect(panelRect);

				painter->setRenderHint(QPainter::Antialiasing);
				//Draw cubic
				cubicPath.moveTo(P5);
				cubicPath.cubicTo(P6,P7,P8);
				cubicPath.lineTo(P9);
				cubicPath.lineTo(P10);
				cubicPath.closeSubpath();
				
				painter->setPen(Qt::NoPen);
				painter->setBrush(QBrush(VGrad));				
				painter->drawPath(cubicPath);

				painter->setRenderHint(QPainter::Antialiasing,false);

				painter->setPen(QColor(255,255,255,100));
				painter->drawLine(P11,P12);

			}
			break;
		case PE_DockletIndicator:
			if ( const QIrStyleOptionDockletBundle * di = qstyleoption_cast< const QIrStyleOptionDockletBundle * >(option) ) {
				QRect r = option->rect;
				QColor color = option->palette.foreground().color();
				QImage img(di->area == QIrAbstractDock::DA_BottomDockArea ? qir_docklet_indicator_up : 
					di->area == QIrAbstractDock::DA_TopDockArea ? qir_docklet_indicator_down :
					di->area == QIrAbstractDock::DA_LeftDockArea ? qir_docklet_indicator_right : qir_docklet_indicator_left);
				QRect imgR(QPoint(),img.size());
				
				QRect pixRect(QPoint(),QSize(14,14));
				QPixmap pixmap(QSize(14,14));
				pixmap.fill(Qt::transparent);
				QPainter pixmapPainter(&pixmap);
				pixmapPainter.setRenderHint(QPainter::Antialiasing);

					
				QRadialGradient radGrad(pixRect.center(),7,pixRect.topLeft());


				radGrad.setColorAt(0,QColor(255,255,255));
				radGrad.setColorAt(1,QColor(255,255,255,0));

				pixmapPainter.setPen(QColor(0,0,0,50));
				pixmapPainter.setBrush(QBrush(radGrad));
				pixmapPainter.drawEllipse(pixRect);
				pixmapPainter.end();
				
				if ( !img.isNull() ) {
					pixRect.moveCenter(r.center());
					imgR.moveCenter(r.center());
					img.setColor(1,QColor(255,255,255).rgba());
					p->blurFilter->setRadius(2);
					p->blurFilter->draw(painter,pixRect.topLeft(),pixmap);
					img.setColor(1,color.rgba());
					painter->drawImage(imgR,img);
				}
				
			}
			break;
		case PE_PanelDockTipLabel :
				if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
					int topMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelTopMargin,tip), 
						bottomMargin = pixelMetric((QStyle::PixelMetric)QIrDockStyle::PM_DockTipLabelBottomMargin,tip), 
						topAdjust = 0, bottomAdjust = 0;
					QRect rect = tip->rect.adjusted(1,1,-1,-1);
					QPoint center = tip->targetRect.center(), P1, P2, P3;
					QColor color = tip->palette.highlight().color();
					bool horizontal = false;
					int h = rect.height(), miH = h / 2;
					
					QLinearGradient grad(rect.topLeft(),rect.bottomLeft());

					grad.setColorAt(0,color.light(130));
					grad.setColorAt(1,color.dark(130));

					switch ( tip->area ) {
						case QIrAbstractDock::DA_BottomDockArea :
							if ( bottomMargin > topMargin )
								bottomAdjust = bottomMargin - topMargin;
							center.rx() = qBound(rect.left() + miH,center.x(),rect.right() - miH);
							P1 = QPoint(center.x(),rect.bottom());
							P2 = QPoint(center.x() + bottomAdjust,rect.bottom() - bottomAdjust);
							P3 = QPoint(center.x() - bottomAdjust,rect.bottom() - bottomAdjust);
							horizontal = true;
							break;
						case QIrAbstractDock::DA_TopDockArea :
							if ( bottomMargin < topMargin )
								topAdjust = topMargin - bottomMargin;
							center.rx() = qBound(rect.left() + miH,center.x(),rect.right() - miH);
							P1 = QPoint(center.x(),rect.top());
							P2 = QPoint(center.x() + topAdjust,rect.top() + topAdjust + 1);
							P3 = QPoint(center.x() - topAdjust,rect.top() + topAdjust + 1);
							horizontal = true;
							break;
						default :
							break;
					}
					rect.adjust(0,topAdjust,0,-bottomAdjust);
				
					QPainterPath path;

					painter->save();
					painter->setRenderHint(QPainter::Antialiasing);
					path.addRoundedRect(rect,rect.height() / 2,rect.height() / 2);
					if ( horizontal ) {
						QPainterPath trianglePath;

						trianglePath.moveTo(P2);
						trianglePath.lineTo(P1);
						trianglePath.lineTo(P3);
						trianglePath.closeSubpath();

						path = path.united(trianglePath);
					}

					painter->setPen(color.darker(150));
					painter->setBrush(QBrush(grad));
					painter->drawPath(path);
					painter->restore();
				}
				break;
		default:
			QIrDefaultDockStyle::drawPrimitive(pe,option,painter,w);
			break;
	}	
}
void QIrStyledDockStyle::drawControl(QStyle::ControlElement ce, const QStyleOption * option, QPainter * painter, const QWidget * w) const
{
	switch ( ce ) {
		case CE_Docklet:
			if (const QIrStyleOptionDocklet * di = qstyleoption_cast< const QIrStyleOptionDocklet * >(option) ) {
				
				QRect r = di->rect;
				
				if ( di->separator ) {					
					QPoint P1, P2, center = P1 = P2 = r.center();
					QImage img(( di->orientation ==  Qt::Horizontal ? qir_docklet_separator_v : qir_docklet_separator_h)); 
					QRect r = option->rect;
					int w = r.width(), h = r.height();
					
					if ( di->orientation == Qt::Horizontal ) {
						w = qMin(16,w);
						r.adjust(0,2,0,2);
					}
					else {
						h = qMin(16,h);
						r.adjust(2,0,-2,0);
					}
					r.setSize(QSize(w,h));
					r.moveCenter(option->rect.center());					

					img.setColor(1,QColor(255,255,255,40).rgba());
					img.setColor(2,QColor(0,0,0,60).rgba());
					img.setColor(3,QColor(255,255,255,70).rgba());
					
					painter->setPen(Qt::NoPen);
					painter->setBrush(QBrush(img));
					painter->drawRect(r);

				} else {
					QIrDefaultDockStyle::drawControl(ce,option,painter,w);
				}
			}
			break;
			case CE_DockTipLabel :
				if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
					QRect contentRect = subElementRect((QStyle::SubElement)QIrDockStyle::SE_DockTipLabelContents,tip);
					QColor color = tip->palette.highlightedText().color();

					painter->save();
					painter->setRenderHint(QPainter::Antialiasing);
					drawPrimitive((QStyle::PrimitiveElement)QIrDockStyle::PE_PanelDockTipLabel,tip,painter);
					painter->setRenderHint(QPainter::Antialiasing,false);
					painter->setFont(tip->font);
					painter->setPen(QColor(20,20,20,200));
					painter->drawText(contentRect.adjusted(0,1,0,1),Qt::AlignCenter,tip->text);

					painter->setPen(color);
					painter->drawText(contentRect,Qt::AlignCenter,tip->text);
					painter->restore();
				}
			break;
		default:
			QIrDefaultDockStyle::drawControl(ce,option,painter,w);
			break;
	}
}
int QIrStyledDockStyle::pixelMetric(QStyle::PixelMetric pm, const QStyleOption * option , const QWidget * w) const
{
	switch ( pm ) {
		case PM_DockletIndicatorHeight :
			return 25;
		case PM_DockLayoutTopMargin :
		case PM_DockLayoutBottomMargin :
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {
				if ( dock->orientation == Qt::Horizontal )
					return 9;
				else
					return 40;			
			}
			break;
		case PM_DockLayoutLeftMargin :
		case PM_DockLayoutRightMargin :
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {
				if ( dock->orientation == Qt::Horizontal )
					return 40;
				else
					return 9;
			}
			break;
		case PM_DockTipLabelTopMargin :
			if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
				if ( tip->area == QIrAbstractDock::DA_TopDockArea )
					return 16;
				else
					return 6;
			}
			break;
		case PM_DockTipLabelBottomMargin :
			if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
				if ( tip->area == QIrAbstractDock::DA_BottomDockArea )
					return 16;
				else
					return 6;
			}
			break;
		case PM_DockTipLabelRightMargin :
		case PM_DockTipLabelLeftMargin :
			if ( const QIrStyleOptionDockTipLabel * tip = qstyleoption_cast< const QIrStyleOptionDockTipLabel * >(option) ) {
                            Q_UNUSED(tip)
                            return 16;
			}
			break;
		default:
			break;
	}
	return QIrDefaultDockStyle::pixelMetric(pm,option,w);
}
int QIrStyledDockStyle::styleHint(QStyle::StyleHint hint, const QStyleOption * option, const QWidget * widget, QStyleHintReturn * returnData) const
{
	switch ( hint ) {
		case QIrDockStyle::SH_DockTipFont:
			if ( QStyleHintReturnVariant * ret = qstyleoption_cast< QStyleHintReturnVariant * >(returnData) ) {
				QFont font;

				font.setFamily("arial");
				font.setPointSize(10);
				font.setBold(true);				
				ret->variant = font;
				return 0;
			}
		default:
			break;
	}
	return QIrDefaultDockStyle::styleHint(hint,option,widget,returnData);
}
QRect QIrStyledDockStyle::subElementRect(QStyle::SubElement se, const QStyleOption * option, const QWidget * widget) const
{
	QRect rect;

	switch ( se ) {
		case QIrDockStyle::SE_DockPanelRect :
			if ( const QIrStyleOptionDock * dock = qstyleoption_cast< const QIrStyleOptionDock * >(option) ) {

				return dock->frameRect.adjusted(6,6,-6,-6);
			}
			break;
		default:
			break;
	}
	return QIrDefaultDockStyle::subElementRect(se,option,widget);
}
void QIrStyledDockStyle::handleBundleEnteredImpl()
{	
}
void QIrStyledDockStyle::handleBundleLeftImpl()
{
}
void QIrStyledDockStyle::handleBundleMousePressedImpl()
{
	QIrDockletBundle * bundle = qobject_cast< QIrDockletBundle * >(sender());
	
	if ( bundle ) {
		QIrGraphicsMirrorEffect * effect = qobject_cast<QIrGraphicsMirrorEffect * >(bundle->docklet()->graphicsEffect());

		effect->setUseGrayScale(true);
	}
}
void QIrStyledDockStyle::handleBundleMouseReleasedImpl()
{
	QIrDockletBundle * bundle = qobject_cast< QIrDockletBundle * >(sender());

	if ( bundle ) {
		QIrGraphicsMirrorEffect * effect = qobject_cast<QIrGraphicsMirrorEffect * >(bundle->docklet()->graphicsEffect());

		effect->setUseGrayScale(false);
	}
}
void QIrStyledDockStyle::handleDockAreaChangedImpl()
{
	QIrDock * dock = qobject_cast< QIrDock * >(sender());
	QIrGraphicsMirrorEffect * effect;

	foreach (QIrDockletBundle * bundle, dock->view()->bundles(false) ) {
                if ( (effect = qobject_cast< QIrGraphicsMirrorEffect * >(bundle->docklet()->graphicsEffect())) ) {
			effect->setMirrorPosition(mirrorPositionForDockArea(dock->dockArea()));
		}
	}
}

void QIrStyledDockStyle::setEffects( QIrDockletBundle * bundle )
{
	QIrDocklet * docklet = bundle->docklet();
	QIrGraphicsMirrorEffect * effect = new QIrGraphicsMirrorEffect(docklet);
	
	docklet->setGraphicsEffect(effect);
	effect->setMirrorPosition(mirrorPositionForDockArea(docklet->dock()->dockArea()));
	bundle->invalidate();
}

QIR_END_NAMESPACE
