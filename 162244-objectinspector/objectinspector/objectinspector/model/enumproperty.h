#ifndef ENUMPROPERTY_H
#define ENUMPROPERTY_H

#include    "propertytreenode.h"
#include    <QStringList>

class EnumProperty : public PropertyTreeNode
{
    Q_OBJECT

    QStringList fenumeratedProperties;

protected:
    void        populateProperties();
public:
    explicit    EnumProperty(QObject *parent = 0);
    int         indexOfValue()  const;
    const       QStringList &   enumeratedProperties()  const;
    QString     stringValue();
};

#endif // ENUMPROPERTY_H
