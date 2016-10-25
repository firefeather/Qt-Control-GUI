#include    "cobjectinspectortreemodel.h"
#include    <QMetaProperty>
#include    <QStringList>
#include    <QColor>
#include    "flagpropertynode.h"
#include    "sizepropertynode.h"
#include    "enumproperty.h"
#include    "floatsizepropertynode.h"
#include    "cpropertycommand.h"
#include    "cpropertyresetcommand.h"
#include    "transformpropertynode.h"

CObjectInspectorTreeModel::CObjectInspectorTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    frootNode   =   new TreeNode(this);
    frootNode->setType(TreeNode::rootNode);
    fmode = pmDesignableAll;
    fsupportedTypes<<QVariant::Int;
    fsupportedTypes<<QVariant::LongLong;
    fsupportedTypes<<QVariant::UInt;
    fsupportedTypes<<QVariant::ULongLong;
    fsupportedTypes<<QVariant::Size;
    fsupportedTypes<<QVariant::SizeF;
    fsupportedTypes<<QVariant::String;
    fsupportedTypes<<QVariant::Color;
    fsupportedTypes<<QVariant::Font;
    fsupportedTypes<<QVariant::Bool;
    fsupportedTypes<<QVariant::Double;
    fsupportedTypes<<QVariant::Transform;
}

CObjectInspectorTreeModel::~CObjectInspectorTreeModel()
{
    delete  frootNode;
}

Qt::ItemFlags CObjectInspectorTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags   oldFlags = QAbstractItemModel::flags(index);
    if(index.isValid())
    {
        oldFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    return  oldFlags;
}

QVariant CObjectInspectorTreeModel::data(const QModelIndex &index, int role) const
{
    if(!frootNode || !index.isValid() || index.column()<0 || index.column()>=maxColumn)
        return  QVariant();

    TreeNode * node = nodeForIndex(index);

    if(node && node!=frootNode)
    {
        switch (role) {
        case Qt::DisplayRole:
        switch (index.column())
        {
            case    columnName:
                return  node->objectName();
            case    columnValue:
                return  node->stringValue();
            default:
                break;
        }
        break;
        case Qt::EditRole:
        {
            if(index.column()==columnValue)
            {
                PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(node);
                if(propertyNode)
                {
                    return  propertyNode->value();
                }
            }
        }
        break;
        case CObjectInspectorTreeModel::TreeNodeRole:
        switch (index.column())
        {
        case    columnValue:
            {
                PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(node);
                return  propertyNode?QVariant::fromValue(propertyNode):QVariant();
            }
        default:
            break;
        }
        break;
        case    Qt::BackgroundColorRole:
        {
            PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(node);
            return  colorByType(propertyNode->value().type());
        }
        case    Qt::ToolTipRole:
        {
            PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(node);
            if(propertyNode)
                return  QString(propertyNode->value().typeName());
        }
            break;
        default:
            break;
        }
    }

    return  QVariant();
}

QVariant CObjectInspectorTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole && section>=0 && section<maxColumn)
    {
        static  const   QString headers[maxColumn] = {trUtf8("Property"),trUtf8("Value")};
        return  headers[section];
    }
    return  QVariant();
}

int CObjectInspectorTreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column()!=0)
        return  0;
    TreeNode * node = nodeForIndex(parent);
    return  node?node->childCount():0;
}

int CObjectInspectorTreeModel::columnCount(const QModelIndex &parent) const
{
    return  parent.isValid() && parent.column()!=0 ? 0: maxColumn;
}

QModelIndex CObjectInspectorTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!frootNode || row<0 || column<0 || column>=maxColumn || (parent.isValid() && parent.column()!=0))
        return  QModelIndex();

    TreeNode    *   parentNode = nodeForIndex(parent);
    Q_ASSERT(parentNode);
    if(TreeNode * item = dynamic_cast<TreeNode*>(parentNode->childAt(row)))
    {
        QModelIndex createdIndex = createIndex(row,column,item);
        item->setModelIndex(createdIndex);
        connect(item,SIGNAL(updateModel(QModelIndex,QModelIndex)),this,SIGNAL(dataChanged(QModelIndex,QModelIndex)));
        return  createdIndex;
    }
    return  QModelIndex();
}

QModelIndex CObjectInspectorTreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return  QModelIndex();
    if(TreeNode * childNode = nodeForIndex(child))
    {
        if(TreeNode * parentNode = dynamic_cast<TreeNode*>(childNode->parent()))
        {
            if(parentNode == frootNode)
                return  QModelIndex();
            if(TreeNode * grandParentNode = dynamic_cast<TreeNode*>(parentNode->parent()))
            {
                int row = grandParentNode->childRow(parentNode);
                return  createIndex(row,0,parentNode);
            }
        }
    }
    return  QModelIndex();
}

bool CObjectInspectorTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return  false;
}

bool CObjectInspectorTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && index.column()==columnValue)
    {
        if(value.isValid())
        switch (role)
        {
        case Qt::EditRole:
        {
            PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(nodeForIndex(index));
            if(propertyNode && !propertyNode->isExpandable())
            {
                propertyNode->setValue(value);
                emit    dataChanged(index,index);
                return  true;
            }
        }
            break;
        default:
            break;
        }
    }
    return  false;
}

TreeNode * CObjectInspectorTreeModel::nodeForIndex(const QModelIndex &index) const
{
    if(index.isValid())
    {
        if(TreeNode * node = static_cast<TreeNode*>(index.internalPointer()))
            return  node;
    }
    return  frootNode;
}

void CObjectInspectorTreeModel::setObjects(const QList<QObject*> & ObjectList)
{
    if(fobjects!=ObjectList)
    {
        beginResetModel();
        frootNode->clear();
        fobjects = ObjectList;
        if(!fobjects.isEmpty())
        {
            filterProps();
        }
        endResetModel();
    }
}

const QList<QObject*> & CObjectInspectorTreeModel::objects() const
{
    return  fobjects;
}

TreeNode *CObjectInspectorTreeModel::rootNode() const
{
    return  frootNode;
}

void CObjectInspectorTreeModel::clear()
{
    beginResetModel();
    fobjects.clear();
    frootNode->clear();
    endResetModel();
}

void CObjectInspectorTreeModel::update()
{
    beginResetModel();
    endResetModel();
}

void CObjectInspectorTreeModel::setMode(CObjectInspectorTreeModel::PropertyMode Mode)
{
    if(fmode!=Mode)
    {
        beginResetModel();
        fmode = Mode;
        frootNode->clear();
        filterProps();
        endResetModel();
    }
}

void CObjectInspectorTreeModel::setValueForObjects(const QVariant &Value)
{
    PropertyTreeNode    *   senderNode = qobject_cast<PropertyTreeNode*>(sender());
    if(senderNode && !fobjects.isEmpty())
    {
        const QString & propertyName = senderNode->targetPropertyName();
//        if(undoStack())
//        {
//            CPropertyCommand    *   propCommand = new CPropertyCommand(propertyName,fobjects,fobjects.first()->property(qPrintable(propertyName)),Value);
//            propCommand->updateCommandText();
//            undoStack()->push(propCommand);
//        }
//        else
        foreach (QObject * object, fobjects)
        {
            object->setProperty(qPrintable(propertyName),Value);
        }
    }
}

void CObjectInspectorTreeModel::setValueForObjects(const QString &propertyName, const QVariant &propertyValue)
{
    if(fobjects.isEmpty())
        return;

    const   char    *   property_c_string = qPrintable(propertyName);

//    if(undoStack())
//    {
//        CPropertyCommand    *   propCommand = new CPropertyCommand(propertyName,fobjects,fobjects.first()->property(qPrintable(propertyName)),propertyValue);
//        propCommand->updateCommandText();
//        undoStack()->push(propCommand);
//    }
//    else
    foreach (QObject * object, fobjects)
    {
        object->setProperty(property_c_string,propertyValue);
    }
}

void CObjectInspectorTreeModel::resetValuesForObjects()
{
    PropertyTreeNode    *   senderNode = qobject_cast<PropertyTreeNode*>(sender());
    if(senderNode && !fobjects.isEmpty())
    {
        int index;
        const QString & propertyName = senderNode->targetPropertyName();
//        if(undoStack())
//        {
//            CPropertyResetCommand   *   resetCommand = new CPropertyResetCommand(propertyName,fobjects,fobjects.first()->property(qPrintable(propertyName)),QVariant());
//            resetCommand->updateCommandText();
//            undoStack()->push(resetCommand);
//        }
//        else
        foreach (QObject * object, fobjects)
        {
            index = object->metaObject()->indexOfProperty(qPrintable(propertyName));
            if(index>=0)
            {
                object->metaObject()->property(index).reset(object);
            }
        }
    }
}

void CObjectInspectorTreeModel::enumerateProps(QObject *object, CObjectInspectorTreeModel::PropertyMode Mode, QStringList &propNameList)
{
    const   QMetaObject *   metaObject = object->metaObject();
    switch (Mode)
    {
    case pmAll:
        for(int propIndex = 0;propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
                propNameList<<currentMetaProperty.name();
        }
        break;
    case    pmDesignableAll:
        for(int propIndex = 0;propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
            if(currentMetaProperty.isDesignable())
                propNameList<<currentMetaProperty.name();
        }
        break;
    case    pmDesignableThisOnly:
        for(int propIndex = metaObject->propertyOffset();propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
            if(currentMetaProperty.isDesignable())
                propNameList<<currentMetaProperty.name();
        }
        break;
    case    pmThisOnly:
        for(int propIndex = metaObject->propertyOffset();propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
                propNameList<<currentMetaProperty.name();
        }
        break;
    }
}

void CObjectInspectorTreeModel::filterProps()
{
    if(fobjects.isEmpty())
        return;

    QList<QStringList>    fpropsList;
    QStringList           fresultList;
    //сначала заполняем все свойства для всех объектов
    foreach (QObject * object, fobjects)
    {
        QStringList   list;
        enumerateProps(object,fmode,list);
        fpropsList<<list;
    }



    //теперь будем отсекать различающиеся, что бы оставить только совпадающие
    bool    shouldAdd = false;

    for(QList<QStringList>::const_iterator it = fpropsList.constBegin(); it!=fpropsList.constEnd(); ++it)
    {
        const QStringList & currentPropList = *it;
        if(currentPropList.isEmpty())
            continue;

        foreach(const QString & currentProperty, currentPropList)
        {
            for(QList<QStringList>::const_iterator jt=it; jt!=fpropsList.constEnd(); ++jt)
            {
                shouldAdd = true;

                if(jt == it)
                    continue;

                const QStringList &   oppositeList = *jt;

                if(!oppositeList.contains(currentProperty))
                {
                    shouldAdd = false;
                    break;
                }
            }

            if(shouldAdd && !fresultList.contains(currentProperty))
            {
                fresultList<<currentProperty;
            }
        }
    }



    //теперь проходимся по всем объектам и находим свойства, у которых одинаковое значение
    QStringList filteredList;
    QVariant    prevValue;
    QObject *   object;
    QString     propertyName;
    int         indexOfProperty;

    for(QStringList::iterator it=fresultList.begin();it!=fresultList.end();it++)
    {
        shouldAdd = true;
        QList<QObject*>::iterator first = fobjects.begin();

        object = *first;

        const QMetaObject   *   firstMetaObject = object->metaObject();
        propertyName = *it;
        indexOfProperty = firstMetaObject->indexOfProperty(qPrintable(propertyName));
        if(indexOfProperty<0)
            continue;

        QMetaProperty   metaProperty = firstMetaObject->property(indexOfProperty);
        if(!fsupportedTypes.contains(metaProperty.type()))
            continue;

        prevValue = object->property(qPrintable(propertyName));
        first++;
        for(QList<QObject*>::iterator jt=first;jt!=fobjects.end();jt++)
        {
            if((*jt)->property(qPrintable(propertyName))!=prevValue)
            {
                shouldAdd = false;
                break;
            }
        }
        if(shouldAdd)
            filteredList<<*it;
    }

    fresultList.clear();

    //теперь можно публиковать свойства
    int index;
    const QMetaObject *   firstObject = fobjects.first()->metaObject();

    PropertyTreeNode    *   propertyNode;
    QObject *   fobject = objects().first();
    foreach (const QString & propName, filteredList)
    {
        index = firstObject->indexOfProperty(qPrintable(propName));
        if(index>=0)
        {
            QMetaProperty           metaProp = firstObject->property(index);
            if(!metaProp.isWritable())
                continue;

            propertyNode = NULL;
            switch (metaProp.type())
            {
            case    QVariant::Int:
            case    QVariant::LongLong:
            case    QVariant::UInt:
            case    QVariant::ULongLong:
            {
                if(metaProp.isFlagType())
                {
                    propertyNode = new FlagPropertyNode;
                }
                else
                if(metaProp.isEnumType())
                {
                    propertyNode = new EnumProperty;
                }
                else
                {
                    propertyNode = new PropertyTreeNode;
                    propertyNode->setEditable();
                }
            }
                break;
            case    QVariant::Size:
                propertyNode    =   new SizePropertyNode;
                break;
            case    QVariant::SizeF:
                propertyNode    =   new FloatSizePropertyNode;
                break;
            case    QVariant::String:
            case    QVariant::Color:
            case    QVariant::Font:
            {
                propertyNode    =   new PropertyTreeNode;
                propertyNode->setEditable();
            }
                break;
            case    QVariant::Bool:
            case    QVariant::Double:
                propertyNode    =   new PropertyTreeNode;
                break;
            case    QVariant::Transform:
                propertyNode    =   new TransformPropertyNode;
                break;
            default:
                continue;
                break;
            }
            if(propertyNode)
            {
                propertyNode->setEditable(metaProp.isWritable());
                propertyNode->setResetable(metaProp.isResettable());
                propertyNode->setType(TreeNode::propertyNode);
                propertyNode->setObjectName(propName);
                propertyNode->setTargetPropertyName(propName);
                propertyNode->setMetaProperty(metaProp);
                QVariant v = fobject->property(qPrintable(propName));
                propertyNode->setValue(v);
                connect(propertyNode,SIGNAL(valueChanged(QVariant)),this,SLOT(setValueForObjects(QVariant)));
                connect(propertyNode,SIGNAL(resetProperty()),this,SLOT(resetValuesForObjects()));
                frootNode->insertChild(propertyNode);
            }
        }
    }
}

const QColor CObjectInspectorTreeModel::colorByType(QVariant::Type VarType)
{
    switch (VarType)
    {
        case    QVariant::Char:
        case    QVariant::String:
        case    QVariant::Url:
        {
            static  const   QColor lightGreenColor(QColor::fromRgb(0xcc,0xff,0xcc));
            return  lightGreenColor;
        }
        case    QVariant::Double:
        case    QVariant::Int:
        case    QVariant::UInt:
        case    QVariant::LongLong:
        case    QVariant::ULongLong:
        {
            static  const   QColor lightBlueColor(QColor::fromRgb(0xcc,0xcc,0xff));
            return  lightBlueColor;
        }

        case    QVariant::Size:
        case    QVariant::SizeF:
        case    QVariant::Rect:
        case    QVariant::RectF:
        case    QVariant::Point:
        case    QVariant::PointF:
        case    QVariant::Line:
        case    QVariant::LineF:
        {
            static  const   QColor lightCyanColor(QColor::fromRgb(0xcc,0xff,0xff));
            return  lightCyanColor;
        }

        case    QVariant::Font:
        case    QVariant::Brush:
        case    QVariant::Icon:
        case    QVariant::Image:
        case    QVariant::Bitmap:
        case    QVariant::Cursor:
        case    QVariant::Pen:
        case    QVariant::Color:
        case    QVariant::Transform:
        {
        static  const   QColor lightYellowColor(QColor::fromRgb(0xff,0xff,0xcc));
        return  lightYellowColor;
        }
        case    QVariant::Bool:
        {
            static  const   QColor lightRedColor(QColor::fromRgb(0xff,0xcc,0xcc));
            return  lightRedColor;
        }
    default:
        return  Qt::white;
    }
}
