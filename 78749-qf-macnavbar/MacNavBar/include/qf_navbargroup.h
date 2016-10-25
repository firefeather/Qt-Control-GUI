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

#ifndef _QF_NAVBARGROUP_H_
#define _QF_NAVBARGROUP_H_

// Qt4 Headers
#include <QWidget>

// Quartica Headers
#include <QfNavBarItem>
#include <QfUiExport>

class QFUI_EXPORT QfNavBarGroup : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(bool isExpanded READ isExpanded WRITE expand)

	public:
		QfNavBarGroup (QWidget *parent = 0);
		QfNavBarGroup (const QString& title, QWidget *parent = 0);
		~QfNavBarGroup();

		// Methods - Add Item
		void addItem (QfNavBarItem *item);
		void addItem (QfNavBarItem *item, int index);

		// Methods - Create and Add Item
		QfNavBarItem *addItem (const QString& text);
		QfNavBarItem *addItem (const QPixmap& icon, const QString& text);
		QfNavBarItem *addItem (const QPixmap& icon, const QString& text, int index);

		// Methods
		bool containsItem (QfNavBarItem *item);

		// GET Properties
		QString title (void) const;
		bool isExpanded (void) const;

		// SET Properties
		void setTitle (const QString& title);
		void setTitleColor (const QColor& color);

	signals:
		void selected (QfNavBarGroup *group, QfNavBarItem *item);
		void expanded (QfNavBarGroup *group);

	public slots:
		void expand (bool expand);

	private slots:
		void onItemSelected (QfSelectableWidget *item);
		void onTitleClicked (void);

	private:
		class Private;
		Private *d;
};

#endif // !_QF_NAVBARGROUP_H_

