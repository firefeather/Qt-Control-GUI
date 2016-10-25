#include "cflagsmodel.h"

CFlagsModel::CFlagsModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int CFlagsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return  fenumerator.keyCount();
}

QVariant CFlagsModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        switch (role) {
        case    Qt::DisplayRole:
            return  fenumerator.key(index.row());
        case    Qt::CheckStateRole:
            return  fflags.at(index.row())?Qt::Checked:Qt::Unchecked;
        default:
            break;
        }
    }
    return  QVariant();
}

bool CFlagsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid())
    {
        switch (role)
        {
        case Qt::CheckStateRole:
        {
            fflags[index.row()]=value.toBool();
            emit    dataChanged(index,index);
            QString compiledFlags;
            makeFlagsString(compiledFlags);
            emit    flagsChanged(compiledFlags);
            return  true;
        }
        default:
            break;
        }
    }
    return  false;
}

Qt::ItemFlags CFlagsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags   oldFlags = QAbstractListModel::flags(index);
    oldFlags |= Qt::ItemIsUserCheckable;
    return  oldFlags;
}

const QMetaEnum &CFlagsModel::enumerator() const
{
    return  fenumerator;
}

void CFlagsModel::setEnumerator(const QMetaEnum &Enumerator)
{
    beginResetModel();
    fenumerator = Enumerator;
    fflags.resize(fenumerator.keyCount());
    fflags.fill(false);
    endResetModel();
    emit    typeChanged(fenumerator.name());
}

void CFlagsModel::setFlagsValue(const QVariant &Value)
{
    beginResetModel();
    uint    rawFlag = Value.toUInt();
    for(int i=0;i<fenumerator.keyCount();i++)
    {
        fflags[i] = (rawFlag & fenumerator.value(i));
    }
    endResetModel();
}

void CFlagsModel::makeFlagsString(QString &resultString)
{
    int index = 0;
    resultString.clear();

    foreach (const bool currentFlag, fflags)
    {
        if(currentFlag)
        {
            const   char    *   valueKey = fenumerator.valueToKey(fenumerator.value(index));
            if(valueKey)
            {
                resultString+=valueKey;
            }
            else
            {
                static  const   QString wrongFlag("WRONG_FLAG_INDEX(%1)");
                resultString+=wrongFlag.arg(index);
            }
            resultString+="|";
            }
        index++;
    }
    if(!resultString.isEmpty())
        resultString.chop(1);
}
