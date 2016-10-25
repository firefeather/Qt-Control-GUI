#include "iisValidators.h"

/****************************************************************************
**
** TIntValidator class
**
*****************************************************************************/

TIntValidator* TIntValidator::s_IntValidator = new TIntValidator();
TIntValidator* TIntValidator::s_ByteValidator = new TIntValidator(0, 255);
TIntValidator* TIntValidator::s_WordValidator = new TIntValidator(0, 65535);
TIntValidator* TIntValidator::s_PositiveValidator = new TIntValidator(1);
TIntValidator* TIntValidator::s_ZeroPositiveValidator = new TIntValidator(0);

TIntValidator::TIntValidator(int minimum, int maximum, QObject *parent) :
	m_minimum(minimum),
	m_maximum(maximum),
	TValueValidator(parent)
	{}

TValueValidator::EValidateResult TIntValidator::checkValue(QVariant& value)
{
	bool b;
	int i = value.toInt(&b);
	if (!b) {
		double d = value.toDouble(&b);
		if (!b) return VR_BAD;
		if (d == HUGE_VAL) i = INT_MAX;	
		else if (d == -HUGE_VAL) i = INT_MIN;
		else i = d;
	}
	if (i < m_minimum) {
		value = QString::number(m_minimum);
		return VR_FIXED;
	}
	if (i > m_maximum) {
		value = QString::number(m_maximum);
		return VR_FIXED;
	}
	QString v2 = QString::number(i);
	if (v2 != value) {
		value = v2;
		return VR_FIXED;
	}
	return VR_OK;
}

void TIntValidator::setMinimum(int min)
{
    m_minimum = min;
}

void TIntValidator::setMaximum(int max)
{
    m_maximum = max;
}

void TIntValidator::setRange(int min, int max)
{
    if (min > max)
        qSwap(min, max);

    m_minimum = min;
    m_maximum = max;
}

/****************************************************************************
**
** TDoubleValidator class
**
*****************************************************************************/

TDoubleValidator* TDoubleValidator::s_DoubleValidator = new TDoubleValidator();

TDoubleValidator::TDoubleValidator(double minimum, double maximum, int decimals, QObject *parent) :
	m_minimum(minimum),
	m_maximum(maximum),
	m_decimals(decimals),
	TValueValidator(parent)
	{}

TValueValidator::EValidateResult TDoubleValidator::checkValue(QVariant& value)
{
	bool b;
	double i = value.toDouble(&b);
	if (!b) return VR_BAD;
	if (i < m_minimum) {
		value = QString::number(m_minimum, 'f', m_decimals);
		return VR_FIXED;
	}
	if (i > m_maximum) {
		value = QString::number(m_maximum, 'f', m_decimals);
		return VR_FIXED;
	}
	QString v2 = QString::number(i, 'f', m_decimals);
	if (v2 != value) {
		value = v2;
		return VR_FIXED;
	}
	return VR_OK;
}

void TDoubleValidator::setMinimum(double min)
{
    m_minimum = min;
}

void TDoubleValidator::setMaximum(double max)
{
    m_maximum = max;
}

void TDoubleValidator::setRange(double min, double max)
{
    if (min > max)
        qSwap(min, max);

    m_minimum = min;
    m_maximum = max;
}

/****************************************************************************
**
** TRegExpValidator class
**
** A Validator for values matching given regular expression.
**
*****************************************************************************/

TRegExpValidator::TRegExpValidator(QRegExp &regexp, QObject *parent) :
	m_regexp(regexp),
	TValueValidator(parent)
	{}

TValueValidator::EValidateResult TRegExpValidator::checkValue(QVariant& value)
{
	if (m_regexp.isValid()) {
		if (m_regexp.exactMatch(value.toString())) 
			return VR_OK;
		else
			return VR_BAD;
	}

	return VR_OK;
}

void TRegExpValidator::setRegExp(QRegExp &regexp)
{
	m_regexp = regexp;
}

