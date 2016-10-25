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

#ifndef QSLIDE_H
#define QSLIDE_H

#include <QtGui>
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT QSlide : public QWidget
{
    Q_OBJECT
    Q_ENUMS(Effect)
    Q_PROPERTY(Effect effect READ effect WRITE setEffect);
    Q_PROPERTY(QColor colorTx READ colorTx WRITE setColorTx);
    Q_PROPERTY(QColor colorBg READ colorBg WRITE setColorBg);
    Q_PROPERTY(QString text READ text WRITE setText);
    Q_PROPERTY(int timer READ timerV WRITE setTimer);
    Q_PROPERTY(bool start READ isStart WRITE startTmr);

public:

    QSlide(QWidget *parent = 0);
	QSize minimumSizeHint() const;
    QSize sizeHint() const;
    enum Effect
    {
        sliding=1, intermittent=2
    };
    Effect effect() const
    {
        return typeEff;
    }
    QColor colorTx() const
    {
        return textCol;
    }
    QColor colorBg() const
    {
        return backCol;
    }
    QString text() const
    {
        return textOn;
    }
    int timerV() const
    {
        return timerVal;
    }
    bool isStart() const
    {
        return timerState;
    }
    

signals:

	void timeOut();


public slots:

	void setColorTx(QColor);
	void setColorBg(QColor);
	void setEffect(Effect);
	void setText(QString);
	void setTimer(int);
	void startTmr(bool);
	
protected:

   void paintEvent(QPaintEvent *);
   void timerEvent(QTimerEvent *);
   void paintBorder();
   void	paintText();
   void	paintGrid();
   


private:

	int timerVal;
	int idTimer;
	double numChar;
	double length;
	bool on;
	bool timerState;
	QString textMsg;
	QString textOff;
	QString textOn;
	QColor textCol;
	QColor backCol;
	Effect typeEff;


};

#endif
