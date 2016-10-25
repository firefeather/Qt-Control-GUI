#ifndef TBOOLVALUE_H
#define TBOOLVALUE_H

#include <iisDefines.h>

#include <iisVB.h>

/****************************************************************************/

class LIBVB4_EXPORT BoolValueList : public TItemList
{
public:
	// Constructor.
	BoolValueList();
};

/****************************************************************************/

// TBoolValue class

// Represents a bool value class

/****************************************************************************/

class LIBVB4_EXPORT TBoolValue : public TBaseListValue
{
public:
	// Constructor.
	// "parent" 		is the TValueBrowser parent (can not be 0).
	// "name" 			is the name of the item (text shown for user).
	// "page"			is the number of the page the item should be moved (-1 by default, which means the current page).
	// "id"				is the unique identifier of the item (-1 by default, which means no id).
	// "tt"				is the type QCheckListItem::Type of the item (QCheckListItem::Controller by default).
	TBoolValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	// Constructor.
	// "parent" 		is the TValueBrowser parent (can not be 0).
	// "name" 			is the name of the item (text shown for user).
	// "id"				is the unique identifier of the item (-1 by default, which means no id).
	// "tt"				is the type QCheckListItem::Type of the item (QCheckListItem::Controller by default).
	TBoolValue (TBaseItem *parent, const QString& name, int id = -1);
	// Destructor.
	virtual ~TBoolValue() {};

	// Assigns a value "value" to the item.
	virtual bool	setValue(const QVariant &value);
	// Assigns a special value "svalue" to the item.
	virtual void	setSpecial(uint svalue);

	// Assigns a bool value to the item.
	bool			setValue(bool val);

	// Returns bool value of the item.
	virtual bool	boolean() const		{ return m_set.value.toBool(); }

	// Writes item's "value" to bool.
	virtual void	writeValue(bool &value) 	
	{ value = m_set.value.toBool(); }

	// Returns item's type name.
	virtual char*	ItemID() { return "TBoolValue"; };

	virtual bool	autoIcon() const { return true; }
	virtual bool	autoIconActive() const { return true; }

protected:
	// Called internally to update editor geometry.
	//virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

	virtual int iconSpace() const 
	{
		return qApp->style()->pixelMetric(QStyle::PM_IndicatorWidth) + 6
			/*qApp->style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing)*/;
	}

	// Called internally when the item is clicked by mouse.
	virtual void	onClickItem(int col);

	static BoolValueList s_BoolList;
};

#endif
