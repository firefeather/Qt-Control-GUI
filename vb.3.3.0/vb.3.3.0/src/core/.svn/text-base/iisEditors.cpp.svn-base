#include "iisEditors.h"


//***************************************************************************//
//
// TButtonEditor Class
//
// It is the class for external Button Editor which could be assigned for 
// Item Editor.
//
//***************************************************************************//

QWidget* TButtonEditor::createEditor(TDropList *parent)
{
	m_dropList = parent;
	m_editedValue = m_dropList->item();
	return 0;
}

//***************************************************************************//
//
// TPopupButtonEditor Class
//
// It is the class for external Button Editor which introduces a tool button 
// by clicking on which you could show some pop-up window for editing.
//
//***************************************************************************//

QWidget* TPopupButtonEditor::createEditor(TDropList *parent)
{
	TButtonEditor::createEditor(parent);

	m_button = new QToolButton(parent);
	m_button->setFixedWidth(20);
//	m_button->setUsesTextLabel(true);
	m_button->setText("...");
	m_button->setToolTip(tr("Pick value from dialog"));
	connect(m_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	return m_button;
}

//***************************************************************************//
//
// TSpinButtonEditor class
//
//***************************************************************************//

QWidget* TSpinButtonEditor::createEditor(TDropList *parent)
{
	TButtonEditor::createEditor(parent);

	QWidget *w = new QWidget(parent);
	w->setFixedWidth(20);
	w->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	m_up = new QToolButton(w);
	m_up->setArrowType(Qt::UpArrow);
	m_up->setAutoRepeat(true);
	m_down = new QToolButton(w);
	m_down->setArrowType(Qt::DownArrow);
	m_down->setAutoRepeat(true);

	connect(m_up, SIGNAL(clicked()), this, SLOT(buttonClickedUp()));
	connect(m_down, SIGNAL(clicked()), this, SLOT(buttonClickedDown()));

	QVBoxLayout *vbl = new QVBoxLayout(w);
	vbl->setMargin(0);
	vbl->setSizeConstraint(QLayout::SetNoConstraint);
	vbl->setSpacing(0);
	vbl->addWidget(m_up);
	vbl->addWidget(m_down);

	return w;
}
 

//***************************************************************************//
//
// TComboEditor Class
//
// It is the base class for external Combo (in-list) Editor which could be assigned for 
// Item Editor.
//
//***************************************************************************//

TComboEditor::TComboEditor() :
	m_text(QObject::tr("Pick..."))
{
}
