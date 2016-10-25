#ifndef iisValidators_H
#define iisValidators_H

#include <math.h>
#include <QtCore>

/****************************************************************************
**
** TValueValidator class
**
** Class is used as the base for all value validators.
**
*****************************************************************************/

/*! \class TValueValidator 
 * \nosubgrouping
 * \brief Abstract class which is used as the base for all value validators.

	Validators are used for check input from the user. 
	
	Every validator should reimplement checkValue(QString& value) method. 
	This method checks input \value (normally passed from item editor) and returns a result
	of the validation (see #EValidateResult enum). If the validator can fix invalid value,
	it should write correct value back to \value and return #VR_FIXED. If the value can not be
	fixed, it should return #VR_BAD. If the value is good, it should return #VR_OK respectively.

	\sa checkValue()
 */

class TValueValidator : public QObject
{
public:
	enum EValidateResult { 
		VR_BAD,		///< means that validation has failed.
		VR_OK,		///< means that validation has passed.
		VR_FIXED	///< means that validated value has corrected automatically during validation.
	};

	// Constructor.
	TValueValidator(QObject *parent = 0) : QObject(parent) {};
	// Destructor.
	virtual ~TValueValidator() {};

	/*! This method checks input \value (normally passed from item editor) and returns a result
	of the validation (see #EValidateResult enum). 
	
	If the validator can fix invalid value,
	it should write correct value back to \value and return #VR_FIXED. If the value can not be
	fixed, it should return #VR_BAD. If the value is good, it should return #VR_OK respectively.
	*/
	virtual EValidateResult checkValue(QVariant& value) = 0;
};

/****************************************************************************
**
** TIntValidator class
**
** A Validator for Integer values.
**
*****************************************************************************/

/*! \class TIntValidator 
 * \nosubgrouping
 * \brief A validator for integer values.

	This class provides a validator that ensures a string contains a valid integer within a specified range.
 */

class TIntValidator : public TValueValidator
{
public:
	/// Constructor.
	/// @param minimum is the minimum allowed value, INT_MIN by default.
	/// @param maximum is the maximum allowed value, INT_MAX by default.
	TIntValidator(int minimum = INT_MIN, int maximum = INT_MAX, QObject *parent = 0);
	virtual ~TIntValidator() {};

	/// Validates a "value" normally passed from item editor.
	/// If the value is lower than "minimum", it will be fixed to "minimum".
	/// If the value is greater than "maximum", it will be fixed to "maximum".
	/// If the value is not integer, the validation will fail.
	/// \sa minimum(), maximum()
	virtual EValidateResult checkValue(QVariant& value);
	

	/*! Returns low range limit.
     *  \sa setMinimum(), setRange()
	*/
	int minimum() const	{ return m_minimum; }

	/*! Sets low range limit to \a min.
     *  \sa minimum()
     *  \since 3.2.2
	*/
    void setMinimum(int min);

	/*! Returns high range limit.
     *  \sa setMinimum(), setRange()
	*/
	int maximum() const	{ return m_maximum; }

	/*! Sets high range limit to \a max.
     *  \sa maximum()
     *  \since 3.2.2
	*/
    void setMaximum(int max);

	/*! Sets low range limit to \a min and high range limit to \a max.
     *  \sa setMinimum(), setMaximum()
     *  \since 3.2.2
	*/
    void setRange(int min, int max);



    /*! Provides a validator for byte values.
        \since 3.2
    */ 
    static TIntValidator    *s_ByteValidator;

    /*! Provides a validator for word values.
        \since 3.2
    */ 
    static TIntValidator    *s_WordValidator;

    /*! Provides a validator for positive values (>0).
        \since 3.2
    */ 
    static TIntValidator    *s_PositiveValidator;

    /*! Provides a validator for positive values (>=0).
        \since 3.2
    */ 
    static TIntValidator    *s_ZeroPositiveValidator;

    /*! Provides a validator for integer values.
        \since 3.2
    */ 
	static TIntValidator	*s_IntValidator;

protected:
	int	m_minimum, m_maximum;
};

/****************************************************************************
**
** TDoubleValidator class
**
** A Validator for Double values.
**
*****************************************************************************/

/*! \class TDoubleValidator 
 * \nosubgrouping
 * \brief A validator for double values.

	This class provides a validator that ensures a string contains a valid double within a specified range.
 */

class TDoubleValidator : public TValueValidator
{
public:
	/// Constructor.
	/// @param minimum is the minimum allowed value, -HUGE_VAL by default.
	/// @param maximum is the maximum allowed value, HUGE_VAL by default.
	/// @param decimals is the max number of decimals, 5 by default.
	TDoubleValidator(double minimum = -HUGE_VAL, double maximum = HUGE_VAL, int decimals = 5, QObject *parent = 0);
	virtual ~TDoubleValidator() {};

	/// Validates a "value" normally passed from item editor.
	/// If the value is lower than "minimum", it will be fixed to "minimum".
	/// If the value is greater than "maximum", it will be fixed to "maximum".
	/// If the value is not a double, the validation will fail.
	/// \sa minimum(), maximum()
	virtual EValidateResult checkValue(QVariant& value);

	/*! Returns low range limit.
     *  \sa setMinimum(), setRange()
	*/
	double minimum() const	{ return m_minimum; }

	/*! Sets low range limit to \a min.
     *  \sa minimum()
     *  \since 3.2.2
	*/
    void setMinimum(double min);

	/*! Returns high range limit.
     *  \sa setMinimum(), setRange()
	*/
	double maximum() const	{ return m_maximum; }

	/*! Sets high range limit to \a max.
     *  \sa maximum()
     *  \since 3.2.2
	*/
    void setMaximum(double max);

	/*! Sets low range limit to \a min and high range limit to \a max.
     *  \sa setMinimum(), setMaximum()
     *  \since 3.2.2
	*/
    void setRange(double min, double max);



    /*! Provides a validator for double values.
        \since 3.2
    */
	static TDoubleValidator	*s_DoubleValidator;

protected:
	double	m_minimum, m_maximum;
	int		m_decimals;
};

/****************************************************************************
**
** TRegExpValidator class
**
** A Validator for values matching given regular expression.
**
*****************************************************************************/

/*! \class TRegExpValidator 
 * \nosubgrouping
 * \brief A validator for string values.

	This class is used to check a string against a regular expression.
 */

class TRegExpValidator : public TValueValidator
{
public:
	/// Constructor.
	/// @param regexp is regular expression used to validate the value.
	TRegExpValidator(QRegExp &regexp, QObject *parent = 0);
	virtual ~TRegExpValidator() {};

	/// Validates a "value" normally passed from item editor. 
	/// If the value does not match given regular expression, the validation will fail.
	/// If the value matches the regular expression (or regular expression is invalid), the validation is ok.
	/// \sa regexp(), setRegExp()
	virtual EValidateResult checkValue(QVariant& value);

	/// Returns regular expression.
	/// \sa setRegExp()
	const QRegExp& regexp() const	{ return m_regexp; }

	/// Sets \a regexp regular expression.
	/// \sa regexp()
	void setRegExp(QRegExp &regexp);

protected:
	QRegExp		m_regexp;
};

/***************************************************************************/

#endif
