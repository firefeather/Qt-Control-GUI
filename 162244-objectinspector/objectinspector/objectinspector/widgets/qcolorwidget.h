#ifndef QCOLORWIDGET_H
#define QCOLORWIDGET_H

#include <QWidget>
#include "objectinspector_global.h"

class OBJECTINSPECTORSHARED_EXPORT QColorWidget : public QWidget
{
    Q_OBJECT

    QColor  fcolor;
    QColor  fhelpColor;
    QBrush  fmaskBrush;

protected:

    void    paintEvent(QPaintEvent * );

public:
    explicit QColorWidget(QWidget *parent = 0);
    
    const   QColor  &   color()     const;
    const   QColor  &   helpColor() const;

public slots:

    void    setColor(const QColor & Value);
    void    setHelpColor(const QColor & Value);
    
signals:

    void    colorChanged();
};

#endif // QCOLORWIDGET_H
