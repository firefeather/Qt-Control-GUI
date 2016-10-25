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
#include <QVBoxLayout>
#include <QLabel>

// Quartica Headers
#include "qf_clickablelabel.h"
#include "qf_navbargroup.h"

// =============================================================================
//  NavBarGroup: PRIVATE Class
// =============================================================================
class QfNavBarGroup::Private {
	public:
		QList<QfNavBarItem *> listItems;
		QfClickableLabel *labelTitle;
		QVBoxLayout *layout;
		bool isExpanded;

	public:
		void initialize (QfNavBarGroup *group);
};

void QfNavBarGroup::Private::initialize (QfNavBarGroup *group) {
	// Initialize Members
	layout = new QVBoxLayout;
	labelTitle = new QfClickableLabel;

	// Setup Expanded Flag
	isExpanded = true;

	// Add Layout Items
	layout->addWidget(labelTitle);

	// Setup Label Title
	QFont font = labelTitle->font();
	font.setBold(true);
	font.setPixelSize(10);
	labelTitle->setFont(font);
	labelTitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	// Setup Layout
	layout->setSpacing(1);
	layout->setContentsMargins(0, 0, 0, 0);
	group->setLayout(layout);
}

// =============================================================================
//  NavBarGroup: PUBLIC Constructors/Destructors
// =============================================================================
QfNavBarGroup::QfNavBarGroup (QWidget *parent)
	: QWidget(parent), d(new QfNavBarGroup::Private)
{
	d->initialize(this);

	// Add Events
	connect(d->labelTitle, SIGNAL(clicked()), this, SLOT(onTitleClicked()));
}

QfNavBarGroup::QfNavBarGroup (const QString& title, QWidget *parent)
	: QWidget(parent), d(new QfNavBarGroup::Private)
{
	d->initialize(this);
	d->labelTitle->setText(title);

	// Add Events
	connect(d->labelTitle, SIGNAL(clicked()), this, SLOT(onTitleClicked()));
}

QfNavBarGroup::~QfNavBarGroup() {
	delete d;
	d = NULL;
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods - Add Item
// =============================================================================
void QfNavBarGroup::addItem (QfNavBarItem *item) {
	// Insert Item Space
	item->insertSpacing(0, 10);

	// Insert Item into Item List
	d->listItems.append(item);

	// Add Item to Layout
	d->layout->addWidget(item);

	// Add Event
	connect(item, SIGNAL(selected(QfSelectableWidget *)), 
			this, SLOT(onItemSelected(QfSelectableWidget *)));
}

void QfNavBarGroup::addItem (QfNavBarItem *item, int index) {
	// Insert Item Space
	item->insertSpacing(0, 20);

	// Insert Item into Item List
	d->listItems.insert(index, item);

	// Add Item to Layout
	d->layout->insertWidget(index, item);

	// Add Event
	connect(item, SIGNAL(selected(QfSelectableWidget *)), 
			this, SLOT(onItemSelected(QfSelectableWidget *)));
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods - Create and Add Item
// =============================================================================
QfNavBarItem *QfNavBarGroup::addItem (const QString& text) {
	QfNavBarItem *item = new QfNavBarItem(text);
	addItem(item);
	return(item);
}

QfNavBarItem *QfNavBarGroup::addItem (const QPixmap& icon, const QString& text) {
	QfNavBarItem *item = new QfNavBarItem(icon, text);
	addItem(item);
	return(item);
}

QfNavBarItem *QfNavBarGroup::addItem (const QPixmap& icon, const QString& text, int index) {
	QfNavBarItem *item = new QfNavBarItem(icon, text);
	addItem(item, index);	
	return(item);
}

// =============================================================================
//  NavBarGroup: PUBLIC Methods
// =============================================================================
bool QfNavBarGroup::containsItem (QfNavBarItem *item) {
	return(d->listItems.contains(item));
}

// =============================================================================
//  NavBarGroup: PUBLIC Get Properties
// =============================================================================
QString QfNavBarGroup::title (void) const {
	return(d->labelTitle->text());
}

bool QfNavBarGroup::isExpanded (void) const {
	return(d->isExpanded);
}

// =============================================================================
//  NavBarGroup: PUBLIC Set Properties
// =============================================================================
void QfNavBarGroup::setTitle (const QString& title) {
	d->labelTitle->setText(title);
}

void QfNavBarGroup::setTitleColor (const QColor& color) {
	QPalette palette = d->labelTitle->palette();
	palette.setColor(QPalette::WindowText, color);
	d->labelTitle->setPalette(palette);
}

// =============================================================================
//  NavBarGroup: PUBLIC Slots
// =============================================================================
void QfNavBarGroup::expand (bool expand) {
	if (d->isExpanded == expand) return;

	if (expand) {
		foreach (QfNavBarItem *item, d->listItems) {
			d->layout->addWidget(item);
			item->show();
		}
	} else {
		foreach (QfNavBarItem *item, d->listItems) {
			d->layout->removeWidget(item);
			item->hide();
		}
	}

	d->isExpanded = expand;

	// Raise Expanded Event
	emit expanded(this);
}

// =============================================================================
//  NavBarGroup: PROTECTED Methods
// =============================================================================

// =============================================================================
//  NavBarGroup: PRIVATE Slots
// =============================================================================
void QfNavBarGroup::onItemSelected (QfSelectableWidget *item) {
	emit selected(this, (QfNavBarItem *) item);
}

void QfNavBarGroup::onTitleClicked (void) {
	expand(!d->isExpanded);
}

// =============================================================================
//  NavBarGroup: PRIVATE Methods
// =============================================================================

