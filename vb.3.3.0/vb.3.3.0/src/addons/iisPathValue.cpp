#include "iisPathValue.h"

#include <QFileDialog>
#include <math.h>

/****************************************************************************
**
** TPathButtonEditor class
**
*****************************************************************************/

TPathButtonEditor::TPathButtonEditor() : 
TPopupButtonEditor()
{
}

void TPathButtonEditor::buttonClicked()
{
	SValue &v = m_editedValue->value();
	TPathValue *pv = (TPathValue*)m_editedValue;

	QFileDialog fd;
	fd.setFileMode(pv->pickMode());
	fd.setFilter(pv->filter());
	fd.setWindowTitle(pv->text(0));
	QString f = (v.type != VT_SPECIAL ? v.value.toString() : QString::null);
	QFileInfo fi(f);
	if (fi.isDir())
		fd.setDirectory(f);
	else {
		fd.setDirectory(fi.dir());
		fd.selectFile(fi.fileName());
	}
	if (fd.exec() == QDialog::Accepted)
		doPickValue(fd.selectedFiles().first());

 //   QString s = QFileDialog::getOpenFileName(0, tr("Âûבמנ פאיכא"), v.type != VT_SPECIAL ? v.value.toString() : QString::null, tr("Âסו פאיכû (*.*)"));
	//if (!s.isNull()) 
	//	doPickValue(s);
}

/****************************************************************************
**
** TPathValue class
**
*****************************************************************************/

TPathButtonEditor TPathValue::s_PathButtonEditor;

TPathValue::TPathValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
	init();
}

TPathValue::TPathValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	init();
}

void TPathValue::init()
{
	setButtonEditor(&s_PathButtonEditor);

	m_mode = QFileDialog::AnyFile;
	m_filter = "*.*";
}

