#ifndef CPROPERTYRESETCOMMAND_H
#define CPROPERTYRESETCOMMAND_H

#include "cobjectcommand.h"

class CPropertyResetCommand : public CObjectCommand
{
public:
    CPropertyResetCommand(const QString & propertyName, const QList<QObject*> & objects, const QVariant & oldValue, const QVariant & newValue, QUndoCommand * parent = 0);
    void    undo();
    void    redo();
    void    updateCommandText();
    int     id() const;
    bool    mergeWith(const QUndoCommand *other);
};

#endif // CPROPERTYRESETCOMMAND_H
