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
#ifndef QIRGRAPHICSMIRROREFFECT_H
#define QIRGRAPHICSMIRROREFFECT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QIron API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QGraphicsEffect>
#include "../Common/qirobject.h"

QIR_BEGIN_NAMESPACE

class QIrGraphicsMirrorEffectPrivate;

class QIRONSHARED_EXPORT QIrGraphicsMirrorEffect : public QGraphicsEffect, public QIrObject
{
	Q_OBJECT
	QIR_DECLARE_PRIVATE(QIrGraphicsMirrorEffect)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
	Q_PROPERTY(MirrorPosition mirrorPosition READ mirrorPosition WRITE setMirrorPosition)
	
public:
	enum MirrorPosition {
		OnTop,
		OnBottom,
		OnLeft,
		OnRight
	};

	QIrGraphicsMirrorEffect( QObject * parent = 0 );
	~QIrGraphicsMirrorEffect();

	bool useGrayScale() const;
	void setUseGrayScale( bool use );

	QRectF boundingRectFor( const QRectF & rect ) const;

	MirrorPosition mirrorPosition() const;
	void setMirrorPosition( MirrorPosition position );

	qreal opacity() const;
	void setOpacity( qreal o );

protected:
        void draw(QPainter * painter);

signals:
	void opacityChanged( qreal );
};

QIR_END_NAMESPACE

#endif // QIRGRAPHICSMIRROREFFECT_H
