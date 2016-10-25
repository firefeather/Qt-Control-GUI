/********************************************************************** 
**
** This file is part of QIron Toolkit.
**
** Copyright (C) 2009-2020 Dzimi Mve Alexandre <dzimiwine@gmail.com>
** 
** Contact: dzimiwine@gmail.com
** 
** QIron is a free toolkit developed in Qt by Dzimi Mve A.; you can redistribute 
** sources and libraries of this library and/or modify them under the terms of 
** the GNU Library General Public License version 3.0 as published by the 
** Free Software Foundation and appearing in the file LICENSE.txt included in 
** the packaging of this file.
** Please review the following information to ensure the GNU General 
** Public License version 3.0 requirements will be met: 
** http://www.gnu.org/copyleft/gpl.html.
**
** This SDK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
**********************************************************************/
#ifndef QIRON_EXPORT_H
#define QIRON_EXPORT_H

#include "qirglobal.h"

#if defined(Q_OS_WIN)
	#if defined(QIRON_LIBRARY_EXPORT)
		#define QIRONSHARED_EXPORT Q_DECL_EXPORT
	#elif defined(QIRON_LIBRARY_IMPORT)
		# define QIRONSHARED_EXPORT Q_DECL_IMPORT
	#else
		#define QIRONSHARED_EXPORT
	#endif // QIRON_LIBRARY_EXPORT
#else
	#define QIRONSHARED_EXPORT
#endif // Q_OS_WIN

#endif // QIRON_EXPORT_H
