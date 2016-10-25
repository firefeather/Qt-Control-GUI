#include "sizepropertynode.h"
#include <QSize>

SizePropertyNode::SizePropertyNode(QObject *parent) :
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

    setValue(QSize());
}

QString SizePropertyNode::stringValue()
{
    QSize   sizeValue = value().toSize();
    return  QString("%1 x %2").arg(sizeValue.width()).arg(sizeValue.height());
}

TreeNode::NodeType SizePropertyNode::type() const
{
    return  propertyNode;
}

void SizePropertyNode::updateWidth(const QVariant &Value)
{
    int     intValue = Value.toInt();
    QSize   sizeValue = value().toSize();
    if(sizeValue.width()!=intValue)
    {
        sizeValue.setWidth(intValue);
        fwidthNode->setValue(intValue);
        emit    valueChanged(sizeValue);
        setValue(sizeValue);
        updateModelIndex();
    }
}

void SizePropertyNode::updateHeight(const QVariant &Value)
{
    int     intValue = Value.toInt();
    QSize   sizeValue = value().toSize();
    if(sizeValue.height()!=intValue)
    {
        sizeValue.setHeight(intValue);
        fheightNode->setValue(intValue);
        emit    valueChanged(sizeValue);
        setValue(sizeValue);
        updateModelIndex();
    }
}


void SizePropertyNode::updateValue()
{
    QSize   sizeValue = value().toSize();
    fwidthNode->setValue(sizeValue.width());
    fheightNode->setValue(sizeValue.height());
}

void SizePropertyNode::populateProperties()
{
    fwidthNode->setWritable(metaProperty().isWritable());
    fheightNode->setWritable(metaProperty().isWritable());
}
