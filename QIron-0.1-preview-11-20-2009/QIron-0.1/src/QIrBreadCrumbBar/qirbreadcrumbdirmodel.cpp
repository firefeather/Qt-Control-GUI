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
#include <QAction>
#include <QMenu>
#include <QDir>
#include <QDirModel>
#include <QModelIndex>
#include <QLibrary>
#include <QFileIconProvider>
#include <QApplication>
#include <QFileInfo>
#include <QFile>
#include <QUrl>
#include <QMimeData>
#include <QFileSystemWatcher>
#include "qirbreadcrumbdirmodel.h"
#include "qirdir_helper.h"

QIR_BEGIN_NAMESPACE

        QDirModel * QIrBreadCrumbDirModel::m_dirModel = 0;
QIrBreadCrumbDirModelMenuManager * QIrBreadCrumbDirModel::m_menuManager = 0;

class QIrBreadCrumbDirModelMenuAction : public QAction 
{
public:
    QIrBreadCrumbDirModelMenuAction(const QIcon & icon, const QString & fileName, const QString & path, QObject * parent) :
            QAction(icon,fileName,parent) {
        setData(path);
    }
};

////////////////////////////
//QIrBreadCrumbDirModelMenu
///////////////////////////
class QIrBreadCrumbDirModelMenu : public QMenu
{
public:
    QIrBreadCrumbDirModelMenu() : QMenu(0), m_need(true) { }

    inline QIrAbstractBreadCrumbModel::Filters filter() const { return m_filter; }
    void setFilter( QIrAbstractBreadCrumbModel::Filters filter );
    inline bool needBeRebuilt() const { return m_need; }
    inline void setNeedBeRebuilt( bool need ) { m_need = need; }

    bool m_need;

private:
    QIrAbstractBreadCrumbModel::Filters m_filter;
};

void QIrBreadCrumbDirModelMenu::setFilter( QIrAbstractBreadCrumbModel::Filters filter )
{
    if ( m_filter == filter )
        return;
    m_filter = filter;
    m_need = true;
}

///////////////////////////////////
//QIrBreadCrumbDirModelMenuManager
///////////////////////////////////
QIrBreadCrumbDirModelMenuManager::QIrBreadCrumbDirModelMenuManager( QObject * parent ) : QObject(parent)
{
    m_watcher = new QFileSystemWatcher(this);
    m_mainMenu = new QMenu;
    m_driveMenu = new QMenu;
    connect(m_watcher,SIGNAL(directoryChanged( QString )),this,SLOT(handleDirectoryChanged( QString )));
}
QIrBreadCrumbDirModelMenuManager::~QIrBreadCrumbDirModelMenuManager()
{
    delete m_mainMenu;
    delete m_driveMenu;
    foreach (QString path, m_menuMapping.keys()) {
        delete m_menuMapping.take(path);
    }
}
QMenu * QIrBreadCrumbDirModelMenuManager::mainMenu() const 
{ 
    return m_mainMenu;
}
QMenu * QIrBreadCrumbDirModelMenuManager::driveMenu() const
{ 
    return m_driveMenu;
}
void QIrBreadCrumbDirModelMenuManager::registerPath(const QString & path )
{
    QString tempPath = path;

    if ( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    if ( m_watcher->directories().contains(tempPath) )
        return;
    m_watcher->addPath(tempPath);
    m_menuMapping[tempPath] = new QIrBreadCrumbDirModelMenu;
}
bool QIrBreadCrumbDirModelMenuManager::isRegisteredPath( const QString & path ) const
{
    QString tempPath = path;

    if ( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    return m_watcher->directories().contains(tempPath);
}

QIrBreadCrumbDirModelMenu * QIrBreadCrumbDirModelMenuManager::menu( const QString & path) const
{
    QString tempPath = path;

    if ( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    if ( !isRegisteredPath(tempPath) )
        return 0;
    return m_menuMapping[tempPath];
}
void QIrBreadCrumbDirModelMenuManager::clean()
{
    foreach (QString path, m_watcher->directories()) {
        QDir dir(path);

        if ( !dir.exists() ) {
            m_watcher->removePath(path);
            delete m_menuMapping.take(path);
        }
    }
}
void QIrBreadCrumbDirModelMenuManager::handleDirectoryChanged( const QString & path )
{
    m_menuMapping[path]->setNeedBeRebuilt(true);
}


/////////////////////////
//QIrBreadCrumbDirModel
////////////////////////
QIrBreadCrumbDirModel::QIrBreadCrumbDirModel()
{
    if ( !m_dirModel )
        m_dirModel = new QDirModel(qApp);
    if ( !m_menuManager )
        m_menuManager = new QIrBreadCrumbDirModelMenuManager(qApp);
    setItemModel(m_dirModel);
    setFilter(0);
}
QIrBreadCrumbDirModel::~QIrBreadCrumbDirModel()
{
}
QString QIrBreadCrumbDirModel::defaultPath() const
{
    return QDir::homePath();
}
QString QIrBreadCrumbDirModel::cleanPath( const QString & path ) const
{
    makeCurrent();
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    QDir dir(QDir::cleanPath(path));
    QString tempPath;
    if ( dir.exists() ) {
        tempPath = QDir::toNativeSeparators(dir.absolutePath());
        if ( !tempPath.endsWith(QDir::separator()) )
            tempPath += QDir::separator();
        return tempPath;
    }
#endif
    return QDir::toNativeSeparators(QDir::cleanPath(path));
}
bool QIrBreadCrumbDirModel::isValid( const QString & path ) const
{
    bool valid;

    makeCurrent();
    if ( path == QIrDirHelper::myComputer() )
        return true;
    QModelIndex index = m_dirModel->index(cleanPath(path));
    valid = index.isValid();
    return valid;
}
QIrBreadCrumbModelNodeList QIrBreadCrumbDirModel::splitPath( const QString & path ) const
{

    QStringList list = QIrDirHelper::splitPath(cleanPath(path));
    QString tempPath;
    QIrBreadCrumbModelNodeList nodeList;
    bool isDir;

    makeCurrent();
    if ( path == QIrDirHelper::myComputer() ) {
        nodeList.append(QIrBreadCrumbModelNode(path,QIrBreadCrumbModelNode::Global,this));
        return nodeList;
    }
    isDir = m_dirModel->isDir(m_dirModel->index(path));
    nodeList.append(QIrBreadCrumbModelNode(QIrDirHelper::myComputer(),QIrBreadCrumbModelNode::Global,this));
    for (int i = 0; i < list.count(); i++) {
        tempPath = QIrDirHelper::setupPath(list,i);
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
        if ((i == list.count() - 1) && isDir)
#else
        if ((i == list.count() - 1) && isDir && list.count() > 1)
#endif
            tempPath += QDir::separator();

        bool Dir = QDir(tempPath).exists();

        nodeList.append(QIrBreadCrumbModelNode(tempPath,i == 0 ? QIrBreadCrumbModelNode::Root : Dir ? QIrBreadCrumbModelNode::Container : QIrBreadCrumbModelNode::Leaf,this));
    }
    return nodeList;
}
QIcon QIrBreadCrumbDirModel::icon( const QIrBreadCrumbModelNode & node ) const
{
    return m_dirModel->fileIcon(m_dirModel->index(node.path()));
}
QString QIrBreadCrumbDirModel::label( const QIrBreadCrumbModelNode & node ) const
{

    if ( node.type() == QIrBreadCrumbModelNode::Root )
        return QIrDirHelper::driveLabel(node.path());
    else if ( node.type() == QIrBreadCrumbModelNode::Global )
        return node.path();
    else {
        QModelIndex index = m_dirModel->index(node.path());

        return m_dirModel->fileName(index);
    }
}
QMimeData * QIrBreadCrumbDirModel::mimeData( const QIrBreadCrumbModelNode & node ) const
{
    QModelIndex index;

    makeCurrent();
    index = m_dirModel->index(node.path());
    if ( !index.isValid() )
        return 0;
    return m_dirModel->mimeData(QModelIndexList() << index);

}
QMenu * QIrBreadCrumbDirModel::buildMenu(const QIrBreadCrumbModelNode & node)
{
    QFileInfo info;
    QString name;
    QString folder;
    QModelIndex index;

    makeCurrent();
    if ( node.path().isEmpty() ) {
        QMenu * menu = m_menuManager->driveMenu();

        if ( !menu->actions().isEmpty() )
            return menu;

        for (int i = QIrDirHelper::Home; i <=  QIrDirHelper::Video; i++) {
            folder = QIrDirHelper::specialFolder((QIrDirHelper::SpecialFolder)i);
            if ( folder.isEmpty() )
                continue;
            index = m_dirModel->index(folder);
            menu->addAction(new QIrBreadCrumbDirModelMenuAction(m_dirModel->fileIcon(index),m_dirModel->fileName(index),m_dirModel->filePath(index),menu));
        }
        return menu;
    }
    if ( node.type() == QIrBreadCrumbModelNode::Global ) {
        QMenu * menu = m_menuManager->mainMenu();

        menu->clear();
        foreach (QFileInfo info, QDir::drives()) {
            folder = info.absoluteFilePath();
            index = m_dirModel->index(folder);
            menu->addAction(new QIrBreadCrumbDirModelMenuAction(m_dirModel->fileIcon(index),QIrDirHelper::driveLabel(folder),m_dirModel->filePath(index),menu));
        }
        return menu;
    } else {
        int count;
        QModelIndex childIndex;

        m_menuManager->clean();
        index = m_dirModel->index(node.path());
        if ( !index.isValid() )
            return 0;
        if ( !m_menuManager->isRegisteredPath(node.path()) )
            m_menuManager->registerPath(qPrintable(node.path()));
        QIrBreadCrumbDirModelMenu * menu = m_menuManager->menu(node.path());

        menu->setFilter(filter());
        if ( !menu || (menu && !menu->needBeRebuilt()))
            return menu;
        menu->clear();
        count = m_dirModel->rowCount(index);
        for (int i = 0; i < count; i++) {
            childIndex = index.child(i,0);
            if ( childIndex.isValid() )
                menu->addAction(new QIrBreadCrumbDirModelMenuAction(m_dirModel->fileIcon(childIndex),m_dirModel->fileName(childIndex),m_dirModel->filePath(childIndex),menu));
        }
        menu->setNeedBeRebuilt(false);
        return menu;
    }
}
void QIrBreadCrumbDirModel::makeCurrent() const
{
    QDir::Filters filter = QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Readable;

    if ( this->filter() & QIrAbstractBreadCrumbModel::AllNodes )
        filter |= QDir::Files;
    if ( this->filter() & QIrAbstractBreadCrumbModel::Hidden )
        filter |= QDir::Hidden;
    m_dirModel->setFilter(filter);
}

QIR_END_NAMESPACE
