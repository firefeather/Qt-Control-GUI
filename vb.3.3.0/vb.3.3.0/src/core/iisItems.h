#ifndef iisItems_H
#define iisItems_H

#include "iisItemList.h"
#include "iisEditors.h"
#include "iisValidators.h"
#include "iisValueBrowser.h"

#include <QtGui>
#include <QtCore>

class TBaseItem;
class TDropList;
class TValue;
class TButtonEditor;

/****************************************************************************/

// enum EValueType describes a type of Value Item.
// VT_VALUE		- means regular value (picked from value's pre-defined list).
// VT_SPECIAL	- means special value (picked from special values' pre-defined list).
// VT_OWN		- means own value (entered with keyboard).
enum EValueType { VT_VALUE, VT_SPECIAL, VT_OWN };

/****************************************************************************/

/*! \struct SValue iisItems.h
 * \nosubgrouping
 * \brief Struct SValue holds a value as QVariant and its flags.
 */
struct SValue {
	EValueType	type;		//!< type of the "value"
	QVariant	value;		//!< the value as QVariant
};


/****************************************************************************
**
** TValueItemDelegate class
**
*****************************************************************************/

/*! \class TValueItemDelegate
 * \nosubgrouping
 * \brief Delegate for drawing an item. May be reimplemented in child classes (but usually shouldn't).
 */
class TValueItemDelegate : public QItemDelegate
{
public:
	TValueItemDelegate(TValueBrowser * parent = 0) : QItemDelegate(parent) {};

protected:
	virtual void		paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	virtual QWidget*	createEditor( QWidget * parentW, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	virtual void		updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	virtual QSize		sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

/****************************************************************************
**
** TBaseItem class
**
** A base class for Value Browser items.
**
*****************************************************************************/

/*! \class TBaseItem
 * \nosubgrouping
 * \brief A base class for Value Browser items. Should not be used directly.
 */
class TBaseItem: public QTreeWidgetItem, virtual public QItemDelegate
{
	friend class TValueBrowser;
	friend class TValueItemDelegate;

public:
	/// Constructor.
	/// "parent" 		is the TValueBrowser parent (can not be 0).
	/// "name" 			is the name of the item (text shown for user).
	/// "page"			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TBaseItem(TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	/// Constructor.
	/// "parent" 		is the TValueBrowser parent (can not be 0).
	/// "name" 			is the name of the item (text shown for user).
	/// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TBaseItem(TBaseItem *parent, const QString& name, int id = -1);
	// Destructor.
	virtual ~TBaseItem();

	/*! Sets the item's check flag enabled state to \a b.
	*/
	void			setCheckable( bool b = true );

	/*! Sets the item's check state to \a state.
	*/
	void			setChecked( Qt::CheckState state );

	/*! Sets the item enabled astate to \a b.
	*/
	virtual void	setEnabled ( bool b = true );
	
	/*! Sets the item visible state to \a b"
	*/
	virtual void	setVisible ( bool b = true );

#if QT_VERSION < 0x040200
	/*! Sets the item expand state to \a b.
	*/
    void            setExpanded( bool b )
    { browser()->setItemExpanded(this, b); }
#endif

	/*! Returns item's hint.
	*/
	const QString&	hint() const		{ return m_hint; }
	
	/*! Assigns a hint "hint" to the item.
	*/
	virtual void	setHint ( const QString& hint );

	/*! Sets icon to the item.
		\since 3.3
	*/
	inline void setItemIcon(const QIcon &icon) { QTreeWidgetItem::setIcon(0, icon); }

	/*! Returns item's icon.
		\since 3.3
	*/
	inline QIcon itemIcon() const { return QTreeWidgetItem::icon(0); }

	/*! Returns item's parent.
	*/
	TValueBrowser*	browser() const		{ return (TValueBrowser*)treeWidget(); };

	/*! Returns item's page.
	*/
	unsigned int	page() const		{ return m_page; }


	/*! Item's identifier.
	*/
	int				id;

	/*! Returns item's type name.
	*/
	virtual char*	ItemID() = 0;

	/*! Returns true if the item is editable, false if not.
	*/
	virtual bool	editable() = 0;

	/*! Returns true if the item is visible, false if not.
        Visibility is page-independent.
        \since 3.2
	*/
    virtual bool	visible()   { return m_visible; }

	/*! Returns true if the item provides a value, false if not.
	*/
	virtual bool	hasValue() = 0;

	/*! Copies current data into stored set.
	*/
	virtual void	storeSet(bool emitChanges = false);

	/*! Copies stored data into current set.
	*/
	virtual void	restoreSet();



	/// Sets check property name to \a name. This name will be used for settings file while TValueBrowser::writeDataSet() or
	/// TValueBrowser::readDataSet() is called, to store item's check state. 
	/// If \a name is empty, no check property will be stored/read to/from settings file.
	/// \sa checkProperyName(), addCheckProperty()
	void	setCheckPropertyName(const QString& name);

	/// Reads check property name.
	/// \sa setCheckPropertyName(), addCheckProperty()
	const QString&	checkProperyName() const { return m_checkPropertyName; }

	/// Sets linked value for check flag to \a linked, and type to \a type.
	/// If \a linked is 0 then no linked value will be connected with check.
	/// \sa addCheckProperty()
	void	setCheckLinked(void *linked, int type = TValueBrowser::LT_BOOL);

	/*! Quick method to enable check state, set check property name to \a name,
		linked value to \a linked, and type to \a type.
		\sa setCheckLinked(), checkProperyName(), setCheckPropertyName()
	*/
	void	addCheckProperty(const QString& name = QString::null, void *linked = 0, int type = TValueBrowser::LT_BOOL);


	/*! Reads value from linked variable and puts it to the browser.
	*/
	virtual void	updateFromLinked();

	/*! Stores current value from the browser to linked one.
	*/
	virtual void	updateLinked();


	/*! Stores linked value to \a set.
	*/
	virtual void	writeDataSet(QSettings &set) = 0;

	/*! Reads linked value from \a set.
	*/
	virtual void	readDataSet(QSettings &set) = 0;


	/*! Sets level of the item to \a level.
    *   This is need to be used for own values only, followed with quick-added children.
	*/
    bool            setItemLevel(int level = 0);

protected:	
	// Called by constructors.
	void			init(const QString& name, int page, int id);

	// Called internally when the item receives the focus.
	virtual void	onEnterItem() { browser()->setFocus(); };
	// Called internally when the item leaves the focus.
	virtual void	onLeaveItem() {};

	// Called internally when the item is clicked by mouse.
	virtual void	onClickItem(int /*col*/) {};

    /*! Called internally when the item changes.
        \since 3.2
    */
    virtual void    onChangeItem(int /*col*/);

	// internal
	void			updateCheckState();

	// Sets up item's private properties. Called internally.
	virtual void	setup(void);

	// internal
	virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) 
	{
		QItemDelegate::paint(painter, option, index);
	}

	virtual void	drawDisplay(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, const QString & text) const;

    /*! Returns icon for data picked out of list. 
     *  Should be reimplemented in subclasses in order to provide own icons.
    */
    virtual const QIcon   itemIcon()
    {
        return QIcon();
    }

    /*! Returns size between icon and editor line. 
     *  Sometimes should be reimplemented in subclasses in order to provide correct distance.
    */
	virtual int iconSpace() const 
	{
		return qApp->style()->pixelMetric(QStyle::PM_SmallIconSize) + 6;
	}



	// Holds number of item's page.
	int	            m_page;
	// Holds item's hint.
	QString			m_hint;
	// Holds item's check state
	Qt::CheckState	m_check, m_storedCheck;
    // Holds item's visibility
    bool            m_visible;

	// Used for binding
	QString			m_checkPropertyName;
	void			*m_checkLinked;
	int				m_checkLinkedType;

	// Statics for last values
	static int		s_defaultPage;
};


/****************************************************************************
**
** TBaseValue class
**
** A base class for Value Browser value items.
**
*****************************************************************************/

/*! \class TBaseValue
 * \nosubgrouping
 * \brief A base class for items which provide editable values. 
 * Internally it provides support of simple QVariant-based value and all the 
 * facilities for drawing, editing, validating, storing and reading.
 */

class TBaseValue: public TBaseItem
{
	friend class TValueBrowser;

public:
	/// Constructor.
	/// "parent" 		is the TValueBrowser parent (can not be 0).
	/// "name" 			is the name of the item (text shown for user).
	/// "page"			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TBaseValue(TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	/// Constructor.
	/// "parent" 		is the TValueBrowser parent (can not be 0).
	/// "name" 			is the name of the item (text shown for user).
	/// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TBaseValue(TBaseItem *parent, const QString& name, int id = -1);
	// Destructor.
	virtual ~TBaseValue();

	/// Assigns a value "value" to the item.
	virtual bool	setValue(const QVariant &value);
	/// Assigns a special value "svalue" to the item.
	virtual void	setSpecial(uint svalue);

	/// Returns item's value.
	virtual SValue&	value() 	{ return m_set; }

    /// Returns item's textual representation.
    /// \since 3.2
    virtual QString valueText() const { return string(); }

    /// Returns current item's value as QVariant.
	virtual QVariant	variant() const 	{ return m_set.value; }
    /// Casts current item's value to integer.
    /// Use this method only if you're sure that value has type of integer,
    /// or else result may be unexpected.
	virtual int			number() const		{ return m_set.value.toInt(); }
    /// Casts current item's value to double.
    /// Use this method only if you're sure that value has type of double,
    /// or else result may be unexpected.
	virtual double		numberF() const		{ return m_set.value.toDouble(); }
    /// Casts current item's value to string.
	virtual QString		string() const		{ return m_set.value.toString(); }

	/// Writes item's "value" to variant.
	virtual void	writeValue(QVariant &value) 	
	{ value = m_set.value; }
	/// Writes item's "value" to int.
	virtual void	writeValue(int &value) 	
	{ value = ceil(m_set.value.toDouble()); }
	/// Writes item's "value" to double.
	virtual void	writeValue(double &value) 	
	{ value = m_set.value.toDouble(); }
	/// Writes item's "value" to string.
	virtual void	writeValue(QString &value) 	
	{ value = m_set.value.toString(); }

	/// Returns item's type name.
	virtual char*	ItemID() { return "SValue"; };

	/// Returns true as the item is editable.
	virtual bool	editable() { return true; };

    virtual void	setVisible ( bool b = true );

	/// Returns true as the item provides a value.
	virtual bool	hasValue() { return true; }

	/// Returns a Validator assigned to the item.
	TValueValidator*	validator() const	{ return m_validator; }
	/// Assigns a Validator "validator" to the item.
	void			setValidator(TValueValidator* validator);

	/// Assigns external Button Editor to the item.
	void			setButtonEditor(TButtonEditor *editor);
	/// Returns Button Editor assigned to the item.
	TButtonEditor*	buttonEditor() const { return m_buttonEditor; }

	/// Stores current data.
	virtual void	storeSet(bool emitChanges = false);
	/// Restores data.
	virtual void	restoreSet();
	/// Compares two values and returns result of equality.
	virtual inline bool	valueChanged(const QVariant &v1, const QVariant &v2) { return v1 != v2; }
	/// Checks if value (NOT checkstate) has been changed.
	inline bool valueChanged() const { return m_wasChanged; }

    /*! Allows special values if \a allow is set to true.
        \since 3.2
        \sa isSpecialsAllowed()
    */
    void    allowSpecials(bool allow = true);

    /*! Returns true if special values allowed, false otherwise.
        \since 3.2
        \sa allowSpecials()
    */
    bool    isSpecialsAllowed() const { return m_SpecialsAllowed; }

	// Returns the private list of the static values, or 0 if there is no list assigned.
	virtual TItemList*	list() const	{ return 0; }


	/// Sets property name to \a name. A property name will be used for settings file while TValueBrowser::writeDataSet() or
	/// TValueBrowser::readDataSet() is called. 
	/// If \a name is empty, no property will be stored/read to/from settings file.
	/// \sa properyName()
	void	setPropertyName(const QString& name);

	/// Reads property name.
	/// \sa properyName()
	const QString&	properyName() const { return m_propertyName; }

	/// Sets linked value to \a linked with type \a type.
	/// If \a linked is 0 then no linked value will be connected.
    void	setLinked(void *linked, int type = TValueBrowser::LT_OWNTYPE);

	///// Sets getter method to \a getter with type \a type.
	///// If \a getter is 0 then no getter will be connected.
 //   void    setGetter(void *getter, void *obj, int type);

	///// Sets setter method to \a setter with type \a type.
	///// If \a setter is 0 then no setter will be connected.
 //   void    setSetter(void *getter, void *obj, int type);

	/*! Reads value from linked variable and puts it to the browser.
	*/
	virtual void	updateFromLinked();

	/*! Stores current value from the browser to linked one.
	*/
	virtual void	updateLinked();


	/*! Stores linked value to \a set.
	*/
	virtual void	writeDataSet(QSettings &set);

	/*! Reads linked value from \a set.
	*/
	virtual void	readDataSet(QSettings &set);




    /*! Shows expandable items if \a on is true, hides if false.
        Most common usage is to call this from the parent item's constructor,
        passing default children state as the parameter.
     * \since 3.2
    */
    void            showChildren(bool on = true);



	// Called internally when the item receives the focus.
	virtual void	onEnterItem();
	// Called internally to set this item as current in Item Editor and Drop-Down box.
	virtual bool	setCurrentItem();
	// Called internally when the item leaves the focus.
	virtual void	onLeaveItem();

	// Called internally to fill Item Editor when the item is selected.
	virtual void	fillEditor();
	// Called internally to clarify is icons are created on-the-fly.
	virtual bool	autoIcon() const { return false; }
	// Called internally.
	virtual bool	autoIconActive() const { return !m_item && autoIcon(); }

protected:	
	// Sets up item's visual appearance. Called internally.
	virtual void	setupValue(void);
	
	// Called internally when the item has been picked from the list "list"
	// with id "id", and its value is "text".
	// If "list" is 0, "text" contains value entered with keyboard.	
	virtual void	valuePicked(TItemList *list, TItem *item, const QVariant &value);

	// Called internally to validate the \a value using the validator assigned to the item.
	virtual TValueValidator::EValidateResult	validate(QVariant& value);

//	// Called internally to create Item Editor when the item is selected.
	virtual QWidget*	createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;


	virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;


	// internal
	virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index );  



	bool			doUpdateLinked(QVariant &var);
	bool			doUpdateFromLinked(QVariant &var);



    /*! Called internally when the child item changes its value.
        \since 3.2
    */
    virtual void    updateFromChild(TBaseValue *child);

    /*! Called from updateFromChild().
        Reimplement this in order to get current item's value and update the children.
        \since 3.2
    */
    virtual void    doUpdateFromChild(TBaseValue *child) {};
    
    /*! Called internally when the parent item changes its value.
        \since 3.2
    */
    virtual void    updateChildren();

    /*! Called from updateChildren().
        Reimplement this in order to get current item's value and update the children.
        \since 3.2
    */
    virtual void    doUpdateChildren() {};

    /*! Reimplement this in order to create children items.
        Call registerChild(child) after the \a child has been created.
        \since 3.2
    */
    virtual void    doCreateChildren() {}
    
    /*! Must be called from doCreateChildren() in order to register \a child item.
        \since 3.2
    */
    void registerChild(TBaseValue *child);

    /*! Reimplement this in order to set visible state of children items to \a on.
        \since 3.2
    */
    virtual void    doShowChildren(bool on) { Q_UNUSED(on); }

    /*! Marks item as a child (it will be drawn in another color).
        Normally, it is called from registerChild() and should not be called directly.
        \since 3.2
    */
    void            setChildMode() { m_child = true; }




	SValue			m_set, m_storedSet;
    bool            m_child;
	mutable TDropList		*m_editor;
	TValueValidator	*m_validator;
	TButtonEditor	*m_buttonEditor;
	bool			m_wasChanged, m_SpecialsAllowed, m_childLock;
	TItem			*m_item;

	void			*m_linked;
    //void            *m_getter, *m_setter;
    //void            *m_getterObj, *m_setterObj;
	int				m_propertyType;
	QString			m_propertyName;


    // Used for expandable items
    bool m_expandables, m_exp_created;
    QList<TBaseValue*> m_children;
};

/****************************************************************************
**
** TBaseGroup class
**
** Represents Value Browser group items (i.e. non-editable group separators).
**
****************************************************************************/

/*! \class TBaseGroup
 * \nosubgrouping
 * \brief Representsgroup items (i.e. non-editable group separators).
 */

class TBaseGroup: public TBaseItem
{
public:
	/// Constructor.
	/// "parent" 		is the TValueBrowser parent (can not be 0).
	/// "name" 			is the name of the item (text shown for user).
	/// "page"			is the number of the page the item should be moved (-1 by default, which means the current page).
    /// "subgroup"      if true, group is drawn as a subgroup (lighter).
	/// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TBaseGroup(TValueBrowser *parent, const QString& name, int page = -1, bool subgroup = false, int id = -1);
	// Destructor.
	virtual ~TBaseGroup() {};

	/// Returns item's type name.
	virtual char*	ItemID() { return "TBaseGroup"; };
	/// Returns false (item is not editable).
	virtual bool	editable() { return false; };
	/// Returns false (item does not provide a value).
	virtual bool	hasValue() { return false; }

	/// Sets section name to \a name. A section will be used for settings file while TValueBrowser::writeDataSet() or
	/// TValueBrowser::readDataSet() is called. 
	/// If \a name is empty, no section will be used.
	/// \sa sectionName()
	void	setSectionName(const QString& name);

	/// Reads section name.
	/// \sa setSectionName()
	const QString&	sectionName() const { return m_sectionName; }


	/*! Will create a section in \a set.
		\sa sectionName(), setSectionName()
	*/
	virtual void	writeDataSet(QSettings &set);

	/*! Will set current section in \a set.
		\sa sectionName(), setSectionName()
	*/
	virtual void	readDataSet(QSettings &set);

protected:
	// internal
	virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index );

	QString			m_sectionName;
};

/****************************************************************************
**
** TBaseListValue class
**
** Represents abstract Value Browser value items with list support.
**
*****************************************************************************/

/*! \class TBaseListValue
 * \nosubgrouping
 * \brief Represents abstract value item with list support. Should not be used directly.
 */

class TBaseListValue: public TBaseValue
{
public:
	// Constructor.
	// "parent" 		is the TValueBrowser parent (can not be 0).
	// "name" 			is the name of the item (text shown for user).
	// "list"			is the private list of the static values which can be picked by the user (0 by default, which means no list).
	// "page"			is the number of the page the item should be moved (-1 by default, which means the current page).
	// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TBaseListValue(TValueBrowser *parent, const QString& name, TItemList *list = 0, int page = -1, int id = -1/*, Type tt = Controller*/);
	// Constructor.
	// "parent" 		is the TValueBrowser parent (can not be 0).
	// "name" 			is the name of the item (text shown for user).
	// "list"			is the private list of static values which can be picked by the user (0 by default, which means no list).
	// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TBaseListValue(TBaseItem *parent, const QString& name, TItemList *list = 0, int id = -1/*, Type tt = Controller*/);
	// Destructor.
	virtual ~TBaseListValue() {};

	// Returns the private list of the static values, or 0 if there is no list assigned.
	virtual TItemList*	list() const	{ return m_list; }

	// Assigns a value "value" to the item.
	virtual bool	setValue(const QVariant &value);

	// Returns item's type name.
	virtual char*	ItemID() { return "TBaseList"; };


	// Sets the private list of the static values which can be picked by the user (0 removes the list assigned previously).
	void			setList(TItemList *list);

protected:	
  
	// Called internally to validate the value "text" using the validator assigned to the item. 
	virtual TValueValidator::EValidateResult	validate(QVariant& text);
	// Called internally to fill Item Editor when the item is selected.
	virtual void	fillEditor();
	// Called internally to set this item as current in Item Editor and Drop-Down box.
	virtual bool	setCurrentItem();

protected:	
	TItemList		*m_list;
};

//****************************************************************************
//**
//** TListValue class
//**
//** Represents common Value Browser value items with list support.
//**
//*****************************************************************************/

/*! \class TListValue
 * \nosubgrouping
 * \brief Represents common value item with list support. May be used directly
 * with TItemList class as drop-down list of items, or may be subclassed in
 * order to build own list-based values.
 */

class TListValue: public TBaseListValue
{
public:
	/// Constructor.
	/// "parent" 		is the TValueBrowser parent (can not be 0).
	/// "name" 			is the name of the item (text shown for user).
	/// "list"			is the private list of the static values which can be picked by the user (0 by default, which means no list).
	/// "page"			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TListValue(TValueBrowser *parent, const QString& name, TItemList *list = 0, int page = -1, int id = -1/*, Type tt = Controller*/);
	/// Constructor.
	/// "parent" 		is the TValueBrowser parent (can not be 0).
	/// "name" 			is the name of the item (text shown for user).
	/// "list"			is the private list of static values which can be picked by the user (0 by default, which means no list).
	/// "id"				is the unique identifier of the item (-1 by default, which means no id).
	TListValue(TBaseItem *parent, const QString& name, TItemList *list = 0, int id = -1/*, Type tt = Controller*/);
	// Destructor.
	virtual ~TListValue() {};

	// Sets the private list of the static values which can be picked by the user (0 removes the list assigned previously).
//	void			setList(TItemList *list);
	/// Allows to use own values if "ownText" is true.
	void			allowOwnValues(bool allow = true);
	/// Returns true if the usage of the own values is allowed, false if not.
	bool			isOwnValues() const	{ return m_useOwnText; }
	/// Allows to append own text to the private list if "allow" is true.
	void			allowListAppending(bool allow = true);
	/// Returns true if the appending of the own values to the private list is allowed, false if not.
	bool			isListAppending() const	{ return m_allowListAppend; }

	/// Sets default behavior for using own values in new list items to \a b.
	static void		setDefaultUseOwnValues(bool b)			{ s_useOwnText = b; }
    /// Sets default behavior for appending new values to new list items to \a b.
	static void		setDefaultAllowListAppending(bool b)	{ s_allowListAppend = b; }

	/// Assigns a value "value" to the item.
	virtual bool	setValue(const QVariant &value);

	/// Returns item's type name.
	virtual char*	ItemID() { return "TList"; };

protected:	
	/// Called internally to validate the value "text" using the validator assigned to the item. 
	virtual TValueValidator::EValidateResult	validate(QVariant& value);
	/// Called internally to set this item as current in Item Editor and Drop-Down box.
	virtual bool	setCurrentItem();
	/// Called internally to append a value to the end of list; if the list is empty - creates it.
	virtual void	appendValue(const QVariant& val);

protected:	
	bool			m_useOwnText, m_allowListAppend;

	// Statics for last values
	static bool		s_useOwnText, s_allowListAppend;
};

/***************************************************************************/

#endif
