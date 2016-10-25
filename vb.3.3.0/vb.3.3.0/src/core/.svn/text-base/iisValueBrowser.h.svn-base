/****************************************************************************
**
** iisValueBrowser 3.3.0
**
*****************************************************************************/

#ifndef TVALUEBROWSER_H
#define TVALUEBROWSER_H

#include "iisItemList.h"
#include "iisValidators.h"
#include "iisEditors.h"
#include "iisDropList.h"

#include <QtGui>
#include <QtCore>

class TBaseItem;
class TBaseGroup;
class TBaseValue;
class TIntegerValue;
class TDoubleValue;
class TListValue;
class TBoolValue;
class TColorValue;
class TColorList;
class TTextValue;
class TFontValue;
class TFontList;
class TPointValue;
class TSizeValue;
class TRectValue;
class TRangeValue;
class TPathValue;
class TPointValidator;
class TSizeValidator;
class TRectValidator;
class TRangeValidator;

struct SValue;


#define VB_VERSION_STR  "3.3.0"

#define ToolTipFontRole     Qt::UserRole + 10

/****************************************************************************
**
** SpecialValueList class
**
** Represents a list of pre-defined (special) values.
**
*****************************************************************************/

/*! \class SpecialValueList iisValueBrowser.h
 * \nosubgrouping
 * \brief A list representing special values of an item.

	If a Special Value List is set for TValueBrowser, then every item editor
	will be filled with special values in the list. 
 */

class SpecialValueList : public TItemList
{
public:
	// Constructor.
	SpecialValueList();

	enum specialId {
			ID_NOSPECIAL	= 0,	
			ID_EMPTY		= 1,	///< Empty value
			ID_NOVALUE		= 2,	///< No value
			ID_UNUSED		= 4,	///< Unised value
			ID_DEFAULT		= 8,	///< Default value
			ID_BAD			= 16	///< Bad value
	};
}; 


/****************************************************************************
**
** TValueBrowser class
**
*****************************************************************************/

/*! \class TValueBrowser iisValueBrowser.h
 * \nosubgrouping
 * \brief Value Browser widget.
 */

class TValueBrowser: public QTreeWidget
{    
	Q_OBJECT

	friend class TValueItemDelegate;

public:
	/// Constructor.
	/// @param parent 			is the parent widget (0 by default).
	/// @param name 			is the internal name (0 by default).
	/// @param itemDefHeight	is the default height of an item (18 by default).
	TValueBrowser ( QWidget * parent = 0, unsigned int itemDefHeight = 18 );
	
	// Destructor.
	virtual			~TValueBrowser() {};

	/*! Returns an item with identified "id", 0 if not found.
	*/
	TBaseItem*		getItem(int id);
	
	/*! Returns the default height of an item.
	*/
	unsigned int	itemDefHeight() const	{ return m_ItemDefHeight; }

	/*! Returns the revert button icon.
	 *  \sa setRevertIcon(), s_defaultRevertIcon
	 *  \since 3.3.0
	*/
	const QIcon&	revertIcon() const { return m_revertIcon; }
	/*! Sets the revert button icon to \a icon.
	 *  \sa revertIcon()
	 *  \since 3.3.0
	*/
	void			setRevertIcon(const QIcon &icon) { m_revertIcon = icon; }

    /*! Returns width scale keeped by leftmost columns.
     *  \sa setWidthScale()
     *  \since 3.1.3
    */
    unsigned int    widthScale() const { return m_WidthScale; }
    /*! Sets width scale keeped by leftmost columns (i.e. 50 = 1/2 of total size, 75 = 3/4 and so on).
     *  \a scale can be in range between 5 and 95.
     *  \sa widthScale()
     *  \since 3.1.3
    */
    void            setWidthScale(unsigned int scale);


	/*! Returns the number of the current page.
	*/
	int				page() const			{ return m_page; }

	/*! Returns true if Value Browser is in edit mode, false if not.
	*/
	bool			checkEditor() const		{ return !m_Edit; }

	/*! Returns Special Value List assigned privately.
		\sa setSpecialList()
	*/
	SpecialValueList*	specialList() const		{ return m_specialList; }
	
	/*! Sets Special Value List "nameList" privately. By default, TValueBrowser does not have any Special Value List set.
	// If "nameList" is NULL, list will be removed.
	// If "freeOld" is true, list assigned previously will be deleted.
		\sa specialList()
	*/
	void			setSpecialList(SpecialValueList* nameList = 0, bool freeOld = false);

	/*! Sets change information mode (default is true).
		If the value is true, then all the changes made before the data set was stored
		will be highlighted.
		\sa changeInfo()
	*/
	void			setChangeInfo(bool ci);
	
	/*! Returns change information mode.
		\sa setChangeInfo()
	*/
	bool			changeInfo() const		{ return m_changeInfo; }


	/*! Enumerates types of linked values.
	*/
	enum	ELinkedType {
		LT_INT,
		LT_UINT,
		LT_LONG,
		LT_ULONG,
		LT_LONGLONG,
		LT_ULONGLONG,
		LT_FLOAT,
		LT_DOUBLE,
		LT_BOOL,
		LT_STRING,
		LT_COLOR,
        LT_FONT,
        LT_SIZE,
        LT_POINT, // for Point and Range values
        LT_RECT,
		LT_VARIANT,
        LT_OWNTYPE
	};

	/*! Enumerates storing policies.
		\sa storingPolicy(), setStoringPolicy(), writeDataSet(), readDataSet()
	*/
	enum	EStoringPolicy {
		SP_RAW,		///< imports/exports current dataset (shown in Browser)
		SP_STORED,	///< imports/exports stored dataset
		SP_LINKED	///< imports/exports linked data
	};

	/*! Sets level of \a item to \a level.
    *   This is need to be used for own values only, followed with quick-added children.
	*/
    bool            setItemLevel(TBaseItem *item, int level);

	/*! Quick-add method to add a page and make it current one.
	*/
	int				addPage();

	/*! Quick-add method to add a group.
	*/
	TBaseGroup*		addGroup(const QString &name, const QString &sectionName = QString::null, bool sub = false);

	/*! Quick-add method to add an integer.
	*/
	TIntegerValue*	addInt(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, int type = LT_INT, TIntValidator *validator = 0);

	/*! Quick-add method to add real value.
	*/
	TDoubleValue*	addReal(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, int type = LT_DOUBLE, TDoubleValidator *validator = 0);

	/*! Quick-add method to add a list. By default, own values and autoappending are off.
	*/
	TListValue*		addList(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, int type = LT_INT, TItemList *list = 0, bool own = false, bool append = false);

	/*! Quick-add method to add a string or ordinal variant.
	*/
	TBaseValue*		addValue(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, int type = LT_STRING, TValueValidator *validator = 0);

	/*! Quick-add method to add a boolean.
	*/
	TBoolValue*		addBool(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0);

	/*! Quick-add method to add a color. By default, own values are on, autoappending is off, and the list is filled by 16 base colors.
	*/
	TColorValue*	addColor(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, TColorList *list = 0);

	/*! Quick-add method to add a multiline text.
	*/
	TTextValue*		addText(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0);

	/*! Quick-add method to add a font.
        \since 3.2
	*/
	TFontValue*	    addFont(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, TFontList *list = 0);

	/*! Quick-add method to add a point.
        \since 3.2.1
	*/
	TPointValue*	addPoint(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, TPointValidator *validator = 0);
                                        
	/*! Quick-add method to add a size.
        \since 3.2.1
	*/
	TSizeValue*	    addSize(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, TSizeValidator *validator = 0);

	/*! Quick-add method to add a rectangle.
        \since 3.2.1
	*/
	TRectValue*	    addRect(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, TRectValidator *validator = 0);

	/*! Quick-add method to add a range.
        \since 3.2.4
	*/
	TRangeValue*	addRange(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0, TRangeValidator *validator = 0);

    /*! Quick-add method to add a file path.
	*/
	TPathValue*		addPath(int level, const QString &name, const QString &hint = QString::null, const QString &propertyName = QString::null,
						void *linked = 0);

	/*! Sets current storing policy to \a policy. 
		\sa storingPolicy(), writeDataSet(), readDataSet()
	*/
	void			setStoringPolicy(EStoringPolicy policy);

	/*! Retrieves current storing policy. Default is TValueBrowser::SP_RAW.
		\sa setStoringPolicy(), writeDataSet(), readDataSet()
	*/
	EStoringPolicy	storingPolicy() const { return m_storingPolicy; }


	/*! Stores linked values to \a set using current storing policy.
		\sa storingPolicy(), setStoringPolicy(), readDataSet()
	*/
	void			writeDataSet(QSettings &set);

	/*! Reads linked values from \a set using current storing policy.
		\sa storingPolicy(), setStoringPolicy(), writeDataSet()
	*/
	void			readDataSet(QSettings &set);


    void            updateEditorGeometries() 
    { QTreeWidget::updateEditorGeometries(); }




	/*! Holds Special Value List which is common for all the Value Browsers.
	*/
	static SpecialValueList	s_specialList;
	/*! Holds default revert icon which is common for all the Value Browsers
	 * (remains until setRevertIcon() called).
	 * \since 3.3
	 * \sa revertIcon(), setRevertIcon()
	*/
	static QIcon s_defaultRevertIcon;

public slots:
	/*! Clears all items and pages count.
     * \since 3.2.4
	*/
    void            clear();

	/*! Makes a page with index "page" current. Does nothing if such page does not exist.
	*/
	void			switchPage(int page);

	/*! Makes a page with index "page" current. Does nothing if such page does not exist.
     *  Added for compatibility with MVC-based views. For example, this slot can be connected
     *  directly with QAbstractItemView::activated() signal, to provide listview-based menu.
     * \since 3.2.4
	*/
	void			switchPage(const QModelIndex & index);

	/*! Stores current data. 
		The data is stored into internal dataset.
		\sa restoreSet()
	*/
	void			storeSet(bool emitChanges = false);

	/*! Restores data previously stored in internal dataset by storeSet() method.
		\sa storeSet()
	*/
	void			restoreSet();


	/*! Reads values from linked variables and puts them to the browser.
	*/
	void			updateFromLinked();

	/*! Stores current values from the browser to linked ones.
	*/
	void			updateLinked();

signals:
	/*! Emitted when the value of the item "item" is changed.
	*/
	void			ItemChanged(TBaseItem *item);
	
	/*! Emitted when the item's "item" hint is changed, or item has become current.
	// You can connect this signal with your slot which will display this hint.
	// @param item  	item caused hint change,
	// @param itemName	is the name of the item,
	// @param hint		is the hint text.
	*/
	void			HintChanged(const QString &hint, TBaseItem *item = 0, const QString &itemName = 0);

	void			ItemHint(const QString &hint);
	
	/*! Emitted when the value of the item "item" is picked by the user.
	*/
	void			ValuePicked(TBaseItem *item, const SValue& value);
	
	///*! Emitted when the value or check state of the item "item" is changed while storing.
	//*/
	//void			ValueChanged(TBaseItem *item, TValue *linkedValue);

protected slots:
	// Called internally when the item "item" changes its selection state.
	virtual void	selectionChanged ( QTreeWidgetItem *item, QTreeWidgetItem* prevItem );
	// Called internally when the item "item" changes its state.
	virtual void	onItemChanged ( QTreeWidgetItem *item, int );
	// Called internally when an item has been picked from the list "list"
	// with id "id", and its value is "text".
	// If "list" is 0, "text" contains value entered with keyboard.
	void			valuePicked(TItemList *list, TItem *item, const QVariant &value);
	// Called internally when an editor has invoked restore command for current value under editing.
	void			restoreSetCurrent();

	virtual void	onItemClicked ( QTreeWidgetItem *item, int );

protected:
	// Called internally when the item's "item" hint is changed.
	// Default implementation emits "HintChanged(...)" signal.
	void			doHintChange(TBaseItem *item);
	// Called internally when the item "item" is changed.
	// Default implementation emits "ItemChanged(item)" signal.
	virtual void	onItemChanged(TBaseItem *item) { emit ItemChanged(item); }

	// Resize event handler
	virtual void	resizeEvent ( QResizeEvent * event );
	// Hide event handler
	virtual void	hideEvent ( QHideEvent * event );
    // General event handler
    virtual bool	viewportEvent ( QEvent * event );

	// members
	unsigned int	m_ItemDefHeight, m_WidthScale;
	int				m_page;
	SpecialValueList *m_specialList;
	bool			m_Edit, m_changeInfo;

	QList<TBaseItem*>	m_levels;
	EStoringPolicy	m_storingPolicy;

	QIcon			m_revertIcon;
};

/****************************************************************************/

#endif
