#include "flagpropertynode.h"

static  const   char    *   indexProperty = "flag_enum_index";

FlagPropertyNode::FlagPropertyNode(QObject *parent):
    PropertyTreeNode(parent)
{
    setExpandable(true);
}

void FlagPropertyNode::populateProperties()
{
    if(metaProperty().isValid() && metaProperty().isFlagType())
    {
        QMetaEnum   fenumerator = metaProperty().enumerator();
        int keyCount = fenumerator.keyCount();
        fflags.resize(keyCount);
        for(int i=0;i<fenumerator.keyCount();i++)
        {
            PropertyTreeNode    *   flagNode = new PropertyTreeNode;
            const   char    *   keyName = fenumerator.key(i);
            flagNode->setObjectName(keyName);
            flagNode->setTargetPropertyName(keyName);
            flagNode->setValue(false);
            flagNode->setType(detailedNode);
            flagNode->setProperty(indexProperty,i);
            flagNode->setWritable(metaProperty().isWritable());
            connect(flagNode,SIGNAL(valueChanged(QVariant)),this,SLOT(flagClicked(QVariant)));
            insertChild(flagNode);
            fflags[i]=flagNode;
        }
    }
}

QString FlagPropertyNode::stringValue()
{
    static QString flagsText;
    makeFlagsString(flagsText);
    return  flagsText;
}

void FlagPropertyNode::flagClicked(const QVariant &Value)
{
    Q_UNUSED(Value)

    QString     flagsText;
    makeFlagsString(flagsText);
    emit    valueChanged(flagsText);

    TreeNode    *   lastNode   = qobject_cast<TreeNode*>(children().last());
    Q_ASSERT(lastNode);
    emit updateModel(modelIndex().parent(),lastNode->modelIndex());
}

void FlagPropertyNode::makeFlagsString(QString &resultString)
{
    resultString.clear();
    foreach (const PropertyTreeNode * currentFlagNode, fflags)
    {
        if(currentFlagNode->value().toBool())
        {
            resultString+=currentFlagNode->targetPropertyName();
            resultString+="|";
        }
    }
    if(!resultString.isEmpty())
        resultString.chop(1);
    else
        resultString = trUtf8("Не задано");
}

void FlagPropertyNode::updateValue()
{
    if(fflags.isEmpty())
        return;

    uint rawFlag = value().toUInt();
    QMetaEnum   fenumerator = metaProperty().enumerator();
    for(int i=0;i<fenumerator.keyCount();i++)
    {
        fflags[i]->setValue(bool(rawFlag & fenumerator.value(i)));
    }
}
