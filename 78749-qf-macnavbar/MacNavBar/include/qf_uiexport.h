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

#ifndef _QFUI_EXPORT_H_
#define _QFUI_EXPORT_H_

#ifdef Q_OS_WIN
	#ifdef QFUI_MAKEDLL
		#define QFUI_EXPORT		Q_DECL_EXPORT 
	#else
		#define QFUI_EXPORT		Q_DECL_IMPORT 
	#endif
#else
	#define QFUI_EXPORT	
#endif


#ifdef QT_NO_DEBUG
	#ifdef QFUI_DEBUG
		#undef QFUI_DEBUG
	#endif
#elif defined(QFUI_DEBUG)
	#include <QDebug>

	#define QFUI_DEBUG_STRING	"QfUserInterface Debug:"
#endif

#endif // !_QFUI_EXPORT_H_

