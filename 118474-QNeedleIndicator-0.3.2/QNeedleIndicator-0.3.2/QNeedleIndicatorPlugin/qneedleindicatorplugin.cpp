/***************************************************************************
 *   Copyright (C) 2010 by Tn                                              *
 *   thenobody@poczta.fm                                                   *
 *                                                                         *
 *   Foobar is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of        *
 *   the License, or (at your option) any later version.                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                         *
 ***************************************************************************/

#include "../qneedleindicator.h"
#include "qneedleindicatorplugin.h"

 #include <QtPlugin>

 QNeedleIndicatorPlugin::QNeedleIndicatorPlugin(QObject *parent) : QObject(parent)
 {
     initialized = false;
 }

 void QNeedleIndicatorPlugin::initialize(QDesignerFormEditorInterface * /* core */)
 {
     if (initialized)
         return;

     initialized = true;
 }

 bool QNeedleIndicatorPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *QNeedleIndicatorPlugin::createWidget(QWidget *parent)
 {
     return new QNeedleIndicator(parent);
 }

 QString QNeedleIndicatorPlugin::name() const
 {
     return "QNeedleIndicator";
 }

 QString QNeedleIndicatorPlugin::group() const
 {
     return "Tn's Indicators";
 }

 QIcon QNeedleIndicatorPlugin::icon() const
 {
     return QIcon();
 }

 QString QNeedleIndicatorPlugin::toolTip() const
 {
     return "";
 }

 QString QNeedleIndicatorPlugin::whatsThis() const
 {
     return "";
 }

 bool QNeedleIndicatorPlugin::isContainer() const
 {
     return false;
 }

 QString QNeedleIndicatorPlugin::domXml() const
 {
     return "<widget class=\"QNeedleIndicator\" name=\"needleIndicator\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>100</width>\n"
            "   <height>100</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>Needle indicator</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>Indicator displays qreal number</string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString QNeedleIndicatorPlugin::includeFile() const
 {
     return "qneedleindicator.h";
 }

 Q_EXPORT_PLUGIN2(customwidgetplugin, QNeedleIndicatorPlugin)
