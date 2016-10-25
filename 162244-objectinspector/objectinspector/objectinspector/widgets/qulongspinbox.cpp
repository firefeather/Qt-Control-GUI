#include "qulongspinbox.h"
#include <QLineEdit>

void QULongSpinBox::stepBy(int steps)
{
    fvalue+=steps;
    updateLineEdit();
}

QAbstractSpinBox::StepEnabled QULongSpinBox::stepEnabled()   const
{
    QAbstractSpinBox::StepEnabled f;
    if(fvalue>fmin)
        f|=QAbstractSpinBox::StepDownEnabled;
    if(fvalue<fmax)
        f|=QAbstractSpinBox::StepUpEnabled;
    return  f;
}

void QULongSpinBox::showEvent(QShowEvent *)
{
    updateLineEdit();
}

void QULongSpinBox::updateLineEdit()
{
    lineEdit()->setText(QString::number(fvalue));
}

QULongSpinBox::QULongSpinBox(QWidget *parent) :
    QAbstractSpinBox(parent)
{
    fvalue = 0LL;
    fmin = 0ULL;
    fmax = ULONG_LONG_MAX;
    flineEdit = new QLineEdit;
    connect(this,SIGNAL(editingFinished()),this,SLOT(applyData()));
    setLineEdit(flineEdit);
    fvalidator.setRegExp(QRegExp("^[0-9]{0,19}$"));
    lineEdit()->setValidator(&fvalidator);
}

quint64 QULongSpinBox::value() const
{
    return  fvalue;
}

quint64 QULongSpinBox::minimum() const
{
    return  fmin;
}

quint64 QULongSpinBox::maximum() const
{
    return  fmax;
}

void QULongSpinBox::setValue(const quint64 &value)
{
    if(fvalue!=value)
    {
        fvalue = value;
        updateLineEdit();
        emit    valueChanged(fvalue);
    }
}

void QULongSpinBox::setMinimum(const quint64 &value)
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

void QULongSpinBox::setMaximum(const quint64 &value)
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

void QULongSpinBox::applyData()
{
    setValue(lineEdit()->text().toLongLong());
}
