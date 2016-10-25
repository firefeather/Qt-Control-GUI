#include "iisBoolValue.h"

/****************************************************************************/

BoolValueList::BoolValueList(void) : TItemList()
{
	setItem(false, QObject::tr("False"));
	setItem(true, QObject::tr("True"));
}

BoolValueList TBoolValue::s_BoolList;

/****************************************************************************/

// TBoolValue class

// Represents a boolean value class

/****************************************************************************/

TBoolValue::TBoolValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseListValue(parent, name, &s_BoolList, page, id)
{
	setValue(false);
}

TBoolValue::TBoolValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseListValue(parent, name, &s_BoolList, id)
{
	setValue(false);
}

bool TBoolValue::setValue(const QVariant &value)
{
	return setValue(value.toBool());
}

bool TBoolValue::setValue(bool val)
{
	setCheckState(1, val ? Qt::Checked : Qt::Unchecked);
	return TBaseListValue::setValue(QVariant(val));
}

void TBoolValue::setSpecial(uint svalue)
{
	setCheckState(1, Qt::PartiallyChecked);
	TBaseListValue::setSpecial(svalue);
}

//void TBoolValue::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
//{
//	if (editor) 
//		editor->setGeometry(option.rect.adjusted(
//			qApp->style()->pixelMetric(QStyle::PM_IndicatorWidth) + qApp->style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing),
//			0,-1,0));
//}

void TBoolValue::onClickItem(int col)
{
	if (col) {
		if (m_set.type == VT_SPECIAL)
			setValue(true);
		else
			setValue(!boolean());
	}
}
