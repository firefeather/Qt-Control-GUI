#include "iisTextValue.h"
#include "iisTextEditorDialog.h"

#include <math.h>

/****************************************************************************
**
** TTextButtonEditor class
**
*****************************************************************************/

TTextButtonEditor::TTextButtonEditor() : 
TPopupButtonEditor()
{
}

void TTextButtonEditor::buttonClicked()
{
	SValue &v = m_editedValue->value();
	QString s = iisTextEditorDialog::getText(v.type != VT_SPECIAL ? v.value.toString() : QString::null);
	if (!s.isNull()) 
		doPickValue(s);
}

/****************************************************************************
**
** TTextValue class
**
*****************************************************************************/

TTextButtonEditor TTextValue::s_TextButtonEditor;

TTextValue::TTextValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
	setButtonEditor(&s_TextButtonEditor);
}

TTextValue::TTextValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	setButtonEditor(&s_TextButtonEditor);
}

//QString TTextValue::valueText() const
//{
//    QString s = m_set.value.toString();
//    bool isRich = Qt::mightBeRichText(s);
//    if (isRich) {
//        return "rich";
//    }
//    return s;
//}
//
