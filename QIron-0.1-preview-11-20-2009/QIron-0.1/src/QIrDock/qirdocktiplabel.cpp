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
#include <QApplication>
#include <QPainter>
#include <QtDebug>
#include "qirdocktiplabel.h"
#include "../Common/ui/qirwidget_ui.h"
#include "qirdock.h"
#include "qirdockstyle.h"
#include "qirstyleoptiondock.h"

QIR_BEGIN_NAMESPACE

////////////////////
//QIrDockTipLabelUi
////////////////////
class QIrDockTipLabelUi : public QIrWidgetUi
{
	QIR_DECLARE_WIDGET(QIrDockTipLabel)

public:
	QIrDockTipLabelUi() : dock(0), opacity(1.0) { }
	~QIrDockTipLabelUi() { }

	QIrDock * dock;
	QString text;
	QRect targetVisualRect;
	qreal opacity;

protected:
	void init();
};

void QIrDockTipLabelUi::init()
{
	QIR_W(QIrDockTipLabel);

	w->setAttribute(Qt::WA_NoChildEventsForParent);
}


//////////////////
//QIrDockTipLabel
//////////////////
QIrDockTipLabel::QIrDockTipLabel(QIrDock * dock, QWidget * parent) : QWidget(parent), QIrWidget(* new QIrDockTipLabelUi)
{
	QIR_UI(QIrDockTipLabel);

	ui->dock = dock;
	ui->setupUi();
}
QIrDockTipLabel::~QIrDockTipLabel()
{
}
QSize QIrDockTipLabel::sizeHint() const
{
	QIR_UI(const QIrDockTipLabel);

	QIrSubStyle * style = ui->dock->subStyle();
	QIrStyleOptionDockTipLabel option;
	int w = 0, h = 0;

	if ( !style )
		return QSize(30,30);
	
	initStyleOption(&option);
    // calculate contents size...
    QString s(text());
    bool empty = s.isEmpty();
    if ( empty )
        s = QString::fromLatin1("XXXX");
	QFontMetrics fm(font());
    QSize sz = fm.size(Qt::TextShowMnemonic, s);

    if ( !empty || !w )
        w += sz.width();
    if ( !empty || !h )
        h = qMax(h, sz.height());
    option.rect.setSize(QSize(w, h));

	return (style->sizeFromContents((QStyle::ContentsType)QIrDockStyle::CT_DockTipLabel,&option, QSize(w,h),this).expandedTo(QApplication::globalStrut()));
}
qreal QIrDockTipLabel::opacity() const
{
	QIR_UI(const QIrDockTipLabel);

	return ui->opacity;
}
void QIrDockTipLabel::setOpacity( qreal opacity )
{
	QIR_UI(QIrDockTipLabel);

	if ( qFuzzyCompare(ui->opacity,opacity) )
		return;
	ui->opacity = opacity;
	update();
}

QString QIrDockTipLabel::text() const
{
	return ui_func()->text;
}
void QIrDockTipLabel::setText( const QString & text )
{
	QIR_UI(QIrDockTipLabel);

	if ( ui->text == text )
		return;
	ui->text = text;
}
QRect QIrDockTipLabel::targetVisualRect() const
{
	return ui_func()->targetVisualRect;
}
void QIrDockTipLabel::setTargetVisualRect( const QRect & rect )
{
	ui_func()->targetVisualRect = rect;
}
void QIrDockTipLabel::paintEvent( QPaintEvent * )
{
	QIR_UI(QIrDockTipLabel);
	QIrSubStyle * style = ui->dock->subStyle();
	QIrStyleOptionDockTipLabel option;
	QPainter painter(this);
	
	if ( !style )
		return;
	initStyleOption(&option);
	painter.setOpacity(ui->opacity);
	
	QPixmap pixmap(size());
	pixmap.fill(Qt::transparent);
	QPainter pixmapPainter(&pixmap);

	//draw frame
	style->drawControl((QStyle::ControlElement)QIrDockStyle::CE_DockTipLabel,&option,&pixmapPainter);
	pixmapPainter.end();
	painter.drawPixmap(rect(),pixmap);
}
void QIrDockTipLabel::initStyleOption( QIrStyleOptionDockTipLabel * option ) const
{
	QIR_UI(const QIrDockTipLabel);

	option->initFrom(this);
	option->font = font();
	option->targetRect = QRect(mapFromParent(ui->targetVisualRect.topLeft()),ui->targetVisualRect.size());
	option->area = ui->dock->dockArea();
	option->text = ui->text.isEmpty() ? QString::fromLatin1("...") : ui->text;
}

QIR_END_NAMESPACE
