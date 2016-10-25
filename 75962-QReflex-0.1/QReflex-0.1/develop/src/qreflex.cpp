/***************************************************************************
 *   Copyright (C) 2008 - Giuseppe Cigala                                  *
 *   g_cigala@virgilio.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "qreflex.h"

QReflex::QReflex(QWidget *parent) : QWidget(parent)
{
    text = "QReflex";
    font = QFont("Arial", 8, QFont::Normal);
    shear = 0.1;
    colorBg = Qt::black;
    colorTx = Qt::white;
    reflexColorBg = Qt::black;
    reflexColorTx = Qt::white;
    alphaFst = 20;
    alphaSnd = 120;
    alphaTxt = 100;
    reflexLgh = 0;

}


void QReflex::paintEvent(QPaintEvent *)
{
    paintWidget();

}

void QReflex::paintWidget()
{
    QPainter painter(this);
    painter.setWindow(-60, -20, 120, 60);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient linGrad(0, -15, 0, -13);
    linGrad.setColorAt(0, Qt::white);
    linGrad.setColorAt(1, colorBg);
    linGrad.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad);
    painter.setPen(QPen(colorBg));
    painter.drawRect(-40, -15, 80, 15);

    painter.setPen(QPen(colorTx));
    painter.setFont(font);
    QRectF R1 (-40, -15, 80, 15);
    painter.drawText(R1, Qt::AlignCenter, text);


    double pi = 3.14159;
    double a    = (pi/180.0) * 180.0;
    double sina = sin(2.0*a);
    double cosa = cos(2.0*a);
    QTransform reflex(cosa, sina, sina, -cosa, 0, 0);
    painter.setTransform(reflex);
    painter.shear(0, shear);


    QLinearGradient linGrad2(0, -15-reflexLgh, 0, -5-(reflexLgh/2));
    reflexColorFst = colorBg;
    reflexColorSnd = colorBg;
    reflexColorBord = colorBg;
    reflexColorFst.setAlpha(alphaFst);
    reflexColorSnd.setAlpha(alphaSnd);
    reflexColorBord.setAlpha(10);
    reflexColorTx = colorTx;
    reflexColorTx.setAlpha(alphaTxt);

    linGrad2.setColorAt(0, reflexColorFst);
    linGrad2.setColorAt(1, reflexColorSnd);
    linGrad2.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad2);
    painter.setPen(QPen(reflexColorBord));
    painter.drawRect(-40, -15-reflexLgh, 80, 15+reflexLgh);
    QRectF R2 (-40, -15-reflexLgh, 80, 15+reflexLgh);
    painter.setPen(QPen(reflexColorTx));
    painter.drawText(R2, Qt::AlignCenter, text);


}

void QReflex::setColorBg(QColor color)
{
    colorBg = color;
    update();
}
void QReflex::setColorTx(QColor color)
{
    colorTx = color;
    update();
}
void QReflex::setAlphaFst(int dim)
{
    alphaFst = dim;
    update();
}
void QReflex::setAlphaSnd(int dim)
{
    alphaSnd = dim;
    update();
}
void QReflex::setAlphaTxt(int dim)
{
    alphaTxt = dim;
    update();
}
void QReflex::setShear(double dim)
{
    shear = dim;
    update();
}
void QReflex::setReflexLgh(int lgh)
{
    reflexLgh = lgh;
    update();
}
void QReflex::setText(QString tx)
{
    text = tx;
    update();
}
void QReflex::setFont(QFont fnt)
{
    font = fnt;
    update();
}

QSize QReflex::minimumSizeHint() const
{
    return QSize(9, 4);
}

QSize QReflex::sizeHint() const
{
    return QSize(90, 40);
}


