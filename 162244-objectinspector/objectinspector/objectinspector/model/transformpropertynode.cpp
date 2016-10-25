#include "transformpropertynode.h"
#include <QTransform>

TransformPropertyNode::TransformPropertyNode(QObject *parent):
    PropertyTreeNode(parent)
{
    setExpandable(true);

    PropertyTreeNode    *   node;

    for(int i=0;i<9;i++)
    {
        node = new PropertyTreeNode;
        node->setType(detailedNode);
        node->setValue(0.0);
        fmatrix[i]=node;
        insertChild(node);
    }

    fmatrix[0]->setObjectName("m11");
    fmatrix[1]->setObjectName("m12");
    fmatrix[2]->setObjectName("m13");
    fmatrix[3]->setObjectName("m21");
    fmatrix[4]->setObjectName("m22");
    fmatrix[5]->setObjectName("m23");
    fmatrix[6]->setObjectName("m31");
    fmatrix[7]->setObjectName("m32");
    fmatrix[8]->setObjectName("m33");

    setValue(QTransform());
}

QString TransformPropertyNode::stringValue()
{
    QTransform  ftransform = value().value<QTransform>();
    return  QString("[%1 %2 %3][%4 %5 %6][%7 %8 %9]")
            .arg(ftransform.m11()).arg(ftransform.m12()).arg(ftransform.m13())
            .arg(ftransform.m21()).arg(ftransform.m22()).arg(ftransform.m23())
            .arg(ftransform.m31()).arg(ftransform.m32()).arg(ftransform.m33());
}

TreeNode::NodeType TransformPropertyNode::type() const
{
    return  propertyNode;
}


void TransformPropertyNode::updateValue()
{
    QTransform   transform = value().value<QTransform>();
    fmatrix[0]->setValue(transform.m11());
    fmatrix[1]->setValue(transform.m12());
    fmatrix[2]->setValue(transform.m13());
    fmatrix[3]->setValue(transform.m21());
    fmatrix[4]->setValue(transform.m22());
    fmatrix[5]->setValue(transform.m23());
    fmatrix[6]->setValue(transform.m31());
    fmatrix[7]->setValue(transform.m32());
    fmatrix[8]->setValue(transform.m33());
}

void TransformPropertyNode::populateProperties()
{
    for(int i=0;i<9;i++)
    {
        fmatrix[i]->setWritable(metaProperty().isWritable());
        fmatrix[i]->setResetable(metaProperty().isResettable());
    }
}
