#ifndef TTEXTVALUE_H
#define TTEXTVALUE_H

#include <iisDefines.h>

#include <iisVB.h>

/****************************************************************************
**
** TTextButtonEditor class
**
** Button editor for TTextValue.
**
*****************************************************************************/

class LIBVB4_EXPORT TTextButtonEditor : public TPopupButtonEditor
{
public:
	TTextButtonEditor();

protected slots:
	virtual void	buttonClicked();
};

/****************************************************************************
**
** TTextValue class
**
*****************************************************************************/

/*! \class TTextValue 
 * \nosubgrouping
 * \brief Multiline text input.

	This class supports multiline textual values and corresponding facilities for editing.
 */

class LIBVB4_EXPORT TTextValue: public TBaseValue
{
public:
	TTextValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	TTextValue (TBaseItem *parent, const QString& name, int id = -1);

    //virtual QString valueText() const;

	// Standard button editor.
	static TTextButtonEditor s_TextButtonEditor;
};

/****************************************************************************/

#endif