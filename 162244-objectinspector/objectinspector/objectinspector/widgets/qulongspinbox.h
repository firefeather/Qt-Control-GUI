#ifndef QULONGLONGSPINBOX_H
#define QULONGLONGSPINBOX_H

#include <QAbstractSpinBox>
#include <QRegExpValidator>
#include <QLineEdit>

class QULongSpinBox : public QAbstractSpinBox
{
    Q_OBJECT

    quint64          fvalue;
    quint64          fmin,fmax;
    QLineEdit   *    flineEdit;

protected:

    QRegExpValidator  fvalidator;
    void    stepBy(int steps);
    QAbstractSpinBox::StepEnabled   stepEnabled()   const;
    void    showEvent(QShowEvent *);
    void    updateLineEdit();

public:
    explicit QULongSpinBox(QWidget *parent = 0);

    quint64  value()     const;
    quint64  minimum()   const;
    quint64  maximum()   const;

signals:

    void    valueChanged(quint64 value);

public slots:

    void    setValue(const quint64 & value);
    void    setMinimum(const quint64 & value);
    void    setMaximum(const quint64 & value);

private slots:

    void    applyData();

};

#endif // QULONGLONGSPINBOX_H
