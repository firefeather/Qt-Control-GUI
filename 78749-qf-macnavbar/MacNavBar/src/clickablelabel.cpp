/* 
 *  This file is part of Quartica.
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 *
 *  Quartica is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Quartica is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Quartica.  If not, see <http://www.gnu.org/licenses/>.
 */

// Qt4 Headers
#include <QMouseEvent>

// Quartica Headers
#include "qf_clickablelabel.h"

// =============================================================================
//  ClickableLabel: PRIVATE Class
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Constructors/Destructors
// =============================================================================
QfClickableLabel::QfClickableLabel (QWidget *parent, Qt::WindowFlags f)
	: QLabel(parent, f)
{
}

QfClickableLabel::QfClickableLabel (const QString& text, QWidget *parent, Qt::WindowFlags f)
	: QLabel(text, parent, f)
{
}

QfClickableLabel::~QfClickableLabel() {
}

// =============================================================================
//  ClickableLabel: PUBLIC Methods
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Get Properties
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Set Properties
// =============================================================================

// =============================================================================
//  ClickableLabel: PUBLIC Slots
// =============================================================================

// =============================================================================
//  ClickableLabel: PROTECTED Methods
// =============================================================================
void QfClickableLabel::mouseReleaseEvent (QMouseEvent *event) {
	QLabel::mouseReleaseEvent(event);

	// Accept Event
	event->setAccepted(true);

	// Raise Clicked Event
	emit clicked();
	emit clicked(this);	
}

// =============================================================================
//  ClickableLabel: PRIVATE Slots
// =============================================================================

// =============================================================================
//  ClickableLabel: PRIVATE Methods
// =============================================================================

