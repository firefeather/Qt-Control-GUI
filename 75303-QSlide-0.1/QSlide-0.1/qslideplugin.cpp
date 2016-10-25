/***************************************************************************
 *   Copyright (C) 2008 - Giuseppe Cigala                                  *
 *   g_cigala@virgilio.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "qslide.h"
#include "qslideplugin.h"

#include <QtPlugin>

QSlidePlugin::QSlidePlugin(QObject *parent)
        : QObject(parent)
{
    initialized = false;
}

void QSlidePlugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized)
        return;

    initialized = true;
}

bool QSlidePlugin::isInitialized() const
{
    return initialized;
}

QWidget *QSlidePlugin::createWidget(QWidget *parent)
{
    return new QSlide(parent);
}

QString QSlidePlugin::name() const
{
    return "QSlide";
}

QString QSlidePlugin::group() const
{
    return "Lab Widgets";
}

QIcon QSlidePlugin::icon() const
{
    return QIcon(":qslide.png");
}

QString QSlidePlugin::toolTip() const
{
    return "";
}

QString QSlidePlugin::whatsThis() const
{
    return "";
}

bool QSlidePlugin::isContainer() const
{
    return false;
}

QString QSlidePlugin::domXml() const
{
    return "<widget class=\"QSlide\" name=\"qslide\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>300</width>\n"
           "   <height>30</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string>Sliding text</string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>Sliding text</string>\n"
           " </property>\n"
           " </widget>\n";
}

QString QSlidePlugin::includeFile() const
{
    return "qslide.h";
}

Q_EXPORT_PLUGIN2(customwidgetplugin, QSlidePlugin)
