#ifndef iisEditors_H
#define iisEditors_H

#include <QtGui>
#include <QtCore>

#include "iisDropList.h"

class TBaseValue;

//***************************************************************************//
//
// TButtonEditor Class
//
// It is the base class for external Button Editor which could be assigned for 
// Item Editor.
//
//***************************************************************************//

class TButtonEditor	: public QObject
{
	friend class TDropList;

public:
	virtual QWidget* createEditor(TDropList *parent);
	
protected:
	void		doPickValue(QVariant v)
	{ m_dropList->doPicked(v); }

	TDropList	*m_dropList;
	TBaseValue	*m_editedValue;
};

//***************************************************************************//
//
// TPopupButtonEditor Class
//
// It is the class for external Button Editor which introduces a tool button 
// by clicking on which you could show some pop-up window for editing.
//
//***************************************************************************//

class TPopupButtonEditor : public TButtonEditor
{
	Q_OBJECT
public:
	virtual QWidget* createEditor(TDropList *parent);

protected:
	QToolButton		*m_button;

protected slots:
	virtual void	buttonClicked() = 0;
};

//***************************************************************************//
//
// TSpinButtonEditor class
//
// Provides Button Editor with spin (up-down arrows) control.
//
//***************************************************************************//

class TSpinButtonEditor : public TButtonEditor
{
	Q_OBJECT
public:
	virtual QWidget* createEditor(TDropList *parent);

protected:
	QToolButton		*m_up, *m_down;

protected slots:
	virtual void	buttonClickedUp() = 0;
	virtual void	buttonClickedDown() = 0;
};

//***************************************************************************//
//
// TComboEditor Class
//
// It is the base class for external Combo (in-list) Editor which could be assigned for 
// Item Editor.
//
//***************************************************************************//

class TComboEditor
{
	friend class TDropList;

public:
	TComboEditor();
	virtual ~TComboEditor() {};

	const QString& text() const 
	{ return m_text; }

	virtual void invoke(TBaseValue *item) = 0;

protected:
	void	doPickValue(QVariant v)
	{ m_dropList->doPicked(v); }

	TDropList	*m_dropList;
	QString		m_text;
};

////***************************************************************************//

#endif
