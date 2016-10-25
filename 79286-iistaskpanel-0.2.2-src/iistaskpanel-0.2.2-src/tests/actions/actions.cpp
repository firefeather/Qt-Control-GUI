#include "actions.h"

#include <iisTaskPanel>

test::test(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
    ui.setupUi(this);

    iisTaskBox *tb1 = new iisTaskBox(QPixmap(":/images/mac/filenew.png"), "Group of Tasks", true, this);
    ui.panel->addWidget(tb1);

    tb1->addAction(ui.actionEnabled);
    connect(ui.actionEnabled, SIGNAL(triggered()), this, SLOT(task2()));

    tb1->addAction(ui.actionDisabled);
    connect(ui.actionDisabled, SIGNAL(triggered()), this, SLOT(task3()));


    iisTaskBox *tb2 = new iisTaskBox(QPixmap(":/images/mac/filesave.png"), "Another Group", true, this);
    ui.panel->addWidget(tb2);

    QAction *task1 = new QAction("About Qt", this);
    task1->setToolTip("[F1] Some information about Qt");
    task1->setShortcut(QKeySequence("F1"));
    tb2->addAction(task1);
    connect(task1, SIGNAL(triggered()), this, SLOT(task1()));

    tb2->addAction(ui.actionExit);
    connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));


    ui.panel->addStretch();
}

test::~test()
{

}

void test::task1()
{
     QMessageBox::aboutQt(0);
}

void test::task2()
{
    ui.actionDisabled->setEnabled(!ui.actionDisabled->isEnabled());
    ui.actionDisabled->setText(ui.actionDisabled->isEnabled() ? "Changed" : "Disabled again");
}

void test::task3()
{
    if (isMaximized())
        showNormal();
    else
        showMaximized();
}

