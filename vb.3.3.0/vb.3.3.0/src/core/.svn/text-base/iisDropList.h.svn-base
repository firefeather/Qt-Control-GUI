#ifndef TDROPLIST_H
#define TDROPLIST_H

#include <QtGui>
#include <QtCore>

class TItem;
class TItemList;
class TBaseValue;
class TButtonEditor; 
class TComboEditor; 

//***************************************************************************//
//
// TComboBox Class
//
// It is the internal class which represents drop-down item list.
//
// In most cases, there is no need to modify or use this class directly
// within your Application.
//
//***************************************************************************//

class TComboBox : public QComboBox
{
	friend class TDropList;

	Q_OBJECT

public:
	TComboBox( QWidget *parent, TBaseValue *item );

	void setupText();
	void restoreText();

    virtual void showPopup();

	inline bool filled() const { return m_listLoaded; }

signals:
	void restoreInvoked(void);

protected slots:
    void onHighlight(int index);

protected:
	virtual void keyPressEvent ( QKeyEvent *event );
	virtual void wheelEvent ( QWheelEvent *event );
	virtual void mouseClickEvent ( QMouseEvent *event );
	virtual void mouseDoubleClickEvent ( QMouseEvent *event );
    virtual void focusOutEvent ( QFocusEvent *event );

    void confirmEnter();
    bool listUpDown(int i);
	void loadList();

	QString		m_lastText;
	TBaseValue	*m_item;

	bool m_listLoaded;
	
	//QString		m_lastStyle;
};

//***************************************************************************//
//
// TComboDelegate Class
//
// It is the internal class which represents drop-down item list delegate.
//
// In most cases, there is no need to modify or use this class directly
// within your Application.
//
//***************************************************************************//

class TComboDelegate : public QItemDelegate
{
public:
	TComboDelegate( QObject * parent = 0);

	virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

protected:
	QAbstractItemModel *m_model;
};

//***************************************************************************//
//
// TDropList Class
//
// It is the internal class which wraps drop-down list editor.
//
// In most cases, there is no need to modify or use this class directly
// within your Application.
//
//***************************************************************************//

class TDropList : public QWidget
{
	friend class TComboBox;
	friend class TButtonEditor;
	friend class TComboEditor;

	Q_OBJECT

public:
	TDropList( QWidget * parent, TBaseValue * item );
	virtual ~TDropList() {};

	void		addList(TItemList *list);
	void		addSeparator();
	void		addComboEditor(TComboEditor *editor);

	void		setButtonEditor(TButtonEditor *editor); 

	void		clear()			{ m_box->clear(); };
	bool		isEmpty()		{ return !(m_box->count()); };

	bool		setCurrent(TItemList *list/*, const QVariant &value*/);
	void		prepare()		{ m_box->loadList(); }

	bool		ownText() const		{ return m_owntext; }
	void		enableOwnText(bool enable = true);

	virtual void	setVisualMode(bool highlighted);

	TBaseValue* item() const	{ return m_item;	} 

signals:
	void		picked (TItemList *list, TItem *item, const QVariant &value);
	void		restoreValueInvoked();

protected:
	virtual void	focusInEvent ( QFocusEvent * event );

	TComboBox	*m_box;
	QLineEdit	*lineedit;
	bool		updated, m_owntext, m_skipPress;
	TBaseValue	*m_item;
	QHBoxLayout	*m_layout;
	QWidget		*m_buttonEditor; 
	QPushButton *m_revertEditor;

protected slots:
	void		onPickItem(int);
	void		doPicked(QVariant v);
};

////***************************************************************************//

#endif
