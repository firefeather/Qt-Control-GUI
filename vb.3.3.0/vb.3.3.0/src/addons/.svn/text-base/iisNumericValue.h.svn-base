#ifndef TNUMERICVALUE_H
#define TNUMERICVALUE_H

#include <iisDefines.h>

#include <iisVB.h>

/****************************************************************************
**
** TNumButtonEditor class
**
*****************************************************************************/

class LIBVB4_EXPORT TNumButtonEditor : public TSpinButtonEditor
{
public:
	TNumButtonEditor();

	// sets Step for spin editing (one click +/-)
	double			step() const					{ return m_step;	}
	void			setStep(double step)			{ m_step = step;	}

	// sets OverSpin mode (i.e. cyclic spinning values)
	bool			overSpin() const				{ return m_overspin;	}
	void			enableOverSpin(bool b = true)	{ m_overspin = b;	}

	// sets Logical Count mode (i.e. adds Step for the last non-zero digit 
	// of the value, including fraction)
	// (in this mode, Step is assumed to be integral, without fraction part)
	bool			logCount() const				{ return m_logcount;	}
	void			enableLogCount(bool b = true)	{ m_logcount = b;	}

protected:
	double			m_step;
	bool			m_overspin, m_logcount;

	void			doProcess(bool up);

protected slots:
	virtual void	buttonClickedUp();
	virtual void	buttonClickedDown();
};

/****************************************************************************
**
** TIntegerValue class
**
*****************************************************************************/

class LIBVB4_EXPORT TIntegerValue: public TBaseValue
{
public:
	TIntegerValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	TIntegerValue (TBaseItem *parent, const QString& name, int id = -1);

    /*! Returns current value as Integer.
        \since 3.3
    */
    int valueInt() const 
    { return m_set.value.toInt(); }

	// Standard button editor.
	static TNumButtonEditor *s_NumButtonEditor;
};

/****************************************************************************/

/****************************************************************************
**
** TDoubleValue class
**
*****************************************************************************/

class LIBVB4_EXPORT TDoubleValue: public TBaseValue
{
public:
	TDoubleValue (TValueBrowser *parent, const QString& name, int page = -1, int id = -1);
	TDoubleValue (TBaseItem *parent, const QString& name, int id = -1);

    /*! Returns current value as Double.
        \since 3.3
    */
    double valueDouble() const 
    { return m_set.value.toDouble(); }

	// Standard button editor.
	static TNumButtonEditor *s_NumButtonEditor;
};

/****************************************************************************/

#endif