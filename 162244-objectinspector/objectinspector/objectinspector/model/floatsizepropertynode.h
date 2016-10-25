#ifndef FLOATSIZEPROPERTYNODE_H
#define FLOATSIZEPROPERTYNODE_H

#include "propertytreenode.h"
#include <QSizeF>

class FloatSizePropertyNode : public PropertyTreeNode
{
Q_OBJECT

    PropertyTreeNode *   fwidthNode;
    PropertyTreeNode *   fheightNode;

    void    updateValue();
    void    populateProperties();
public:
    FloatSizePropertyNode(QObject *parent = 0);
    QString     stringValue();
    NodeType    type()  const;

private slots:

    void        updateWidth(const QVariant & Value);
    void        updateHeight(const QVariant & Value);
};

#endif // FLOATSIZEPROPERTYNODE_H
