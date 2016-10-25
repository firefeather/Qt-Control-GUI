/********************************************************************** 
**
** This file is part of QIron Toolkit.
**
** Copyright (C) 2009-2010 Dzimi Mve Alexandre <dzimiwine@gmail.com>
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
#ifndef QIRDESIGNPATTERNS_H
#define QIRDESIGNPATTERNS_H

#define QIR_DOUBLE_CHECKED_LOCKING( Class ) \
	Class(); \
public: \
	~Class(); \
	static Class * instance() { \
		if (!m_instance && canInstanciate()) { \
			QMutexLocker lock(&m_mutex); \
			if( !m_instance && canInstanciate()) \
				m_instance = new Class();\
		} \
		return m_instance;\
	} \
private: \
	static bool canInstanciate(); \
	static QMutex m_mutex; \
	static QIrScopedPointer< Class > m_instance;

#endif // QIRDESIGNPATTERNS_H