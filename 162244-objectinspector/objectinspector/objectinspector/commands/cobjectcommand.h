#ifndef COBJECTCOMMAND_H
#define COBJECTCOMMAND_H

#include <QUndoCommand>
#include <QVariant>

class CObjectCommand : public QUndoCommand
{
    QString         fpropName;
protected:
    QList<QObject*> fobjects;
private:
    QVariant        foldValue;
    QVariant        fnewValue;
protected:

    inline  const   QString &   propertyName()  const { return fpropName; }
    inline  const   QVariant    &   oldValue()  const { return foldValue; }
    inline  const   QVariant    &   newValue()  const { return fnewValue; }
    inline  void    setNewValue(const QVariant & Value){ fnewValue = Value; }
    virtual void    updateCommandText() = 0;

public:
    CObjectCommand(const QString & propertyName, const QList<QObject*> & objects, const QVariant & oldValue = QVariant(), const QVariant & newValue = QVariant(), QUndoCommand * parent = 0);
};

#endif // COBJECTCOMMAND_H
