#ifndef TCOLOR_H
#define TCOLOR_H

#include <iisDefines.h>

#include <iisVB.h>

/****************************************************************************/

// TColorValidator class

// A Validator for Color values.

/****************************************************************************/

/*! \class TColorValidator
 * \nosubgrouping
 * \brief A validator for color values.
 */

class LIBVB4_EXPORT TColorValidator : public TValueValidator
{
public:
	// Constructor.
	TColorValidator(QObject *parent = 0);
	// Destructor.
	virtual ~TColorValidator() {};

    /*!
	    Validates a \a value normally passed from item editor. 
        \sa TValueValidator
    */
	virtual EValidateResult checkValue(QVariant& value);


	/// Standard color input validator.
	static TColorValidator	s_ColorValidator;
};

/****************************************************************************/

// TColorList class

// A list holding Color values.

/****************************************************************************/

/*! \class TColorList
 * \nosubgrouping
 * \brief A common list of color values.
 */

class LIBVB4_EXPORT TColorList : public TItemList
{
public:
	TColorList();

	virtual TItem*	setItem (const QVariant &_value, const QString &_text = QString::null, QIcon *_icon = 0);
	virtual TItem*	findItem(const QVariant &_value);

    /// Returns static list of colors registered in Qt (as returns QColor::colorNames()).
	static TColorList& registeredColorList();
    /// Returns static list of 16 predefined colors.
	static TColorList& baseColorList();

    /// Returns icon representing \a color with width \a w and height \a h.
	static QIcon	makeIcon(const QColor &color, int w = 24, int h = 12);

protected:
	static TColorList	m_regColorList;
	static TColorList	m_baseColorList;
};

/****************************************************************************
**
** TColorButtonEditor class
**
*****************************************************************************/

class LIBVB4_EXPORT TColorButtonEditor : public TPopupButtonEditor
{
protected:
	virtual void	buttonClicked();
};

//***************************************************************************//
//
// TColorComboEditor Class
//
//***************************************************************************//

class LIBVB4_EXPORT TColorComboEditor : public TComboEditor
{
public:
	TColorComboEditor();
	virtual ~TColorComboEditor() {};

	virtual void	invoke(TBaseValue *item);
};

/****************************************************************************/

// TColorValue class

// Represents a color value class which allows to store QColor

/****************************************************************************/

/*! \class TColorValue
 * \nosubgrouping
 * \brief A color value class which allows to store color values as QColor.
 */

class LIBVB4_EXPORT TColorValue : public TListValue
{
public:
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name			is the name of the item (text shown for user).
	/// @param list			is the private list of the static values which can be picked by the user (s_colorList by default).
	/// @param page			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TColorValue (TValueBrowser *parent, const QString& name, TColorList *list = &TColorList::baseColorList(), int page = -1, int id = -1);
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name 			is the name of the item (text shown for user).
	/// @param list			is the private list of static values which can be picked by the user (s_colorList by default).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TColorValue (TBaseItem *parent, const QString& name, TColorList *list = &TColorList::baseColorList(), int id = -1);
	// Destructor.
	virtual ~TColorValue();

	/// Returns color value of the item.
	QColor			color() const		{ return QColor(m_set.value.toString()); }

	/// Writes item's \a value as QColor.
	void			writeValue(QColor &value) 	
	{ value = QColor(m_set.value.toString()); }


	virtual void	writeDataSet(QSettings &set);


	// Returns item's type name.
	virtual char*	ItemID() { return "TColorValue"; }

	virtual bool	autoIcon() const { return true; }


	// Set default init values.
    /// Sets color list for all the newly created TColorValue's to \a list.
	static void		setDefaultColorList(TColorList *list)   { s_colorList = list; }
    /// Sets usage of own values for all the newly created TColorValue's to \a b.
	static void		setDefaultUseOwnValues(bool b)			{ s_useOwnText = b; }
    /// Sets autoappending to color list for all the newly created TColorValue's to \a b.
	static void		setDefaultAllowListAppending(bool b)	{ s_allowListAppend = b; }
    /// Sets expandable items for all the newly created TColorValue's to \a b.
    static void		setDefaultExpandableItems(bool b)	    { s_showExpandables = b; }


	// Standard button editor.
	static TColorButtonEditor s_ColorButtonEditor;

	// Standard combo editor.
	static TColorComboEditor s_ColorComboEditor;

protected:	
	// Sets up item's visual appearance. Called internally.
	virtual void	setupValue(void);

	// Called internally to update editor geometry.
	//virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

	// Called internally to fill Item Editor when the item is selected.
	virtual void	fillEditor();

	// Called internally to append a value to the end of list; if the list is empty - creates it.
	virtual void	appendValue(const QVariant& val);

    // internal
    virtual const QIcon   itemIcon();

    // internal
    void    construct();

    // internal
    virtual void    doUpdateFromChild(TBaseValue *child);
    virtual void    doUpdateChildren();
    virtual void    doCreateChildren();

    QIcon m_icon;

    TIntegerValue *ivRed, *ivGreen, *ivBlue;

	// Statics for last values
	static TColorList* s_colorList;
	static bool		s_useOwnText;
	static bool	 s_allowListAppend;
	static bool	  s_showExpandables;
};

#endif
