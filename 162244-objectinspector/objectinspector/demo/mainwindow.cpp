#include "mainwindow.h"
#include <QTableView>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new CObjectInspectorTreeModel(this);
    delegate = new CObjectInspectorTreeDelegate(this);
    delegate->setModel(model);

    ui->treeView->setModel(model);
    ui->treeView->setItemDelegateForColumn(0,NULL);
    ui->treeView->setItemDelegateForColumn(1,delegate);

    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    QList<QObject*> list;
    list<<this;

    model->setObjects(list);

}

MainWindow::~MainWindow()
{
    delete ui;
}
