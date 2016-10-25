#ifndef TPATHVALUE_H
#define TPATHVALUE_H

#include <iisDefines.h>

#include <iisVB.h>

/****************************************************************************
**
** TPathButtonEditor class
**
** Button editor for TPathValue.
**
*****************************************************************************/

class LIBVB4_EXPORT TPathButtonEditor : public TPopupButtonEditor
{
public:
	TPathButtonEditor();

protected slots:
	virtual void	buttonClicked();
};

/****************************************************************************
**
** TPathValue class
**
*****************************************************************************/

/*! \class TPathValue 
 * \nosubgrouping
 * \brief File path input.

	This class supports file path input using QFileDialog methods.
 */

class LIBVB4_EXPORT TPathValue: public TBaseValue
{
public:
	TPathValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	TPathValue (TBaseItem *parent, const QString& name, int id = -1);

	void setPickMode(QFileDialog::FileMode mode) { m_mode = mode; }
	inline QFileDialog::FileMode pickMode() const { return m_mode; }

	void setFilter(const QString &filter) { m_filter = filter; }
	inline const QString& filter() const { return m_filter; }

	// Standard button editor.
	static TPathButtonEditor s_PathButtonEditor;

protected:
	void init(); 

	QFileDialog::FileMode m_mode;
	QString m_filter;
};

/****************************************************************************/

#endif