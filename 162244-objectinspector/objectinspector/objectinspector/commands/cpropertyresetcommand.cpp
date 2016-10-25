#include "cpropertyresetcommand.h"
#include <QMetaProperty>

CPropertyResetCommand::CPropertyResetCommand(const QString &propertyName, const QList<QObject *> &objects, const QVariant &oldValue, const QVariant &newValue, QUndoCommand *parent):
    CObjectCommand(propertyName,objects,oldValue,newValue,parent)
{

}

void CPropertyResetCommand::undo()
{
    if(oldValue().isValid() && !propertyName().isEmpty())
    {
        foreach (QObject * object, fobjects)
            object->setProperty(qPrintable(propertyName()),oldValue());
    }
}


void CPropertyResetCommand::redo()
{
    int index;
    foreach (QObject * object, fobjects)
    {
        index = object->metaObject()->indexOfProperty(qPrintable(propertyName()));
        if(index>=0)
        {
            object->metaObject()->property(index).reset(object);
        }
    }
}

void CPropertyResetCommand::updateCommandText()
{
    static  const   QString propertyText(QObject::trUtf8("Сброс $'%1' у %2 объектов"));
    setText(propertyText.arg(this->propertyName()).arg(fobjects.size()));
}

int CPropertyResetCommand::id() const
{
    return  3;
}

bool CPropertyResetCommand::mergeWith(const QUndoCommand *other)
{
    if(other->id()!=id())
        return  false;

    const CPropertyResetCommand   *   resetCommand = static_cast<const CPropertyResetCommand*>(other);
    if(resetCommand->fobjects!=fobjects || resetCommand->propertyName()!=propertyName())
        return  false;

    return  true;
}
