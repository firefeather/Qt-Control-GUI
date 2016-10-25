#include "iisNumericValue.h"

#include <math.h>

/****************************************************************************
**
** TNumButtonEditor class
**
*****************************************************************************/

TNumButtonEditor::TNumButtonEditor() : 
TSpinButtonEditor(),
m_step(1.0),
m_overspin(false),
m_logcount(false)
{
}

void TNumButtonEditor::buttonClickedUp()
{
	doProcess(true);
}

void TNumButtonEditor::buttonClickedDown()
{
	doProcess(false);
}

void TNumButtonEditor::doProcess(bool up)
{
	SValue &v = m_editedValue->value();
	if (v.type == VT_SPECIAL) 
		doPickValue(0);
	else {
		bool ok;
		double dv = v.value.toDouble(&ok);
		if (!ok) {
			doPickValue(0);
			return;	// unsupported
		}

		if (m_logcount) {
			double dvl, stp;
			modf(m_step, &stp);	// only integral part of step is used

			QString s = v.value.toString();
			QString s_int = s.section('.', 0, 0);
			QString s_frac = s.section('.', 1, 1);
			double dv_int = s_int.toDouble();
			double dv_frac = s_frac.toDouble();

			int dpov = pow((double)10, s_frac.length());

			dvl = (dv_int*dpov) + (dv_int >= 0 ? dv_frac : -dv_frac);
			dv = dvl/dpov;	// correction of dv
			stp /= dpov;

			while (dvl != 0 && fmod(dvl,10) == 0) {
				dvl = floor(dvl / 10);
				stp *= 10;
			}

			if (up) {
				if (dv < 0 && dv_int >= -stp && stp >= 10) stp /= 10;
			} else {
				if (dv > 0 && dv_int <= stp && stp >= 10) stp /= 10;
			}
			doPickValue(up ? dv+stp : dv-stp);
		} else
			doPickValue(up ? dv+m_step : dv-m_step);

		if (m_overspin) {
			double dv1 = m_editedValue->value().value.toDouble(&ok);
			if (!ok) return;	// unsupported

			if (up) {
				if (dv1 <= dv) {
					doPickValue(-HUGE_VAL);
				}
			} else {
				if (dv1 >= dv) {
					doPickValue(HUGE_VAL);
				}
			}
		}
	}
}

/****************************************************************************
**
** TIntegerValue class
**
*****************************************************************************/

TNumButtonEditor* TIntegerValue::s_NumButtonEditor = new TNumButtonEditor();

TIntegerValue::TIntegerValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
	setButtonEditor(s_NumButtonEditor);
    setValidator(TIntValidator::s_IntValidator);
}

TIntegerValue::TIntegerValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	setButtonEditor(s_NumButtonEditor);
    setValidator(TIntValidator::s_IntValidator);
}

/****************************************************************************
**
** TDoubleValue class
**
*****************************************************************************/

TNumButtonEditor* TDoubleValue::s_NumButtonEditor = new TNumButtonEditor();

TDoubleValue::TDoubleValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
	setButtonEditor(s_NumButtonEditor);
    setValidator(TDoubleValidator::s_DoubleValidator);
}

TDoubleValue::TDoubleValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	setButtonEditor(s_NumButtonEditor);
    setValidator(TDoubleValidator::s_DoubleValidator);
}

