#ifndef FLAGPROPERTYNODE_H
#define FLAGPROPERTYNODE_H

#include    <QVector>
#include    <QMetaEnum>
#include    "propertytreenode.h"

class FlagPropertyNode : public PropertyTreeNode
{
Q_OBJECT

    QVector<PropertyTreeNode*>   fflags;
    void            makeFlagsString(QString & resultString);

protected:
    void            updateValue();

public:
    FlagPropertyNode(QObject * parent = 0);
    void            populateProperties();
    QString         stringValue();

private slots:
    void    flagClicked(const QVariant & Value);
};

#endif // FLAGPROPERTYNODE_H
