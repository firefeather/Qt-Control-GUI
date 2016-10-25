#include "cobjectcommand.h"

CObjectCommand::CObjectCommand(const QString &propertyName, const QList<QObject *> &objects, const QVariant &oldValue, const QVariant &newValue, QUndoCommand *parent):
    QUndoCommand(parent),fpropName(propertyName),fobjects(objects),foldValue(oldValue),fnewValue(newValue)
{
}
