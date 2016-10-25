#include "cpropertycommand.h"

CPropertyCommand::CPropertyCommand(const QString &propertyName, const QList<QObject *> &objects, const QVariant &oldValue, const QVariant &newValue, QUndoCommand *parent):
    CObjectCommand(propertyName,objects,oldValue,newValue,parent)
{

}

void CPropertyCommand::undo()
{
    if(oldValue().isValid() && !propertyName().isEmpty())
    {
        foreach (QObject * object, fobjects)
            object->setProperty(qPrintable(propertyName()),oldValue());
    }
}

void CPropertyCommand::redo()
{
    if(newValue().isValid() && !propertyName().isEmpty())
    {
        foreach (QObject * object, fobjects)
            object->setProperty(qPrintable(propertyName()),newValue());
    }
}

bool CPropertyCommand::mergeWith(const QUndoCommand *other)
{
    if(other->id()!=id())
        return  false;

    const CPropertyCommand    *   propertyCommand = static_cast<const CPropertyCommand*>(other);
    if(propertyCommand->fobjects!=fobjects || propertyCommand->propertyName()!=propertyName())
        return  false;

    setNewValue(propertyCommand->newValue());
    updateCommandText();

    return  true;
}

int CPropertyCommand::id() const
{
    return  2;
}

void CPropertyCommand::updateCommandText()
{
    static  const   QString propertyText(QObject::trUtf8("$'%1' %2 -> %3 у %4 объектов"));
    setText(propertyText.arg(this->propertyName()).arg(this->oldValue().toString()).arg(this->newValue().toString()).arg(fobjects.size()));
}
