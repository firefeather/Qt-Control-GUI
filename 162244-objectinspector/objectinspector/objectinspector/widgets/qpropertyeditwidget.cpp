#include <QResizeEvent>
#include <QPainter>
#include "qpropertyeditwidget.h"

QPropertyEditWidget::QPropertyEditWidget(QWidget *parent) :
    QWidget(parent)
{
    fwidget = NULL;
    flinkedNode = NULL;

    flayout = new QHBoxLayout(this);
    flayout->setContentsMargins(0,0,0,0);
    flayout->setSpacing(1);

    fcontainer = new QWidget(this);
    fcontainer->setAutoFillBackground(true);
    fcontainer->setContentsMargins(0,0,0,0);

    feditButton = new QToolButton(this);
    feditButton->setIcon(QIcon(":/icons/pencil"));
    feditButton->setMaximumSize(26,26);
    feditButton->setMinimumSize(10,10);
    feditButton->setVisible(false);

    fresetButton = new QToolButton(this);
    fresetButton->setText("<");
    fresetButton->setMaximumSize(15,26);
    fresetButton->setMinimumSize(5,10);
    fresetButton->setVisible(false);

    flayout->addWidget(fcontainer,1);
    flayout->addWidget(feditButton);
    flayout->addWidget(fresetButton);

    setFocusProxy(feditButton);
    setLayout(flayout);
    connect(feditButton,SIGNAL(clicked()),this,SIGNAL(editButtonClicked()));
    connect(fresetButton,SIGNAL(clicked()),this,SIGNAL(resetButtonClicked()));

    recalculate();
}

QWidget *QPropertyEditWidget::widget() const
{
    return fwidget;
}

void QPropertyEditWidget::setEditable(bool Value)
{
    feditButton->setVisible(Value);
    recalculate();
}

void QPropertyEditWidget::setResatable(bool Value)
{
    fresetButton->setVisible(Value);
    recalculate();
}

void QPropertyEditWidget::setLinkedTreeNode(PropertyTreeNode *Node)
{
    flinkedNode = Node;
}

PropertyTreeNode *QPropertyEditWidget::linkedTreeNode() const
{
    return  flinkedNode;
}

void QPropertyEditWidget::setWidget(QWidget *Widget)
{
    if(fwidget!=Widget)
    {
        if(fwidget)
        {
            fwidget->setParent(NULL);
        }
        fwidget = Widget;
        if(fwidget)
        {
            fwidget->setParent(fcontainer);
            recalculate();
        }
    }
}


void QPropertyEditWidget::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event)
    recalculate();
}

void QPropertyEditWidget::recalculate()
{
    if(fwidget)
    {
        int w = width();
        if(fresetButton->isVisibleTo(this))
            w-=fresetButton->width();
         if(feditButton->isVisibleTo(this))
            w-=feditButton->width();
        fwidget->resize(w,height());
    }
}

