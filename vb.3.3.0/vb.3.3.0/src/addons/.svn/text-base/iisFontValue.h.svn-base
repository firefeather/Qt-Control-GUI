#ifndef TFONTVALUE_H
#define TFONTVALUE_H

#include <iisDefines.h>

#include <iisVB.h>

/****************************************************************************/

// TFontList class

// A list holding Font values.

/****************************************************************************/

/*! \class TFontList
 * \nosubgrouping
 * \brief A common list of font values.
 */

class LIBVB4_EXPORT TFontList : public TItemList
{
public:
	TFontList();

	virtual TItem* setItem (const QVariant &_value, const QString &_text = QString::null, QIcon *_icon = 0);

    virtual bool paintComboItem(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index,
        QVariant &data);

    virtual bool compareComboItem(const QVariant &list_value, const QVariant &item_value);

    virtual void highlightComboItem(QVariant &data, int index, QComboBox *box, QPoint hintPos);

    virtual QSize sizeHintComboItem(QVariant &data, const QStyleOptionViewItem &option) const;

    /// Returns static list of installed fonts.
	static TFontList& fullFontList();

    /// Returns static list of default font sizes.
	static TItemList& defaultFontSizeList();

    /// Returns icon representing \a font with width \a w and height \a h.
	static QIcon	makeIcon(const QFont &font, int w = 24, int h = 12);

protected:
	static TFontList	m_fullFontList;
    static TItemList    m_fontSizeList;

#if QT_VERSION >= 0x040200
    static QFontComboBox *m_fakeCombo;
    static QAbstractItemDelegate *m_fakeFontDelegate;
#endif

    static QIcon    *m_truetype;
		static QIcon  *m_bitmap;
};

/****************************************************************************
**
** TFontButtonEditor class
**
*****************************************************************************/

class LIBVB4_EXPORT TFontButtonEditor : public TPopupButtonEditor
{
protected:
	virtual void	buttonClicked();
};

//***************************************************************************//
//
// TFontComboEditor Class
//
//***************************************************************************//

class LIBVB4_EXPORT TFontComboEditor : public TComboEditor
{
public:
	TFontComboEditor();
	virtual ~TFontComboEditor() {};

	virtual void	invoke(TBaseValue *item);
};

/****************************************************************************/

// TFontValue class

// Represents a color value class which allows to store QFont

/****************************************************************************/

/*! \class TFontValue
 * \nosubgrouping
 * \brief A color value class which allows to store color values as QFont.
 */

class LIBVB4_EXPORT TFontValue : public TListValue
{
public:
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name			is the name of the item (text shown for user).
	/// @param list			is the private list of the static values which can be picked by the user (s_fontList by default).
	/// @param page			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TFontValue (TValueBrowser *parent, const QString& name, TFontList *list = &TFontList::fullFontList(), int page = -1, int id = -1);
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name 			is the name of the item (text shown for user).
	/// @param list			is the private list of static values which can be picked by the user (s_fontList by default).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TFontValue (TBaseItem *parent, const QString& name, TFontList *list = &TFontList::fullFontList(), int id = -1);
	// Destructor.
	virtual ~TFontValue();

	/// Returns font value of the item.
    QFont			font() const		{ return m_set.value.value<QFont>(); }

    virtual QString valueText() const; /*{ return font().family(); }*/


    virtual bool	setValue(const QVariant &value);

	virtual bool	valueChanged(const QVariant &v1, const QVariant &v2);

	/// Writes item's \a value as QFont.
	//void			writeValue(QFont &value) 	
	//{ value = QFont(m_set.value.toString()); }


	virtual void	writeDataSet(QSettings &set);


	// Returns item's type name.
	virtual char*	ItemID() { return "TFontValue"; }

	virtual bool	autoIcon() const { return true; }

	// Set default init values.

    /// Sets font list for all the newly created TFontValue's to \a list.
	static void		setDefaultFontList(TFontList *list)   { s_fontList = list; }
    /// Sets font size list for all the newly created TFontValue's to \a list.
	static void		setDefaultFontSizeList(TItemList *list)   { s_fontSizeList = list; }
    /// Sets font family for all the newly created TFontValue's to \a font.
    /// \sa defaultFont()
	static void		setDefaultFont(const QString& font)   { s_defaultFont = font; }
    /// Sets expandable items for all the newly created TFontValue's to \a b.
    static void		setDefaultExpandableItems(bool b)	    { s_showExpandables = b; }

    /// Returns font family for all the newly created TFontValue's.
    /// \sa setDefaultFont()
    static QFont    defaultFont()  { return s_defaultFont; }


	// Standard button editor.
	static TFontButtonEditor s_FontButtonEditor;

	// Standard combo editor.
	static TFontComboEditor s_FontComboEditor;

protected:	
    //virtual void    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	virtual void	onEnterItem();
	virtual void	onLeaveItem();

	// Sets up item's visual appearance. Called internally.
	virtual void	setupValue(void);

	// Called internally to fill Item Editor when the item is selected.
	virtual void	fillEditor();

	// Called internally to append a value to the end of list; if the list is empty - creates it.
	virtual void	appendValue(const QVariant& val);

    // internal
    virtual const QIcon   itemIcon();

    // internal
    void    construct();

    // internal
    //virtual void    updateFromChild(TBaseItem *child);
    //virtual void    updateChildren();
    virtual void    doUpdateFromChild(TBaseValue *child);
    virtual void    doUpdateChildren();
    virtual void    doCreateChildren();

    QIcon m_icon;

    //TListValue  *lvFamily;
    TBoolValue  *bvBold, *bvItalic, *bvUnderline, *bvStriked;
    TListValue   *ivSize;

	// Statics for last values
	static TFontList *s_fontList;
    static TItemList *s_fontSizeList;
	static bool		/*s_useOwnText, s_allowListAppend, */s_showExpandables;
    static QFont s_defaultFont;
	static QFont  s_tooltipFont;
};

#endif
