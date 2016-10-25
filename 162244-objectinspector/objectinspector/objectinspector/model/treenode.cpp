#include "treenode.h"

TreeNode::TreeNode(QObject *parent) :
    QObject(parent)
{
    ftype = unknownType;
}

QObject *TreeNode::takeChild(int index) const
{
    if(index<children().size())
    {
        QObject *   item = children().at(index);
        item->setParent(NULL);
        return  item;
    }
    return  NULL;
}

int TreeNode::childCount() const
{
    return  children().size();
}

QObject *TreeNode::childAt(int index) const
{
    return  index<children().size()?children().at(index):NULL;
}

void TreeNode::insertChild(QObject *child)
{
    Q_ASSERT(child);
    child->setParent(this);
}

int TreeNode::childRow(QObject *child)
{
    return  children().indexOf(child);
}

void TreeNode::clear()
{
    for(QObjectList::const_iterator it=children().constBegin();it!=children().constEnd();++it)
    {
        QObject * object = *it;
        object->setParent(NULL);
        delete  object;
    }
    fmodelIndex = QModelIndex();
}

void TreeNode::setType(TreeNode::NodeType Type)
{
    ftype = Type;
}

TreeNode::NodeType TreeNode::type() const
{
    return  rootNode;
}

QString TreeNode::stringValue()
{
    return  QString();
}

void TreeNode::setModelIndex(const QModelIndex &index)
{
    fmodelIndex = index;
}

const QModelIndex &TreeNode::modelIndex() const
{
    return  fmodelIndex;
}
