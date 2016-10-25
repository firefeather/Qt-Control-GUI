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

#include <QtGui/QMessageBox>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QMenu>
#include <QtGui/QHeaderView>
#include <QtGui/QInputDialog>
#include <QtGui/QScrollBar>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>

#include "twdatazoom.h"
#include "tdpreviewdialog.h"
#include "twdateinterval.h"
#include "tdtextdialog.h"
#include "tdintervaldialog.h"
#include "tpsqlzoomparameter.h"
#include "csvexporter.h"

//WARNING:  there is no transaction in progress
//bizonyos gombokra mozog a kurzor pl: backspace
//show Inactive rosszul indít -megmutat mindent az elején

//=====================================================================
//Constructor with Filter
//=====================================================================

TWDataZoom::TWDataZoom(TCAbstractDataModel *p_model,QHash <int, QSqlRecord> *p_resultSet, QWidget* parent) : QWidget( parent)
{
        init();
        setHandlers(p_model,p_resultSet);
}

//=====================================================================
//Constructor for DesignerPlugin
//=====================================================================
TWDataZoom::TWDataZoom(QWidget* parent) : QWidget( parent)
{
        init();
}

//=====================================================================
// common init ()
//=====================================================================
void TWDataZoom::init()
{
        filters=NULL;
        params=NULL;
        model=NULL;
        resultSet=NULL;
        doFirstTimeAtShowEvent=true;
        hiddenColumns.clear();
        showAggregators=true;  //default
        ui.setupUi( this );
        enableFilterWidgets(false);

        this->setFocusProxy(ui.tableView);

        //Setup table header
        header=ui.tableView->horizontalHeader();
        header->setStretchLastSection (true);
        header->setMovable(true);
        setTableVerticalHeaderHidden(true);

        //After this doesnt work section size restore:
        //header->setDefaultSectionSize ( 100 );

        //header->setHighlightSections(true);
        header->setContextMenuPolicy (Qt::CustomContextMenu);


        //Scrollbar
        ui.tableView->verticalScrollBar()->setTracking(false);
        connect (ui.tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onHeader_clicked(int)));


        //Actions
        print = new QAction(QIcon(":/16x16/icons/16x16/actions/fileprint.png"),tr("&Print"),this);
        preview = new QAction(QIcon(":/16x16/icons/16x16/actions/preview.png"),tr("Print pre&view"),this);
        pdf = new QAction(QIcon(":/16x16/icons/16x16/mime/pdf.png"),tr("&Export to PDF"),this);
        csv = new QAction(QIcon(":/16x16/icons/16x16/apps/kcalc.png"),tr("E&xport to CSV (Excel)"),this);
        paramAction = new QAction(tr("Edit Data Broswer parameters"),this);
        showInactive_action = new QAction(tr("&Show Inactive"),this);
        showAggregators_action = new QAction(tr("Show &Aggregators"),this);

        showInactive_action->setCheckable(true);
        showAggregators_action->setCheckable(true);
        showInactive_action->setChecked(false); //default
        showAggregators_action->setChecked(true);

        //Menus
        toolsMenu = new QMenu(this);
        toolsMenu->addAction(print);
        toolsMenu->addAction(preview);
        toolsMenu->addAction(pdf);
        toolsMenu->addAction(csv);
        toolsMenu->addSeparator();
        toolsMenu->addAction(paramAction);
        toolsMenu->addSeparator();
        toolsMenu->addAction(showInactive_action);
        toolsMenu->addAction(showAggregators_action);

        ui.toolsPushButton->setMenu(toolsMenu);

        connect (print, SIGNAL(triggered(bool)), this, SLOT(onPrint_triggered()));
        connect (preview, SIGNAL(triggered(bool)), this, SLOT(onPreview_triggered()));
        connect (pdf, SIGNAL(triggered(bool)), this, SLOT(onPDF_triggered()));
        connect (csv, SIGNAL(triggered(bool)), this, SLOT(onCSV_triggered()));
        connect (paramAction, SIGNAL(triggered(bool)), this, SLOT(onParamAction_triggered()));
        connect (showInactive_action, SIGNAL(triggered(bool)), this, SLOT(onShowInactive_triggered(bool)));
        connect (showAggregators_action, SIGNAL(triggered(bool)), this, SLOT(onShowAggregators_triggered(bool)));

        printer = new QPrinter(QPrinter::HighResolution);

        timer = new QTimer();


        ui.tableView->installEventFilter(this);
        ui.masterFilterLineEdit->installEventFilter(this);
}

//=====================================================================
//Destructor
//=====================================================================
TWDataZoom::~TWDataZoom()
{
        //for designer
        if (model==NULL) {
                return;
        }

        //commit transactions, and remove dbs
        model->closeConnections();
}

//=====================================================================
//=====================================================================
// PUBLIC FUNCTIONS
//=====================================================================
//=====================================================================
//45ms
void TWDataZoom::setHandlers(TCAbstractDataModel * p_model, QHash< int, QSqlRecord > * p_resultSet)
{
        Q_CHECK_PTR(p_resultSet);
        resultSet=p_resultSet;
        Q_CHECK_PTR(p_model);
        model=p_model;

        model->setSelectedColumn(0);
        connect (model, SIGNAL(beginQuery(QString)), this, SLOT(onBeginQuery(QString)));
        connect (model, SIGNAL(endQuery(QString)), this, SLOT(onEndQuery(QString)));
	
        //set params
        params=model->getZoomParamsObject();

        //Set filter
        filters=model->getFilterObject();

        //filters->startLogTimer("TWDataZoom::setHandlers");


        //Load Filters for current user
        if (!filters->loadAllFilters()) {
                QString err=filters->getErrors().join("\n");
                QMessageBox::warning(this,tr("Warning !"), tr("[%1-%2]\n%3").arg(__FILE__).arg(__LINE__).arg(err) , QMessageBox::Ok);
        }


        //if no filter must insert "DEFAULT" filter
        int numCols=model->columnCount();
        if (filters->numRows()==0) {
                if (!filters->insertDefaultFilter(numCols)) {
                        QString err=filters->getErrors().join("\n");
                        QMessageBox::information(this,trUtf8("Warning !"), tr("[%1-%2]\n%3").arg(__FILE__).arg(__LINE__).arg(err) , QMessageBox::Ok);
                }
        }


        //First item is last used (with index=0)
        ui.comboBox->insertItems(0,filters->getFilterList());


        //Activate Filter, load data
        refreshComboData();


        //initialize master filter label at first time
        QString mf=filters->getMasterFilterColumn();
        if ( !mf.isEmpty() ) {
                setMasterFilterColumn(mf);
        } else {
                //first time
                setMasterFilterColumn(params->getHeaderNamesFilteredByType().first());
        }

        int idx = params->getColumns().indexOf(mf);
        if (idx==-1) {
                idx=0;
        }
	
	
	setShowInactives(false);//hide inactives - default
	if (!model->testActiveColumnExists()) {
		showInactive_action->setEnabled(false);//disbale "show inactive" menu item
	} else {
		showInactive_action->setEnabled(true); //enable "show inactive" menu item
	}

        //move current index to column
        ui.tableView->setCurrentIndex(model->index(0,idx)); //index() do realRowCount 



        //enable widgets
        enableFilterWidgets(true);


        //firs time, the current text is the first text in combo box
        oldComboText=ui.comboBox->currentText();
        oldComboIdx=0; //hold comboBox previous index

        //Setup Menu
        connect (header,SIGNAL(customContextMenuRequested ( const QPoint & )), this,  SLOT ( onHeader_customContextMenuRequested ( const QPoint &)));
        connect (model, SIGNAL(layoutChanged()), this, SLOT(onLayoutChanged()));

        //filters->endLogTimer("TWDataZoom::setHandlers");
}

//=====================================================================
//Reload combo data
//=====================================================================
void TWDataZoom::refreshComboData()
{
        //21ms filters->startLogTimer("TWDataZoom::refreshComboData");

        //Current index must do via string because item order can change
        QString currentUniqueText=ui.comboBox->currentText();

        //Load filters
        filters->loadAllFilters();

        //setup comboBox
        ui.comboBox->clear();
        ui.comboBox->insertItems(0,filters->getFilterList());

        int newIdx=ui.comboBox->findText(currentUniqueText,Qt::MatchExactly);
        //After delete no match
        if (newIdx<0) {
                newIdx=0;
        }

        filters->activateFilter(newIdx); //fetch filter data from table
        ui.comboBox->setCurrentIndex(newIdx);
        updateWidgetsFromCurrentFilter();

        //filters->endLogTimer("TWDataZoom::refreshComboData");
}

//=====================================================================
//External function for refreshing view
//=====================================================================
void TWDataZoom::refreshAll()
{
        on_saveFilterPushButton_clicked(); //refreshComboData() included
        refreshData();
}

//=====================================================================
//External function for
//=====================================================================
void TWDataZoom::setMasterFilter(QString masterFilter)
{
        //Whitespace removed from the start and the end
        masterFilter=masterFilter.trimmed();
        ui.masterFilterLineEdit->blockSignals(true);
        ui.masterFilterLineEdit->setText(masterFilter);
        ui.masterFilterLineEdit->blockSignals(false);

        filters->setMasterFilterText(masterFilter);
}

//=====================================================================
// Set Mask for constans filtering data
//=====================================================================

void TWDataZoom::setMask(int column,const QString & text)
{
        filters->setHeaderFilter(column,text);
}

//=====================================================================

void TWDataZoom::hideColumn(int column, bool hide)
{
        hiddenColumns[column]=hide;
        if (hide) {
                ui.tableView->hideColumn(column);
                hiddenColumns[column]=hide;
        } else {
                ui.tableView->showColumn(column);
        }
}

//=====================================================================

bool TWDataZoom::isFilteringDisabled(int column)
{
        return disabledFilterColumns.value(column,false);
}

//=====================================================================

void TWDataZoom::setFilteringDisabled(int column, bool set)
{
        disabledFilterColumns[column]=set;
}

//=====================================================================
//=====================================================================
void TWDataZoom::setHeaderHidden(const bool b)
{
        //TODO desiable fast keys QAciton
        m_headerHidden = b;
        ui.zoomHeaderFrame->setHidden( m_headerHidden );
}

//=====================================================================
//=====================================================================
void TWDataZoom::setTableVerticalHeaderHidden(const bool b)
{
        m_tableVerticalHeaderHidden = b ;

        if (m_tableVerticalHeaderHidden) {
                ui.tableView->verticalHeader()->hide();
        } else {
                ui.tableView->verticalHeader()->show();
        }
}

//=====================================================================
//=====================================================================
void TWDataZoom::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
        ui.tableView->setSelectionMode(mode);
}

//=====================================================================
//=====================================================================
void TWDataZoom::setOrder(int section, TCAbstractDataModel::order order)
{
        Q_ASSERT(model!=NULL);

        //if click from another column, start with no order
        header->setSortIndicatorShown(true);
        if (order==TCAbstractDataModel::Ascending) {
                model->setOrderDirection(TCAbstractDataModel::Ascending);
                header->setSortIndicator(section,Qt::AscendingOrder);
        }

        if (order==TCAbstractDataModel::None) {
                header->setSortIndicatorShown(false);
                model->setOrderDirection(TCAbstractDataModel::None);
        }

        if (order==TCAbstractDataModel::Descending) {
                header->setSortIndicator(section,Qt::DescendingOrder);
                model->setOrderDirection(TCAbstractDataModel::Descending);
        }

        model->setSelectedColumn(section);

        //Refresh
        ui.tableView->scrollToTop();
        ui.tableView->clearSelection();
        ui.tableView->reset();
        model->setDoRealFetch(true,TCAbstractDataModel::SetOrderRole);
        ui.tableView->selectionModel()->select(model->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        //Restore current cursor position. Block signals because, won't to call onTableViewSelectionModel_currentColumnChanged() function.
        ui.tableView->setCurrentIndex(model->index(0,section));
        refreshResultSet(TWDataZoom::OrderRole);
}

//=====================================================================
//=====================================================================
void TWDataZoom::setShowAggregators(bool showAggs)
{
        //booleans
        showAggregators_action->setChecked(showAggs);
        showAggregators=showAggs;
        model->setShowAggregators(showAggs);
}

//=====================================================================
//=====================================================================
void TWDataZoom::setShowInactives(bool b)
{
        showInactives=b;
        model->setShowInactives(b);
}


//=====================================================================
//=====================================================================
void TWDataZoom::moveDown()
{
        QItemSelectionModel *smodel= ui.tableView->selectionModel();
        if (smodel==0) {
                return;
        }
        QModelIndexList selectionList =smodel->selectedRows();
        QModelIndex idx = selectionList.last();
        int row=idx.row()+1;
        //last rows
        if (!showAggregators && row>=model->rowCount()) {
                row=row-1;
        }

        if (showAggregators && row>=model->rowCount()-1) {
                row=row-1;
        }

        QModelIndex previous=model->index(row,idx.column());
        ui.tableView->selectionModel()->select(previous, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        ui.tableView->setCurrentIndex(model->index(row,idx.column()));
        ui.tableView->scrollTo(ui.tableView->currentIndex());
        refreshResultSet(TWDataZoom::MoveRole);
}

//=====================================================================
//=====================================================================
void TWDataZoom::moveUp()
{
        QItemSelectionModel *smodel= ui.tableView->selectionModel();
        if (smodel==0) {
                return;
        }
        QModelIndexList selectionList =smodel->selectedRows();
        QModelIndex idx = selectionList.first();
        int row=idx.row()-1;
        if (row<0) {
                row=0;
        }
        QModelIndex previous=model->index(row,idx.column());
        ui.tableView->selectionModel()->select(previous, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui.tableView->setCurrentIndex(model->index(row,idx.column()));
        ui.tableView->scrollTo(ui.tableView->currentIndex());
        refreshResultSet(TWDataZoom::MoveRole);
}

//=====================================================================
//=====================================================================
void TWDataZoom::moveFirst()
{
        ui.tableView->selectionModel()->select(model->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui.tableView->setCurrentIndex(model->index(0,0));
        ui.tableView->scrollTo(ui.tableView->currentIndex());
        refreshResultSet(TWDataZoom::MoveRole);
}

//=====================================================================
//=====================================================================
void TWDataZoom::moveLast()
{
        QModelIndex idx;
        if (showAggregators) {
                idx= model->index(model->rowCount()-2,0);
        } else {
                idx = model->index(model->rowCount()-1,0);
        }
        ui.tableView->selectionModel()->select(idx, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui.tableView->setCurrentIndex(idx);
        ui.tableView->scrollTo(ui.tableView->currentIndex());
        refreshResultSet(TWDataZoom::MoveRole);
}


//=====================================================================
//=====================================================================
// SLOTS:
//=====================================================================
//=====================================================================
void TWDataZoom::on_clearPushButton_clicked()
{
        masterFilterBag.clear();
        ui.masterFilterLineEdit->clear(); // after this on_masterFilterLineEdit_textChanged() will called
}

//---------------------------------------------------------------------
// Filter will send signal to model to refresh data.
// Then model will send signals for TableView about refresh.

void TWDataZoom::on_masterFilterLineEdit_textChanged(QString flt )
{
        setMasterFilter(flt);
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::on_comboBox_activated( int idx )
{
        //save old
        updateFilterFromWidget(oldComboIdx, oldComboText);

        //duplicate or any other ?
        if (!saveFilter()) {
                //restore cobo
                ui.comboBox->setCurrentIndex(oldComboIdx);
                return;
        }

        //load new
        filters->changeCurrentIndex(idx);
        refreshComboData();

        oldComboIdx=idx;
        oldComboText=ui.comboBox->currentText();

}

//---------------------------------------------------------------------

void TWDataZoom::on_saveFilterPushButton_clicked( )
{
        if (filters->getReadOnly()) {
                QMessageBox::warning(this,tr("Warning !"), tr("[%1-%2]\nThis filter is read only !").arg(__FILE__).arg(__LINE__), QMessageBox::Ok);
                return;
        }

        //Check for duplicates
        QStringList fltl = filters->getFilterList();
        fltl.removeAt(ui.comboBox->currentIndex());
        if (fltl.contains(ui.comboBox->currentText())) {
                QMessageBox::warning(this,tr("Warning !"), tr("[%1-%2]\nFilter name in combo box must be unique !").arg(__FILE__).arg(__LINE__) , QMessageBox::Ok);
                return;
        }

        updateFilterFromWidget(ui.comboBox->currentIndex(),ui.comboBox->currentText());

        if (!saveFilter()) {
                return;
        }
        oldComboText=ui.comboBox->currentText();
        refreshComboData();
}

//---------------------------------------------------------------------

void TWDataZoom::on_newFilterPushButton_clicked( )
{
        int ret = QMessageBox::question(this,tr("Question !"), tr("Do You want to save changes on zoom ?") , QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel );
        if (ret==QMessageBox::Cancel) {
                return;
        }
	//SAVE old filter
        if (ret==QMessageBox::Save) {
                //save current
                updateFilterFromWidget(ui.comboBox->currentIndex(),ui.comboBox->currentText());
                saveFilter();
        }

        bool ok;
        QString newFilterName = QInputDialog::getText(this, tr("New Filter"), tr("Please enter a new unique filter name:"), QLineEdit::Normal, tr("filter name") , &ok);

        //cancel
        if (!ok) {
                return;
        }

        //Check Empty
        if (newFilterName.isEmpty()) {
                QMessageBox::warning(this,tr("Warning !"), tr("[%1-%2]\nThe name is empty. Nothing happened !").arg(__FILE__).arg(__LINE__) , QMessageBox::Ok);
                return;
        }

        // Check for duplicates
        if (ui.comboBox->findText(newFilterName) > -1) {
                QMessageBox::warning(this,tr("Warning !"), tr("[%1-%2]\nFilter name must be unique !").arg(__FILE__).arg(__LINE__) , QMessageBox::Ok);
                return;
        }
	//SAVE new filter
        //create row with default values
        if (!filters->newFilter(newFilterName,model->columnCount())) {
                QString err=filters->getErrors().join("\n");
                QMessageBox::warning(this,tr("Warning !"), tr("[%1-%2]\n%3").arg(__FILE__).arg(__LINE__).arg(err) , QMessageBox::Ok);
        }

        //setup new filter
        filters->activateFilter(newFilterName);
        //save with current values
        updateFilterFromWidget(ui.comboBox->currentIndex(),ui.comboBox->currentText());
        //name is not old combo name but new name
        filters->setName(newFilterName);
	//SAVE3        
	//saveFilter();

        //fill comboBox with new values
        filters->loadAllFilters();
        ui.comboBox->clear();
        ui.comboBox->insertItems(0,filters->getFilterList());
        int newIdx=ui.comboBox->findText(newFilterName,Qt::MatchExactly);
        if (newIdx<0) {
                newIdx=0;
        }
        ui.comboBox->setCurrentIndex(newIdx);
}

//---------------------------------------------------------------------

void TWDataZoom::on_deleteFilterPushButton_clicked( )
{
        if (!filters->deleteFilter(ui.comboBox->currentIndex())) {
                QString err=filters->getErrors().join("\n");
                QMessageBox::warning(this,trUtf8("Warning !"), tr("%1").arg(err) , QMessageBox::Ok);
        }
        refreshComboData();
}

//---------------------------------------------------------------------

void TWDataZoom::on_defaultFilterPushButton_clicked( )
{
        for (int i = 0; i <= model->columnCount(); i++) {
                ui.tableView->showColumn(i);
        }
}

//---------------------------------------------------------------------

void TWDataZoom::on_tableView_clicked(const QModelIndex &idx)
{
        //header Filter handling
        int transformedIdx=-1;
        if (!idx.isValid()) {
                return;
        }

        transformedIdx=params->transformHeaderIdxToFilteredIdx(idx.column());
        if (transformedIdx>-1 && ui.masterFilterLineEdit->text().isEmpty()) {
                QStringList hnames=params->getHeaderNames();
                setMasterFilterColumn(hnames[transformedIdx]);
                model->emitRefreshHorizontalHeader(header->count()-1);
        } else {
		//qDebug("TWDataZoom:Can't change master filter column !");
        }
}

//---------------------------------------------------------------------

void TWDataZoom::on_tableView_doubleClicked(const QModelIndex & /*idx*/)
{
        emit accept();
}

//---------------------------------------------------------------------

void TWDataZoom::on_tableView_customContextMenuRequested( const QPoint & /*pos*/ )
{
        toolsMenu->exec(QCursor::pos());
}

//=====================================================================
//=====================================================================
// NON AUTOMATIC SLOTS:
//=====================================================================
//=====================================================================

void TWDataZoom::onselectionModel_SelectionChanged(const QItemSelection & /*selected*/, const QItemSelection & /*deselected*/)
{
        refreshResultSet(TWDataZoom::SelectionChangedRole);
}

//---------------------------------------------------------------------

void TWDataZoom::onPrint_triggered()
{
        TDPreviewDialog *dialog = new TDPreviewDialog (ui.tableView,printer,this);
        dialog->setHeaderText(params->getTitle());
        dialog->print();
        delete dialog;
}

//---------------------------------------------------------------------

void TWDataZoom::onPreview_triggered()
{
        TDPreviewDialog *dialog = new TDPreviewDialog (ui.tableView,printer,this);
        dialog->setHeaderText(params->getTitle());
        dialog->exec();
        delete dialog;
}

//---------------------------------------------------------------------

void TWDataZoom::onPDF_triggered()
{
        TDPreviewDialog *dialog = new TDPreviewDialog (ui.tableView,printer,this);
        dialog->setHeaderText(params->getTitle());
        dialog->exportPdf(params->getTitle());
        delete dialog;
}

//---------------------------------------------------------------------

void TWDataZoom::onCSV_triggered()
{

        CSVExporter *dialog = new CSVExporter (ui.tableView,this);
        dialog->exec();
}

//---------------------------------------------------------------------

void TWDataZoom::onParamAction_triggered()
{
        TPSqlZoomParameter *dialog = new TPSqlZoomParameter (this);
        dialog->exec();
}

//---------------------------------------------------------------------

void TWDataZoom::onShowInactive_triggered(bool si)
{
        setShowInactives(si);
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onShowAggregators_triggered(bool sa)
{
        setShowAggregators(sa);
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeader_customContextMenuRequested ( const QPoint & pos )
{
        menuRequestedHeaderColumn=header->logicalIndexAt(pos);
        QMenu menu;

        header_infoAction = menu.addAction("Current filter is: "+filters->getHeaderFilter(menuRequestedHeaderColumn)+ " ");
        QString ct = params->getColumnType(menuRequestedHeaderColumn);
        header_infoAction->setEnabled(false);
        if (!isFilteringDisabled(menuRequestedHeaderColumn)) {
                header_allAction = menu.addAction("All (*)");
                connect(header_allAction, SIGNAL(triggered()), this, SLOT(onHeaderAll_triggered()));
                if (ct=="QString" || ct=="int" || ct=="qlonglong" || ct=="QDateTime" || ct=="QDate" || ct=="QTime" || ct=="double") {
                        header_likeAction =menu.addAction("~ X...");
                        connect(header_likeAction, SIGNAL(triggered()), this, SLOT(onHeaderLike_triggered()));
                        header_equalAction =menu.addAction("= X...");
                        connect(header_equalAction, SIGNAL(triggered()), this, SLOT(onHeaderEqual_triggered()));
                        header_lessAction =menu.addAction("< X...");
                        connect(header_lessAction, SIGNAL(triggered()), this, SLOT(onHeaderLess_triggered()));
                        header_greaterAction =menu.addAction("> X...");
                        connect(header_greaterAction, SIGNAL(triggered()), this, SLOT(onHeaderGreater_triggered()));
                        header_lessEqualAction = menu.addAction("<= X...");
                        connect(header_lessEqualAction, SIGNAL(triggered()), this, SLOT(onHeaderLessEqual_triggered()));
                        header_greaterEqualAction = menu.addAction(">= X...");
                        connect(header_greaterEqualAction, SIGNAL(triggered()), this, SLOT(onHeaderGreaterEqual_triggered()));
                        header_notEqualAction = menu.addAction("<> X...");
                        connect(header_notEqualAction, SIGNAL(triggered()), this, SLOT(onHeaderNotEqual_triggered()));
                        header_intervalAction = menu.addAction("A <= X <= B");
                        connect(header_intervalAction, SIGNAL(triggered()), this, SLOT(onHeaderInterval_triggered()));
                }

                if (ct=="bool") {
                        header_trueAction = menu.addAction("True");
                        connect(header_trueAction, SIGNAL(triggered()), this, SLOT(onHeaderTrue_triggered()));
                        header_falseAction = menu.addAction("False");
                        connect(header_falseAction, SIGNAL(triggered()), this, SLOT(onHeaderFalse_triggered()));
                }

                menu.addSeparator();
                header_nullAction = menu.addAction("NULL");
                connect(header_nullAction, SIGNAL(triggered()), this, SLOT(onHeaderNull_triggered()));
                header_notNullAction =  menu.addAction("NOT NULL");
                connect(header_notNullAction, SIGNAL(triggered()), this, SLOT(onHeaderNotNull_triggered()));
        }
        if (showAggregators) {
                menu.addSeparator();
                aggregateMenu=menu.addMenu("&Aggregate");
                aggregateGroup = new QActionGroup(this);

                //None
                header_noneAction =aggregateMenu->addAction("No Aggregate");
                header_noneAction->setCheckable(true);
                header_noneAction->setChecked(true);
                aggregateGroup->addAction(header_noneAction);
                connect(header_noneAction, SIGNAL(triggered()), this, SLOT(onHeaderNone_triggered()));
                //Count
                header_countAction =aggregateMenu->addAction("COUNT (Count)");
                header_countAction->setCheckable(true);
                aggregateGroup->addAction(header_countAction);
                connect(header_countAction, SIGNAL(triggered()), this, SLOT(onHeaderCount_triggered()));

                QString currentAgg=filters->getHeaderAggregator(menuRequestedHeaderColumn);
                if (currentAgg=="COUNT") {
                        header_countAction->setChecked(true);
                }

                if (ct=="int" || ct=="qlonglong" || ct=="double") {
                        //Sum
                        header_sumAction =aggregateMenu->addAction("SUM (Sum)");
                        header_sumAction->setCheckable(true);
                        aggregateGroup->addAction(header_sumAction);
                        connect(header_sumAction, SIGNAL(triggered()), this, SLOT(onHeaderSum_triggered()));
                        header_avgAction =aggregateMenu->addAction("AVG (Average)");
                        header_avgAction->setCheckable(true);
                        aggregateGroup->addAction(header_avgAction);
                        connect(header_avgAction, SIGNAL(triggered()), this, SLOT(onHeaderAvg_triggered()));
                        header_minAction =aggregateMenu->addAction("MIN (Minimum)");
                        header_minAction->setCheckable(true);
                        aggregateGroup->addAction(header_minAction);
                        connect(header_minAction, SIGNAL(triggered()), this, SLOT(onHeaderMin_triggered()));
                        header_maxAction =aggregateMenu->addAction("MAX (Maximum)");
                        header_maxAction->setCheckable(true);
                        aggregateGroup->addAction(header_maxAction);
                        connect(header_maxAction, SIGNAL(triggered()), this, SLOT(onHeaderMax_triggered()));

                        if (currentAgg=="SUM") {
                                header_sumAction->setChecked(true);
                        }
                        if (currentAgg=="AVG") {
                                header_avgAction->setChecked(true);
                        }
                        if (currentAgg=="MIN") {
                                header_minAction->setChecked(true);
                        }
                        if (currentAgg=="MAX") {
                                header_maxAction->setChecked(true);
                        }
                }


        }
        menu.addSeparator();
        header_hideAction = menu.addAction("Hide");
        connect(header_hideAction, SIGNAL(triggered()), this, SLOT(onHeaderHide_triggered()));

        menu.exec(header->mapToGlobal(pos));
}

//---------------------------------------------------------------------

void TWDataZoom::onHeader_clicked( int section)
{
        Q_ASSERT(model!=NULL);

        //Rotate order icon on header
        bool rotated=false;

        //if click from another column, start with no order
        if (previousSection!=section) {
                model->setOrderDirection(TCAbstractDataModel::Ascending);
                header->setSortIndicator(section,Qt::AscendingOrder);
        }  else {

                //NONE ->ASC
                if (header->isSortIndicatorShown()==false && !rotated) {
                        model->setOrderDirection(TCAbstractDataModel::Ascending);
                        header->setSortIndicator(section,Qt::AscendingOrder);
                        header->setSortIndicatorShown(true);
                        rotated=true;
                }

                //ASC&VISBLE->NONE
                if (header->isSortIndicatorShown() &&header->sortIndicatorOrder()==Qt::AscendingOrder && !rotated) {
                        header->setSortIndicatorShown(false);
                        model->setOrderDirection(TCAbstractDataModel::None);
                        rotated=true;
                }

                //ASC->DESC
                if ( header->sortIndicatorOrder()==Qt::DescendingOrder && !rotated) {
                        model->setOrderDirection(TCAbstractDataModel::Descending);
                        rotated=true;
                }


        }
        model->setSelectedColumn(section);

        //change masterFilterCombo if LineEdit is empty
        if (ui.masterFilterLineEdit->text()=="*" || ui.masterFilterLineEdit->text().isEmpty()) {
                QString currentHeaderName = params->getHeaderName(section);
                // get transformed idx.
                int idxInCombo = params->transformHeaderIdxToFilteredIdx(section);
                // if found
                if (idxInCombo > -1) {
                        // to save current state
                        setMasterFilterColumn(currentHeaderName);
                }
        }

        previousSection=section;
        //Refresh
        ui.tableView->scrollToTop();
        ui.tableView->clearSelection();
        ui.tableView->reset();
        model->setDoRealFetch(true,TCAbstractDataModel::HeaderClickedRole);
        ui.tableView->selectionModel()->select(model->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        //Restore current cursor position. Block signals because, won't to call onTableViewSelectionModel_currentColumnChanged() function.
        ui.tableView->setCurrentIndex(model->index(0,section));
        refreshResultSet(TWDataZoom::HeaderChangedRole);
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderAll_triggered( )
{
        filters->setHeaderFilter(menuRequestedHeaderColumn,"*");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderTrue_triggered( )
{
        filters->setHeaderFilter(menuRequestedHeaderColumn," = 'true'");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderFalse_triggered( )
{
        filters->setHeaderFilter(menuRequestedHeaderColumn," = 'false'");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderLike_triggered( )
{
        QString filt=commonHeaderFilterDialog();
        if (filt != "*") {
                filters->setHeaderFilter(menuRequestedHeaderColumn," LIKE '"+filt+"'");
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderEqual_triggered( )
{
        QString filt=commonHeaderFilterDialog();
        if (filt != "*") {
                filters->setHeaderFilter(menuRequestedHeaderColumn," = '"+filt+"'");
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderLess_triggered( )
{
        QString filt=commonHeaderFilterDialog();
        if (filt != "*") {
                filters->setHeaderFilter(menuRequestedHeaderColumn," < '"+filt+"'");
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderGreater_triggered( )
{
        QString filt=commonHeaderFilterDialog();
        if (filt != "*") {
                filters->setHeaderFilter(menuRequestedHeaderColumn," > '"+filt+"'");
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderLessEqual_triggered( )
{
        QString filt=commonHeaderFilterDialog();
        if (filt != "*") {
                filters->setHeaderFilter(menuRequestedHeaderColumn," <= '"+filt+"'");
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderGreaterEqual_triggered( )
{
        QString filt=commonHeaderFilterDialog();
        if (filt != "*") {
                filters->setHeaderFilter(menuRequestedHeaderColumn," >= '"+filt+"'");
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderNotEqual_triggered( )
{
        QString filt=commonHeaderFilterDialog();
        if (filt != "*") {
                filters->setHeaderFilter(menuRequestedHeaderColumn," != '"+filt+"'");
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderInterval_triggered( )
{
        QString ct = params->getColumnType(menuRequestedHeaderColumn);
        if ( ct=="QDateTime" || ct=="QDate" ) {

                TWDateInterval *dg = new TWDateInterval(this);
                dg->show();
//                if (dg->exec()==QDialog::Rejected) {
//                      return;
//              }

                int code = dg->getCurrentCode();
                if (code!=0) {
                        filters->setHeaderFilter(menuRequestedHeaderColumn,"×"+QString::number(code));
                } else {
                        QString fromDate = dg->getFromDate().toString(QLocale::system().dateFormat(QLocale::ShortFormat));
                        QString toDate=dg->getToDate().toString(QLocale::system().dateFormat(QLocale::ShortFormat));
                        filters->setHeaderFilter(menuRequestedHeaderColumn,fromDate+"¤"+toDate);
                }
        } else {
                //String Interval dialog
                TDIntervalDialog *dg = new TDIntervalDialog(this);
                if (dg->exec()==QDialog::Rejected) {
                        return;
                }
                filters->setHeaderFilter(menuRequestedHeaderColumn,dg->getFrom()+"¤"+dg->getTo());
        }
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderNull_triggered( )
{
        filters->setHeaderFilter(menuRequestedHeaderColumn," ISNULL " );
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderNotNull_triggered( )
{
        filters->setHeaderFilter(menuRequestedHeaderColumn," NOTNULL ");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderNone_triggered( )
{
        filters->setHeaderAggregator(menuRequestedHeaderColumn,"-");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderMin_triggered( )
{
        filters->setHeaderAggregator(menuRequestedHeaderColumn,"MIN");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderMax_triggered( )
{
        filters->setHeaderAggregator(menuRequestedHeaderColumn,"MAX");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderSum_triggered( )
{
        filters->setHeaderAggregator(menuRequestedHeaderColumn,"SUM");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderAvg_triggered( )
{
        filters->setHeaderAggregator(menuRequestedHeaderColumn,"AVG");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderCount_triggered( )
{
        filters->setHeaderAggregator(menuRequestedHeaderColumn,"COUNT");
        refreshData();
}

//---------------------------------------------------------------------

void TWDataZoom::onHeaderHide_triggered( )
{
        ui.tableView->hideColumn(menuRequestedHeaderColumn);
}

//---------------------------------------------------------------------

QString TWDataZoom::commonHeaderFilterDialog()
{
        QString ct = params->getColumnType(menuRequestedHeaderColumn);
        bool ok;

        //Strings
        if ( ct=="QString") {
                TDTextDialog *dg = new TDTextDialog(this,tr("Please give filter text"), tr("Text:"), tr("Filter text"));
                //Disallow special chars
                QRegExp regexp("(\\w*|\\s*|\\/*|\\=*|\\+*|\\-*|\\_*|\\%*|\\!*|\\?*|\\,*|\\.*|\\:*|\\**|\\{*|\\}*|\\#*|\\&*|\\@*|\\<*|\\>*|\\(*|\\)*|\\\\*)*");
                QValidator *validator = new QRegExpValidator(regexp,this);
                dg->setValidator(validator);
                if (dg->exec()==QDialog::Rejected) {
                        return "*";
                }
                return dg->getText();
        }

        //Integers
        if (ct=="int" || ct=="qlonglong" ) {
                qint64 ret = QInputDialog::getInteger(this, tr("Please give filter number"), tr("Number:"), 0,-2147483647,2147483647,1, &ok);
                if (ok) {
                        return QString::number(ret);
                }
                return "*";

        }

        if (ct=="QDateTime") {
                TDTextDialog *dg = new TDTextDialog(this,tr("Please give ISO date and time"), tr("ISO Date and time (Format: 2001-11-31/22:16)"), "");
                QRegExp regexp("^([0-9]{4}|[*])(-{1}([0-9]{1,2}|[*])){0,2}");
                QValidator *validator = new QRegExpValidator(regexp,this);
                dg->setValidator(validator);
                dg->setInputMask("XXXX-XX-XX/XX:XX;_");
                if (dg->exec()==QDialog::Rejected) {
                        return "*";
                }
                return dg->getText();
        }

        //Date
        if (ct=="QDate") {
                TDTextDialog *dg = new TDTextDialog(this,trUtf8("Please give ISO date"), trUtf8("ISO Date (Format: 2001-11-31)"), "");
                QRegExp regexp("^([0-9]{4}|[*])(-{1}([0-9]{1,2}|[*])){0,2}");
                QValidator *validator = new QRegExpValidator(regexp,this);
                dg->setValidator(validator);
                dg->setInputMask("XXXX-XX-XX;_");
                if (dg->exec()==QDialog::Rejected) {
                        return "*";
                }
                return dg->getText();
        }

        if (ct=="QTime") {
                TDTextDialog *dg = new TDTextDialog(this,tr("Please give time"), tr("Time (Format: 22:10):"), "");
                QRegExp regexp("^([0-9]{2}|[*])(:{1}([0-9]{1,2}|[*])){0,1}");
                QValidator *validator = new QRegExpValidator(regexp,this);
                dg->setValidator(validator);
                dg->setInputMask("XX:XX;_");
                if (dg->exec()==QDialog::Rejected) {
                        return "*";
                }
                return dg->getText();
        }

        //Real
        if ( ct=="double") {
                double ret = QInputDialog::getDouble(this, tr("Please give filter number"),  tr("Number:"), 0,-2147483647,2147483647,1, &ok);
                if (ok) {
                        return QString::number(ret);
                }
                return "*";
        }

        return "*";
}

//=====================================================================
//=====================================================================
//PRIVATE
//=====================================================================
//=====================================================================

//=====================================================================
//sorting emitted by
//=====================================================================
void TWDataZoom::onLayoutChanged()
{
        refreshResultSet(TWDataZoom::LayoutChangedRole);
}

//=====================================================================
//set resultSet by selections
//=====================================================================
void TWDataZoom::refreshResultSet(TWDataZoom::ResultChangeRoles role)
{
        Q_ASSERT(resultSet!=NULL);
        //Resultset handler ,append selected records for resultSet
        QItemSelectionModel *smodel= ui.tableView->selectionModel();
        if (smodel==0) {
                return;
        }
        QModelIndexList selectionList =smodel->selectedRows(); //call rowCount on model  !!!
	QSqlRecord firstRecord;
	resultSet->clear();
        //insert records to resultSet
        for (int i = 0; i < selectionList.size (); i++) {
                QModelIndex idx = selectionList.at (i);
                //cut aggregator Line from resultSet
                if (showAggregators && idx.row() == model->rowCount()-1) {
                } else {
                        QSqlRecord tmpRecord=model->getRecord(idx.row());
			
			if (i==0)
			{
				firstRecord=model->getRecord(idx.row());
			}
                        if (!tmpRecord.isEmpty()) {
                                resultSet->insert(i,tmpRecord);
                        }
                }
        }
	
        emit resultChanged(role);
	
	//signal with simple data
	QString textResult;
	for (int i = 0; i < firstRecord.count(); i++) 
	{
		textResult.append(firstRecord.value(i).toString());	
		if (i!=firstRecord.count()-1)
		{
			textResult.append(";");
		}
	}
	
	emit oneRowResult(textResult);
}

//=====================================================================
//refresh data in view realCount, realFetch
//=====================================================================
void TWDataZoom::refreshData()
{
        //save current item (box)
        QModelIndex currentIdx = ui.tableView->currentIndex();
        model->setDoRealCount(true,TCAbstractDataModel::RefreshDataRole);
        model->setDoRealFetch(true,TCAbstractDataModel::RefreshDataRole);

        //clear multiple selections
        QItemSelectionModel *sm=ui.tableView->selectionModel();
        if (sm) {
                //this will emit SelectionChanged signal, and call refreshResultSet
                //ui.tableView->blockSignals(true);
                sm->select(model->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                //ui.tableView->blockSignals(false);
        }
        //restore current item (box)
        ui.tableView->setCurrentIndex(currentIdx);
        refreshResultSet(TWDataZoom::DataRefreshedRole);
}


//=====================================================================
//Save filter
//=====================================================================
bool TWDataZoom::saveFilter()
{
        if (!filters->saveFilter()) {
                QString err=filters->getErrors().join("\n");
                QMessageBox::warning(this,trUtf8("Warning !"), tr("[%1-%2]\n%3").arg(__FILE__).arg(__LINE__).arg(err) , QMessageBox::Ok);
                return false;
        }
        return true;
}

//=====================================================================
// Widget to filter
//=====================================================================
void TWDataZoom::updateFilterFromWidget(int idx, const QString &name)
{
        filters->changeCurrentIndex(idx);

        //set new values in filter class
        filters->setName(name);

        // save with value
        filters->setMasterFilterText(ui.masterFilterLineEdit->text());

        // save without value
        //filters->setMasterFilterText("");

        filters->setState(header->saveState());

        //First selected row
        QItemSelectionModel *sm = ui.tableView->selectionModel();
        if (sm) {
                QModelIndexList indexes=sm->selectedIndexes();
                if (!indexes.isEmpty()) {
                        QModelIndex idx=indexes.first();
                        filters->setSelectedRow(idx.row());
                } else {
                        filters->setSelectedRow(0);
                }
        }
}

//=====================================================================
//filter to Widget
//=====================================================================
void TWDataZoom::updateWidgetsFromCurrentFilter()
{
        //set masterFilter
        ui.masterFilterLineEdit->blockSignals(true);

        ui.masterFilterLineEdit->setText(filters->getMasterFilterText());
        masterFilterBag=filters->getMasterFilterText();

        ui.masterFilterLineEdit->blockSignals(false);

        //set masterFilterColumn
        QStringList tmp=params->getHeaderNames();
        int idx=tmp.indexOf(filters->getMasterFilterColumn());
        if (idx==-1) {
                idx=0;
        }
        filters->setMasterFilterColumn(tmp[idx]);  //send signal to model

        //set column sizes, order
        header->restoreState(filters->getState());

        //init value (for onHeader_clicked())
        previousSection=header->sortIndicatorSection();

        model->setSelectedColumn(header->sortIndicatorSection());
        //First selected row
        ui.tableView->selectRow(filters->getSelectedRow());

        //if tableView call model->data() function, model will do real Sql Query.
        model->setDoRealFetch(true,TCAbstractDataModel::UpdateWidgetsFromCurrentFilterRole);
}

//=====================================================================
//enable / disable widgets
//=====================================================================
void TWDataZoom::enableFilterWidgets( bool efw)
{
        ui.comboBox->setEnabled(efw);
        ui.saveFilterPushButton->setEnabled(efw);
        ui.newFilterPushButton->setEnabled(efw);
        ui.deleteFilterPushButton->setEnabled(efw);
}

//=====================================================================
//hide widgets
//=====================================================================
void TWDataZoom::setHideFilterWidgets( bool hfw)
{
        hideFilterWidgets=hfw;
        if (hfw) {
                ui.savedFiltersLabel->hide();
                ui.comboBox->hide();
                ui.saveFilterPushButton->hide();
                ui.newFilterPushButton->hide();
                ui.deleteFilterPushButton->hide();
        } else {
                ui.savedFiltersLabel->show();
                ui.comboBox->show();
                ui.saveFilterPushButton->show();
                ui.newFilterPushButton->show();
                ui.deleteFilterPushButton->show();
        }
}

//=====================================================================
//get hide widgets
//=====================================================================
bool TWDataZoom::getHideFilterWidgets()
{
        return hideFilterWidgets;
}

//=====================================================================
//
//=====================================================================
void TWDataZoom::onBeginQuery(QString /*query*/)
{
        setWaitCursor();
        //qDebug()<<query;
}


//=====================================================================
//
//=====================================================================
void TWDataZoom::onEndQuery(QString query)
{
        
	//qDebug()<<"END QUERY:"<<query;
        setArrowCursor();
        refreshResultSet(TWDataZoom::EndQueryRole);

}

//=====================================================================
//set wait cursor and debug query
//=====================================================================
void TWDataZoom::setWaitCursor()
{
        setCursor(Qt::WaitCursor);
        header->setCursor(Qt::WaitCursor);
}

//=====================================================================
//restore cursor
//=====================================================================
void TWDataZoom::setArrowCursor()
{
        setCursor(Qt::ArrowCursor);
        header->setCursor(Qt::ArrowCursor);
}



//=====================================================================
//Called from QTimer via signals & slots. Delayed set.
//=====================================================================
void TWDataZoom::setMasterFilterTxt()
{
        ui.masterFilterLineEdit->setText(masterFilterBag);
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), this,SLOT(setMasterFilterTxt()) );
}

//=====================================================================
// Change master filter column
//=====================================================================
void TWDataZoom::setMasterFilterColumn(QString txt)
{
        ui.masterFilterLabel->setText(txt);
        filters->setMasterFilterColumn(txt);
}


//=====================================================================
//=====================================================================
//EVENTS
//=====================================================================
//=====================================================================

// return true = Eat keypress

bool TWDataZoom::eventFilter(QObject */*obj*/, QEvent *e )
{
        if (e->type()!=QEvent::KeyPress) {
                return false;
        }
        //int currentIndex=ui.masterFilterComboBox->currentIndex();
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        switch (keyEvent->key()) {
        case Qt::Key_Left: {
                //search first searchable column
                // current MasterFilter column hold with model->setMasterFilterColumn();
                QStringList hNames=params->getHeaderNames();
                int currentIdx=hNames.indexOf(filters->getMasterFilterColumn());
                if (currentIdx<0) {
                        break;
                }

                for (int i = currentIdx-1; i >= 0; i--) {
                        int crnt=header->visualIndex(i);
                        //if not found  do nothing
                        if (crnt<0) {
                                break;
                        }

                        QStringList cnames = params->getHeaderNamesFilteredByType();
                        QString txt;
                        txt.clear();
                        int idx=cnames.indexOf(params->getHeaderName(crnt));
                        if (idx!=-1) {
                                //Found
                                ui.tableView->setCurrentIndex(model->index(0,idx));
                                setMasterFilterColumn(cnames[idx]);
                                model->emitRefreshHorizontalHeader(header->count()-1);

                                //reset line edit
                                ui.masterFilterLineEdit->clear();
                                masterFilterBag.clear();
                                break;
                        }
                } //for
                return false;
        }
        case Qt::Key_Right: {
                //search first searchable column
                // current MasterFilter column hold with model->setMasterFilterColumn();
                QStringList hNames=params->getHeaderNames();
                int currentIdx=hNames.indexOf(filters->getMasterFilterColumn());
                if (currentIdx<0) {
                        break;
                }

                for (int i = currentIdx+1; i <= model->columnCount(); i++) {
                        int crnt=header->visualIndex(i);
                        //if not found  do nothing
                        if (crnt<0) {
                                break;
                        }

                        QStringList cnames = params->getHeaderNamesFilteredByType();
                        QString txt;
                        txt.clear(); //?
                        int idx=cnames.indexOf(params->getHeaderName(crnt));
                        if (idx!=-1) {
                                //Found
                                ui.tableView->setCurrentIndex(model->index(0,idx));
                                setMasterFilterColumn(cnames[idx]);
                                model->emitRefreshHorizontalHeader(header->count()-1);

                                //reset line edit
                                ui.masterFilterLineEdit->clear();
                                masterFilterBag.clear();
                                break;
                        }
                } //for

                return false;
        }
        case Qt::Key_Backspace: {
                masterFilterBag.remove(masterFilterBag.length()-1,1);
                connect(timer, SIGNAL(timeout()), this,SLOT(setMasterFilterTxt()) );
                timer->start(200);
                return false;
        }
        case Qt::Key_Return: {
                emit accept();
                return false;
        }
        case Qt::Key_Escape: {
                emit reject();
                return false;
        }
        case Qt::Key_Tab:
        case Qt::Key_Delete:
        case Qt::Key_Insert:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
        case Qt::Key_Select:
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Home:
        case Qt::Key_End:
                return false;

        default: {
                if (!keyEvent->text().isNull()) {
                        masterFilterBag=masterFilterBag+keyEvent->text();
                        connect(timer, SIGNAL(timeout()), this,SLOT(setMasterFilterTxt()) );
                        timer->start(200);
                }
        } //defalut
        } //KeyPress

        //other events
        return false;
}
//---------------------------------------------------------------------

void TWDataZoom::showEvent ( QShowEvent * /*event*/ )
{
        //for designer
        if (model==NULL) {
                return;
        }

        if (doFirstTimeAtShowEvent) {
                doFirstTimeAtShowEvent=false;
                //set model
                if (ui.tableView->model()!=model) {
                        ui.tableView->setModel(model); //here do count()

                        //connect selection signal
                        connect (ui.tableView->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection &, const QItemSelection & ) ), this, SLOT(onselectionModel_SelectionChanged(const QItemSelection &, const QItemSelection &)));
                }
                //fetch data first time, before tableWiew->data()
                model->setDoRealFetch(true,TCAbstractDataModel::ShowEventRole);
                model->data(model->index(filters->getSelectedRow(),0),Qt::DisplayRole); //invite sql fetch

                ui.tableView->setFocus(Qt::MouseFocusReason);
		//select row make count(*) with active filtering
                ui.tableView->selectRow(filters->getSelectedRow()); //emit selectionChanged signal

                //restore selection
                on_tableView_clicked(model->index(filters->getSelectedRow(),0));
                refreshResultSet(TWDataZoom::ShowEventRole);

                //set hidden columns, hideColumn() works only after show()
                for (int i = 0; i <= model->columnCount(); i++) {
                        bool hide=hiddenColumns.value(i,false);
                        if (hide) {
                                ui.tableView->hideColumn(i);
                        }
                }
        }

}
