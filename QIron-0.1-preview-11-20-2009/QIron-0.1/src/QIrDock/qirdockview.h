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
#ifndef QIRDOCKVIEW_H
#define QIRDOCKVIEW_H

#include <QGraphicsView>
#include "../Common/qirobject.h"
#include "../Common/qirwidget.h"

QIR_BEGIN_NAMESPACE

class QIrDock;
class QIrDocklet;
class QIrDockletBundle;
class QIrDockletBundlePrivate;
class QIrDockEffectManager;
class QIrDockStyle;
class QIrDockView;
class QIrDockViewUi;
class QIrStyleOptionDocklet;

class QIRONSHARED_EXPORT QIrDockView : public QGraphicsView, public QIrWidget
{
	Q_OBJECT
	QIR_DECLARE_UI(QIrDockView);
	friend class QIrDock;
	friend class QIrDockBundle;

public:
	QIR_WIDGET_CAST

	explicit QIrDockView( QIrDock * dock );
	~QIrDockView();
	
	QSize boundingSize() const;

	QIrDockletBundle * bundle( QIrDocklet * docklet ) const;

	QList< QIrDockletBundle * > bundles( bool onlyVisible = true ) const;

	QIrDock * dock() const;

	int dockletSpacing() const;

	int extendSize() const;
	int normalSize() const;
	
	QIrDockEffectManager * effectManager() const;
	void setEffectManager( QIrDockEffectManager * );
	
	int extendOffset() const;	

	QSize sizeHint() const;

	inline void emitResetViewRequested() { emit resetViewRequested(); }

	void adjustLayers();
	
signals:
	void resetViewRequested();

protected:
	friend class QIrDockEffectManager;

	void contextMenuEvent( QContextMenuEvent * );
	void enterEvent( QEvent * );
	void leaveEvent( QEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void resizeEvent( QResizeEvent * );

        void deferPosEvaluation( int delay );

protected slots:
	void slotHandleMaximumIconSizeChanged( int );
	void slotHandleMinimumIconSizeChanged( int );
	void slotHandleDiferredPosEvaluation();
	
private:
	enum SizeType {
		NormalSize,
		ExtendSize
	};

	void insertBundle(int index, QIrDockletBundle * );
	void removeBundle( QIrDockletBundle * );
	
	bool areEventsToEffectBlocked() const;
	void blockEventsToEffect( bool block );

	int calculateSize( QIrDockView::SizeType st ) const;

	void evaluatePos( const QPoint & );

	void hoverDockletChanged(QIrDocklet * current, QIrDocklet * previous);	
};

QIR_END_NAMESPACE

#endif // QIRDOCKVIEW_H
