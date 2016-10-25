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
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QPainter>

// Quartica Headers
#include "qf_navbar.h"

// =============================================================================
//  NavBar: PRIVATE Class
// =============================================================================
class QfNavBar::Private {
	public:
		QVBoxLayout *layout;

		// Colors
		QColor colorBackground;
		QColor colorSelection;

		// Current Selection
		QfNavBarGroup *groupSelected;
		QfNavBarItem *itemSelected;
};

// =============================================================================
//  NavBar: PUBLIC Constructors/Destructors
// =============================================================================
QfNavBar::QfNavBar (QWidget *parent)
	: QWidget(parent), d(new QfNavBar::Private)
{
	// Initialize Members
	d->layout = new QVBoxLayout;
	d->itemSelected = NULL;
	d->groupSelected = NULL;

	// Setup Colors
	d->colorBackground = QColor(0xdf, 0xe4, 0xea);
	d->colorSelection = QColor(0xa6, 0xa6, 0xa6);

	// Setup Layout
	d->layout->addStretch(2);
	setLayout(d->layout);
}

QfNavBar::~QfNavBar() {
	delete d;
	d = NULL;
}

// =============================================================================
//  NavBar: PUBLIC Methods - Groups Related
// =============================================================================
void QfNavBar::addGroup (QfNavBarGroup *group) {
	// Set Font
	group->setTitleColor(QColor(0x65, 0x71, 0x80));

	// Add Events
	connect(group, SIGNAL(selected(QfNavBarGroup *, QfNavBarItem *)), 
			this, SLOT(onItemSelected(QfNavBarGroup *, QfNavBarItem *)));

	// Add to Layout
	d->layout->insertWidget(d->layout->count() - 1, group);
}

QfNavBarGroup *QfNavBar::addGroup (const QString& title) {
	QfNavBarGroup *group = new QfNavBarGroup(title);
	addGroup(group);
	return(group);
}

bool QfNavBar::containsGroup (QfNavBarGroup *group) {
	int itemCount = d->layout->count();

	for (int i = 0; i < itemCount; ++i) {
		QWidget *widget = d->layout->itemAt(i)->widget();
		if (widget == group) return(true);
	}
	
	return(false);
}

// =============================================================================
//  NavBar: PUBLIC Methods - Items Related
// =============================================================================
void QfNavBar::addItem (QfNavBarGroup *group, QfNavBarItem *item) {
	group->addItem(item);
}

QfNavBarItem *QfNavBar::addItem (QfNavBarGroup *group, const QString& text) {
	return(group->addItem(text));
}

QfNavBarItem *QfNavBar::addItem (QfNavBarGroup *group, const QPixmap& icon, const QString& text) {
	return(group->addItem(icon, text));
}


// =============================================================================
//  NavBar: PUBLIC Get Properties
// =============================================================================
QColor QfNavBar::backgroundColor (void) const {
	return(d->colorBackground);
}

QColor QfNavBar::selectionColor (void) const {
	return(d->colorSelection);
}

// =============================================================================
//  NavBar: PUBLIC Set Properties
// =============================================================================
void QfNavBar::setBackgroundColor (const QColor& bgColor) {
	d->colorBackground = bgColor;
}

void QfNavBar::setSelectionColor (const QColor& selectionColor) {
	d->colorSelection = selectionColor;
}

// =============================================================================
//  NavBar: PUBLIC Slots
// =============================================================================

// =============================================================================
//  NavBar: PROTECTED Methods
// =============================================================================
void QfNavBar::paintEvent (QPaintEvent *event) {
	QWidget::paintEvent(event);

	// Recall Update() if painter area is not complete!
	if (event->rect().x() > 0 || event->rect().y() > 0)
		update();

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.fillRect(event->rect(), d->colorBackground);

	// Draw Selection
	if (d->groupSelected != NULL && d->groupSelected->isExpanded() && d->itemSelected != NULL) {
		QPoint pos = d->groupSelected->pos() + d->itemSelected->pos();
		int width = geometry().width();

		int r = d->colorSelection.red();
		int g = d->colorSelection.green();
		int b = d->colorSelection.blue();

		p.fillRect(0, pos.y() - 1, width, 1, QColor(r - 0x26, g - 0x26, b - 0x26));

		QLinearGradient linearGrad(QPointF(0, pos.y()), QPointF(0, pos.y() + d->itemSelected->height()));
		linearGrad.setColorAt(0, d->colorSelection);
		linearGrad.setColorAt(1, QColor(r - 0x3b, g - 0x3b, b - 0x3b));
		p.fillRect(0, pos.y(), width, d->itemSelected->height(), linearGrad);
	}

	p.end();
}

// =============================================================================
//  NavBar: PRIVATE Slots
// =============================================================================
void QfNavBar::onItemSelected (QfNavBarGroup *group, QfNavBarItem *item) {
	if (d->itemSelected != NULL && d->itemSelected != item) {
		d->itemSelected->setFont(item->font());
		d->itemSelected->setTextColor(Qt::black);
		d->itemSelected->unselect();
	}

	d->groupSelected = group;
	d->itemSelected = item;

	QFont font = d->itemSelected->font();
	font.setPixelSize(9);
	font.setBold(true);
	d->itemSelected->setFont(font);
	d->itemSelected->setTextColor(Qt::white);

	update();
}

void QfNavBar::onGroupExpanded (QfNavBarGroup *group) {
	Q_UNUSED(group)

	update();
}

// =============================================================================
//  NavBar: PRIVATE Methods
// =============================================================================


