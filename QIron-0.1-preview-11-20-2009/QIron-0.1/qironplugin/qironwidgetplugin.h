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
#ifndef QIRONWIDGETPLUGIN_H
#define QIRONWIDGETPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class QIronWidgetPlugin : public QDesignerCustomWidgetInterface
{
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	QIronWidgetPlugin(const QString & name, const QString & includeFile, 
		const QIcon & icon = QIcon(), bool container = false);

	inline bool isContainer() const { return m_container; }
	inline bool isInitialized() const { return m_initialized; }
	inline QIcon icon() const { return m_icon; }
	inline QString group() const { return "QIron Widgets"; }
	inline QString includeFile() const { return m_include; }
	inline QString name() const { return m_name; }
	inline virtual QString toolTip() const { return QString(); }
	inline virtual QString whatsThis() const { return QString(); }
	virtual void initialize( QDesignerFormEditorInterface * core );
protected:
        void setInitialized( bool i ) { m_initialized = i; }
private:
	bool m_initialized;
        QString m_name;
        QString m_include;
        QIcon m_icon;
        bool m_container;
};

#endif // QIRONWIDGETPLUGIN_H
