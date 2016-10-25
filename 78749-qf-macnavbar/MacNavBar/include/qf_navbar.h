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

#ifndef _QF_NAVBAR_H_
#define _QF_NAVBAR_H_

// Qt4 Headers
#include <QWidget>

// Quartica Headers
#include <QfNavBarGroup>
#include <QfUiExport>

class QFUI_EXPORT QfNavBar : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)
	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

	public:
		QfNavBar (QWidget *parent = 0);
		~QfNavBar();

		// Methods - Groups Related
		void addGroup (QfNavBarGroup *group);
		QfNavBarGroup *addGroup (const QString& title);

		bool containsGroup (QfNavBarGroup *group);

		// Methods - Items Related
		void addItem (QfNavBarGroup *group, QfNavBarItem *item);
		QfNavBarItem *addItem (QfNavBarGroup *group, const QString& text);
		QfNavBarItem *addItem (QfNavBarGroup *group, const QPixmap& icon, const QString& text);

		// GET Properties
		QColor selectionColor (void) const;
		QColor backgroundColor (void) const;

		// SET Properties
		void setBackgroundColor (const QColor& bgColor);
		void setSelectionColor (const QColor& selectionColor);

	protected:
		void paintEvent (QPaintEvent *event);

	private slots:
		void onGroupExpanded (QfNavBarGroup *group);
		void onItemSelected (QfNavBarGroup *group, QfNavBarItem *item);

	private:
		class Private;
		Private *d;
};

#endif // !_QF_NAVBAR_H_

