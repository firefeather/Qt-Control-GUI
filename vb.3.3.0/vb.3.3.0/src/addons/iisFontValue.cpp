#include "iisFontValue.h"
#include "iisNumericValue.h"
#include "iisBoolValue.h"

#include <QPainter>
#include <QFontDialog> 

/****************************************************************************/

// TFontList class

// A list holding Font values.

/****************************************************************************/

QIcon* TFontList::m_truetype = 0;
QIcon* TFontList::m_bitmap = 0;

#if QT_VERSION >= 0x040200
QFontComboBox* TFontList::m_fakeCombo = 0;
QAbstractItemDelegate* TFontList::m_fakeFontDelegate = 0;
#endif

TFontList TFontList::m_fullFontList;
TItemList TFontList::m_fontSizeList;

TFontList::TFontList() : TItemList(true)
{
}

QIcon TFontList::makeIcon(const QFont &font, int w, int h)
{
    if (!m_truetype) {
        m_truetype = new QIcon(QLatin1String(":/trolltech/styles/commonstyle/images/fonttruetype-16.png"));
        m_bitmap = new QIcon(QLatin1String(":/trolltech/styles/commonstyle/images/fontbitmap-16.png"));
    }

    QPixmap pm(24,20);
    QIcon ico(pm);
    pm = ico.pixmap(24,20);
    QPainter p(&pm);
    QFont f(font);
    f.setPixelSize(pm.height());
    p.setFont(f);
    p.fillRect(pm.rect(), Qt::white);
    p.drawText(pm.rect(), Qt::AlignCenter, "Aa");
    return QIcon(pm);
    //if (QFontDatabase().isSmoothlyScalable(font.family()))
    //    return *m_truetype;
    //
    //return *m_bitmap;
}

TItem* TFontList::setItem(const QVariant &_value, const QString &_text, QIcon *)
{
    QString family;
    QString txt(_text);
    if (txt.isEmpty()) txt = _value.toString();

    switch (_value.type()) {
        case QVariant::Font:
            family = _value.value<QFont>().toString();
            break;
        case QVariant::String: 
            family = _value.toString();
            break;
        default:
            family = txt;
    }

	return TItemList::setItem(family, txt/*, new QIcon(makeIcon(QFont(family)))*/);
}


TFontList& TFontList::fullFontList()
{
	if (m_fullFontList.isEmpty()) {	
        // fill the list
        QStringList l (QFontDatabase().families());
        m_fullFontList << l;

        //// #### workaround with the current font
//#ifdef Q_OS_WIN32
        QString s = TFontValue::defaultFont().family();
        if (!l.contains(s))
            m_fullFontList << s;
//#endif
	}
	
	return m_fullFontList;
}

TItemList& TFontList::defaultFontSizeList()
{
    if (m_fontSizeList.isEmpty()) {
        // fill the list
        m_fontSizeList << QFontDatabase::standardSizes();
    }

    return m_fontSizeList;
}

bool TFontList::paintComboItem(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index,
        QVariant &data)
{
#if QT_VERSION >= 0x040200
    if (!m_fakeCombo) {
        m_fakeCombo = new QFontComboBox();
        m_fakeFontDelegate = m_fakeCombo->itemDelegate();
    }

    painter->setPen(Qt::black);
    m_fakeFontDelegate->paint(painter, option, index);
    return false;
#endif

    return true;
}

bool TFontList::compareComboItem(const QVariant &list_value, const QVariant &item_value)
{
    return (list_value.value<QFont>().family() == item_value.value<QFont>().family());
}

void TFontList::highlightComboItem(QVariant &data, int index, QComboBox *box, QPoint hintPos)
{
    if (data.isValid()) {
#if QT_VERSION >= 0x040200
        QToolTip::setFont(QFont(data.toString()));
#endif
        QToolTip::showText(hintPos, data.toString());
    }
}

QSize TFontList::sizeHintComboItem(QVariant &data, const QStyleOptionViewItem &option) const
{
    return QSize(20,20);
    //QString text = data.toString();
    //QFont font(option.font);
    //font.setFamily(text);
    //font.setPointSize(font.pointSize() * 3/2);
    //QFontMetrics fontMetrics(font);
    //return QSize(fontMetrics.width(text), fontMetrics.lineSpacing());
}

/****************************************************************************
**
** TFontButtonEditor class
**
*****************************************************************************/

void TFontButtonEditor::buttonClicked()
{
	SValue &v = m_editedValue->value();
    bool ok;
    QFont c = QFontDialog::getFont(&ok, v.type != VT_SPECIAL ? ((TFontValue*)m_editedValue)->font() : QFont());
	if (ok) {
		doPickValue(c);
	}
}

//***************************************************************************//
//
// TFontComboEditor Class
//
//***************************************************************************//

TFontComboEditor::TFontComboEditor() : TComboEditor()
{
	m_text = QObject::tr("Pick font...");
}

void TFontComboEditor::invoke(TBaseValue *item)
{
	SValue &v = item->value();
    bool ok;
	QFont c = QFontDialog::getFont(&ok, v.type != VT_SPECIAL ? ((TFontValue*)item)->font() : QFont());
	if (ok) {
		doPickValue(c);
	}
}

/****************************************************************************/

// TFontValue class

// Represents a font value class which allows to store QFont

/****************************************************************************/

TFontButtonEditor TFontValue::s_FontButtonEditor;
TFontComboEditor TFontValue::s_FontComboEditor;

bool TFontValue::s_showExpandables = true;

TFontList*	TFontValue::s_fontList = 0;
TItemList*	TFontValue::s_fontSizeList = 0;
QFont		TFontValue::s_defaultFont, TFontValue::s_tooltipFont;

TFontValue::TFontValue(TValueBrowser *parent, const QString& name, TFontList *list /* = 0 */, int page /* = -1 */, int id /* = -1 */) :
TListValue(parent, name, list, page, id)
{
    construct();
}

TFontValue::TFontValue(TBaseItem *parent, const QString& name, TFontList *list /* = 0 */, int id /* = -1 */) :
TListValue(parent, name, list, id)
{
	construct();
}

void TFontValue::construct()
{
    setButtonEditor(&s_FontButtonEditor);

    showChildren(s_showExpandables);

    QFont fnt(s_defaultFont);    
    setValue(fnt);
}

TFontValue::~TFontValue()
{
}

//void TFontValue::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    if (editor && !m_item) 
//		editor->setGeometry(option.rect.adjusted(
//			qApp->style()->pixelMetric(QStyle::PM_SmallIconSize) +       
//            qApp->style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing),
//			0,-1,0));
//    else
//        TListValue::updateEditorGeometry(editor, option, index);
//}

void TFontValue::onEnterItem()
{
    s_tooltipFont = QToolTip::font();
    TListValue::onEnterItem();
}

void TFontValue::onLeaveItem()
{
    QToolTip::setFont(s_tooltipFont);
    TListValue::onLeaveItem();
}

bool TFontValue::setValue(const QVariant &value)
{
    QFont f;

    if (value.type() == QVariant::Font) {
        f = value.value<QFont>();
    } 
    else
    if (value.type() == QVariant::String) {
        f = font();
        f.setFamily(value.toString());
    }
    else
        return false;

    if (f.family().isEmpty())
        return false;

    // this is workaround - QFontDatabase::families() returns not all values
    TItem* it = m_list->findItemByText(f.family());
    if (!it){
	    TItemListIterator i(*m_list);
	    while (i.hasNext()) {
		    it = i.next();
            if (it->text.startsWith(f.family())) {
			    f.setFamily((*it).text);
                break;
            }
	    }
    }

    m_set.type = VT_VALUE;
    m_set.value = f;
    m_item = 0;

    setupValue();

    return true;

    //return TListValue::setValue(f);
}

bool TFontValue::valueChanged(const QVariant &v1, const QVariant &v2)
{
	QFont f1 = v1.value<QFont>(), f2 = v2.value<QFont>();
	//return (f1 != f2); // this does not work correctly in Qt <= 4.3
	return (f1.family() != f2.family() ||
			f1.pixelSize() != f2.pixelSize() ||
			f1.pointSize() != f2.pointSize() ||
			f1.bold() != f2.bold() ||
			f1.italic() != f2.italic() ||
			f1.underline() != f2.underline() ||
			f1.strikeOut() != f2.strikeOut()
			);
}

const QIcon TFontValue::itemIcon()
{
    if (m_set.type != VT_SPECIAL)
        return m_icon;

    return TListValue::itemIcon();
}

QString TFontValue::valueText() const
{
    QFont fnt(font());
    return QString("%1 [%2]").arg(fnt.family()).arg(fnt.pointSize());
}

void TFontValue::setupValue(void)
{
    m_icon = TFontList::makeIcon(m_set.value.value<QFont>());

    if (m_set.type != VT_SPECIAL)
        setData(1, ToolTipFontRole, font());
    else
        setData(1, ToolTipFontRole, QFont());


    TListValue::setupValue();
}

void TFontValue::appendValue(const QVariant& val)
{
	// create list if it is empty
	if (!m_list) m_list = new TFontList();

	TListValue::appendValue(val);
}

void TFontValue::fillEditor()
{
	TListValue::fillEditor();

	m_editor->addSeparator();
	m_editor->addComboEditor(&s_FontComboEditor);
}


void TFontValue::writeDataSet(QSettings &set)
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


void TFontValue::doCreateChildren()
{
    if (!s_fontSizeList) 
        s_fontSizeList = &TFontList::defaultFontSizeList();
	
    ivSize = new TListValue(this, tr("Size"), s_fontSizeList);
    ivSize->allowOwnValues();
    ivSize->setValidator(TIntValidator::s_PositiveValidator);
    ivSize->setButtonEditor(TIntegerValue::s_NumButtonEditor);
    
    bvBold = new TBoolValue(this, tr("Bold"));
    bvItalic = new TBoolValue(this, tr("Italic"));
    bvUnderline = new TBoolValue(this, tr("Underline"));
    bvStriked = new TBoolValue(this, tr("Striked Out"));

    registerChild(ivSize);
    registerChild(bvBold);
    registerChild(bvItalic);
    registerChild(bvUnderline);
    registerChild(bvStriked);
}

void TFontValue::doUpdateFromChild(TBaseValue * /*child*/)
{
    QFont fnt(font().family(), ivSize->number(), bvBold->boolean() ? QFont::Bold : QFont::Normal, bvItalic->boolean());
    fnt.setUnderline(bvUnderline->boolean());
    fnt.setStrikeOut(bvStriked->boolean());
    setValue(fnt);
}

void TFontValue::doUpdateChildren()
{
    QFont fnt = font();
    ivSize->setValue(fnt.pointSize());
    bvBold->setValue(fnt.bold());
    bvItalic->setValue(fnt.italic());
    bvUnderline->setValue(fnt.underline());
    bvStriked->setValue(fnt.strikeOut());
}
