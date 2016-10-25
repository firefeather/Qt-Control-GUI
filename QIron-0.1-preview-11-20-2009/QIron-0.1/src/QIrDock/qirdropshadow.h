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
#ifndef QIRDROPSHADOW_H
#define QIRDROPSHADOW_H

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

#include <QRect>
#include <QPainterPath>
#include "../Common/qiron_export.h"

class QPainter;

QIR_BEGIN_NAMESPACE

class QIRONSHARED_EXPORT QIrDropShadow
{
public:
	enum DropShadowRegion {
		TopLeft,
		Left,
		BottomLeft,
		Bottom,
		BottomRight,
		Right,
		TopRight,
		Top
	};
	enum DropShadowEdge {
		None = 0,
		TopEdge = 1,
		LeftEdge = 2,
		BottomEdge = 4,
		RightEdge = 8,
		All = 15
	};
	Q_DECLARE_FLAGS(DropShadowEdges,DropShadowEdge);
	
	explicit QIrDropShadow(const QRect & rect = QRect());
	~QIrDropShadow() { };
	
	inline void setVisible( bool v ) { m_visible = v; }
	inline bool isVisible() const { return m_visible; }
	
	inline void setEdges( DropShadowEdges e ) { m_edges = e; }
	inline void setEdge(DropShadowEdge edge, bool shown = true) { m_edges = shown ? m_edges | edge : m_edges & ~edge; }
	inline DropShadowEdges edges() const { return m_edges; }
	
	inline void setOpacity( qreal o ) { m_opacity = o; }
	inline qreal opacity() { return m_opacity; }

	inline QRect rect() const { return m_rect; }
	inline void setRect( const QRect & rect ) { m_rect = rect; }

	QPainterPath clipPath() const { return m_path; }
	void setClipPath( const QPainterPath & p ) { m_path = p; }

	void paint( QPainter * );

private:
	QRect m_rect;
	qreal m_opacity;
	DropShadowEdges m_edges;
	bool m_visible;
	QPainterPath m_path;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QIrDropShadow::DropShadowEdges)

QIR_END_NAMESPACE

#endif // QIRDROPSHADOW_H
