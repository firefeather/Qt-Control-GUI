#include "iisColorValue.h"
#include "iisNumericValue.h"

#include <QPainter>
#include <QColorDialog> 

/****************************************************************************/

// TColorValidator class

// A Validator for Color values.

/****************************************************************************/

TColorValidator::TColorValidator(QObject *parent) :
TValueValidator(parent)
{
	//m_autokill = false;
}
 
TValueValidator::EValidateResult TColorValidator::checkValue(QVariant& value)
{
	if (!value.isValid()) return VR_BAD;

    QColor color(qvariant_cast<QColor>(value));
	if (color.isValid()) {
		if (color == value) return VR_OK;
		value = color;
		return VR_FIXED;
	}

    if (value.canConvert(QVariant::UInt)) {
    	uint u = value.toUInt();
        value = QColor(u);
		return VR_FIXED;
	}

	return VR_BAD;
}

/****************************************************************************/

// TColorList class

// A list holding Color values.

/****************************************************************************/

TColorList::TColorList() : TItemList(true)
{
}

QIcon TColorList::makeIcon(const QColor &color, int w, int h)
{
	QPixmap pm(w,h);
	pm.fill(color);
	QPen p(Qt::black, 1);
	QPainter pt(&pm);
	pt.drawRect(pm.rect());
	return QIcon(pm);
}

TItem* TColorList::setItem(const QVariant &_value, const QString &_text, QIcon *)
{
	QVariant val(_value);
	QString txt(_text);
	QColor col;

	if (_value.type() == QVariant::Color) 
		col = QColor(val.toString());
	else {
		TColorValidator cv;
		//QString s = _value.toString();
        QVariant s = _value;
		if (cv.checkValue(s) == TValueValidator::VR_BAD) 
			return 0;
        val = QVariant(col = QColor(txt = s.toString()));
	} 

	return TItemList::setItem(val, txt, new QIcon(makeIcon(col)));
}

TItem* TColorList::findItem(const QVariant &_value)
{
    QString s = _value.toString();
    for (int i = 0; i < count(); i++)
        if (at(i)->value.toString() == s)
            return at(i);
    return 0;
}

TColorList TColorList::m_regColorList;

TColorList& TColorList::registeredColorList()
{
	if (m_regColorList.isEmpty()) {	// fill the list
		QStringList cl = QColor::colorNames();
		QColor c;
		for (QStringList::iterator it = cl.begin(); it != cl.end(); ++it) {
			if ((*it).isEmpty()) continue;
			c.setNamedColor(*it);
			if (c.isValid()) {
				m_regColorList.setItem(c, *it);
			}
		}
	}

	return m_regColorList;
}

TColorList TColorList::m_baseColorList;

TColorList& TColorList::baseColorList()
{
	if (m_baseColorList.isEmpty()) {	// fill the list
		typedef struct {
			const QColor	&color;
			const QString	name;
		} M_Struct;

		const M_Struct m_struct[] = { 
			{	Qt::black,			QT_TR_NOOP("Black")			}, 
			{	Qt::white,			QT_TR_NOOP("White")			}, 
			{	Qt::gray,			QT_TR_NOOP("Gray")			},
			{	Qt::lightGray,		QT_TR_NOOP("Light Gray")	},
			{	Qt::darkGray,		QT_TR_NOOP("Dark Gray")		},
			{	Qt::red,			QT_TR_NOOP("Red")			},
			{	Qt::green,			QT_TR_NOOP("Green")			},
			{	Qt::blue,			QT_TR_NOOP("Blue")			},
			{	Qt::cyan,			QT_TR_NOOP("Cyan")			},
			{	Qt::magenta,		QT_TR_NOOP("Magenta")		},
			{	Qt::yellow,			QT_TR_NOOP("Yellow")		},
			{	Qt::darkRed,		QT_TR_NOOP("Dark Red")		},
			{	Qt::darkGreen,		QT_TR_NOOP("Dark Green")	},
			{	Qt::darkBlue,		QT_TR_NOOP("Dark Blue")		},
			{	Qt::darkCyan,		QT_TR_NOOP("Dark Cyan")		},
			{	Qt::darkMagenta,	QT_TR_NOOP("Dark Magenta")	},
			{	Qt::darkYellow,		QT_TR_NOOP("Dark Yellow")	}
		};

		const M_Struct *clp = m_struct;
		for (int i = 0; i < 17; i++, clp++) {
			m_baseColorList.setItem(clp->color, clp->name);
		}
	}
	
	//qDebug("base");

	return m_baseColorList;
}

/****************************************************************************
**
** TColorButtonEditor class
**
*****************************************************************************/

void TColorButtonEditor::buttonClicked()
{
	SValue &v = m_editedValue->value();
	QColor c = QColorDialog::getColor(v.type != VT_SPECIAL ? QColor(v.value.toString()) : Qt::white);
	if (c.isValid()) {
		doPickValue(c);
	}
}

//***************************************************************************//
//
// TColorComboEditor Class
//
//***************************************************************************//

TColorComboEditor::TColorComboEditor() : TComboEditor()
{
	m_text = QObject::tr("Pick color...");
}

void TColorComboEditor::invoke(TBaseValue *item)
{
	SValue &v = item->value();
	QColor c = QColorDialog::getColor(v.type != VT_SPECIAL ? QColor(v.value.toString()) : Qt::white);
	if (c.isValid()) {
		doPickValue(c);
	}
}

/****************************************************************************/

// TColorValue class

// Represents a color value class which allows to store QColor

/****************************************************************************/

TColorValidator	TColorValidator::s_ColorValidator;
TColorButtonEditor TColorValue::s_ColorButtonEditor;
TColorComboEditor TColorValue::s_ColorComboEditor;

bool TColorValue::s_useOwnText = true;
bool TColorValue::s_allowListAppend = false;
bool TColorValue::s_showExpandables = true;

TColorList*	TColorValue::s_colorList = 0;

TColorValue::TColorValue(TValueBrowser *parent, const QString& name, TColorList *list /* = 0 */, int page /* = -1 */, int id /* = -1 */) :
TListValue(parent, name, list, page, id)
{
    construct();
}

TColorValue::TColorValue(TBaseItem *parent, const QString& name, TColorList *list /* = 0 */, int id /* = -1 */) :
TListValue(parent, name, list, id)
{
	construct();
}

void TColorValue::construct()
{
    setValidator(&TColorValidator::s_ColorValidator);
	setButtonEditor(&s_ColorButtonEditor);

	m_useOwnText = s_useOwnText;
	m_allowListAppend = s_allowListAppend;

    showChildren(s_showExpandables);
}

TColorValue::~TColorValue()
{
}

//void TColorValue::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
//{
//    if (editor && !m_item) 
//		editor->setGeometry(option.rect.adjusted(
//			qApp->style()->pixelMetric(QStyle::PM_SmallIconSize) +       
//            qApp->style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing),
//			0,-1,0));
//    else
//        TListValue::updateEditorGeometry(editor, option, index);
//}

const QIcon TColorValue::itemIcon()
{
    if (m_set.type != VT_SPECIAL)
        return m_icon;

    return TListValue::itemIcon();
}

void TColorValue::setupValue(void)
{
    m_icon = TColorList::makeIcon(m_set.value.value<QColor>());

    TListValue::setupValue();
}

void TColorValue::appendValue(const QVariant& val)
{
	// create list if it is empty
	if (!m_list) m_list = new TColorList();

	TListValue::appendValue(val);
}

void TColorValue::fillEditor()
{
	TListValue::fillEditor();

	m_editor->addSeparator();
	m_editor->addComboEditor(&s_ColorComboEditor);
}


void TColorValue::writeDataSet(QSettings &set)
{
	TBaseItem::writeDataSet(set);

	if (m_propertyName.isEmpty()) return;

	switch (browser()->storingPolicy()) {
		case TValueBrowser::SP_RAW:
			set.setValue(m_propertyName, m_set.value.toString());
			set.setValue(m_propertyName+".Type", m_set.type);
			break;
		case TValueBrowser::SP_STORED:
			set.setValue(m_propertyName, m_storedSet.value.toString());
			set.setValue(m_propertyName+".Type", m_storedSet.type);
			break;
		case TValueBrowser::SP_LINKED: 
		{
			if (!m_linked) return;
			QVariant v;
			if (doUpdateFromLinked(v))
				set.setValue(m_propertyName, v.toString());
			break;
		}
	}
}



void TColorValue::doCreateChildren()
{
    ivRed = new TIntegerValue(this, tr("Red"));
    ivRed->setValidator(TIntValidator::s_ByteValidator);
    ivGreen = new TIntegerValue(this, tr("Green"));
    ivGreen->setValidator(TIntValidator::s_ByteValidator);
    ivBlue = new TIntegerValue(this, tr("Blue"));
    ivBlue->setValidator(TIntValidator::s_ByteValidator);

    registerChild(ivRed);
    registerChild(ivGreen);
    registerChild(ivBlue);
}

void TColorValue::doUpdateFromChild(TBaseValue* /*child*/)
{
    setValue(QColor(ivRed->number(), ivGreen->number(), ivBlue->number()));
}

void TColorValue::doUpdateChildren()
{
    QColor clr = color();
    ivRed->setValue(clr.red());
    ivGreen->setValue(clr.green());
    ivBlue->setValue(clr.blue());
}
