#ifndef QPROPERTYEDITWIDGET_H
#define QPROPERTYEDITWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include "objectinspector_global.h"

class   PropertyTreeNode;

class OBJECTINSPECTORSHARED_EXPORT QPropertyEditWidget : public QWidget
{
    Q_OBJECT

    QToolButton         *   feditButton;
    QToolButton         *   fresetButton;
    QWidget             *   fwidget;
    QWidget             *   fcontainer;
    QHBoxLayout         *   flayout;
    PropertyTreeNode    *   flinkedNode;

protected:

    void    resizeEvent(QResizeEvent *event);
    void    recalculate();

public:
    explicit QPropertyEditWidget(QWidget *parent = 0);
    
    QWidget *   widget()    const;

    void    setEditable(bool Value = true);
    void    setResatable(bool Value = true);
    void    setLinkedTreeNode(PropertyTreeNode * Node);
    PropertyTreeNode    *   linkedTreeNode()    const;

signals:

    void    editButtonClicked();
    void    resetButtonClicked();
    void    valueChanged();
    
public slots:

    void    setWidget(QWidget * Widget);
};

#endif // QPROPERTYEDITWIDGET_H
