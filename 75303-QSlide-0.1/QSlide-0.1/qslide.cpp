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


#include "qslide.h"

QSlide::QSlide(QWidget *parent) : QWidget(parent)
{
    textCol = Qt::green;
    backCol = Qt::black;
    textMsg = "Sliding text .. ";
    numChar = textMsg.size();
    length = (100/16)*numChar+1;
    typeEff = sliding;
    on = true;
    textOff = "";
    textOn = textMsg;
    timerVal = 300;
    idTimer = 0;
    timerState = false;
}


void QSlide::paintEvent(QPaintEvent *)
{
    paintBorder();
    paintText();
    paintGrid();
}

void QSlide::paintBorder()
{
    QPainter painter(this);
    painter.setWindow(0, 0, length, 14);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF border(0, 0, length, 14);
    painter.setBrush(backCol);
    painter.drawRect(border);

}

void QSlide::paintText()
{
    QPainter painter(this);
    painter.setWindow(0, 0, length, 14);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF textR(1, -1, length, 14);
    painter.setPen(QPen(textCol));
    QFont font("Fixed", 10, QFont::Bold);
    QFontMetrics fm (font);
    painter.setFont(font);
    painter.drawText(textR, Qt::AlignVCenter, textMsg);

    emit timeOut();

}

void QSlide::paintGrid()
{
    QPainter painter(this);
    painter.setWindow(0, 0, length, 14);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(60, 60, 60), 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    for (int i = 0; i <=length; i++)
    {
        painter.drawLine(1*i, 1, 1*i, 13);
    }
    for (int i = 0; i <=14; i++)
    {
        painter.drawLine(1, 1*i, length-1, 1*i);
    }

}

void QSlide::timerEvent(QTimerEvent *)
{
    if (timerState == true)
    {
        if (typeEff == sliding)
        {
            int size = textMsg.size();
            QString tmp = textMsg;
            QString rest = tmp.remove(0, 1);
            QString first = textMsg.remove(1, size-1);
            textMsg = rest.append(first);
            update();
        }
        if (typeEff == intermittent)
        {
            on = !on;
            if (on)
            {
                textMsg = textOn;
                update();
            }
            if (!on)
            {
                textMsg = textOff;
                update();
            }
        }
    }
    if (timerState == false)
    {
        textMsg = textOn;
        update();
    }

}

void QSlide::setColorTx(QColor colorTx)
{
    textCol = colorTx;
    update();
}

void QSlide::setColorBg(QColor colorBg)
{
    backCol = colorBg;
    update();
}

void QSlide::setEffect(Effect effect)
{
    typeEff = effect;
    textMsg = textOn;
    update();
}

void QSlide::setText(QString text)
{
    textMsg = text;
    textOn = text;
    numChar = textMsg.size();
    length = (100/16)*numChar+1;
    update();
}


void QSlide::setTimer(int timer)
{
    killTimer(idTimer);
    timerVal = timer;
    timerState = false;
    update();
}

void QSlide::startTmr(bool start)
{
    timerState = start;

    if (timerState == true)
    {
        idTimer = startTimer(timerVal);
        update();
    }
    if (timerState == false)
    {
        killTimer(idTimer);
        update();
    }

}

QSize QSlide::minimumSizeHint() const
{
    return QSize(length*0.3, 3);
}

QSize QSlide::sizeHint() const
{
    return QSize(length*3, 30);
}


