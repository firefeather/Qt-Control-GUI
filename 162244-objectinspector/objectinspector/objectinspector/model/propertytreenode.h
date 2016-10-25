#ifndef PROPERTYTREENODE_H
#define PROPERTYTREENODE_H

#include "treenode.h"
#include <QMetaProperty>
#include <QObject>

class PropertyTreeNode : public TreeNode
{
Q_OBJECT

    QString             ftargetPropertyName;
    QVariant            fvalue;
    QMetaProperty       fmetaProperty;
    bool                fexpandable;
    bool                fresetable;
    bool                feditable;
    bool                fwritable;

protected:

    void                updateModelIndex();

public:

    PropertyTreeNode(QObject *parent = 0);
    const                   QString &   targetPropertyName()    const;
    void                    setTargetPropertyName(const QString & PropertyName);
    virtual     void        populateProperties();

    const       QVariant    &   value() const;
    void        setExpandable(bool Value = true);
    bool        isExpandable()  const;
    bool        isWritable()    const;

    QString     stringValue();

    virtual void    updateValue();

    bool        isResetable()   const;
    bool        isEditable()    const;
    const       QMetaProperty   &   metaProperty() const;

    void        setEditable(bool Value = true);
    void        setResetable(bool Value = true);
    void        setMetaProperty(const QMetaProperty & Property);

    void        setWritable(bool Value);

signals:

    void        valueChanged(QVariant value);

public  slots:

    void        setValue(const QVariant & Value);
    void        setValue(int Value);
    void        setValue(double Value);
    void        setValue(const QString & Value);
    void        setValue(bool Value);
    void        setValue(const QColor & Value);
    void        setValue(const QFont & Value);
};

#endif // PROPERTYTREENODE_H
