#include "floatsizepropertynode.h"

FloatSizePropertyNode::FloatSizePropertyNode(QObject *parent):
    PropertyTreeNode(parent)
{
    setExpandable(true);

    fwidthNode = new PropertyTreeNode;
    fwidthNode->setType(detailedNode);
    fwidthNode->setValue(0);
    fwidthNode->setObjectName("width");
    connect(fwidthNode,SIGNAL(valueChanged(QVariant)),this,SLOT(updateWidth(QVariant)));
    insertChild(fwidthNode);

    fheightNode = new PropertyTreeNode;
    fheightNode->setType(detailedNode);
    fheightNode->setValue(0);
    fheightNode->setObjectName("height");
    connect(fheightNode,SIGNAL(valueChanged(QVariant)),this,SLOT(updateHeight(QVariant)));
    insertChild(fheightNode);

    setValue(QSizeF());
}

QString FloatSizePropertyNode::stringValue()
{
    QSizeF      sizeValue = value().toSizeF();
    return      QString("%1 x %2").arg(sizeValue.width()).arg(sizeValue.height());
}

TreeNode::NodeType FloatSizePropertyNode::type() const
{
    return  propertyNode;
}

void FloatSizePropertyNode::updateWidth(const QVariant &Value)
{
    double   floatValue = Value.toFloat();
    QSizeF   sizeValue = value().toSizeF();
    if(sizeValue.width()!=floatValue)
    {
        sizeValue.setWidth(floatValue);
        fwidthNode->setValue(floatValue);
        emit    valueChanged(sizeValue);
        setValue(sizeValue);
        updateModelIndex();
    }
}

void FloatSizePropertyNode::updateHeight(const QVariant &Value)
{
    double   floatValue = Value.toFloat();
    QSizeF   sizeValue = value().toSizeF();
    if(sizeValue.height()!=floatValue)
    {
        sizeValue.setHeight(floatValue);
        fheightNode->setValue(floatValue);
        emit    valueChanged(sizeValue);
        setValue(sizeValue);
        updateModelIndex();
    }
}


void FloatSizePropertyNode::updateValue()
{
    QSizeF   sizeValue = value().toSizeF();
    fwidthNode->setValue(sizeValue.width());
    fheightNode->setValue(sizeValue.height());
}

void FloatSizePropertyNode::populateProperties()
{
    fwidthNode->setWritable(metaProperty().isWritable());
    fheightNode->setWritable(metaProperty().isWritable());
}


