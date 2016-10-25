#ifndef SIZEPROPERTYNODE_H
#define SIZEPROPERTYNODE_H

#include "propertytreenode.h"
#include <QSize>

class SizePropertyNode : public PropertyTreeNode
{
    Q_OBJECT

    PropertyTreeNode *   fwidthNode;
    PropertyTreeNode *   fheightNode;

    void    updateValue();
    void    populateProperties();

public:

    explicit    SizePropertyNode(QObject *parent = 0);
    QString     stringValue();

    NodeType    type()  const;
    
private slots:

    void        updateWidth(const QVariant & Value);
    void        updateHeight(const QVariant & Value);
};

#endif // SIZEPROPERTYNODE_H
