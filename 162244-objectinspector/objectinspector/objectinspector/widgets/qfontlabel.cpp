#include "qfontlabel.h"

QFontLabel::QFontLabel(QWidget *parent):
    QLabel(parent)
{
    fcurrentFont = QFont();
    setText(fcurrentFont.toString());
}

void QFontLabel::setCurrentFont(const QFont &Value)
{
    if(fcurrentFont!=Value)
    {
        fcurrentFont = Value;
        setText(fcurrentFont.toString());
        emit    fontChanged();
    }
}

const QFont &QFontLabel::currentFont() const
{
    return  fcurrentFont;
}
