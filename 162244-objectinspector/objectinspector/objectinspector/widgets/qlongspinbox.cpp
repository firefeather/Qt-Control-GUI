#include "qlongspinbox.h"
#include <QLineEdit>

void QLongSpinBox::stepBy(int steps)
{
    fvalue+=steps;
    updateLineEdit();
}

QAbstractSpinBox::StepEnabled QLongSpinBox::stepEnabled()   const
{
    QAbstractSpinBox::StepEnabled f;
    if(fvalue>fmin)
        f|=QAbstractSpinBox::StepDownEnabled;
    if(fvalue<fmax)
        f|=QAbstractSpinBox::StepUpEnabled;
    return  f;
}

void QLongSpinBox::showEvent(QShowEvent *)
{
    updateLineEdit();
}

void QLongSpinBox::updateLineEdit()
{
    lineEdit()->setText(QString::number(fvalue));
}

QLongSpinBox::QLongSpinBox(QWidget *parent) :
    QAbstractSpinBox(parent)
{
    fvalue = 0LL;
    fmin = LONG_LONG_MIN;
    fmax = LONG_LONG_MAX;
    flineEdit = new QLineEdit;
    connect(this,SIGNAL(editingFinished()),this,SLOT(applyData()));
    setLineEdit(flineEdit);
    fvalidator.setRegExp(QRegExp("^-?[0-9]{0,19}$"));
    lineEdit()->setValidator(&fvalidator);
}

qint64 QLongSpinBox::value() const
{
    return  fvalue;
}

qint64 QLongSpinBox::minimum() const
{
    return  fmin;
}

qint64 QLongSpinBox::maximum() const
{
    return  fmax;
}

void QLongSpinBox::setValue(const qint64 &value)
{
    if(fvalue!=value)
    {
        fvalue = value;
        updateLineEdit();
        emit    valueChanged(fvalue);
    }
}

void QLongSpinBox::setMinimum(const qint64 &value)
{
    if(fmin!=value)
    {
        fmin = value;
        if(fvalue<fmin)
        {
            fvalue = fmin;
            updateLineEdit();
        }
    }
}

void QLongSpinBox::setMaximum(const qint64 &value)
{
    if(fmax!=value)
    {
        fmax = value;
        if(fvalue>fmax)
        {
            fvalue = fmax;
            updateLineEdit();
        }
    }
}

void QLongSpinBox::applyData()
{
    setValue(lineEdit()->text().toLongLong());
}
