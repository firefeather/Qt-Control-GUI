#include "iisDropList.h"

#include "iisItemList.h"
#include "iisItems.h"
#include "iisEditors.h"
#include "iisValueBrowser.h"

#include <QApplication>
#include <QAbstractItemModel>
 
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

#define ListRole		Qt::UserRole + 1
#define ListIndexRole	Qt::UserRole + 2
#define EditorRole		Qt::UserRole + 3

#ifdef Q_OS_WIN32
#define Y_SHIFT 24
#else
#define Y_SHIFT 16
#endif

// added for compatibility with Qt 4.1.x
#if QT_VERSION < 0x040200

#if QT_POINTER_SIZE == 4
#   define  quintptr quint32
#else
#   define  quintptr quint64
#endif

#endif

//***************************************************************************//
//
// TComboBox Class
//
//***************************************************************************//

TComboBox::TComboBox( QWidget *parent, TBaseValue *item ) : QComboBox(parent)
{
	m_item = item;
	m_listLoaded = false;

	setEditable(true);
	setupText();

	setItemDelegate(new TComboDelegate(this));

    connect(this, SIGNAL(highlighted(int)), this, SLOT(onHighlight(int)));

	//m_lastStyle = styleSheet();
	//setStyleSheet("padding: 1px 1px 1px 3px;");
}

void TComboBox::showPopup()
{
	loadList();

    QComboBox::showPopup();

	TValueBrowser *vb = m_item->browser();

    // let's find container among the children
    const QObjectList &kids = children();
    for (int i = 0; i < kids.count(); i++) {
        QFrame *fr = dynamic_cast<QFrame*>(kids.at(i));
        if (fr) {
            int w1 = vb->header()->sectionViewportPosition(1);
            QPoint pos = vb->viewport()->pos();
            pos = vb->mapToGlobal(pos);
            QRect r = fr->geometry();
            r.setX(w1 + pos.x() - 1);
            fr->setGeometry(r);
            break;
        }
    }
}

void TComboBox::setupText()
{
    m_lastText = lineEdit()->text();
	lineEdit()->selectAll();
    lineEdit()->setFrame(false);
}

void TComboBox::restoreText()
{
	lineEdit()->setText(m_lastText);
	lineEdit()->selectAll();
}

void TComboBox::focusOutEvent ( QFocusEvent * event )
{
    switch (event->reason()) {
        case Qt::PopupFocusReason:
        case Qt::OtherFocusReason:
        case Qt::ActiveWindowFocusReason:

// Workaround - in 4.1.4, extra Qt::TabFocusReason is called
// UNFIXED: item value not updated when leaving window by tab
#if QT_VERSION < 0x040200 || QT_VERSION >= 0x040300
        case Qt::TabFocusReason:
#endif
 
            break;
        default: 
            confirmEnter();
    }

    QComboBox::focusOutEvent(event);
}

void TComboBox::confirmEnter()
{
	for (int i = 0; i < count(); i++) {
		QVariant v = itemData(i, Qt::UserRole);

#if QT_VERSION >= 0x040200
		if (v.isValid() && !(itemText(i).compare(lineEdit()->text(), Qt::CaseInsensitive))) 
#else
		if (v.isValid() && (itemText(i).toLower() == lineEdit()->text().toLower()))
#endif
		{
			setCurrentIndex(i);
			emit activated(i);
			return;
		}
	} 
	emit activated(-1);
}

bool TComboBox::listUpDown(int i)
{
	//loadList();

    QVariant v = itemData(i, Qt::UserRole);
	if (v.isValid()) {
		setCurrentIndex(i);

        TItemList *l = (TItemList*)(itemData(i, ListRole).toUInt());
        if (l)
        {
//	    int h = qApp->activeWindow()->y()
//		- qApp->activeWindow()->geometry().y();
//		qDebug() << h;
	
            l->highlightComboItem(v, i, this, 

                //                m_item->browser()->mapToGlobal(
                //		    parentWidget()->pos() + rect().bottomLeft()    )
                //    mapToGlobal( pos() + geometry().bottomLeft()    )
                //    - QPoint(2, 16)
                    mapToGlobal(QPoint(-2, height()-Y_SHIFT))
                );
        }

		lineEdit()->selectAll();
		emit activated(i);
	
        return true;
    }

    return false;
}

void TComboBox::keyPressEvent( QKeyEvent * e )
{
	if (e->key() == Qt::Key_Down) {
		e->accept();

		if ((e->modifiers() & Qt::ShiftModifier)) {
			loadList();
			if (count()) {
				int i = currentIndex();
				while (i < count()) {
					if (listUpDown(++i))
						break;
				}
			}
		} else 
			if (e->modifiers() & Qt::AltModifier) {
				showPopup();
			} else {
                confirmEnter();
				QApplication::sendEvent(m_item->browser(), e);
			}

		return;
	}

	if (e->key() == Qt::Key_Up) {
		e->accept();
		if ((e->modifiers() & Qt::ShiftModifier)) {
			loadList();
			if (count()) {
				int i = currentIndex();
				while (i > 0) {
					if (listUpDown(--i))
						break;
				}
			}
		} else {
            confirmEnter();
			QApplication::sendEvent(m_item->browser(), e);
		} 

		return;
	}

	if (e->key() == Qt::Key_Escape) {
		e->accept();
		if (e->modifiers() & Qt::ShiftModifier) {	// restore stored value
			emit restoreInvoked();
		} else {	// cancel last entered
			restoreText();
		}
		return;
	}

	loadList();

	if (e->key() == Qt::Key_Return) {
        confirmEnter();
		return;
	}

	QComboBox::keyPressEvent(e);
}

void TComboBox::wheelEvent( QWheelEvent * e )
{
	QKeyEvent ek(QEvent::KeyPress,
		e->delta() >= 0 ? Qt::Key_Up : Qt::Key_Down,
		e->modifiers() | Qt::ShiftModifier );
	QApplication::sendEvent( this, &ek );
}

void TComboBox::mouseClickEvent( QMouseEvent * event )
{
}

void TComboBox::mouseDoubleClickEvent( QMouseEvent * event )
{
}

void TComboBox::onHighlight(int index)
{
    QVariant v = itemData(index, Qt::UserRole);
    if (v.isValid()) {
        TItemList *l = (TItemList*)(itemData(index, ListRole).toUInt());
        if (l) {
            l->highlightComboItem(v, index, this, 
                //m_item->browser()->mapToGlobal(parentWidget()->pos() + rect().bottomRight())
		            mapToGlobal(rect().topRight() + QPoint(-2, height()-Y_SHIFT))
                );
        }
    }
}

void TComboBox::loadList()
{
	if (!m_listLoaded) {
		m_listLoaded = true;
		clear();
		m_item->fillEditor();

		//if (!count()) 
		//	setStyleSheet("padding: 1px 1px 1px 1px;");

		m_item->setCurrentItem();

		//TItemList *list = m_item->list();

		//if (list) {
		//	QString v = m_item->valueText()/*value.toString()*/;
		//	for (int i = 0; i < count(); i++) {
		//		TItemList *l = (TItemList*)(itemData(i, ListRole).toUInt());
		//		if ((l == list) && l->compareComboItem(itemData(i, Qt::UserRole), m_item->value().value)) {
		//			setCurrentIndex(i);
		//			setupText();
		//			return;
		//		}
		//	}
		//}		
	}
}

//***************************************************************************//
//
// TComboDelegate Class

//***************************************************************************//

TComboDelegate::TComboDelegate( QObject * parent ) : QItemDelegate(parent)
{
	m_model = ((TComboBox*)parent)->model();
}

void TComboDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const 
{
    QVariant v = m_model->data(index, Qt::UserRole);
	if (v.isValid() || m_model->data(index, EditorRole).isValid()) {
        // draw item
        TItemList *l = (TItemList*)(m_model->data(index, ListRole).toUInt());
        bool b = true;
        QStyleOptionViewItem opt(option);
        if (l)
            b = l->paintComboItem(painter, opt, index, v);
        if (b)
		    QItemDelegate::paint(painter, opt, index);
	} else {
        // draw separator
		painter->setPen(Qt::gray);
		QRect r = option.rect.adjusted(2,0,-2,-(option.rect.height()/2));
		painter->drawLine(QLine(r.bottomLeft(), r.bottomRight()));
	}
}

QSize TComboDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QVariant v = m_model->data(index, Qt::UserRole);
    if (v.isValid()) {
        TItemList *l = (TItemList*)(m_model->data(index, ListRole).toUInt());
        if (l) {
            QSize s = l->sizeHintComboItem(v, option);
            if (s.isValid())
                return s;
        }
    }

    return QItemDelegate::sizeHint(option, index);
}

//****************************************************************************
//**
//** TDropList class
//**
//*****************************************************************************/

TDropList::TDropList( QWidget * parent, TBaseValue * item ) :
	m_owntext(false),
    m_skipPress(false),
	m_item(item),
	m_buttonEditor(0),
	QWidget(parent)
{
	m_box = new TComboBox(this, item);
	lineedit = m_box->lineEdit();
	m_box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	connect(m_box, SIGNAL(activated(int)), this, SLOT(onPickItem(int)));
	connect(m_box, SIGNAL(restoreInvoked()), this, SIGNAL(restoreValueInvoked()));
	
	updated = false;

	QHBoxLayout *hl = new QHBoxLayout(this);
	hl->setMargin(0);
	hl->setSpacing(0);
	hl->addWidget(m_box);

	m_revertEditor = new QPushButton(m_item->browser()->revertIcon(), "", this);
	if (m_revertEditor->icon().isNull())
		//m_revertEditor->setText("<");
		m_revertEditor->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
	m_revertEditor->setFixedWidth(20);
	m_revertEditor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
	m_revertEditor->setToolTip(tr("Restore previous value (Shift+Esc)"));
	connect(m_revertEditor, SIGNAL(clicked()), this, SIGNAL(restoreValueInvoked()));
	hl->addWidget(m_revertEditor);
}

void TDropList::enableOwnText(bool enable)
{
	m_owntext = enable;
}

void TDropList::addList(TItemList *list)
{
	updated = false;
	if (!list) return;

	TItem *it;
	for (int i = 0; i < list->count(); i++) {
		it = list->at(i);
		m_box->addItem(it->icon ? *(it->icon) : QIcon(), it->text, it->value);
		m_box->setItemData(m_box->count()-1, (quintptr)list, ListRole);
		m_box->setItemData(m_box->count()-1, i, ListIndexRole);
	}
}

void TDropList::addSeparator()
{
	updated = false;
	m_box->addItem("", QVariant());	// invalid
}

void TDropList::addComboEditor(TComboEditor *editor)
{
	if (!editor) return;

	m_box->addItem(editor->text(), QVariant());	// invalid
	m_box->setItemData(m_box->count()-1, (quintptr)editor, EditorRole);

	editor->m_dropList = this;
}


bool TDropList::setCurrent(TItemList *list/*, const QVariant &value*/)
{
//	//qDebug(value.toString());
    m_box->setCurrentIndex(-1);
    m_item->browser()->updateEditorGeometries();

	if (!m_box->filled()) {
		m_box->addItem(m_item->autoIconActive() ? QIcon() : m_item->icon(1), m_item->text(1));
		//lineedit->setText(m_item->text(1));
		m_box->setupText();
		    m_item->browser()->updateEditorGeometries();
		return true;
	}

	if (list) {
		QString v = m_item->valueText()/*value.toString()*/;
		for (int i = 0; i < m_box->count(); i++) {
			TItemList *l = (TItemList*)(m_box->itemData(i, ListRole).toUInt());
            if ((l == list) && l->compareComboItem(m_box->itemData(i, Qt::UserRole), m_item->value().value)) {
				m_box->setCurrentIndex(i);
				m_box->setupText();
				return true;
			}
		}
	}

	// item not found...
	if (!m_owntext)	return false;

	// else we have to set specified text...   
    lineedit->setText(m_item->valueText());
	m_box->setupText();
	return true;
}

void TDropList::setVisualMode(bool highlighted)
{
	QFont f(lineedit->font());
	f.setBold(highlighted);
	lineedit->setFont(f);
}

void TDropList::onPickItem(int index)
{
//	qDebug() << "index: " << index << " text: " << lineedit->text();
	//m_box->loadList();

	if (!m_box->filled())	// no list load -> no changes
		return;
	
	// index can be invalid!
	if (index < 0 || index >= m_box->count()) {
		emit picked(0, 0, lineedit->text());
		return;
	}

	QVariant v = m_box->itemData(index, Qt::UserRole);
	
//	qDebug() << "v: " << v;

	if (v.isValid()) {
		TItemList *l = (TItemList*)(m_box->itemData(index, ListRole).toUInt());
		if (l) {
			int idx = m_box->itemData(index, ListIndexRole).toUInt();
			emit picked(l, l->at(idx), v);
		} else
			emit picked(0, 0, lineedit->text());
	} else {
		m_box->restoreText();
		TComboEditor *ed = (TComboEditor*)(m_box->itemData(index, EditorRole).toUInt());	
		if (ed) 
			ed->invoke(m_item);
	}
}

void TDropList::focusInEvent ( QFocusEvent * event )
{
	m_box->setFocus();
	lineedit->selectAll();

    QWidget::focusInEvent(event);
}

void TDropList::setButtonEditor(TButtonEditor *editor)
{
	// remove old editor
	if (m_buttonEditor) {
		delete m_buttonEditor;
		m_buttonEditor = 0;
	}

	// set new editor
	if (editor) {
		m_buttonEditor = editor->createEditor(this);
        m_buttonEditor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
		QHBoxLayout *hbl = (QHBoxLayout*)layout();
		hbl->insertWidget(hbl->count()-1, m_buttonEditor);
		m_buttonEditor->show();
	}
}

void TDropList::doPicked(QVariant v)
{
	m_box->loadList();
	emit picked(0, 0, v/*.toString()*/);
}
