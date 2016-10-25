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
#include <QHBoxLayout>
#include <QLabel>

// Quartica Headers
#include "qf_navbaritem.h"

// =============================================================================
//  NavBarItem: PRIVATE Class
// =============================================================================
class QfNavBarItem::Private {
	public:
		QHBoxLayout *layout;
		QLabel *labelIcon;
		QLabel *labelText;

	public:
		void initialize (QfNavBarItem *item);
};

void QfNavBarItem::Private::initialize (QfNavBarItem *item) {
	// Initialize Members
	layout = new QHBoxLayout;
	labelIcon = new QLabel;
	labelText = new QLabel;

	// Setup Text Label
	labelText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	labelIcon->setFixedWidth(18);

	// Setup Font
	QFont font = labelText->font();
	font.setPixelSize(10);
	labelText->setFont(font);

	// Add Items to Layout
	layout->addWidget(labelIcon);
	layout->addWidget(labelText);

	// Setup Layout
	layout->setContentsMargins(1, 1, 1, 1);
	item->setLayout(layout);
}

// =============================================================================
//  NavBarItem: PUBLIC Constructors/Destructors
// =============================================================================
QfNavBarItem::QfNavBarItem (QWidget *parent)
	: QfSelectableWidget(parent), d(new QfNavBarItem::Private)
{
	d->initialize(this);
}

QfNavBarItem::QfNavBarItem (const QString& text, QWidget *parent)
	: QfSelectableWidget(parent), d(new QfNavBarItem::Private)
{
	d->initialize(this);
	d->labelText->setText(text);
}

QfNavBarItem::QfNavBarItem (const QPixmap& icon, const QString& text, QWidget *parent)
	: QfSelectableWidget(parent), d(new QfNavBarItem::Private)
{
	d->initialize(this);

	setIcon(icon);
	d->labelText->setText(text);
}

QfNavBarItem::~QfNavBarItem() {
	delete d;
	d = NULL;
}

// =============================================================================
//  NavBarItem: PUBLIC Methods
// =============================================================================
void QfNavBarItem::addSpacing (int size) {
	d->layout->addSpacing(size);
}

void QfNavBarItem::addWidget (QWidget *widget, int stretch) {
	d->layout->addWidget(widget, stretch);
}


void QfNavBarItem::insertSpacing (int index, int size) {
	d->layout->insertSpacing(index, size);
}

void QfNavBarItem::insertWidget (int index, QWidget *widget, int stretch) {
	d->layout->insertWidget(index, widget, stretch);
}

// =============================================================================
//  NavBarItem: PUBLIC Get Properties
// =============================================================================
QString QfNavBarItem::text (void) const {
	return(d->labelText->text());
}

const QPixmap *QfNavBarItem::icon (void) const {
	return(d->labelIcon->pixmap());
}

// =============================================================================
//  NavBarItem: PUBLIC Set Properties
// =============================================================================
void QfNavBarItem::setIcon (const QPixmap& icon) {
	if (icon.height() > 20)
		d->labelIcon->setPixmap(icon.scaledToHeight(20, Qt::SmoothTransformation));
	else
		d->labelIcon->setPixmap(icon);
}

void QfNavBarItem::setText (const QString& text) {
	d->labelText->setText(text);
}

void QfNavBarItem::setTextColor (const QColor& color) {
	QPalette palette = d->labelText->palette();
	palette.setColor(QPalette::WindowText, color);
	d->labelText->setPalette(palette);
}

// =============================================================================
//  NavBarItem: PUBLIC Slots
// =============================================================================

// =============================================================================
//  NavBarItem: PROTECTED Methods
// =============================================================================

// =============================================================================
//  NavBarItem: PRIVATE Slots
// =============================================================================

// =============================================================================
//  NavBarItem: PRIVATE Methods
// =============================================================================

