#ifndef CFLAGSMODEL_H
#define CFLAGSMODEL_H

#include <QAbstractListModel>
#include <QMetaEnum>
#include <QVector>
#include    "objectinspector_global.h"

class OBJECTINSPECTORSHARED_EXPORT  CFlagsModel : public QAbstractListModel
{
    Q_OBJECT

    QMetaEnum               fenumerator;
    QVector<bool>           fflags;

    void    makeFlagsString(QString & resultString);

public:

    explicit CFlagsModel(QObject *parent = 0);

    int             rowCount(const QModelIndex &parent) const;
    QVariant        data(const QModelIndex &index, int role) const;
    bool            setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags   flags(const QModelIndex &index) const;

    const   QMetaEnum   &   enumerator()    const;
    void    setEnumerator(const QMetaEnum & Enumerator);
    void    setFlagsValue(const QVariant & Value);

signals:

    void    flagsChanged(QString    flagsValue);
    void    typeChanged(QString     TypeName);
};

#endif // CFLAGSMODEL_H
