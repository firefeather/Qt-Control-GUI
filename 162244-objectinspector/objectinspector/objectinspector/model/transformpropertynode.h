#ifndef TRANSFORMPROPERTYNODE_H
#define TRANSFORMPROPERTYNODE_H

#include "propertytreenode.h"



class TransformPropertyNode : public PropertyTreeNode
{
    Q_OBJECT

    PropertyTreeNode *   fmatrix[9];

    void    updateValue();
    void    populateProperties();
public:
    TransformPropertyNode(QObject *parent = 0);
    QString     stringValue();

    NodeType    type()  const;

};

#endif // TRANSFORMPROPERTYNODE_H
