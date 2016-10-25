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
#include "../Common/private/qirobject_p.h"
#include <QPainter>
#include "qirgraphicsmirroreffect.h"
#include <private/qpixmapfilter_p.h>
#include <QtDebug>

QIR_BEGIN_NAMESPACE

/////////////////////////////////
//QIrGraphicsMirrorEffectPrivate
////////////////////////////////
class QIrGraphicsMirrorEffectPrivate : public QIrObjectPrivate
{
	QIR_DECLARE_OBJECT(QIrGraphicsMirrorEffect);

public:
	QIrGraphicsMirrorEffectPrivate() : isFullyOpaque(false), isFullyTransparent(false), opacity(0.5), useGray(false) {}
	~QIrGraphicsMirrorEffectPrivate() {}

	QPixmapBlurFilter * filter;
	QPixmapColorizeFilter * grayFilter;

	QIrGraphicsMirrorEffect::MirrorPosition mirrorPosition;
	bool isFullyOpaque;
	bool isFullyTransparent;
	qreal opacity;
	bool useGray;

protected:
	void init();
};

void QIrGraphicsMirrorEffectPrivate::init()
{
	QIR_O(QIrGraphicsMirrorEffect);

	filter = new QPixmapBlurFilter(o);
	grayFilter = new QPixmapColorizeFilter(o);
	grayFilter->setColor(Qt::black);

	mirrorPosition = QIrGraphicsMirrorEffect::OnBottom;
}

QIrGraphicsMirrorEffect::QIrGraphicsMirrorEffect( QObject * parent ) : QGraphicsEffect(parent), QIrObject(* new QIrGraphicsMirrorEffectPrivate)
{
	QIR_P(QIrGraphicsMirrorEffect);

	p->setup();
}
QIrGraphicsMirrorEffect::~QIrGraphicsMirrorEffect()
{
}
QIrGraphicsMirrorEffect::MirrorPosition QIrGraphicsMirrorEffect::mirrorPosition() const
{
	QIR_P(const QIrGraphicsMirrorEffect);

	return p->mirrorPosition;
}
void QIrGraphicsMirrorEffect::setMirrorPosition( QIrGraphicsMirrorEffect::MirrorPosition pos )
{
	QIR_P(QIrGraphicsMirrorEffect);

	p->mirrorPosition = pos;
}
bool QIrGraphicsMirrorEffect::useGrayScale() const
{
	QIR_P(const QIrGraphicsMirrorEffect);

	return p->useGray;
}
void QIrGraphicsMirrorEffect::setUseGrayScale( bool use )
{
	QIR_P(QIrGraphicsMirrorEffect);

	if ( p->useGray == use )
		return;
	p->useGray = use;
	update();
}
qreal QIrGraphicsMirrorEffect::opacity() const
{
	QIR_P(const QIrGraphicsMirrorEffect);

	return p->opacity;
}
void QIrGraphicsMirrorEffect::setOpacity( qreal opacity )
{
    QIR_P(QIrGraphicsMirrorEffect);

    opacity = qBound(qreal(0.0), opacity, qreal(1.0));

    if (qFuzzyCompare(p->opacity, opacity))
        return;

    p->opacity = opacity;
    if ((p->isFullyTransparent = qFuzzyIsNull(p->opacity)))
        p->isFullyOpaque = 0;
    else
        p->isFullyOpaque = qFuzzyIsNull(p->opacity - 1);
    update();
    emit opacityChanged(opacity);
}
QRectF QIrGraphicsMirrorEffect::boundingRectFor( const QRectF & rect ) const
{
	QIR_P(const QIrGraphicsMirrorEffect);

	if ( !rect.isValid() )
		return rect;

	switch ( p->mirrorPosition ) {
			case OnBottom :
				return rect.adjusted(0,0,0,rect.height());
			case OnTop :
				return rect.adjusted(0,-rect.height(),0,0);
			case OnLeft :
				return rect.adjusted(-rect.width(),0,0,0);
			default :
				return rect.adjusted(0,0,rect.width(),0);
	}
}
void QIrGraphicsMirrorEffect::draw(QPainter * painter)
{
	QIR_P(QIrGraphicsMirrorEffect);

        // Transparent mirror: no reflection to draw.
	if ( p->isFullyTransparent ) {
                drawSource(painter);
        return;
	}

	painter->save();
    
	QPoint offset, refletOffset;
    if (sourceIsPixmap()) {

    } else {
        // Draw pixmap in device coordinates to avoid pixmap scaling;
		QTransform worldTransform = painter->worldTransform();
		
		// Calculate source bounding rect in logical and device coordinates.
                QRectF srcBrect = sourceBoundingRect();
		QRect srcDeviceBrect = worldTransform.mapRect(srcBrect).toAlignedRect();
		
                srcDeviceBrect &= sourceBoundingRect(Qt::DeviceCoordinates).toRect();
		QSize sz = srcDeviceBrect.size();
		offset = srcDeviceBrect.topLeft();
		refletOffset = offset;

                QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);
		QTransform restoreTransform = painter->worldTransform();
		painter->setWorldTransform(QTransform());
		
		if ( p->useGray ) {
			QPainter pixmapPainter(&pixmap);
			p->grayFilter->draw(&pixmapPainter,QPoint(),pixmap);
			pixmapPainter.end();		
		}
		painter->drawPixmap(offset,pixmap);

		QImage img;

		switch ( p->mirrorPosition ) {
			case OnBottom :
				img = pixmap.toImage().mirrored(0,true);
				refletOffset.ry() += -img.size().height() + 2 * sz.height();
				break;
			case OnTop :
				img = pixmap.toImage().mirrored(0,true);
				refletOffset.ry() += - sz.height();
				break;
			case OnLeft :
				img = pixmap.toImage().mirrored(true,0);
				refletOffset.rx() += -sz.width();
				break;
			default:
				img = pixmap.toImage().mirrored(true,0);
				refletOffset.rx() += -img.size().width() + 2 * sz.width();
				break;
		}
		painter->setOpacity(p->opacity);
		p->filter->draw(painter,refletOffset,QPixmap::fromImage(img));
		painter->setWorldTransform(restoreTransform);
     }
    painter->restore();
}
QIR_END_NAMESPACE
