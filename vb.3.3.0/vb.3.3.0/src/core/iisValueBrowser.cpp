#include "iisValueBrowser.h"
#include "iisItems.h"
#include "iisItemList.h"
#include "iisValidators.h"
#include "iisEditors.h"
#include "iisDropList.h"

#include "iisVBAddons.h"

/****************************************************************************
**
** SpecialValueList class
**
*****************************************************************************/

SpecialValueList::SpecialValueList(void) : TItemList()
{
	setItem(ID_DEFAULT, QObject::tr("Default"));
	setItem(ID_NOVALUE, QObject::tr("No Value"));
	setItem(ID_UNUSED, QObject::tr("Unused"));
	setItem(ID_EMPTY, QObject::tr("Empty"));
	setItem(ID_BAD, QObject::tr("Bad Value"));
}

/****************************************************************************
**
** TValueBrowser class
**
*****************************************************************************/

SpecialValueList TValueBrowser::s_specialList;

QIcon TValueBrowser::s_defaultRevertIcon;

TValueBrowser::TValueBrowser ( QWidget * parent, unsigned int itemDefHeight):
	m_page(0),
	m_specialList(0),
	m_Edit(false),
	m_changeInfo(true),
	m_ItemDefHeight(itemDefHeight),
    m_WidthScale(66),
	m_storingPolicy(SP_RAW),

	QTreeWidget (parent)
{
	TBaseItem::s_defaultPage = 0;

	m_revertIcon = s_defaultRevertIcon;

	//setSorting(-1);
	setRootIsDecorated(true);
	setIndentation(12);
	//setFocusPolicy(QWidget::StrongFocus);
	setUniformRowHeights(true);

	setHeaderLabels(QStringList() << tr("Item") << tr("Value"));
	header()->setMovable(false);

	setItemDelegate(new TValueItemDelegate(this));

	connect(this, SIGNAL(currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * )), 
		SLOT(selectionChanged ( QTreeWidgetItem *, QTreeWidgetItem * )) );

	connect(this, SIGNAL(itemChanged ( QTreeWidgetItem *, int )), 
		SLOT(onItemChanged ( QTreeWidgetItem *, int )) );

	connect(this, SIGNAL(itemClicked ( QTreeWidgetItem *, int )), 
		SLOT(onItemClicked ( QTreeWidgetItem *, int )) );
}


void TValueBrowser::resizeEvent ( QResizeEvent * event )
{
	QTreeWidget::resizeEvent(event);

	//if (header()->sectionSize(0) > event->size().width())
		header()->resizeSection(0, event->size().width() * m_WidthScale / 100 /*2 / 3*/);
	//else
		//header()->resizeSection(0, header()->sectionSize(0)+(event->size().width() - event->oldSize().width()));
}

void TValueBrowser::hideEvent ( QHideEvent * event )
{
	// #### This leads to crash in 4.3, don't know what this even was needed for...
	//selectionChanged(0, currentItem());
}

bool TValueBrowser::viewportEvent ( QEvent * event )
{
// #### This was cut from Qt 4.2.3 sources - due to absence of void QWidget::helpEvent(...)
#ifndef QT_NO_TOOLTIP
    if (event->type() == QEvent::ToolTip) {
        if (!isActiveWindow())
            return false;
        QHelpEvent *he = static_cast<QHelpEvent*>(event);
        QVariant tooltip = model()->data(indexAt(he->pos()), Qt::ToolTipRole);
        if (qVariantCanConvert<QString>(tooltip)) {

#if QT_VERSION >= 0x040200
            QVariant ttfont = model()->data(indexAt(he->pos()), ToolTipFontRole);
            if (ttfont.isValid())
                QToolTip::setFont(ttfont.value<QFont>());
            else
                QToolTip::setFont(QFont());
#endif

            QToolTip::showText(he->globalPos(), tooltip.toString(), this);
            return true;
        }
    }
#endif

    return QTreeWidget::viewportEvent(event);
}

void TValueBrowser::setWidthScale(unsigned int scale)
{
    if (scale < 5 || scale > 95) return;
    m_WidthScale = scale;
    update();
}

void TValueBrowser::clear()
{
    m_page = 0;
    TBaseItem::s_defaultPage = 0;

    QTreeWidget::clear();
}

void TValueBrowser::setSpecialList(SpecialValueList* nameList, bool freeOld)
{
	if (freeOld && m_specialList) delete m_specialList;
	m_specialList = nameList;
}


void TValueBrowser::selectionChanged ( QTreeWidgetItem *item, QTreeWidgetItem* prevItem )
{
	blockSignals(true);

	if (prevItem)
		((TBaseItem*)prevItem)->onLeaveItem();

	if (item)
		((TBaseItem*)item)->onEnterItem();

	blockSignals(false);

	// emit hint change
	doHintChange((TBaseItem*)item);
}

TBaseItem* TValueBrowser::getItem(int id)
{
	TBaseItem *lvi;
    QTreeWidgetItemIterator it(this);
    while ( lvi = (TBaseItem*)(*it) ) {
        if ( lvi->id == id )
			return lvi;
        ++it;
	}
	return NULL;
}

void TValueBrowser::switchPage(int page)
{
	if (page != m_page) {
		m_page = page;

		QTreeWidgetItemIterator it(this);
		TBaseItem *v;
		while ( v = (TBaseItem*)(*it) ) {
            v->setVisible(v->visible());
			++it;
		}

		v = dynamic_cast<TBaseItem*>(currentItem());
		if (v && !v->isHidden())
			doHintChange(v);
		else
			doHintChange(0);
	}
}

void TValueBrowser::switchPage(const QModelIndex & index)
{
    switchPage(index.row());
}

void TValueBrowser::doHintChange(TBaseItem *item)
{
	if (item) {
		emit HintChanged(item->hint(), item, item->text(0));
		emit ItemHint(item->hint());
	} else {
		emit HintChanged(QString::null, 0, QString::null);
		emit ItemHint(QString::null);
	}
}


void TValueBrowser::valuePicked(TItemList *list, TItem *item, const QVariant &value)
{
	TBaseValue *bi = dynamic_cast<TBaseValue*>(currentItem());
    if (!bi) return;

	if (value == bi->variant())
		return;

    m_Edit = true;
	QVariant v(value);
	bi->valuePicked(list, item, v);
	m_Edit = false;
	emit ValuePicked(bi, bi->value());
}

void TValueBrowser::restoreSetCurrent()
{
	TBaseValue *bi = (TBaseValue*)currentItem();
	bi->restoreSet();
}

//void TValueBrowser::doValueChanged(TBaseItem *item, TValue *linkedValue)
//{
//	if (item)
//		emit ValueChanged(item, linkedValue);
//	else
//		emit ValueChanged(0, 0);
//}

void TValueBrowser::storeSet(bool emitChanges)
{
	QTreeWidgetItemIterator it(this);
	TBaseItem *v;
	while ( v = (TBaseItem*)(*it) ) {
		v->storeSet(emitChanges);
		++it;
	}
}

void TValueBrowser::restoreSet()
{
	QTreeWidgetItemIterator it(this);
	TBaseItem *v;
	while ( v = (TBaseItem*)(*it) ) {
		v->restoreSet();
		++it;
	}
}

void TValueBrowser::onItemChanged(QTreeWidgetItem *item, int column)
{
    TBaseItem *itm = dynamic_cast<TBaseItem*>(item);
    if (itm)
        itm->onChangeItem(column);
}

void TValueBrowser::setChangeInfo(bool ci)
{
	if (ci != m_changeInfo) {
		m_changeInfo = ci;
		//// update change info...
		//QTreeWidgetItemIterator it(*this);
		//TBaseItem *v;
		//while (it.hasNext()) {
		//	v = (TBaseItem*)(it.next());
		//	if(v->hasValue())
		//		if (((TBaseValue*)v)->valueChanged()) {
		//			v->repaint();
		//			((TBaseValue*)v)->setCurrentItem();	// ??? review !!!
		//		}
		//}
	}
}

void TValueBrowser::onItemClicked(QTreeWidgetItem *item, int column)
{
	((TBaseItem*)item)->onClickItem(column);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Defines for quick add methods

#define CHECKITEMLEVEL \
	if (level < 0 || name.isEmpty()) return 0; \
	int l = level-1; \
	if (l > m_levels.size()) \
		l = m_levels.size()-1;

#define SETITEMLEVEL if (level >= m_levels.size()) m_levels.append(item); else m_levels[l+1] = item; 

#define SETITEMPROPERTIES \
	item->setHint(hint); \
	item->setPropertyName(propertyName); \
	item->setLinked(linked, type);

#define CHILDITEM l >= 0

#define PARENT m_levels[l]

bool TValueBrowser::setItemLevel(TBaseItem *item, int level)
{
    if (!item) return false;
    QString name = item->text(0);
    CHECKITEMLEVEL
    SETITEMLEVEL
    return true;
}

///////////////////////////////////////////////////////////////////////////////

// Page quick add

int	TValueBrowser::addPage()
{
	return ++TBaseItem::s_defaultPage;
}

///////////////////////////////////////////////////////////////////////////////

// Group quick add

TBaseGroup* TValueBrowser::addGroup(const QString &name, const QString &sectionName, bool subgroup)
{
	if (name.isEmpty()) return 0;

	m_levels.clear();

	TBaseGroup *bg =  new TBaseGroup(this, name, -1, subgroup);
	bg->setSectionName(sectionName);

	return bg;
}

///////////////////////////////////////////////////////////////////////////////

// Values quick add

TIntegerValue* TValueBrowser::addInt(int level, const QString &name, const QString &hint, const QString &propertyName,
				void *linked, int type, TIntValidator *validator)
{
	CHECKITEMLEVEL;

	TIntegerValue *item;

	if (CHILDITEM)
		item = new TIntegerValue(PARENT, name);
	else
		item = new TIntegerValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	if (validator) item->setValidator(validator);

	return item;
}


TDoubleValue* TValueBrowser::addReal(int level, const QString &name, const QString &hint, const QString &propertyName,
				void *linked, int type, TDoubleValidator *validator)
{
	CHECKITEMLEVEL;

	TDoubleValue *item;

	if (CHILDITEM)
		item = new TDoubleValue(PARENT, name);
	else
		item = new TDoubleValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	if (validator) item->setValidator(validator);

	return item;
}


TBaseValue* TValueBrowser::addValue(int level, const QString &name, const QString &hint, const QString &propertyName,
				void *linked, int type, TValueValidator *validator)
{
	CHECKITEMLEVEL;

	TBaseValue *item;

	if (CHILDITEM)
		item = new TBaseValue(PARENT, name);
	else
		item = new TBaseValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	if (validator) item->setValidator(validator);

	return item;
}


TBoolValue*	TValueBrowser::addBool(int level, const QString &name, const QString &hint, const QString &propertyName, void *linked)
{
	CHECKITEMLEVEL;

	TBoolValue *item;
	const int type = LT_BOOL;

	if (CHILDITEM)
		item = new TBoolValue(PARENT, name);
	else
		item = new TBoolValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	return item;
}


TTextValue* TValueBrowser::addText(int level, const QString &name, const QString &hint, const QString &propertyName, void *linked)
{
	CHECKITEMLEVEL;

	TTextValue *item;
	const int type = LT_STRING;

	if (CHILDITEM)
		item = new TTextValue(PARENT, name);
	else
		item = new TTextValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	return item;
}


TColorValue* TValueBrowser::addColor(int level, const QString &name, const QString &hint, const QString &propertyName, 
				void *linked, TColorList *list)
{
	CHECKITEMLEVEL;

	TColorValue *item;
	const int type = LT_COLOR;
	if (!list) list = &TColorList::baseColorList();

	if (CHILDITEM)
		item = new TColorValue(PARENT, name, list);
	else
		item = new TColorValue(this, name, list);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	item->allowOwnValues();
	//item->allowListAppending();

	return item;
}


TListValue* TValueBrowser::addList(int level, const QString &name, const QString &hint, const QString &propertyName,
				void *linked, int type, TItemList *list, bool own, bool append)
{
	CHECKITEMLEVEL;

	TListValue *item;

	if (CHILDITEM)
		item = new TListValue(PARENT, name, list);
	else
		item = new TListValue(this, name, list);

	item->allowOwnValues(own);
	item->allowListAppending(append);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	return item;
}


TFontValue* TValueBrowser::addFont(int level, const QString &name, const QString &hint, const QString &propertyName, 
				void *linked, TFontList *list)
{
	CHECKITEMLEVEL;

	TFontValue *item;
	const int type = LT_FONT;
	if (!list) list = &TFontList::fullFontList();

	if (CHILDITEM)
		item = new TFontValue(PARENT, name, list);
	else
		item = new TFontValue(this, name, list);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	return item;
}

TPointValue* TValueBrowser::addPoint(int level, const QString &name, const QString &hint, const QString &propertyName, 
				void *linked, TPointValidator *validator)
{
	CHECKITEMLEVEL;

	TPointValue *item;
	const int type = LT_POINT;

	if (CHILDITEM)
		item = new TPointValue(PARENT, name);
	else
		item = new TPointValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

    if (validator) item->setValidator(validator);

	return item;
}

TSizeValue* TValueBrowser::addSize(int level, const QString &name, const QString &hint, const QString &propertyName, 
				void *linked, TSizeValidator *validator)
{
	CHECKITEMLEVEL;

	TSizeValue *item;
	const int type = LT_SIZE;

	if (CHILDITEM)
		item = new TSizeValue(PARENT, name);
	else
		item = new TSizeValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

    if (validator) item->setValidator(validator);

	return item;
}

TRectValue* TValueBrowser::addRect(int level, const QString &name, const QString &hint, const QString &propertyName, 
				void *linked, TRectValidator *validator)
{
	CHECKITEMLEVEL;

	TRectValue *item;
	const int type = LT_RECT;

	if (CHILDITEM)
		item = new TRectValue(PARENT, name);
	else
		item = new TRectValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

    if (validator) item->setValidator(validator);

	return item;
}

TRangeValue* TValueBrowser::addRange(int level, const QString &name, const QString &hint, const QString &propertyName, 
				void *linked, TRangeValidator *validator)
{
	CHECKITEMLEVEL;

	TRangeValue *item;
	const int type = LT_POINT;

	if (CHILDITEM)
		item = new TRangeValue(PARENT, name);
	else
		item = new TRangeValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

    if (validator) item->setValidator(validator);

	return item;
}


TPathValue*	TValueBrowser::addPath(int level, const QString &name, const QString &hint, const QString &propertyName, void *linked)
{
	CHECKITEMLEVEL;

	TPathValue *item;
	const int type = LT_STRING;

	if (CHILDITEM)
		item = new TPathValue(PARENT, name);
	else
		item = new TPathValue(this, name);

	SETITEMLEVEL;

	SETITEMPROPERTIES;

	return item;
}

///////////////////////////////////////////////////////////////////////////////

void TValueBrowser::updateFromLinked()
{
	QTreeWidgetItemIterator it(this);
	TBaseItem *v;
	while ( v = (TBaseItem*)(*it) ) {
		v->updateFromLinked();
		++it;
	}
}

void TValueBrowser::updateLinked()
{
	QTreeWidgetItemIterator it(this);
	TBaseItem *v;
	while ( v = (TBaseItem*)(*it) ) {
		v->updateLinked();
		++it;
	}
}


void TValueBrowser::setStoringPolicy(EStoringPolicy policy)
{
	m_storingPolicy = policy;
}

void TValueBrowser::writeDataSet(QSettings &set)
{
	QTreeWidgetItemIterator it(this);
	TBaseItem *v;
	while ( v = (TBaseItem*)(*it) ) {
		v->writeDataSet(set);
		++it;
	}
}

void TValueBrowser::readDataSet(QSettings &set)
{
	QTreeWidgetItemIterator it(this);
	TBaseItem *v;
	while ( v = (TBaseItem*)(*it) ) {
		v->readDataSet(set);
		++it;
	}
}
