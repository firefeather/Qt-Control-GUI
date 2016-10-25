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

// Quartica UI Headers
#include "qf_selectablewidget.h"

// =============================================================================
//  SelectableWidget: PRIVATE Class
// =============================================================================
class QfSelectableWidget::Private {
	public:
		bool isSelected;
};

// =============================================================================
//  SelectableWidget: PUBLIC Constructors/Destructors
// =============================================================================
QfSelectableWidget::QfSelectableWidget (QWidget *parent)
	: QWidget(parent), d(new QfSelectableWidget::Private)
{
	d->isSelected = false;
}

QfSelectableWidget::~QfSelectableWidget() {
	delete d;
	d = NULL;
}

// =============================================================================
//  SelectableWidget: PUBLIC Methods
// =============================================================================

// =============================================================================
//  SelectableWidget: PUBLIC Get Properties
// =============================================================================
bool QfSelectableWidget::isSelected (void) const {
	return(d->isSelected);
}

// =============================================================================
//  SelectableWidget: PUBLIC Set Properties
// =============================================================================

// =============================================================================
//  SelectableWidget: PUBLIC Slots
// =============================================================================
void QfSelectableWidget::select (void) {
	select(true);
}

void QfSelectableWidget::select (bool isSelected) {
	if (d->isSelected == isSelected) return;

	d->isSelected = isSelected;

	// if is selected raise event
	if (isSelected) {
		emit selected();
		emit selected(this);
	}

	update();
}

void QfSelectableWidget::unselect (void) {
	select(false);
}

// =============================================================================
//  SelectableWidget: PROTECTED Methods
// =============================================================================
void QfSelectableWidget::mouseReleaseEvent (QMouseEvent *event) {
	QWidget::mouseReleaseEvent(event);

	// Left Click, Select/Unselect Item
	if (event->button() == Qt::LeftButton)
		select(!d->isSelected);
}

// =============================================================================
//  SelectableWidget: PRIVATE Slots
// =============================================================================

// =============================================================================
//  SelectableWidget: PRIVATE Methods
// =============================================================================

