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

#ifndef QREFLEX_H
#define QREFLEX_H

#include <QtGui>
#include <QtDesigner/QDesignerExportWidget>
#include <math.h>


class QDESIGNER_WIDGET_EXPORT QReflex : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor colorBg READ colorBack WRITE setColorBg);
    Q_PROPERTY(QColor colorTx READ colorTxt WRITE setColorTx);
    Q_PROPERTY(int alphaFst READ alphaFirst WRITE setAlphaFst);
    Q_PROPERTY(int alphaSnd READ alphaSecond WRITE setAlphaSnd);
    Q_PROPERTY(int alphaTxt READ alphaText WRITE setAlphaTxt);
    Q_PROPERTY(int reflexLgh READ reflex WRITE setReflexLgh);
    Q_PROPERTY(double shear READ shearDim WRITE setShear);
    Q_PROPERTY(QString text READ txt WRITE setText);
    Q_PROPERTY(QFont font READ fnt WRITE setFont);

    QColor colorBack() const
    {
        return colorBg;
    }
    QColor colorTxt() const
    {
        return colorTx;
    }
    QString txt() const
    {
        return text;
    }
    QFont fnt() const
    {
        return font;
    }
    int alphaFirst() const
    {
        return alphaFst;
    }
    int alphaSecond() const
    {
        return alphaSnd;
    }
    int alphaText() const
    {
        return alphaTxt;
    }
    int reflex() const
    {
        return reflexLgh;
    }
    double shearDim() const
    {
        return shear;
    }


public:

    QReflex(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;


signals:


public slots:

    void setColorBg(QColor);
    void setColorTx(QColor);
    void setAlphaFst(int);
    void setAlphaSnd(int);
    void setAlphaTxt(int);
    void setReflexLgh(int);
    void setShear(double);
    void setText(QString);
    void setFont(QFont);


protected:

    void paintEvent(QPaintEvent *);
    void paintWidget();



private:

    QString text;
    double shear;
    QColor colorBg;
    QColor colorTx;
    QColor reflexColorBg;
    QColor reflexColorBord;
    QColor reflexColorFst;
    QColor reflexColorSnd;
    QColor reflexColorTx;
    QFont font;
    int alphaFst;
    int alphaSnd;
    int alphaTxt;
    int sizeTx;
    int reflexLgh;


};

#endif
