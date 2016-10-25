#ifndef CPROPERTYCOMMAND_H
#define CPROPERTYCOMMAND_H

#include    "cobjectcommand.h"

class CPropertyCommand : public CObjectCommand
{
public:
    CPropertyCommand(const QString & propertyName, const QList<QObject*> & objects, const QVariant & oldValue, const QVariant & newValue, QUndoCommand * parent = 0);
    void    undo();
    void    redo();
    bool    mergeWith(const QUndoCommand *other);
    int     id()    const;
    void    updateCommandText();
};

#endif // CPROPERTYCOMMAND_H
