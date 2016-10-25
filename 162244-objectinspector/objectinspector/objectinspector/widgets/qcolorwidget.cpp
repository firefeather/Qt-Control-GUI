#include <QPainter>
#include <QStyle>
#include <QStyleOptionButton>
#include "qcolorwidget.h"

QColorWidget::QColorWidget(QWidget *parent) :
    QWidget(parent)
{
    fcolor = Qt::white;
    fhelpColor = Qt::black;
    fmaskBrush.setTextureImage(QImage(":/icons/mask"));
}

const QColor &QColorWidget::color() const
{
    return  fcolor;
}

const QColor &QColorWidget::helpColor() const
{
    return  fhelpColor;
}

void QColorWidget::paintEvent(QPaintEvent *)
{
    QRect   frect = frameGeometry();
    QPainter    painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(fmaskBrush);
    painter.drawRect(frect);
    painter.setPen(Qt::black);
    painter.setBrush(fcolor);
    painter.drawPolygon(frect);

}

void QColorWidget::setColor(const QColor &Value)
{
    if(fcolor!=Value)
    {
        fcolor = Value;
        update();
        emit    colorChanged();
    }
}

void QColorWidget::setHelpColor(const QColor &Value)
{
    if(fhelpColor!=Value)
    {
        fhelpColor = Value;
        update();
    }
}
