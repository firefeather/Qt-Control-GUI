#ifndef TCOORD_H
#define TCOORD_H

#include <iisDefines.h>

#include <iisVB.h>

/****************************************************************************/

// TPointValidator class

// A Validator for Point values.

/****************************************************************************/

/*! \class TPointValidator
 * \nosubgrouping
 * \brief A validator for QPoint values.
 */

class LIBVB4_EXPORT TPointValidator : public TValueValidator
{
public:
    /// Constructor.
	/// @param xmin is the minimum allowed x value, INT_MIN by default.
    /// @param xmax is the maximum allowed x value, INT_MAX by default.
	/// @param ymin is the minimum allowed y value, INT_MIN by default.
    /// @param ymax is the maximum allowed y value, INT_MAX by default.
    TPointValidator(QObject *parent = 0, int xmin = INT_MIN, int xmax = INT_MAX, int ymin = INT_MIN, int ymax = INT_MAX);
	// Destructor.
	virtual ~TPointValidator() {};

	virtual EValidateResult checkValue(QVariant& value);


	/// Standard point input validator.
	static TPointValidator	*s_PointValidator;

protected:
	int	m_xmin, m_xmax, m_ymin, m_ymax;
};

/****************************************************************************/

// TPointValue class

// Represents a point value class which allows to deal with QPoint

/****************************************************************************/

/*! \class TPointValue
 * \nosubgrouping
 * \brief A point value class which allows to work with QPoint values.
 */

class LIBVB4_EXPORT TPointValue : public TBaseValue
{
public:
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name			is the name of the item (text shown for user).
	/// @param page			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TPointValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name 			is the name of the item (text shown for user).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TPointValue (TBaseItem *parent, const QString& name,  int id = -1);
	// Destructor.
	virtual ~TPointValue();

	/// Returns point value of the item.
	QPoint			point() const		{ return m_set.value.toPoint(); }

    virtual QString valueText() const;

	/// Writes item's \a value as QPoint.
	void			writeValue(QPoint &value) 	
	{ value = m_set.value.toPoint(); }


	// Returns item's type name.
	virtual char*	ItemID() { return "TPointValue"; }

    /// Sets expandable items for all the newly created TPointValue's to \a b.
    static void		setDefaultExpandableItems(bool b)	    { s_showExpandables = b; }

protected:	
    // internal
    void    construct();

    // internal
    virtual void    doUpdateFromChild(TBaseValue *child);
    virtual void    doUpdateChildren();
    virtual void    doCreateChildren();

    TIntegerValue *ivX, *ivY;

    static bool s_showExpandables;
};



/****************************************************************************/

// TSizeValidator class

// A Validator for Size values.

/****************************************************************************/

/*! \class TSizeValidator
 * \nosubgrouping
 * \brief A validator for QSize values.
 */

class LIBVB4_EXPORT TSizeValidator : public TValueValidator
{
public:
    /// Constructor.
	/// @param wmin is the minimum allowed width value, INT_MIN by default.
    /// @param wmax is the maximum allowed width value, INT_MAX by default.
	/// @param hmin is the minimum allowed height value, INT_MIN by default.
    /// @param hmax is the maximum allowed height value, INT_MAX by default.
    TSizeValidator(QObject *parent = 0, int wmin = INT_MIN, int wmax = INT_MAX, int hmin = INT_MIN, int hmax = INT_MAX);
	// Destructor.
	virtual ~TSizeValidator() {};

	virtual EValidateResult checkValue(QVariant& value);


	/// Standard size input validator.
	static TSizeValidator	*s_SizeValidator;

protected:
	int	m_wmin, m_wmax, m_hmin, m_hmax;
};

/****************************************************************************/

// TSizeValue class

// Represents a size value class which allows to deal with QSize

/****************************************************************************/

/*! \class TSizeValue
 * \nosubgrouping
 * \brief A size value class which allows to work with QSize values.
 */

class LIBVB4_EXPORT TSizeValue : public TBaseValue
{
public:
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name			is the name of the item (text shown for user).
	/// @param page			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TSizeValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name 			is the name of the item (text shown for user).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TSizeValue (TBaseItem *parent, const QString& name,  int id = -1);
	// Destructor.
	virtual ~TSizeValue();

	/// Returns size value of the item.
	QSize			size() const		{ return m_set.value.toSize(); }

    virtual QString valueText() const;

	/// Writes item's \a value as QSize.
	void			writeValue(QSize &value) 	
	{ value = m_set.value.toSize(); }


	// Returns item's type name.
	virtual char*	ItemID() { return "TSizeValue"; }

    /// Sets expandable items for all the newly created TSizeValue's to \a b.
    static void		setDefaultExpandableItems(bool b)	    { s_showExpandables = b; }

protected:	
    // internal
    void    construct();

    // internal
    virtual void    doUpdateFromChild(TBaseValue *child);
    virtual void    doUpdateChildren();
    virtual void    doCreateChildren();

    TIntegerValue *ivW, *ivH;

    static bool s_showExpandables;
};



/****************************************************************************/

// TRectValidator class

// A Validator for Rectangle values.

/****************************************************************************/

/*! \class TRectValidator
 * \nosubgrouping
 * \brief A validator for QRect values.
 */

class LIBVB4_EXPORT TRectValidator : public TSizeValidator
{
public:
	/// @param wmin is the minimum allowed width value, INT_MIN by default.
    /// @param wmax is the maximum allowed width value, INT_MAX by default.
	/// @param hmin is the minimum allowed height value, INT_MIN by default.
    /// @param hmax is the maximum allowed height value, INT_MAX by default.
    TRectValidator(QObject *parent = 0, int wmin = INT_MIN, int wmax = INT_MAX, int hmin = INT_MIN, int hmax = INT_MAX);
	// Destructor.
	virtual ~TRectValidator() {};

	virtual EValidateResult checkValue(QVariant& value);


	/// Standard rect input validator.
	static TRectValidator	*s_RectValidator;
};

/****************************************************************************/

// TRectValue class

// Represents a rectangular value class which allows to deal with QRect

/****************************************************************************/

/*! \class TRectValue
 * \nosubgrouping
 * \brief A rectangular value class which allows to work with QRect values.
 */

class LIBVB4_EXPORT TRectValue : public TBaseValue
{
public:
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name			is the name of the item (text shown for user).
	/// @param page			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TRectValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name 		is the name of the item (text shown for user).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TRectValue (TBaseItem *parent, const QString& name,  int id = -1);
	// Destructor.
	virtual ~TRectValue();

	/// Returns rect value of the item.
	QRect			rect() const		{ return m_set.value.toRect(); }
	/// Returns size value of the item.
	QSize			size() const		{ return rect().size(); }
	/// Returns point value of the item.
	QPoint			point() const		{ return rect().topLeft(); }

    virtual QString valueText() const;

	/// Writes item's \a value as QRect.
	void			writeValue(QRect &value) 	
	{ value = m_set.value.toRect(); }


	virtual char*	ItemID() { return "TRectValue"; }

    /// Sets expandable items for all the newly created TRectValue's to \a b.
    static void		setDefaultExpandableItems(bool b)	    { s_showExpandables = b; }

protected:	
    // internal
    void    construct();

    // internal
    virtual void    doUpdateFromChild(TBaseValue *child);
    virtual void    doUpdateChildren();
    virtual void    doCreateChildren();

    TIntegerValue *ivW, *ivH;
    TIntegerValue *ivX, *ivY;

    static bool s_showExpandables;
};

/****************************************************************************/

// TRangeValidator class

// A Validator for Range values.

/****************************************************************************/

/*! \class TRangeValidator
* \nosubgrouping
* \brief A validator for Range values.
*/

class LIBVB4_EXPORT TRangeValidator : public TValueValidator
{
public:
	/// Constructor.
	/// @param xmin is the minimum allowed x value, INT_MIN by default.
	/// @param xmax is the maximum allowed x value, INT_MAX by default.
	/// @param ymin is the minimum allowed y value, INT_MIN by default.
	/// @param ymax is the maximum allowed y value, INT_MAX by default.
	TRangeValidator(QObject *parent = 0, double xmin = INT_MIN, double xmax = INT_MAX, double ymin = INT_MIN, double ymax = INT_MAX);
	// Destructor.
	virtual ~TRangeValidator() {};

	virtual EValidateResult checkValue(QVariant& value);


	/// Standard point input validator.
	static TRangeValidator	*s_RangeValidator;

protected:
	double	m_xmin, m_xmax, m_ymin, m_ymax;
};

/****************************************************************************/

// TRangeValue class

// Represents a range value class

/****************************************************************************/

/*! \class TRangeValue
* \nosubgrouping
* \brief A range value class.
*/

class LIBVB4_EXPORT TRangeValue : public TBaseValue
{
public:
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name			is the name of the item (text shown for user).
	/// @param page			is the number of the page the item should be moved (-1 by default, which means the current page).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TRangeValue (TValueBrowser *parent, const QString& name, const QString& name_left="min", const QString& name_right="max", int page = -1, int id = -1);
	/// Constructor.
	/// @param parent 		is the TValueBrowser parent (can not be 0).
	/// @param name 			is the name of the item (text shown for user).
	/// @param id			is the unique identifier of the item (-1 by default, which means no id).
	TRangeValue (TBaseItem *parent, const QString& name, const QString& name_left="min", const QString& name_right="max",  int id = -1);
	// Destructor.
	virtual ~TRangeValue();

	/// Returns range value of the item.
	QPointF			range() const		{ return m_set.value.toPointF(); }

	virtual QString valueText() const;

	/// Writes item's \a value as QPointF.
	void			writeValue(QPointF &value) 	
	{ value = m_set.value.toPointF(); }


	// Returns item's type name.
	virtual char*	ItemID() { return "TRangeValue"; }

	/// Sets expandable items for all the newly created TRangeValue's to \a b.
	static void		setDefaultExpandableItems(bool b)	    { s_showExpandables = b; }

protected:	
	// internal
	void    construct();

	// internal
	virtual void    doUpdateFromChild(TBaseValue *child);
	virtual void    doUpdateChildren();
	virtual void    doCreateChildren();

	TDoubleValue *ivX, *ivY;
	QString m_name_left,m_name_right;

	static bool s_showExpandables;
};

#endif
