/*
 * Table View Print & Preview dialog
 * Copyright (C) 2004-2008 by Gordos Kund / QnD Co Bt.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact gordos.kund@gmail.com with any questions on this license.
 */

#ifndef TDPREVIEWDIALOG_H
#define TDPREVIEWDIALOG_H
#include <QtGui/QDialog>
#include <ui_tdpreviewdialog.h>
#include <QtGui/QTableView> //checked
#include <QtGui/QPrinter> //checked
#include <QtGui/QTextLength> //checked
#include <QtGui/QFileDialog> //checked

/*! \class TDPreviewDialog
 *  \brief TDPreviewDialog dialog
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */

class QGraphicsScene;
class QAbstractItemModel;

// Text preview widget
class TDPreviewDialog : public QDialog
{
        Q_OBJECT
public:
	TDPreviewDialog(QTableView *p_tableView, QPrinter * p_printer,  QWidget *parent=0);
        virtual ~TDPreviewDialog();
	virtual void setHeaderText(const QString &text);
	virtual void print();
	virtual int exec();
	virtual void exportPdf(const QString &filename);

private slots:
	virtual void on_setupToolButton_clicked();
	virtual void on_zoomInToolButton_clicked();
	virtual void on_zoomOutToolButton_clicked();
	virtual void on_pageSpinBox_valueChanged(int value);

private:
	virtual void setupPage();
	virtual void paintPage(int pagenum);
	virtual void setupSpinBox();
        Ui_TDPreviewDialog ui;
	QGraphicsView *view;
	QTableView *tableView;
	QPrinter *printer;

	int lines;
	int pages;
	int leftMargin;
	int rightMargin;
	int topMargin;
	int bottomMargin;
	int spacing;
	int headerSize;
	int footerSize;
	int sceneZoomFactor;
	double columnZoomFactor;
	double rowHeight;
	double columnMultiplier;
	QString headerText;
        QString footerText;
	QVector<QTextLength> colSizes;
	QAbstractItemModel *model;
	QGraphicsScene pageScene;
	QFont titleFont;
	QFont headerFont;
	QFont font;
	QFontMetrics *titleFmt;
	QFontMetrics *headerFmt;
	QFontMetrics *fmt;
	QString headerStdText;
};

#endif
