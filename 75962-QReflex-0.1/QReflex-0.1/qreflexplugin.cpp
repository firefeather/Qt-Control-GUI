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

#include "qreflex.h"
#include "qreflexplugin.h"

#include <QtPlugin>

QReflexPlugin::QReflexPlugin(QObject *parent)
        : QObject(parent)
{
    initialized = false;
}

void QReflexPlugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized)
        return;

    initialized = true;
}

bool QReflexPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QReflexPlugin::createWidget(QWidget *parent)
{
    return new QReflex(parent);
}

QString QReflexPlugin::name() const
{
    return "QReflex";
}

QString QReflexPlugin::group() const
{
    return "Lab Widgets";
}

QIcon QReflexPlugin::icon() const
{
    return QIcon(":qreflex.png");
}

QString QReflexPlugin::toolTip() const
{
    return "";
}

QString QReflexPlugin::whatsThis() const
{
    return "";
}

bool QReflexPlugin::isContainer() const
{
    return false;
}

QString QReflexPlugin::domXml() const
{
    return "<widget class=\"QReflex\" name=\"qreflex\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>180</width>\n"
           "   <height>80</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string>Reflex text</string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>Reflex text</string>\n"
           " </property>\n"
           " </widget>\n";
}

QString QReflexPlugin::includeFile() const
{
    return "qreflex.h";
}

Q_EXPORT_PLUGIN2(customwidgetplugin, QReflexPlugin)
