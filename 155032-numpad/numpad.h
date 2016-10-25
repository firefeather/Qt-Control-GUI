/*
 *   Copyright 20012 Maxime Haselbauer <maxime.haselbauer@googlemail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef NUMPAD_H
#define NUMPAD_H

#include <QWidget>

namespace Ui {
    class numpad;
}

class numpad : public QWidget
{
    Q_OBJECT

public:
    explicit numpad(QWidget *parent = 0);
    ~numpad();
    float get_Value();

public slots:
    void clear_value();

signals:
    void value_changed(double);


private:
    Ui::numpad *NUMPAD;
    QString *VALUE;

private slots:
    void append_0();
    void append_1();
    void append_2();
    void append_3();
    void append_4();
    void append_5();
    void append_6();
    void append_7();
    void append_8();
    void append_9();
    void append_point();
    void toggle_plus_minus();

};

#endif // NUMPAD_H
