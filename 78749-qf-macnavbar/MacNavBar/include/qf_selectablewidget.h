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

#ifndef _QF_SELECTABLE_WIDGET_H_
#define _QF_SELECTABLE_WIDGET_H_

// Qt4 Headers
#include <QWidget>

// Quartica Headers
#include <QfUiExport>

class QFUI_EXPORT QfSelectableWidget : public QWidget {
	Q_OBJECT

	Q_PROPERTY(bool isSelected READ isSelected WRITE select)

	public:
		QfSelectableWidget (QWidget *parent = 0);
		~QfSelectableWidget();

		bool isSelected (void) const;

	signals:
		void selected (void);
		void selected (QfSelectableWidget *widget);

	public slots:
		void select (void);
		void select (bool isSelected);
		void unselect (void);

	protected:
		void mouseReleaseEvent (QMouseEvent *event);

	private:
		class Private;
		Private *d;
};

#endif // !_QF_SELECTABLE_WIDGET_H_

