#include "enumproperty.h"

EnumProperty::EnumProperty(QObject *parent) :
    PropertyTreeNode(parent)
{

}

int EnumProperty::indexOfValue() const
{
    QMetaEnum   enumerator = metaProperty().enumerator();
    int         currentEnumValue = value().toInt();
    for(int index = 0; index < enumerator.keyCount(); index++)
    {
        if(enumerator.value(index)==currentEnumValue)
            return  index;
    }
    return  -1;
}

const QStringList & EnumProperty::enumeratedProperties() const
{
    return  fenumeratedProperties;
}

QString EnumProperty::stringValue()
{
    return  QString(metaProperty().enumerator().valueToKey(value().toInt()));
}

void EnumProperty::populateProperties()
{
    setWritable(metaProperty().isWritable());
    fenumeratedProperties.clear();
    QMetaEnum   enumerator = metaProperty().enumerator();
    for(int i=0;i<enumerator.keyCount();i++)
    {
        fenumeratedProperties<<QString(enumerator.key(i));
    }
}
