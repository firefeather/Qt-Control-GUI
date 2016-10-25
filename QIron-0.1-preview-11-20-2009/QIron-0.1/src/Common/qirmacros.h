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
/*!
  \brief This file contains some macros used by QIron classes.
  \file qirmacros.h
  \author Dzimi Mve Alexandre
  \date 07/25/2009
*/
#ifndef QIRMACROS_H
#define QIRMACROS_H

#include <QtGlobal>
#define QIRON_DECLARE_FLAGS Q_DECLARE_FLAGS

#define QIR_DECLARE_UI( Class ) \
inline Class##Ui * ui_func() { return reinterpret_cast< Class##Ui * > (ui_ptr); } \
inline const Class##Ui * ui_func() const { return reinterpret_cast< const Class##Ui * > (ui_ptr); } \
friend class Class##Ui;

#define QIR_DECLARE_WIDGET( Class ) \
inline Class * w_func() { return static_cast< Class * > (w_ptr); } \
inline const Class * w_func() const { return static_cast< const Class * > (w_ptr); } \
friend class Class;


#define QIR_UI( Class ) Class##Ui * ui = ui_func()
#define QIR_W( Class ) Class * w = w_func()

#define QIR_SINGLETON( Class ) \
Class(); \
public:\
static Class * instance() { \
if ( !m_instance ) { static QMutex mutex; QMutexLocker lock(&mutex); if( !m_instance ) m_instance = new Class();} return m_instance;} \
private: static Class * m_instance;

#endif

