#include "iisCoordinateValue.h"
#include "iisNumericValue.h"

#include <QPainter>

/****************************************************************************/

// TPointValidator class

// A Validator for Point values.

/****************************************************************************/

TPointValidator* TPointValidator::s_PointValidator = new TPointValidator();

TPointValidator::TPointValidator(QObject *parent, int xmin, int xmax, int ymin, int ymax) :
	TValueValidator(parent),
	m_xmin(xmin),
	m_xmax(xmax),
	m_ymin(ymin),
	m_ymax(ymax)
	{}

TValueValidator::EValidateResult TPointValidator::checkValue(QVariant& value)
{
    int x = m_xmin;
    int y = m_ymin;

    QVariant sx, sy;

    if (value.canConvert(QVariant::Point)) {
        QPoint p(qvariant_cast<QPoint>(value));
        sx = p.x();
        sy = p.y();
    } else {
        QRegExp rexp("(-?\\d+)\\D*(-?\\d+)");
        int pos = rexp.indexIn(value.toString());
        if (pos == -1) {
            return VR_BAD;
        }

        sx = rexp.cap(1);
        sy = rexp.cap(2);
    }

    TIntValidator v1(m_xmin, m_xmax);
    if (v1.checkValue(sx) != VR_BAD)
        x = sx.toInt();

    TIntValidator v2(m_ymin, m_ymax);
    if (v2.checkValue(sy) != VR_BAD)
        y = sy.toInt();

    QVariant v = QPoint(x, y);
    if (value == v) return VR_OK;
    value = v;
    return VR_FIXED;
}

/****************************************************************************/

// TPointValue class

// Represents a color value class which allows to store QColor

/****************************************************************************/

bool TPointValue::s_showExpandables = true;

TPointValue::TPointValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
    construct();
}

TPointValue::TPointValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	construct();
}

void TPointValue::construct()
{
    setValidator(TPointValidator::s_PointValidator);

    showChildren(s_showExpandables);
}

TPointValue::~TPointValue()
{
}

QString TPointValue::valueText() const
{
    QPoint p = point();
    return QString("%1,%2").arg(p.x()).arg(p.y());
}

/****************************************************************************/

void TPointValue::doCreateChildren()
{
    ivX = new TIntegerValue(this, tr("X coordinate"));
    ivX->setValidator(TIntValidator::s_IntValidator);
    ivY = new TIntegerValue(this, tr("Y coordinate"));
    ivY->setValidator(TIntValidator::s_IntValidator);

    registerChild(ivX);
    registerChild(ivY);
}

void TPointValue::doUpdateFromChild(TBaseValue* /*child*/)
{
    QPoint p(ivX->number(), ivY->number());
    setValue(p);
}

void TPointValue::doUpdateChildren()
{
    QPoint p = point();
    ivX->setValue(p.x());
    ivY->setValue(p.y());
}




/****************************************************************************/

// TSizeValidator class

// A Validator for Size values.

/****************************************************************************/

TSizeValidator* TSizeValidator::s_SizeValidator = new TSizeValidator();

TSizeValidator::TSizeValidator(QObject *parent, int wmin, int wmax, int hmin, int hmax) :
	TValueValidator(parent),
	m_wmin(wmin),
	m_wmax(wmax),
	m_hmin(hmin),
	m_hmax(hmax)
	{}

TValueValidator::EValidateResult TSizeValidator::checkValue(QVariant& value)
{
    int x = m_wmin;
    int y = m_hmin;

    QVariant sx, sy;

    if (value.canConvert(QVariant::Size)) {
        QSize p(qvariant_cast<QSize>(value));
        sx = p.width();
        sy = p.height();
    } else {
        QRegExp rexp("(-?\\d+)\\D*(-?\\d+)");
        int pos = rexp.indexIn(value.toString());
        if (pos == -1) {
            return VR_BAD;
        }

        sx = rexp.cap(1);
        sy = rexp.cap(2);
    }

    TIntValidator v1(m_wmin, m_wmax);
    if (v1.checkValue(sx) != VR_BAD)
        x = sx.toInt();

    TIntValidator v2(m_hmin, m_hmax);
    if (v2.checkValue(sy) != VR_BAD)
        y = sy.toInt();

    QVariant v = QSize(x, y);
    if (value == v) return VR_OK;
    value = v;
    return VR_FIXED;
}

/****************************************************************************/

// TSizeValue class

// Represents a size value class which allows to store QSize

/****************************************************************************/

bool TSizeValue::s_showExpandables = true;

TSizeValue::TSizeValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
    construct();
}

TSizeValue::TSizeValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	construct();
}

void TSizeValue::construct()
{
    setValidator(TSizeValidator::s_SizeValidator);

    showChildren(s_showExpandables);
}

TSizeValue::~TSizeValue()
{
}

QString TSizeValue::valueText() const
{
    QSize s = size();
    return QString("%1,%2").arg(s.width()).arg(s.height());
}

/****************************************************************************/

void TSizeValue::doCreateChildren()
{
    ivW = new TIntegerValue(this, tr("Width"));
    ivW->setValidator(TIntValidator::s_IntValidator);
    ivH = new TIntegerValue(this, tr("Height"));
    ivH->setValidator(TIntValidator::s_IntValidator);

    registerChild(ivW);
    registerChild(ivH);
}

void TSizeValue::doUpdateFromChild(TBaseValue* /*child*/)
{
    QSize s(ivW->number(), ivH->number());
    setValue(s);
}

void TSizeValue::doUpdateChildren()
{
    QSize s = size();
    ivW->setValue(s.width());
    ivH->setValue(s.height());
}



/****************************************************************************/

// TRectValidator class

// A Validator for Rectangle values.

/****************************************************************************/

TRectValidator* TRectValidator::s_RectValidator = new TRectValidator();

TRectValidator::TRectValidator(QObject *parent, int wmin, int wmax, int hmin, int hmax) :
	TSizeValidator(parent, wmin, wmax, hmin, hmax)
{
}

TValueValidator::EValidateResult TRectValidator::checkValue(QVariant& value)
{
    int w = m_wmin;
    int h = m_hmin;

    QVariant sx, sy, sw, sh;

    if (value.canConvert(QVariant::Rect)) {
        QRect p(value.toRect());
        sw = p.width();
        sh = p.height();
        sx = p.x();
        sy = p.y();
    } else {
        QRegExp rexp("(-?\\d+)\\D*(-?\\d+)\\D*(-?\\d+)\\D*(-?\\d+)");
        int pos = rexp.indexIn(value.toString());
        if (pos == -1) {
            return VR_BAD;
        }

        sx = rexp.cap(1);
        sy = rexp.cap(2);
        sw = rexp.cap(3);
        sh = rexp.cap(4);
    }

    TIntValidator v1(m_wmin, m_wmax);
    if (v1.checkValue(sw) != VR_BAD)
        w = sw.toInt();

    TIntValidator v2(m_hmin, m_hmax);
    if (v2.checkValue(sh) != VR_BAD)
        h = sh.toInt();

    QVariant v = QRect(sx.toInt(), sy.toInt(), w, h);
    if (value == v) return VR_OK;
    value = v;
    return VR_FIXED;
}

/****************************************************************************/

// TRectValue class

// Represents a rectangular value class which allows to deal with QRect

/****************************************************************************/

bool TRectValue::s_showExpandables = true;

TRectValue::TRectValue(TValueBrowser *parent, const QString& name, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
    construct();
}

TRectValue::TRectValue(TBaseItem *parent, const QString& name, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	construct();
}

void TRectValue::construct()
{
    setValidator(TRectValidator::s_RectValidator);

    m_expandables = !s_showExpandables;
    showChildren(s_showExpandables);
}

TRectValue::~TRectValue()
{
}

QString TRectValue::valueText() const
{
    QRect s = rect();
    return QString("%1,%2,%3,%4").arg(s.x()).arg(s.y()).arg(s.width()).arg(s.height());
}

/****************************************************************************/

void TRectValue::doCreateChildren()
{
    ivX = new TIntegerValue(this, tr("X coordinate"));
    ivX->setValidator(TIntValidator::s_IntValidator);
    ivY = new TIntegerValue(this, tr("Y coordinate"));
    ivY->setValidator(TIntValidator::s_IntValidator);
    ivW = new TIntegerValue(this, tr("Width"));
    ivW->setValidator(TIntValidator::s_IntValidator);
    ivH = new TIntegerValue(this, tr("Height"));
    ivH->setValidator(TIntValidator::s_IntValidator);

    registerChild(ivX);
    registerChild(ivY);
    registerChild(ivW);
    registerChild(ivH);
}

void TRectValue::doUpdateFromChild(TBaseValue* /*child*/)
{
    QRect s(ivX->number(), ivY->number(), ivW->number(), ivH->number());
    setValue(s);
}

void TRectValue::doUpdateChildren()
{
    QPoint p = point();
    ivX->setValue(p.x());
    ivY->setValue(p.y());

    QSize s = size();
    ivW->setValue(s.width());
    ivH->setValue(s.height());
}

/****************************************************************************/

// TRangeValidator class

// A Validator for PointF values.

/****************************************************************************/

TRangeValidator* TRangeValidator::s_RangeValidator = new TRangeValidator();

TRangeValidator::TRangeValidator(QObject *parent, double xmin, double xmax, double ymin, double ymax) :
TValueValidator(parent),
m_xmin(xmin),
m_xmax(xmax),
m_ymin(ymin),
m_ymax(ymax)
{}

TValueValidator::EValidateResult TRangeValidator::checkValue(QVariant& value)
{
	double x = m_xmin;
	double y = m_ymin;

	QVariant sx, sy;

	if (value.canConvert(QVariant::PointF)) {
		QPointF p(qvariant_cast<QPointF>(value));
		sx = p.x();
		sy = p.y();
	} else {
		//QRegExp rexp("(-?\\d+)\\D*(-?\\d+)");
		QRegExp rexp("(-?\\d+(\\.\\d*)?|-?\\.\\d+)\\D*(-?\\d+(\\.\\d*)?|-?\\.\\d+)");
		int pos = rexp.indexIn(value.toString());
		if (pos == -1) {
			return VR_BAD;
		}

		sx = rexp.cap(1);
		sy = rexp.cap(3);
	}

	TDoubleValidator v1(m_xmin, m_xmax);
	if (v1.checkValue(sx) != VR_BAD)
		x = sx.toDouble();

	TDoubleValidator v2(m_ymin, m_ymax);
	if (v2.checkValue(sy) != VR_BAD)
		y = sy.toDouble();

	QVariant v = QPointF(x, y);
	if (value == v) return VR_OK;
	value = v;
	return VR_FIXED;
}

/****************************************************************************/

// TRangeValue class

// Represents a color value class which allows to store QColor

/****************************************************************************/

bool TRangeValue::s_showExpandables = true;

TRangeValue::TRangeValue(TValueBrowser *parent, const QString& name, const QString& name_left/*="min"*/, const QString& name_right/*="max"*/, int page /* = -1 */, int id /* = -1 */) :
TBaseValue(parent, name, page, id)
{
	m_name_left = name_left;
	m_name_right = name_right;
	construct();
}

TRangeValue::TRangeValue(TBaseItem *parent, const QString& name, const QString& name_left/*="min"*/, const QString& name_right/*="max"*/, int id /* = -1 */) :
TBaseValue(parent, name, id)
{
	m_name_left = name_left;
	m_name_right = name_right;
	construct();
}

void TRangeValue::construct()
{
	setValidator(TRangeValidator::s_RangeValidator);

	showChildren(s_showExpandables);
}

TRangeValue::~TRangeValue()
{
}

QString TRangeValue::valueText() const
{
	QPointF p = range();
	return QString("%1 : %2").arg(p.x()).arg(p.y());
}

/****************************************************************************/

void TRangeValue::doCreateChildren()
{
	ivX = new TDoubleValue(this, m_name_left);
	ivX->setValidator(TDoubleValidator::s_DoubleValidator);
	ivY = new TDoubleValue(this, m_name_right);
	ivY->setValidator(TDoubleValidator::s_DoubleValidator);

	registerChild(ivX);
	registerChild(ivY);
}

void TRangeValue::doUpdateFromChild(TBaseValue* /*child*/)
{
	QPointF p(ivX->numberF(), ivY->numberF());
	setValue(p);
}

void TRangeValue::doUpdateChildren()
{
	QPointF p = range();
	ivX->setValue(p.x());
	ivY->setValue(p.y());
}
