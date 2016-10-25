#include "iisItemList.h"

TItemList::TItemList(bool destroyPixmaps) : 
QList<TItem*>(),
m_destroyPixmaps(destroyPixmaps)
{
}

TItemList::TItemList(const QStringList &list, bool destroyPixmaps) :  
QList<TItem*>(),
m_destroyPixmaps(destroyPixmaps)
{
    for (int i = 0; i < list.count(); i++)
        setItem(i, list.at(i));
}

TItemList::TItemList(QString *list, bool destroyPixmaps) :  
QList<TItem*>(),
m_destroyPixmaps(destroyPixmaps)
{
    for (int i = 0; list[i] != QString::null; i++)
        setItem(i, list[i]);
}



TItemList::~TItemList()
{
	while (!isEmpty()) {
		TItem *it = takeFirst();
		if (m_destroyPixmaps && it->icon)
			delete it->icon;
		delete it;
	}
}


TItemList& TItemList::operator<<(const QString &_text)
{
	setItem(count(), _text);
	return *this;
}

TItemList& TItemList::operator<<(const ITEM& _pair)
{
    setItem(_pair.second, _pair.first);
    return *this;
}

TItemList& TItemList::operator<<(double _val)
{
    setItem(_val, QString::number(_val));
	return *this;
}

TItemList& TItemList::operator<<(const QList<int> &_intList)
{
    for (int i = 0; i < _intList.size(); i++)
        setItem(_intList.at(i));
    return *this;
}

TItemList& TItemList::operator<<(const QList<double> &_doubleList)
{   
    for (int i = 0; i < _doubleList.size(); i++)
        setItem(_doubleList.at(i));
    return *this;
}

TItemList& TItemList::operator<<(const QStringList &_stringList)
{
    for (int i = 0; i < _stringList.size(); i++)
        setItem(_stringList.at(i));
    return *this;
}


TItem* TItemList::setItem(const QVariant &_value, const QString &_text, QIcon *_icon)
{
	TItem *item = new TItem();
	item->icon = _icon;
    item->text = _text.isEmpty() ? _value.toString() : _text;
	item->value = _value;
	append(item);
	return item;
}

TItem* TItemList::findItem(const QVariant &_value)
{
	TItem* it;
	TItemListIterator i(*this);

	while (i.hasNext()) {
		it = i.next();
		if (QVARIANT_COMPARE(it->value, _value))
			return it;
	}

	return 0;
}

TItem* TItemList::findItemByText(const QString &_text)
{
	TItem* it;
	TItemListIterator i(*this);

	while (i.hasNext()) {
		it = i.next();
		if (it->text == _text)
			return it;
	}

	return 0;
}

