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
#include "qirdir_helper.h"
#include <QStringList>
#include <QMap>
#include <QDir>
#include <QObject>
#include <QSysInfo>

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
#include <QSettings>
#include <QLibrary>
#include "../Common/qirwindows.h"
typedef BOOL (WINAPI *PtrGetVolumeInformation)(LPCTSTR lpRootPathName,
                                               LPTSTR lpVolumeNameBuffer,
                                               DWORD nVolumeNameSize,
                                               LPDWORD lpVolumeSerialNumber,
                                               LPDWORD lpMaximumComponentLength,
                                               LPDWORD lpFileSystemFlags,
                                               LPTSTR lpFileSystemNameBuffer,
                                               DWORD nFileSystemNameSize);

static PtrGetVolumeInformation pGetVolumeInformation = 0;
#endif 

////////////////
//QIrDirHelper
////////////////
QMap< QIrDirHelper::SpecialFolder, QString > QIrDirHelper::m_specialFolderMap = QMap< QIrDirHelper::SpecialFolder, QString >();
bool QIrDirHelper::m_initialized = false;

void QIrDirHelper::init()
{
    QString homePath = QDir::homePath(), folder;

    m_specialFolderMap.insert(QIrDirHelper::Home,homePath);
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)	
    QMap< SpecialFolder, QString > registerKeys;

    registerKeys.insert(QIrDirHelper::Desktop,"Desktop");
    registerKeys.insert(QIrDirHelper::Documents,"Personal");
    registerKeys.insert(QIrDirHelper::Music,"My Music");
    registerKeys.insert(QIrDirHelper::Pictures,"My Pictures");
    registerKeys.insert(QIrDirHelper::Video,"My Video");

    QSettings settings(QSettings::UserScope,"Microsoft","Windows");

    foreach (QIrDirHelper::SpecialFolder key, registerKeys.keys()) {
        folder = registerKeys.value(key);
        folder = settings.value(QString("CurrentVersion/Explorer/User Shell Folders/") + folder).toString().replace("%USERPROFILE%",homePath);
        m_specialFolderMap.insert(key,folder);
    }
#else 
    m_specialFolderMap.insert(QIrDirHelper::Desktop,homePath + QDir::separator() + "Desktop");
    m_specialFolderMap.insert(QIrDirHelper::Documents,homePath + QDir::separator() + "Documents");
    m_specialFolderMap.insert(QIrDirHelper::Music,homePath + QDir::separator() + "Music");
    m_specialFolderMap.insert(QIrDirHelper::Pictures,homePath + QDir::separator() + "Pictures");
#if defined(Q_OS_MAC)
    m_specialFolderMap.insert(QIrDirHelper::Video,homePath + QDir::separator() + "Movies");
#else
    m_specialFolderMap.insert(QIrDirHelper::Video,homePath + QDir::separator() + "Videos");
#endif // Q_OS_MAC
#endif // Q_OS_WIN

    m_initialized = true;
}

QStringList QIrDirHelper::splitPath( const QString & path )
{
    QString pathCopy = QDir::toNativeSeparators(path);
    QString sep = QDir::separator();

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    if (pathCopy == QLatin1String("\\") || pathCopy == QLatin1String("\\\\"))
        return QStringList(pathCopy);
    QString doubleSlash(QLatin1String("\\\\"));
    if (pathCopy.startsWith(doubleSlash))
        pathCopy = pathCopy.mid(2);
    else
        doubleSlash.clear();
#endif

    QRegExp re(QLatin1String("[") + QRegExp::escape(sep) + QLatin1String("]"));
    QStringList parts = pathCopy.split(re);

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    if (!doubleSlash.isEmpty())
        parts[0].prepend(doubleSlash);
#else
    if (pathCopy[0] == sep[0]) // read the "/" at the beginning as the split removed it
        parts[0] = QDir::fromNativeSeparators(QString(sep[0]));
#endif
    parts.removeAll("");
    return parts;
}

QString QIrDirHelper::setupPath(const QStringList & list, int index)
{
    QString str;
    int startIndex = 0;

    if ( list.isEmpty() )
        return QString();
    str = list[0];
    startIndex = 1;
    for (int i = startIndex; i <= index; i++) {
        str += QDir::separator() + list[i];
    }
    return str;
}

QString QIrDirHelper::specialFolder( QIrDirHelper::SpecialFolder folder )
{
    if ( folder < QIrDirHelper::Home || folder > QIrDirHelper::Video ) {
        qWarning("QIrDirHelper::specialDirPath(): Invalid special directory.");
        return QString();
    }
    if ( !m_initialized )
        init();
    return m_specialFolderMap[folder];
}
QString QIrDirHelper::myComputer()
{
#if defined(Q_OS_WIN)
    if ( QSysInfo::windowsVersion() < QSysInfo::WV_VISTA )
        return QObject::tr("My Computer");
    else
        return QObject::tr("Computer");
#else
    return QObject::tr("Computer");
#endif
}

QString QIrDirHelper::driveLabel( const QString & drive )
{
#if defined(Q_OS_WIN) && !(Q_OS_WINCE)
    QString name;
    TCHAR volumeName[MAX_PATH + 1] = { 0 };
    TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;
    QString  d = QDir::toNativeSeparators(drive) + QDir::separator();
    LPCTSTR dr = (const TCHAR *) QStringToTCHAR(d);
    static bool tried = false;

    if ( !tried ) {
        tried = true;
        QLibrary lib(QString::fromAscii("Kernel32"));

        if ( !lib.isLoaded() ) {
            lib.load();
        }
        pGetVolumeInformation = (PtrGetVolumeInformation)lib.resolve("GetVolumeInformationW");
    }
    if ( pGetVolumeInformation  ) {
        pGetVolumeInformation(dr,
                              volumeName,
                              MAX_PATH + 1,
                              &serialNumber,
                              &maxComponentLen,
                              &fileSystemFlags,
                              fileSystemName,
                              MAX_PATH + 1);
        name = TCHARToQString(volumeName);
    }
    d = d.remove(QDir::separator());
    if ( name.isEmpty() )
        name = d;
    else
        name = name + QString(" (%1)").arg(d);

    return name;
#else
    return QObject::tr("File System (%1)").arg(drive);
#endif
}
