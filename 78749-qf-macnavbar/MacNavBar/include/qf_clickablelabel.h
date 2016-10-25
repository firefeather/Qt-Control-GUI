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

#ifndef _QF_CLICKABLELABEL_H_
#define _QF_CLICKABLELABEL_H_

// Qt4 Headers
#include <QLabel>

// Quartica Headers
#include <QfUiExport>

class QFUI_EXPORT QfClickableLabel : public QLabel {
	Q_OBJECT

	public:
		QfClickableLabel (QWidget *parent = 0, Qt::WindowFlags f = 0);
		QfClickableLabel (const QString& text, QWidget *parent = 0, Qt::WindowFlags f = 0);
		~QfClickableLabel();

	signals:
		void clicked (void);
		void clicked (QfClickableLabel *label);

	protected:
		void mouseReleaseEvent (QMouseEvent *event);
};

#endif // !_QF_CLICKABLELABEL_H_

