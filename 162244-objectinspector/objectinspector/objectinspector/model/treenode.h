#ifndef TREENODE_H
#define TREENODE_H

#include <QObject>
#include <QVariant>
#include <QModelIndex>

class TreeNode : public QObject
{
Q_OBJECT
public:
    enum        NodeType    {unknownType,rootNode,propertyNode,detailedNode};

private:
    NodeType    ftype;
    QModelIndex fmodelIndex;

public:

    explicit    TreeNode(QObject *parent = 0);
    QObject *   takeChild(int index)    const;
    int         childCount()    const;
    QObject *   childAt(int index)  const;
    void        insertChild(QObject * child);
    int         childRow(QObject * child);
    void        clear();
    void        setType(NodeType Type);
    NodeType    type()          const;
    virtual     QString         stringValue();
    void        setModelIndex(const QModelIndex & index);
    const       QModelIndex &   modelIndex()    const;

signals:
    void        updateModel(const QModelIndex & topLeft,const QModelIndex & bottomRight);
    void        resetProperty();
};

#endif // TREENODE_H
