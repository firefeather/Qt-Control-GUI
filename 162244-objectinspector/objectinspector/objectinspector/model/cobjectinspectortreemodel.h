#ifndef COBJECTINSPECTORTREEMODEL_H
#define COBJECTINSPECTORTREEMODEL_H

#include "objectinspector_global.h"
#include <QSet>
#include <QVariant>
#include <QAbstractItemModel>
#include "propertytreenode.h"

class OBJECTINSPECTORSHARED_EXPORT CObjectInspectorTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum            PropertyMode                {pmAll,pmThisOnly,pmDesignableAll,pmDesignableThisOnly};
    enum            ObjectInspectorModelRole    {TreeNodeRole = Qt::UserRole+1};
    enum            ShowedType                  {ShowReadable,ShowWritable};

private:

    TreeNode    *   frootNode;
    PropertyMode    fmode;


    QList<QObject*>         fobjects;

    void            enumerateProps(QObject * object, PropertyMode Mode, QStringList &propNameList);
    void            filterProps();
    static  const QColor    colorByType(QVariant::Type VarType);

    QSet<QVariant::Type>    fsupportedTypes;

protected:

    TreeNode    *   nodeForIndex(const QModelIndex & index) const;

    void            setValueForObjects(const QString & propertyName, const QVariant & propertyValue);

public:
    explicit CObjectInspectorTreeModel(QObject *parent = 0);
    ~CObjectInspectorTreeModel();

    enum    {columnName,columnValue,maxColumn};

    Qt::ItemFlags   flags(const QModelIndex &index) const;
    QVariant        data(const QModelIndex &index, int role) const;
    QVariant        headerData(int section, Qt::Orientation orientation, int role) const;
    int             rowCount(const QModelIndex &parent) const;
    int             columnCount(const QModelIndex &parent) const;
    QModelIndex     index(int row, int column, const QModelIndex &parent) const;
    QModelIndex     parent(const QModelIndex &child) const;
    bool            setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    bool            setData(const QModelIndex &index, const QVariant &value, int role);

    void            setObjects(const QList<QObject*> & ObjectList);
    const           QList<QObject*>  &  objects()    const;

    TreeNode    *   rootNode()  const;

public  slots:

    void            clear();
    void            update();
    void            setMode(PropertyMode Mode);

private slots:

    void            setValueForObjects(const QVariant & Value);
    void            resetValuesForObjects();
};

Q_DECLARE_METATYPE(CObjectInspectorTreeModel::PropertyMode)

#endif // COBJECTINSPECTORTREEMODEL_H

