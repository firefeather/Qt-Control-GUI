/*
 * DataBrowser data browsing widget and filtering engine
 * Copyright (C) 2004-2008 by Gordos Kund / QnD Co Bt.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact gordos.kund@gmail.com with any questions on this license.
 */

#include "twdatazoomplugin.h"
#include "twdatazoom.h"
#include <QtPlugin>

TWDataZoomPlugin::TWDataZoomPlugin(QObject *parent) : QObject(parent)
{
    initialized = false;
}

/******************************************************/

void TWDataZoomPlugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized)
        return;
    initialized = true;
}

/******************************************************/

bool TWDataZoomPlugin::isInitialized() const
{
    return initialized;
}

/******************************************************/


QWidget *TWDataZoomPlugin::createWidget(QWidget *parent)
{
    return new TWDataZoom(parent);
}

/******************************************************/

QString TWDataZoomPlugin::name() const
{
    return "TWDataZoom";
}

/******************************************************/

QString TWDataZoomPlugin::group() const
{
    return "Thetyss Plugins";
}

/******************************************************/

QIcon TWDataZoomPlugin::icon() const
{
    return QIcon();
}

/******************************************************/

QString TWDataZoomPlugin::toolTip() const
{
    return "";
}

/******************************************************/

QString TWDataZoomPlugin::whatsThis() const
{
    return "";
}

/******************************************************/

bool TWDataZoomPlugin::isContainer() const
{
    return false;
}

/******************************************************/
QString TWDataZoomPlugin::domXml() const
{
        return "<widget class=\"TWDataZoom\" name=\"twDataZoom\">\n"
               " <property name=\"geometry\">\n"
               "  <rect>\n"
               "   <x>0</x>\n"
               "   <y>0</y>\n"
               "   <width>400</width>\n"
               "   <height>400</height>\n"
               "  </rect>\n"
	       " </property>\n"
	       " <property name=\"toolTip\" >\n"
	       "  <string></string>\n"
	       " </property>\n"
	       " <property name=\"whatsThis\" >\n"
	       "  <string></string>\n"
	       " </property>\n"
	       "</widget>\n";
}

/******************************************************/

QString TWDataZoomPlugin::includeFile() const
{
    return "twdatazoom.h";
}

/******************************************************/

QString TWDataZoomPlugin::codeTemplate() const
{
    return "";
}
/******************************************************/

Q_EXPORT_PLUGIN2(twdatazoomplugin, TWDataZoomPlugin)
