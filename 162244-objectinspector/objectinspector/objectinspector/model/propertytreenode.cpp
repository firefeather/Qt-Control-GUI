#include "propertytreenode.h"
#include <QColor>
#include <QFont>

PropertyTreeNode::PropertyTreeNode(QObject *parent):
    TreeNode(parent)
{
    fexpandable = false;
    fresetable = false;
    feditable = false;
    fwritable = false;
}

const QString &PropertyTreeNode::targetPropertyName() const
{
    return  ftargetPropertyName;
}

void PropertyTreeNode::setTargetPropertyName(const QString &PropertyName)
{
    ftargetPropertyName = PropertyName;
}

void PropertyTreeNode::populateProperties()
{
}

const QVariant &PropertyTreeNode::value() const
{
    return  fvalue;
}

void PropertyTreeNode::setValue(const QVariant &Value)
{
    if(fvalue!=Value)
    {
        fvalue = Value;
        updateValue();
        emit    valueChanged(fvalue);
    }
}

void PropertyTreeNode::setValue(int Value)
{
    if(fvalue!=Value)
    {
        fvalue = Value;
        updateValue();
        emit    valueChanged(fvalue);
    }
}

void PropertyTreeNode::setValue(double Value)
{
    if(fvalue!=Value)
    {
        fvalue = Value;
        updateValue();
        emit    valueChanged(fvalue);
    }
}

void PropertyTreeNode::setValue(const QString &Value)
{
    if(fvalue!=Value)
    {
        fvalue = Value;
        updateValue();
        emit    valueChanged(fvalue);
    }
}

void PropertyTreeNode::setValue(bool Value)
{
    if(fvalue!=Value)
    {
        fvalue = Value;
        updateValue();
        emit    valueChanged(fvalue);
    }
}

void PropertyTreeNode::setValue(const QColor &Value)
{
    QVariant    colorValue = QVariant::fromValue(Value);
    if(fvalue!=colorValue)
    {
        fvalue = colorValue;
        updateValue();
        emit    valueChanged(fvalue);
    }
}

void PropertyTreeNode::setValue(const QFont &Value)
{
    QVariant    fontValue = QVariant::fromValue(Value);
    if(fvalue!=fontValue)
    {
        fvalue = fontValue;
        updateValue();
        emit    valueChanged(fvalue);
    }
}

void PropertyTreeNode::setExpandable(bool Value)
{
    fexpandable = Value;
}

bool PropertyTreeNode::isExpandable() const
{
    return  fexpandable;
}

bool PropertyTreeNode::isWritable() const
{
    return  fwritable;
}

QString PropertyTreeNode::stringValue()
{
    return  fvalue.toString();
}

void PropertyTreeNode::updateValue()
{
}

bool PropertyTreeNode::isResetable() const
{
    return  fresetable;
}

bool PropertyTreeNode::isEditable() const
{
    return  feditable;
}

const QMetaProperty &PropertyTreeNode::metaProperty() const
{
    return  fmetaProperty;
}

void PropertyTreeNode::setEditable(bool Value)
{
    feditable = Value;
}

void PropertyTreeNode::setResetable(bool Value)
{
    fresetable = Value;
}

void PropertyTreeNode::setMetaProperty(const QMetaProperty &Property)
{
    fmetaProperty = Property;
    fwritable = Property.isWritable();
    populateProperties();
}

void PropertyTreeNode::setWritable(bool Value)
{
    fwritable = Value;
}

void PropertyTreeNode::updateModelIndex()
{

}
