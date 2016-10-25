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

#ifndef _QF_NAVBARITEM_H_
#define _QF_NAVBARITEM_H_

// Qt4 Headers
#include <QWidget>

// Quartica Headers
#include <QfSelectableWidget>
#include <QfUiExport>

class QFUI_EXPORT QfNavBarItem : public QfSelectableWidget {
	Q_OBJECT

	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QPixmap icon READ icon WRITE setIcon)

	public:
		QfNavBarItem (QWidget *parent = 0);
		QfNavBarItem (const QString& text, QWidget *parent = 0);
		QfNavBarItem (const QPixmap& icon, const QString& text, QWidget *parent = 0);
		~QfNavBarItem();

		// Methods
		void addSpacing (int size);
		void addWidget (QWidget *widget, int stretch = 0);

		void insertSpacing (int index, int size);
		void insertWidget (int index, QWidget *widget, int stretch = 0);

		// GET Properties
		QString text (void) const;
		const QPixmap *icon (void) const;

		// SET Properties
		void setIcon (const QPixmap& icon);
		void setText (const QString& text);
		void setTextColor (const QColor& color);

	private:
		class Private;
		Private *d;
};

#endif // !_QF_NAVBARITEM_H_

