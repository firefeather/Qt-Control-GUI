#ifndef COBJECTINSPECTORTREEDELEGATE_H
#define COBJECTINSPECTORTREEDELEGATE_H

#include "objectinspector_global.h"
#include <QStyledItemDelegate>
#include <QGraphicsObject>
#include <QMetaProperty>
#include <QDoubleSpinBox>
#include "objectinspector_global.h"

class   CObjectInspectorTreeModel;
class   QRailItem;
class   PropertyTreeNode;

class OBJECTINSPECTORSHARED_EXPORT CObjectInspectorTreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    static  const   int             fprivateRows = 4;
    QBrush                          fbrush;
    CObjectInspectorTreeModel   *   fmodel;

public:


    explicit CObjectInspectorTreeDelegate(QObject *parent = 0);
    
    QWidget *   createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void        setEditorData(QWidget *editor, const QModelIndex &index) const;
    void        paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void        setModel(CObjectInspectorTreeModel *Model);
    void        setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private slots:

    void        commitEditorData();
    void        showExternalTextEditor();
    void        showFontDialog();
    void        showColorDialog();
};

#endif // COBJECTINSPECTORTREEDELEGATE_H
