//***************************************************************************//
//
// TItemList Class
//
// It is the internal class which stores List Items in the Map.
//
// In most cases, there is no need to modify or use this class directly
// within your Application.
//
//***************************************************************************//

#ifndef TITEMLIST_H
#define TITEMLIST_H

#include <QtCore>
#include <QtGui>

/****************************************************************************/

#define QVARIANT_COMPARE(a,b) (a == b && b == a)
//#define QVARIANT_COMPARE(a,b) (a == b)

//***************************************************************************//

/*! \class TItem iisItemList.h
 * \nosubgrouping
 * \brief A common item representing one item in drop-down list of value editor.

	Every item contains value, text (description of the value) and optional icon.
 */

class TItem 
{
public:
	QVariant	value;	///< item value which will be set when this item is choosen from the list.
	QIcon		*icon;	///< icon of the item, or 0 if no icon.
	QString		text;	///< text for item value; it will be shown in drop-down list.
};

//***************************************************************************//

typedef QPair<QString, QVariant> ITEM;

//***************************************************************************//

/*! \class TItemList iisItemList.h
 * \nosubgrouping
 * \brief A common list of TItem items.

	This list is used to represent drop-down lists which can contain items consisting of value,
	description and icon. 
	
	\sa TItem
 */

class TItemList : public QList<TItem*>
{
public:
	/*!	Constructor.
		If \a destroyPixmaps is true, then all the icons of list items will be destroyed with the list.
	*/
	TItemList(bool destroyPixmaps = false);

	/*!	Constructor.
        Sets item text to \a list string, values from 0 to list.count()-1.
		If \a destroyPixmaps is true, then all the icons of list items will be destroyed with the list.
	*/
	TItemList(const QStringList &list, bool destroyPixmaps = false);

	/*!	Constructor.
        Sets item text to \a list of strings finished by QString::null, and values from 0 to list size.
		If \a destroyPixmaps is true, then all the icons of list items will be destroyed with the list.
	*/
    TItemList(QString *list, bool destroyPixmaps = false);

	virtual ~TItemList();



	/*!	Constructs and adds a new TItem to the list.
		@param _value	value of the item.
		@param _text	text of the item (will be shown in drop-down list).
		@param _icon	item icon.
		@return newly created TItem.
	*/
	virtual TItem*	setItem(const QVariant &_value, const QString &_text = QString::null, QIcon *_icon = 0);

	/*!	Searches for TItem with value \a _value.
		Returns 0 if not found.
		\sa findItemByText()
	*/
	virtual TItem*	findItem(const QVariant &_value);

	/*!	Searches for TItem with text \a _text.
		Returns 0 if not found.
		\sa findItem()
	*/
	virtual TItem*	findItemByText(const QString &_text);

	/*!	Helper operator to quickly fill lists consisting of simple text items with corresponding values of 0,1,2,3.
		For example:

        \code
        TItemList *listTenzType = new TItemList;

        *listTenzType << "Absent" << "Manual" << "Automatic";	// "Absent" will get the value of 0, "Manual" of 1, "Automatic" of 2
        \endcode
	*/
	TItemList& operator<<(const QString &_text);

	/*!	Helper operator to quickly fill lists consisting of simple doubles with corresponding values.
		For example:

        \code
        TItemList *listSize = new TItemList;

        *listSize << 1 << 2 << 2.5 << 3 << 3.2 << 3.5 << 3.8;	
        \endcode
	*/
	TItemList& operator<<(double _val);


	/*!	Helper operator to quickly fill lists consisting of simple text items with corresponding values.
		For example:

        \code
        TItemList *listTenzType = new TItemList;

        *listTenzType << ITEM("Manual", 10) << ITEM("Automatic", 12);	// "Manual" will get the value of 10, "Automatic" of 12
        \endcode
	*/
	TItemList& operator<<(const ITEM& _pair);


	/*!	Helper operator to to fill with list of integers with corresponding values.
		For example:

        \code
        TItemList fontSizeList;
        fontSizeList = QFontDatabase::standardSizes();	// fontSizeList will be filled with standard font sizes.
        \endcode

        \since 3.2
	*/
    TItemList& operator<<(const QList<int> &_intList);

	/*!	Helper operator to fill with list of doubles with corresponding values.
        \since 3.2
	*/
    TItemList& operator<<(const QList<double> &_doubleList);

	/*!	Helper operator to fill with string list with corresponding values.
		For example:

        \code
        TItemList fontNamesList;
        fontNamesList = QFontDatabase::families();	// fontNamesList will be filled with the names of available font families.
        \endcode

        \since 3.2
	*/
    TItemList& operator<<(const QStringList &_stringList);



    /*! Paints combo box item at \a index.
        Reimplement this if you want to make custom drawing in the drop down combo.
        Return true if you wish to call QItemDelegate::paint() method at the end
        (i.e. only painting options are changed), or false if painting is fully custom.
        \a data is the value of item, \a option is font/palette options to change, 
        \a painter is the active painter.
        \since 3.2
    */
    virtual bool paintComboItem(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index,
        QVariant &data) 
    { return true; }

    /*! Returns true if \a list_value equals \a item_value.
        Reimplement this if you want to make custom search of list item in the drop down combo.
        \since 3.2
    */
    virtual bool compareComboItem(const QVariant &list_value, const QVariant &item_value) 
    { return list_value == item_value; }

    /*! Performs visual actions when an item is highlighted in combo box.
        Reimplement this if you want to make, say, custom tooltips.
        \a data is the value of highlighted item, 
        \a index is the index of data,
        \a box is the combo box,
        \a hintPos is the preferred position for item hint (under the combo if item 
            was changed in closed combo, or right to item if it was changed in open combo).
        \since 3.2
    */
    virtual void highlightComboItem(QVariant &data, int index, class QComboBox *box, QPoint hintPos) {};

    /*! Returns size hint for the current combo item.
        Reimplement this if you want to provide custom heights of combo items.
        \a data is the value of item, \a option is font/palette options. 
        \since 3.2
    */
    virtual QSize sizeHintComboItem (QVariant &data, const QStyleOptionViewItem & option) const { return QSize(); }

//
protected:
	bool			m_destroyPixmaps;
};

typedef QListIterator<TItem*> TItemListIterator;

//***************************************************************************//

#endif
