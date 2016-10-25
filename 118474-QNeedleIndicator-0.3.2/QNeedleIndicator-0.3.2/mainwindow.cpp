/***************************************************************************
 *   Copyright (C) 2010 by Tn                                              *
 *   thenobody@poczta.fm                                                   *
 *                                                                         *
 *   Foobar is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of        *
 *   the License, or (at your option) any later version.                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                         *
 ***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    w1 = new QNeedleIndicator(this);
    w1->setRange(0,10);
    w1->setMajorTicks(11);
    w1->setMinorTicks(8);
    w1->setLabel(QString("Voltage"));

    w2 = new QNeedleIndicator(this);
    w2->setRange(0,10);
    w2->setMajorTicks(5);
    w2->setMinorTicks(4);
    w2->setGapAngle(200);
    w2->setLabelOffset(0.4);
    w2->setLabel(QString("Current"));
    w2->setAnimated(false);
    w2->setDigitFont(QFont("Fixed", 10, QFont::Bold));
    w2->setDigitFormat("%.1f");

    w3 = new QNeedleIndicator(this);
    w3->setRange(0,50);
    w3->setDigitFormat(QString("%.0f"));
    w3->setMajorTicks(11);
    w3->setMinorTicks(0);
    w3->setGapAngle(120);
    w3->setLabel(QString("Pressure"));

    w4 = new QNeedleIndicator(this);
    w4->setRange(-1, 1);
    w4->setMajorTicks(11);
    w4->setMinorTicks(8);
    w4->setLabel(QString("Diff V"));
    w4->setValue(0);

    ui->layout->addWidget(w1);
    ui->layout->addWidget(w2);
    ui->layout->addWidget(w3);
    ui->layout->addWidget(w4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonQuit_clicked()
{
    qApp->quit();
}

void MainWindow::on_buttonTest1_clicked()
{
    static int i = 1;
    w1->setValue(1*(i%11));
    i++;
    updateValuesStatus();
}

void MainWindow::on_buttonTest2_clicked()
{
    static int i = 1;
    w2->setValue(1*(i%11));
    i++;
    updateValuesStatus();
}

void MainWindow::on_buttonTest3_clicked()
{
    static int i = 1;
    w3->setValue(1*(i%51));
    updateValuesStatus();
    i++;
}

void MainWindow::on_buttonTest4_clicked()
{
	static qreal val = -1.0;
	val+=0.1;
	if( val > 1 ) val = -1.0;
	w4->setValue(val);
	updateValuesStatus();
}

void MainWindow::updateValuesStatus() {
	ui->statusBar->showMessage(QString().sprintf("Voltage: %.2f | Current: %.2f | Pressure %.0f | Diff %.2f",
				     w1->getValue(),
				     w2->getValue(),
				     w3->getValue(),
				     w4->getValue())
				);
}


