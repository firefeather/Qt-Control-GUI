#include "iisValueBrowser.h"
#include "iisItems.h"

/****************************************************************************
**
** TValueItemDelegate class
**
*****************************************************************************/

void TValueItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	TBaseItem *it = (TBaseItem*)( ((TValueBrowser*)parent())->itemFromIndex(index) );
	it->paint(painter, option, index);
}

QWidget* TValueItemDelegate::createEditor( QWidget * parentW, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	TBaseItem *it = (TBaseItem*)( ((TValueBrowser*)parent())->itemFromIndex(index) );
	return it->createEditor(parentW, option, index);
}

void TValueItemDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	//if (editor) 
	//	editor->setGeometry(option.rect.adjusted(-1,0,-1,0));
	TBaseItem *it = (TBaseItem*)( ((TValueBrowser*)parent())->itemFromIndex(index) );
	it->updateEditorGeometry(editor, option, index);
}

QSize TValueItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    //return QSize(10, 18);
    return QSize(10, ((TValueBrowser*)parent())->itemDefHeight());
}

/****************************************************************************
**
** TBaseItem class
**
*****************************************************************************/

int TBaseItem::s_defaultPage = 0;
//QIcon TBaseItem::s_nullIcon;

TBaseItem::TBaseItem (TValueBrowser *parent, const QString& name, int page, int id):
	QTreeWidgetItem (parent, QStringList() << name)
{ 
	init(name, page, id);
}

TBaseItem::TBaseItem (TBaseItem *parent, const QString& name, int id):
	QTreeWidgetItem (parent, QStringList() << name)
{ 
	init(name, parent->page(), id);
}

void TBaseItem::init(const QString& name, int page, int id)
{
	this->id = id;
	m_hint = QString::null;

	m_checkPropertyName = QString::null;
	m_checkLinked = 0;
	m_checkLinkedType = TValueBrowser::LT_BOOL;

	setCheckable(false);
	m_check = m_storedCheck = checkState(0);

	if (page < 0)
		m_page = s_defaultPage;
	else
		s_defaultPage = m_page = page;

    setVisible(true);  
    setExpanded(true);
}

TBaseItem::~TBaseItem()
{
}

void TBaseItem::setCheckable( bool b )
{
	setFlags(b ? flags() | Qt::ItemIsUserCheckable : flags() & ~Qt::ItemIsUserCheckable);
	if (b) setCheckState(0, Qt::Unchecked);
}

void TBaseItem::setEnabled ( bool b )
{
	browser()->setItemSelected(this, false);
	setFlags(b ? 
			flags() | Qt::ItemIsEnabled | Qt::ItemIsEditable : 
			flags() &~ ( Qt::ItemIsEnabled | Qt::ItemIsEditable ));
}

void TBaseItem::setVisible ( bool b )
{
    m_visible = b;
	browser()->setItemSelected(this, false);
	browser()->setItemHidden(this, !b || m_page != browser()->page());
}

void TBaseItem::setHint ( const QString& hint )
{
	m_hint = hint;
	// update if it is current item...
//	if (browser()->currentItem() == this) {
//		browser()->doHintChange(this);
	//}
}

void TBaseItem::setup(void)
{
	setSizeHint(0, QSize(0, browser()->itemDefHeight()));
}

void TBaseItem::storeSet(bool emitChanges)
{
	m_storedCheck = m_check;
}

void TBaseItem::restoreSet()
{
	m_check = m_storedCheck;

	if (flags() & Qt::ItemIsUserCheckable)
		setCheckState(0, m_check);
}

void TBaseItem::updateCheckState()
{
	m_check = checkState(0);
}

void TBaseItem::setChecked( Qt::CheckState state )
{
	setCheckState(0, state);
	m_check = state;
}

bool TBaseItem::setItemLevel(int level)
{
    return browser()->setItemLevel(this, level);
}

void TBaseItem::drawDisplay ( QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, const QString & text ) const
{
#if QT_VERSION < 0x040200
	if (text.contains("\n")) {
		QString s = text.section("\n", 0, 0) + " <...>";
		QItemDelegate::drawDisplay(painter, option, rect, s);
        return;
	}
#endif

	QChar nl(8232);	// workaround for Qt >= 4.2.x
	////qDebug() << text;
	////if (text.size() > 1) 
	////	qDebug() << text[1].unicode() << "\n";

	if (text.contains(nl)) {
		QString s = text.section(nl, 0, 0) + " <...>";
		QItemDelegate::drawDisplay(painter, option, rect, s);
        return;
	} 
 
    QItemDelegate::drawDisplay(painter, option, rect, text);
}

void TBaseItem::onChangeItem(int column)
{
	if (!column)
		updateCheckState();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void TBaseItem::setCheckPropertyName(const QString& name)
{
	m_checkPropertyName = name;
}

void TBaseItem::setCheckLinked(void *linked, int type)
{
	m_checkLinked = linked;
	m_checkLinkedType = type;
}

void TBaseItem::addCheckProperty(const QString& name, void *linked, int type)
{
	m_checkPropertyName = name;
	m_checkLinked = linked;
	m_checkLinkedType = type;

	setCheckable(true);
}

void TBaseItem::writeDataSet(QSettings &set)
{
	if (m_checkPropertyName.isEmpty()) return;

	switch (browser()->storingPolicy()) {
		case TValueBrowser::SP_RAW:
			set.setValue(m_checkPropertyName, m_check);
			break;
		case TValueBrowser::SP_STORED:
			set.setValue(m_checkPropertyName, m_storedCheck);
			break;
		case TValueBrowser::SP_LINKED: 
		{
			if (!m_checkLinked) return;
			Qt::CheckState old = m_check;
			updateFromLinked();
			set.setValue(m_checkPropertyName, m_check);
			setChecked(old);
			break;
		}
	}
}

void TBaseItem::readDataSet(QSettings &set)
{
	if (m_checkPropertyName.isEmpty()) return;

	switch (browser()->storingPolicy()) {
		case TValueBrowser::SP_RAW:
		{
			Qt::CheckState cs = (Qt::CheckState) set.value(m_checkPropertyName, m_check).toUInt();
			setChecked(cs);
			break;
		}
		case TValueBrowser::SP_STORED:
			m_storedCheck = (Qt::CheckState) set.value(m_checkPropertyName, m_storedCheck).toUInt();
			break;
		case TValueBrowser::SP_LINKED:
		{
			if (!m_checkLinked) return;
			Qt::CheckState cs = (Qt::CheckState) set.value(m_checkPropertyName).toUInt();
			Qt::CheckState old = m_check;
			setChecked(cs);
			updateLinked();
			setChecked(old);
			break;
		}
	}
}

void TBaseItem::updateFromLinked()
{
	if (!m_checkLinked) return;

	Qt::CheckState b;

	switch (m_checkLinkedType) {
		case TValueBrowser::LT_INT:
			b = (Qt::CheckState)(*(int*)m_checkLinked);
			break;
		case TValueBrowser::LT_UINT:
			b = (Qt::CheckState)(*(unsigned int*)m_checkLinked);
			break;	
		case TValueBrowser::LT_LONG:
			b = (Qt::CheckState)(*(long*)m_checkLinked);
			break;	
		case TValueBrowser::LT_ULONG:
			b = (Qt::CheckState)(*(unsigned int*)m_checkLinked);
			break;	
		case TValueBrowser::LT_LONGLONG:
			b = (Qt::CheckState)(*(qlonglong*)m_checkLinked);
			break;	
		case TValueBrowser::LT_ULONGLONG:
			b = (Qt::CheckState)(*(qulonglong*)m_checkLinked);
			break;	
		case TValueBrowser::LT_FLOAT:
			b = (Qt::CheckState)(int)(*(float*)m_checkLinked);
			break;
		case TValueBrowser::LT_DOUBLE:
			b = (Qt::CheckState)(int)(*(double*)m_checkLinked);
			break;
		case TValueBrowser::LT_BOOL:
			b = *(bool*)m_checkLinked ? Qt::Checked : Qt::Unchecked;
			break;
		case TValueBrowser::LT_STRING:
			b = (Qt::CheckState)(*(QString*)m_checkLinked).toUInt();
			break;
		case TValueBrowser::LT_VARIANT:
			b = (Qt::CheckState)(*(QVariant*)m_checkLinked).toUInt();
			break;
		default:
			return;
	}

	setChecked(b);
}

void TBaseItem::updateLinked()
{
	if (!m_checkLinked) return;

	switch (m_checkLinkedType) {
		case TValueBrowser::LT_INT:
			*(int*)m_checkLinked = m_check;
			break;
		case TValueBrowser::LT_UINT:
			*(unsigned int*)m_checkLinked = m_check;
			break;	
		case TValueBrowser::LT_LONG:
			*(long*)m_checkLinked = m_check;
			break;	
		case TValueBrowser::LT_ULONG:
			*(unsigned long*)m_checkLinked = m_check;
			break;	
		case TValueBrowser::LT_LONGLONG:
			*(qlonglong*)m_checkLinked = m_check;
			break;	
		case TValueBrowser::LT_ULONGLONG:
			*(qulonglong*)m_checkLinked = m_check;
			break;	
		case TValueBrowser::LT_FLOAT:
			*(float*)m_checkLinked = m_check;
			break;
		case TValueBrowser::LT_DOUBLE:
			*(double*)m_checkLinked = m_check;
			break;
		case TValueBrowser::LT_BOOL:
			*(bool*)m_checkLinked = m_check;
			break;
		case TValueBrowser::LT_STRING:
			*(QString*)m_checkLinked = QString::number(m_check);
			break;
		case TValueBrowser::LT_VARIANT:
			*(QVariant*)m_checkLinked = m_check;
			break;
		default:
			return;
	}
}


/****************************************************************************
**
** TBaseValue class
**
*****************************************************************************/

TBaseValue::TBaseValue (TValueBrowser *parent, const QString& name, int page, int id):
	m_editor(0),
	m_validator(0),
	m_wasChanged(0),
	m_buttonEditor(0),
	m_linked(0),
    //m_getter(0), m_setter(0),
	m_item(0),
    m_SpecialsAllowed(true),
    m_childLock(false),
    m_child(false),
    m_expandables(false),
    m_exp_created(false),
	TBaseItem (parent, name, page, id) 
{
	setValue("");
}

TBaseValue::TBaseValue (TBaseItem *parent, const QString& name, int id):
	m_editor(0),
	m_validator(0),
	m_wasChanged(0),
	m_buttonEditor(0),
	m_linked(0),
    //m_getter(0), m_setter(0),
	m_item(0),
    m_SpecialsAllowed(true),
    m_childLock(false),
    m_child(false),
    m_expandables(false),
    m_exp_created(false),
	TBaseItem (parent, name, id) 
{ 
	setValue("");
}


TBaseValue::~TBaseValue()
{
	//if (m_validator && m_validator->autoKill()) {
	//	delete m_validator;
	//	m_validator = 0;
	//}
}

void TBaseValue::setVisible ( bool b )
{
    TBaseItem::setVisible(b);

    if (m_editor)
        m_editor->setFocus();
}

void TBaseValue::setValidator(TValueValidator* validator)
{
	//if (m_validator && m_validator->autoKill())
	//	delete m_validator;
	m_validator = validator;

	// do some validation
	if (m_validator && m_set.type != VT_SPECIAL) {
		//QString s = m_set.value.toString();
        QVariant s = m_set.value;
		TValueValidator::EValidateResult vr = validate(s);
		switch (vr) {
			case TValueValidator::VR_BAD:
				setSpecial(SpecialValueList::ID_BAD);
				break;
			case TValueValidator::VR_FIXED:
				m_set.type = VT_OWN;
				m_set.value = s;
				setupValue();
				break;
		}
	}
}

TValueValidator::EValidateResult TBaseValue::validate(QVariant& value)
{
	if (!m_validator) return TValueValidator::VR_OK;
	return m_validator->checkValue(value);
}

void TBaseValue::onEnterItem()
{
	if (isDisabled()) {
		browser()->setFocus();
		return;
	}

	// setup editor
	browser()->openPersistentEditor(this, 1);	

	// fill editor
	//fillEditor();

	// set current item
	setCurrentItem();

	// show editor
	m_editor->setFocus();
}

void TBaseValue::onLeaveItem()
{
#if QT_VERSION >= 0x040200
    QToolTip::hideText();
#endif

	if (m_editor) {
		m_editor->disconnect();
		m_editor->setButtonEditor(0);
		browser()->closePersistentEditor(this, 1);
		delete m_editor;
		m_editor = 0;
	}
	
	// workaround for issue when value text disappears after leaving the editor  (seen in Qt 4.1.2, ASP Linux)
#if QT_VERSION < 0x040200
    setupValue();
	//setText(1, m_item ? m_item->text : m_set.value.toString());
	//qDebug() << m_set.value;
#endif
}

QWidget* TBaseValue::createEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	m_editor = new TDropList(parent, (TBaseValue*)this);

	m_editor->connect(m_editor, SIGNAL(picked(TItemList*, TItem*, const QVariant&)), browser(), SLOT(valuePicked(TItemList*, TItem*, const QVariant&)));
	m_editor->connect(m_editor, SIGNAL(restoreValueInvoked()),  browser(), SLOT(restoreSetCurrent()));

	m_editor->setButtonEditor(m_buttonEditor);

	return m_editor;
}


void TBaseValue::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	if (editor) {
		if (autoIconActive()) 
			editor->setGeometry(option.rect.adjusted(
				iconSpace(), -1,-1,0));
		else
			editor->setGeometry(option.rect.adjusted(-1,-1,-1,0));
	}
}


void TBaseValue::fillEditor()
{
	// special value list
	if (m_SpecialsAllowed)
        m_editor->addList(browser()->specialList());
}

void TBaseValue::allowSpecials(bool allow)
{
    m_SpecialsAllowed = allow;
    if (m_editor) {
        fillEditor();
        setCurrentItem();
    }
}

bool TBaseValue::setCurrentItem()
{
	if (!m_editor) return false;
	
	m_editor->setVisualMode(m_wasChanged && browser()->changeInfo());
	m_editor->enableOwnText(true);

	if (m_set.type == VT_SPECIAL) {
		m_editor->setCurrent(browser()->specialList()/*, m_set.value*/);
		return true;
	} else {
		//qDebug() << "setCurrent:"  << m_set.value;

		m_editor->setCurrent(0/*, m_set.value*/);
		return false;	// value not recognized
	}
}



// internal
void TBaseValue::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index )
{
	QStyleOptionViewItem op1(option);

	// set up change mode
	if (index.column() && browser()->changeInfo() && m_wasChanged) {
		op1.font.setBold(true);
	}

    // draw as child item
    if (m_child) {
        op1.palette.setColor(QPalette::Text, op1.palette.color(QPalette::Dark).dark());
	//op1.palette.setColor(QPalette::Highlight, Qt::yellow);
//	setData(0, Qt::BackgroundColorRole, op1.palette.color(QPalette::Base).light());
//	setData(1, Qt::BackgroundColorRole, op1.palette.color(QPalette::Background).dark(30));
        op1.font.setItalic(true);
    }

	if (option.state & QStyle::State_Selected) {	
		op1.palette.setColor(QPalette::Highlight, 
		    op1.palette.color(index.column() ? QPalette::Base : QPalette::Background));
		op1.palette.setColor(QPalette::HighlightedText, op1.palette.color(QPalette::Text));
	} 

	TBaseItem::paint(painter, op1, index);

	int x = option.rect.x();
	int y = option.rect.y();
	int h1 = option.rect.height()-1;
	int w1 = option.rect.width()-1;

	painter->setPen(option.palette.color(QPalette::Dark));

	painter->drawLine(x, y+h1, x+w1, y+h1);
	painter->drawLine(x+w1, y, x+w1, y+h1);
}


void TBaseValue::setupValue(void)
{
	// check if value was changed...
	if (m_set.type != m_storedSet.type) 
		m_wasChanged = true;
	else
		m_wasChanged = valueChanged(m_set.value, m_storedSet.value);

	// set the value
	if (m_item) {
		setText(1, m_item->text);
		setData(1, Qt::ToolTipRole, m_item->text);
		setIcon(1, m_item->icon ? *m_item->icon : QIcon());
	} else {
		setText(1, valueText());
		setData(1, Qt::ToolTipRole, valueText()/*m_set.value*/);
		setIcon(1, itemIcon());
	}

	// update value in editor
    if (m_editor/* && browser()->checkEditor()*/) {
		m_editor->prepare();

		setCurrentItem();
    
        // #### workaround to make sure that item is repainted!
        browser()->setItemSelected(this, false);
        browser()->setItemSelected(this, true);
    }

    // update children
    updateChildren();   

    // update parent
    TBaseValue *p = dynamic_cast<TBaseValue*>(QTreeWidgetItem::parent());
    if (p && !p->m_childLock && p->m_set.type != VT_SPECIAL)
        p->updateFromChild(this);
}

void TBaseValue::setSpecial(uint svalue)
{
	m_item = 0;
	m_set.type = VT_OWN;

	if (svalue) {
		SpecialValueList *svl = browser()->specialList();
		if (svl) {
			m_item = svl->findItem(svalue);
			if (m_item) {
				m_set.type = VT_SPECIAL;
				m_set.value = svalue;
			}
		}
	}

	//m_set.type = bool(svalue) ? VT_SPECIAL : VT_VALUE;
	//if (svalue) m_set.value = svalue;

	setupValue();
}

bool TBaseValue::setValue(const QVariant &value)
{
	//QString s = value.toString();
//	qDebug("Value: " + s);
    QVariant s(value);
	TValueValidator::EValidateResult vr = validate(s);

	if (vr == TValueValidator::VR_BAD) return false;	// not valid, do nothing

	m_set.value = (vr == TValueValidator::VR_OK ? value : s);
	m_set.type = VT_OWN;
	m_item = 0;

	setupValue();

	return true;
}

void TBaseValue::valuePicked(TItemList *list, TItem *item, const QVariant &value)
{
	//qDebug() << "picked:"  << m_set.value;

	if (list && list == browser()->specialList()) {		// special value picked
		setSpecial(value.toUInt());
	} else {
		if (!setValue(value)) {
			setCurrentItem();	// update editor
		}
	}
}

void TBaseValue::storeSet(bool emitChanges)
{
	TBaseItem::storeSet();
	m_storedSet = m_set;

	// invalidate current data
	setupValue();
}

void TBaseValue::restoreSet()
{
	m_set = m_storedSet;

	TBaseItem::restoreSet();
	
	// invalidate current data
	if (m_set.type == VT_SPECIAL)
		setSpecial(m_set.value.toInt());
	else
		setValue(m_set.value);
}

void TBaseValue::setButtonEditor(TButtonEditor *editor)
{
	m_buttonEditor = editor;
}

void TBaseValue::setPropertyName(const QString& name)
{
	m_propertyName = name;
}

void TBaseValue::setLinked(void *linked, int type)
{
	m_linked = linked;
	m_propertyType = type;
}

//void TBaseValue::setGetter(void *getter, void *obj, int type)
//{
//	m_getter = getter;
//    m_getterObj = obj;
//	m_propertyType = type;
//}
//
//void TBaseValue::setSetter(void *setter, void *obj, int type)
//{
//	m_setter = setter;
//    m_setterObj = obj;
//	m_propertyType = type;
//}

void TBaseValue::updateFromLinked()
{
	QVariant v;
	if (doUpdateFromLinked(v)) 
		setValue(v);

	TBaseItem::updateFromLinked();
}

bool TBaseValue::doUpdateFromLinked(QVariant &var)
{
	if (!m_linked) return false;

	switch (m_propertyType) {
		case TValueBrowser::LT_INT:
			var = (*(int*)m_linked);
			break;
		case TValueBrowser::LT_UINT:
			var = (*(unsigned int*)m_linked);
			break;	
		case TValueBrowser::LT_LONG:
			var = (*(int*)m_linked);
		/// !!! long is incompatible with QVariant
			break;	
		case TValueBrowser::LT_ULONG:
			var = (*(unsigned int*)m_linked);	/// !!! unsigned long is incompatible with QVariant
			break;	
		case TValueBrowser::LT_LONGLONG:
			var = (*(qlonglong*)m_linked);
			break;	
		case TValueBrowser::LT_ULONGLONG:
			var = (*(qulonglong*)m_linked);
			break;	
		case TValueBrowser::LT_FLOAT:
			var = (*(float*)m_linked);
			break;
		case TValueBrowser::LT_DOUBLE:
			var = (*(double*)m_linked);
			break;
		case TValueBrowser::LT_BOOL:
			var = (*(bool*)m_linked);
			break;
		case TValueBrowser::LT_STRING:
			var = (*(QString*)m_linked);
			break;
		case TValueBrowser::LT_COLOR:
			var = (*(QColor*)m_linked);
			break;
		case TValueBrowser::LT_FONT:
			var = (*(QFont*)m_linked);
			break;
		case TValueBrowser::LT_POINT:
			var = (*(QPoint*)m_linked);
			break;
		case TValueBrowser::LT_SIZE:
			var = (*(QSize*)m_linked);
			break;
		case TValueBrowser::LT_RECT:
			var = (*(QRect*)m_linked);
			break;
		case TValueBrowser::LT_VARIANT:
			var = (*(QVariant*)m_linked);
			break;
		default:
			return false;
	}
	return true;
}

void TBaseValue::updateLinked()
{
	doUpdateLinked(m_set.value);

	TBaseItem::updateLinked();
}

bool TBaseValue::doUpdateLinked(QVariant &var)
{
	if (!m_linked) return false;

	switch (m_propertyType) {
		case TValueBrowser::LT_INT:
			*(int*)m_linked = var.toInt();
			break;
		case TValueBrowser::LT_UINT:
			*(unsigned int*)m_linked = var.toUInt();
			break;	
		case TValueBrowser::LT_LONG:
			*(long*)m_linked = (long)var.toInt();				/// !!! long is incompatible with QVariant
			break;	
		case TValueBrowser::LT_ULONG:
			*(unsigned long*)m_linked = (unsigned long)var.toUInt();	/// !!! unsigned long is incompatible with QVariant
			break;	
		case TValueBrowser::LT_LONGLONG:
			*(qlonglong*)m_linked = var.toLongLong();
			break;	
		case TValueBrowser::LT_ULONGLONG:
			*(qulonglong*)m_linked = var.toULongLong();
			break;	
		case TValueBrowser::LT_FLOAT:
			*(float*)m_linked = (float)var.toDouble();			/// !!! float is incompatible with QVariant
			break;
		case TValueBrowser::LT_DOUBLE:
			*(double*)m_linked = var.toDouble();
			break;
		case TValueBrowser::LT_BOOL:
            //if (m_setter) {
            //    (m_setterObj.*m_setter)(var.toBool());
            //    break;
            //}
			*(bool*)m_linked = var.toBool();
			break;
		case TValueBrowser::LT_STRING:
			*(QString*)m_linked = var.toString();
			break;
		case TValueBrowser::LT_COLOR:
			*(QColor*)m_linked = var.value<QColor>();
			break;
		case TValueBrowser::LT_FONT:
			*(QFont*)m_linked = var.value<QFont>();
			break;
		case TValueBrowser::LT_POINT:
			*(QPoint*)m_linked = var.value<QPoint>();
			break;
		case TValueBrowser::LT_SIZE:
			*(QSize*)m_linked = var.value<QSize>();
			break;
		case TValueBrowser::LT_RECT:
			*(QRect*)m_linked = var.value<QRect>();
			break;
        case TValueBrowser::LT_VARIANT:
			*(QVariant*)m_linked = var;
			break;
		default:
			return false;
	}
	return true;
}



void TBaseValue::writeDataSet(QSettings &set)
{
	TBaseItem::writeDataSet(set);

	if (m_propertyName.isEmpty()) return;

	switch (browser()->storingPolicy()) {
		case TValueBrowser::SP_RAW:
			set.setValue(m_propertyName, m_set.value);
			set.setValue(m_propertyName+".Type", m_set.type);
			break;
		case TValueBrowser::SP_STORED:
			set.setValue(m_propertyName, m_storedSet.value);
			set.setValue(m_propertyName+".Type", m_storedSet.type);
			break;
		case TValueBrowser::SP_LINKED: 
		{
			if (!m_linked) return;
			QVariant v;
			if (doUpdateFromLinked(v))
				set.setValue(m_propertyName, v);
			break;
		}
	}
}

void TBaseValue::readDataSet(QSettings &set)
{
	TBaseItem::readDataSet(set);

	if (m_propertyName.isEmpty()) return;

	switch (browser()->storingPolicy()) {
		case TValueBrowser::SP_RAW:
		{
			SValue s;
			s.type = (EValueType) set.value(m_propertyName+".Type", m_set.type).toInt();
			s.value = set.value(m_propertyName, m_set.value);
			if (s.type == VT_SPECIAL)
				setSpecial(s.value.toUInt());
			else
				setValue(s.value);
			break;
		}
		case TValueBrowser::SP_STORED:
			m_storedSet.value = set.value(m_propertyName, m_storedSet.value);
			m_storedSet.type = (EValueType) set.value(m_propertyName+".Type", m_storedSet.type).toInt();
			break;
		case TValueBrowser::SP_LINKED:
		{
			QVariant v;
			if (!doUpdateFromLinked(v))
				return;
			//if (!m_linked) return;
			v = set.value(m_propertyName, v);
			doUpdateLinked(v);
			break;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void TBaseValue::showChildren(bool on)
{
    if (m_expandables == on) return;
    
    m_expandables = on;
    if (m_expandables) {
        // create
        if (!m_exp_created) {
            doCreateChildren();
            setExpanded(false);
            m_exp_created = true;
        }
        
        // update
        updateChildren();

        // expand
        //setExpanded(true);
    } else {
        if (m_exp_created) {
            TBaseValue *child;
            foreach(child, m_children)
                child->setVisible(false);
        }
    }
}

void TBaseValue::registerChild(TBaseValue *child)
{
    if (!child) return;
    if (!m_children.contains(child)) {
        m_children.append(child);
        child->allowSpecials(false);
        child->setChildMode();
//child->setData(0,Qt::BackgroundColorRole,Qt::gray);
    }
}

void TBaseValue::updateChildren()
{
//    qDebug() << "updateChildren";
    
    if (!m_expandables || !m_exp_created) 
        return;
	
    m_childLock = true;

    bool isVisible = (m_set.type != VT_SPECIAL);

    if (isVisible)
        doUpdateChildren();

    TBaseValue *child;
    foreach(child, m_children)
        child->setVisible(isVisible);
	
    m_childLock = false;
	
//    qDebug() << "updateChildren - end";
}

void TBaseValue::updateFromChild(TBaseValue *child)
{
    if (!m_exp_created) return;
    if (m_children.contains(child))
        doUpdateFromChild(child);
}

/****************************************************************************
**
** TBaseGroup class
**
*****************************************************************************/

TBaseGroup::TBaseGroup (TValueBrowser *parent, const QString& name, int page, bool subgroup, int id) :
	TBaseItem (parent, name, page, id) 
{ 
	setFlags(flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable) );

    QColor bg = browser()->palette().color(QPalette::Dark);
    setBackgroundColor(0, subgroup ? bg.dark(80) : bg);
	setBackgroundColor(1, backgroundColor(0));
    setTextColor(0, subgroup ? bg.dark() : browser()->palette().color(QPalette::Light));
}

void TBaseGroup::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index )
{
#if QT_VERSION >= 0x040300
    if (index.column()) return;
#else
    if (text(1).isEmpty()) setText(1, text(0));
#endif

	QStyleOptionViewItem op1(option);
    op1.rect.setX(0);
    op1.rect.setWidth(QTreeWidgetItem::treeWidget()->width());

    QPalette& pal = op1.palette;
    //pal.setColor(QPalette::Text, pal.color(QPalette::Light));
    pal.setColor(QPalette::Text, textColor(0));

	if (option.state & QStyle::State_Selected) {
		pal.setColor(QPalette::Highlight, pal.color(QPalette::Dark));
		pal.setColor(QPalette::HighlightedText, pal.color(QPalette::Light));
	}

	TBaseItem::paint(painter, op1, index);
}

void TBaseGroup::setSectionName(const QString& name)
{
	m_sectionName = name;
}


void TBaseGroup::writeDataSet(QSettings &set)
{
	if (m_sectionName.isEmpty()) return;

	if (!set.group().isEmpty()) set.endGroup();
	set.beginGroup(m_sectionName);
}

void TBaseGroup::readDataSet(QSettings &set)
{
	writeDataSet(set);
}

/****************************************************************************
**
** TBaseListValue class
**
*****************************************************************************/

TBaseListValue::TBaseListValue (TValueBrowser *parent, const QString& name, TItemList *list, int page, int id):
TBaseValue (parent, name, page, id) 
{
	setList(list);
}

TBaseListValue::TBaseListValue (TBaseItem *parent, const QString& name, TItemList *list, int id):
TBaseValue (parent, name, id) 
{ 
	setList(list);
}

bool TBaseListValue::setValue(const QVariant &value)
{
	// try to find such value in list...
	if (m_list) {
		// first, try to find by value
		TItem *it = m_list->findItem(value);
		if (it) {
			m_item = it;
			m_set.type = VT_VALUE;
			m_set.value = m_item->value;
			setupValue();
			return true;
		}

		// then, by explicit text
		it = m_list->findItemByText(value.toString());
		if (it) {
			m_item = it;
			m_set.type = VT_VALUE;
			m_set.value = m_item->value;
			setupValue();
			return true;
		}
	}

	bool b = TBaseValue::setValue(value);
	return b;
}


void TBaseListValue::setList(TItemList *list)
{
	// really maybe we should do more complex things here that only
	// to assign new list to the member, for example, first destroy it, or so on...
	m_list = list;
    if (m_list && m_list->count()) 
		//setValueById( (m_list->begin()).key() );
		setValue((*m_list->begin())->value);
	else
		setValue(QVariant());
	// to do: update all...
}


void TBaseListValue::fillEditor()
{
	TBaseValue::fillEditor();
	if (m_list) {
		if (!m_editor->isEmpty()) m_editor->addSeparator();
		//qDebug(QString::number((int)(m_list->size())));
		m_editor->addList(m_list);
	}
}

TValueValidator::EValidateResult TBaseListValue::validate(QVariant& value)
{
	if (!m_validator) return TValueValidator::VR_OK;
	return TBaseValue::validate(value);
}

bool TBaseListValue::setCurrentItem()
{
	if (!m_editor) return false;
	m_editor->setVisualMode(m_wasChanged && browser()->changeInfo());

	if (m_set.type == VT_VALUE)
		return m_editor->setCurrent(m_list/*, m_set.value*/);
	else 
		return TBaseValue::setCurrentItem();
}


/****************************************************************************
**
** TListValue class
**
*****************************************************************************/

bool TListValue::s_useOwnText = false;
bool TListValue::s_allowListAppend = false;

TListValue::TListValue (TValueBrowser *parent, const QString& name, TItemList *list, int page, int id):
	m_useOwnText(s_useOwnText),
	m_allowListAppend(s_allowListAppend),
	TBaseListValue (parent, name, list, page, id) 
{
	if (!list) m_useOwnText = true;
}

TListValue::TListValue (TBaseItem *parent, const QString& name, TItemList *list, int id):
	m_useOwnText(s_useOwnText),
	m_allowListAppend(s_allowListAppend),
	TBaseListValue (parent, name, list, id) 
{ 
	if (!list) m_useOwnText = true;
}

bool TListValue::setValue(const QVariant &value)
{
//    qDebug() << "TListValue::setValue";
    
	bool b = TBaseListValue::setValue(value);

	// APPENDING TO LIST
	if (m_list && m_allowListAppend && m_set.type == VT_OWN) {
		appendValue(m_set.value);
		return true;
	}

//    qDebug() << "TListValue::setValue::end";
	return b;

}

TValueValidator::EValidateResult TListValue::validate(QVariant& value)
{
	//if (m_useOwnText) return TValueValidator::VR_OK;
	if (!m_useOwnText) return TValueValidator::VR_BAD;
	return TBaseListValue::validate(value);
}

void TListValue::appendValue(const QVariant& val)
{
	// create list if it is empty
	if (!m_list) m_list = new TItemList();

	m_item = m_list->setItem(val, val.toString());
	m_set.type = VT_VALUE;
	m_set.value = val;
	
	if (m_editor) {
		m_editor->clear();
		fillEditor();
	}

	setupValue();
	setCurrentItem();
}

bool TListValue::setCurrentItem()
{
	if (!m_editor) return false;

	m_editor->enableOwnText(m_useOwnText);

	return TBaseListValue::setCurrentItem();
}

void TListValue::allowOwnValues(bool allow)
{
	m_useOwnText = allow;
//	if (editor) editor->enableOwnText(m_useOwnText);
}

void TListValue::allowListAppending(bool allow)
{
	m_allowListAppend = allow;
}

