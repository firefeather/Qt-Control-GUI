/*
 * DataBrowser data browsing widget and filtering engine
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

#ifndef TWDATAZOOM_H
#define TWDATAZOOM_H

#include <QtGui/QWidget>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtGui/QAction>
#include <QtGui/QPrinter>

#include "ui_twdatazoom.h"
#include "tcabstractzoomparams.h"
#include "tcabstractfilter.h"
#include "tcabstractdatamodel.h"

/*! \class TWDataZoom
 *  \brief Fast data Borwser
 *  \author Kund Gordos
 *  \version 1.0
 *  \date     2008
 */

class QFileDialog;

class TWDataZoom : public QWidget
{
        Q_OBJECT
	Q_ENUMS( ResultChangeRoles )
	Q_PROPERTY(bool headerHidden READ getHeaderHidden WRITE setHeaderHidden)
	Q_PROPERTY(bool hideFilterWidgets READ getHideFilterWidgets WRITE setHideFilterWidgets)
	Q_PROPERTY(bool tableVerticalHeaderHidden READ getTableVerticalHeaderHidden WRITE setTableVerticalHeaderHidden);

public:
	enum ResultChangeRoles {
		RefreshRole=0x0,
		OrderRole=0x1,
	  	ShowEventRole=0x2,
  		MoveRole=0x4,
    		SelectionChangedRole=0x8,
      		HeaderChangedRole=0x10,
		DataRefreshedRole=0x20,
		EndQueryRole=0x40,
		LayoutChangedRole=0x80
	};

	
			
        //Constructor with filters
	TWDataZoom( TCAbstractDataModel *model,QHash <int, QSqlRecord> *,QWidget* parent = 0);
        //Plugin constructor
        TWDataZoom( QWidget* parent = 0);
        ~TWDataZoom();
	//Reload combo's filter
        virtual void refreshComboData();
	//setup master filter text
        virtual void setMasterFilter(QString masterFilter);
	//set filter mask. External filter manipulation function
	virtual void setMask(int column, const QString &text);
	//hide column
	virtual void hideColumn(int column, bool hide);
	//disable filters in headers customContextMenu
	virtual void setFilteringDisabled(int column, bool set);
	virtual bool isFilteringDisabled(int column);
	//setup datat browser
	virtual void setHandlers(TCAbstractDataModel *model,QHash <int, QSqlRecord>* result);
	//transfer function, set selection ode for QTableView
	virtual void setSelectionMode(QAbstractItemView::SelectionMode mode );
	//set sort order in column.
	virtual void setOrder(int section, TCAbstractDataModel::order order);
public slots:
	//property: hide widgets above QTableView
	virtual bool getHeaderHidden() { return m_headerHidden; };
	virtual void setHeaderHidden(const bool b);
	//property: hide QTableViews header
	virtual bool getTableVerticalHeaderHidden() { return m_tableVerticalHeaderHidden; };
	virtual void setTableVerticalHeaderHidden(const bool b);
	//show aggregator line in last column
	virtual void setShowAggregators(bool b);
	//show inactive rows 
	virtual void setShowInactives(bool b);
	//disable some filter handler widget above QTableView
	virtual void setHideFilterWidgets(bool);
	virtual bool getHideFilterWidgets();
	//move selection down
	virtual void moveDown();
	//move selection up
	virtual void moveUp();
	//move selection to first position
	virtual void moveFirst();
	//move selection to last position
	virtual void moveLast();
	//save current state, reload sate, refresh data
	virtual void refreshAll();
private slots:
	//Automatic
	virtual void on_tableView_clicked(const QModelIndex &);
	virtual void on_tableView_doubleClicked(const QModelIndex &);
	virtual void on_tableView_customContextMenuRequested( const QPoint & );
	
	//manual connected
	virtual void onBeginQuery(QString query);
	virtual void onEndQuery(QString query);
	virtual void onselectionModel_SelectionChanged(const QItemSelection &, const QItemSelection &);
	virtual void setMasterFilterTxt(); //Delayed set, called via QTimer
	virtual void onHeader_clicked(int section);
	virtual void onHeader_customContextMenuRequested ( const QPoint & pos );
	virtual void onHeaderAll_triggered();
	virtual void onHeaderTrue_triggered();
	virtual void onHeaderFalse_triggered();
	virtual void onHeaderLike_triggered();
	virtual void onHeaderEqual_triggered();
	virtual void onHeaderLess_triggered();
	virtual void onHeaderInterval_triggered();
	virtual void onHeaderGreater_triggered();
	virtual void onHeaderLessEqual_triggered();
	virtual void onHeaderGreaterEqual_triggered();
	virtual void onHeaderNotEqual_triggered();
	virtual void onHeaderNull_triggered();
	virtual void onHeaderNotNull_triggered();
	virtual void onHeaderNone_triggered();
	virtual void onHeaderMin_triggered();
	virtual void onHeaderMax_triggered();
	virtual void onHeaderSum_triggered( );
	virtual void onHeaderAvg_triggered( );
	virtual void onHeaderCount_triggered();
	virtual void onHeaderHide_triggered();
	virtual void onPrint_triggered();
	virtual void onPreview_triggered();
	virtual void onPDF_triggered();
	virtual void onCSV_triggered();
	virtual void onParamAction_triggered();
	virtual void onShowInactive_triggered(bool);
	virtual void onShowAggregators_triggered(bool);	
	
	//Automatic
	virtual void on_clearPushButton_clicked();
	virtual void on_masterFilterLineEdit_textChanged (QString flt);
	virtual void on_comboBox_activated(int idx);
	virtual void on_saveFilterPushButton_clicked();
	virtual void on_newFilterPushButton_clicked();
	virtual void on_deleteFilterPushButton_clicked();
	virtual void on_defaultFilterPushButton_clicked();
	
private:
	virtual void init();
	virtual bool saveFilter();
	virtual void refreshData();
	virtual void updateWidgetsFromCurrentFilter();
	virtual void updateFilterFromWidget(int idx, const QString &name);
	virtual QString commonHeaderFilterDialog();
	virtual void setMasterFilterColumn(QString txt);
	virtual void enableFilterWidgets(bool);
	virtual void refreshResultSet(TWDataZoom::ResultChangeRoles role);
	virtual void setArrowCursor();
	virtual void setWaitCursor();
	Ui::TWDataZoom ui;
	TCAbstractFilter * filters;
	TCAbstractZoomParams * params;
	TCAbstractDataModel * model;
	QHeaderView *header;
	int oldComboIdx;
	QString oldComboText;
	int previousSection;	
        QMenu *aggregateMenu;
	QMenu *toolsMenu;
	QActionGroup * aggregateGroup;
	QAction *header_infoAction;
        QAction *header_allAction;
        QAction *header_trueAction;
        QAction *header_falseAction;
        QAction *header_likeAction;
        QAction *header_equalAction;
        QAction *header_lessAction;
        QAction *header_greaterAction;
        QAction *header_lessEqualAction;
	QAction *header_intervalAction;
	QAction *header_greaterEqualAction;
        QAction *header_notEqualAction;
        QAction *header_notNullAction;
        QAction *header_nullAction;
	QAction *header_minAction;
	QAction *header_maxAction;
	QAction *header_avgAction;
	QAction *header_sumAction;
	QAction *header_countAction;
	QAction *header_noneAction;
	QAction *header_hideAction;
	QAction *print;
	QAction *preview;
	QAction *pdf;
	QAction *csv;
	QAction *paramAction;
	QAction *showInactive_action;
	QAction *showAggregators_action;
	int menuRequestedHeaderColumn;	
	QHash <int, QSqlRecord> *resultSet;
	QHash <int, bool> disabledFilterColumns;
	QHash <int, bool> hiddenColumns;
	QKeyEvent *evn;
	QPrinter *printer;
	QString masterFilterBag;
	QTimer* timer;
	bool showAggregators;
	bool showInactives;
	bool hideFilterWidgets;
	bool doFirstTimeAtShowEvent;
protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void showEvent ( QShowEvent *event);
	bool m_headerHidden;
	bool m_tableVerticalHeaderHidden;

signals:
	virtual void accept();
	virtual void reject();
	virtual void resultChanged(TWDataZoom::ResultChangeRoles);
	virtual void oneRowResult(QString);
private slots:
	virtual void onLayoutChanged();	

};
#endif
