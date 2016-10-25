#include    <QSpinBox>
#include    <QLineEdit>
#include    <QDoubleSpinBox>
#include    <QComboBox>
#include    <QCheckBox>
#include    <QLabel>
#include    <QFontDialog>
#include    <QColorDialog>
#include    <QPainter>
#include    "textinputdialog.h"
#include    "cobjectinspectortreedelegate.h"
#include    "cobjectinspectortreemodel.h"
#include    "qpropertyeditwidget.h"
#include    "enumproperty.h"
#include    "qcolorwidget.h"
#include    "qfontlabel.h"
#include    "flagpropertynode.h"
#include    "qlongspinbox.h"
#include    "qulongspinbox.h"

CObjectInspectorTreeDelegate::CObjectInspectorTreeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    fmodel = NULL;
    fbrush = QImage(":/icons/mask");
}

QWidget *CObjectInspectorTreeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    if(index.isValid() && fmodel)
    {
        PropertyTreeNode    *   propertyNode = index.data(CObjectInspectorTreeModel::TreeNodeRole).value<PropertyTreeNode*>();
        if(!propertyNode || propertyNode==fmodel->rootNode() || index.column()==0)
            return NULL;

        QWidget *   editor = NULL;

        QVariant    indexValue = index.data(Qt::EditRole);
        if(!indexValue.isValid())
            return  NULL;

        QPropertyEditWidget *   propertyEditorWidget = new QPropertyEditWidget(parent);
        propertyEditorWidget->setLinkedTreeNode(propertyNode);

        if(propertyNode->isExpandable())
        {
            QLabel  *   label = new QLabel;
            label->setWordWrap(false);
            editor = label;
        }
        else
        {
            if(propertyNode->metaProperty().isEnumType())
            {
                QComboBox   *   comboBox = new QComboBox;
                comboBox->blockSignals(true);
                connect(comboBox,SIGNAL(currentIndexChanged(int)),propertyEditorWidget,SIGNAL(valueChanged()));
                comboBox->setFrame(false);
                EnumProperty    *   enumProp = qobject_cast<EnumProperty*>(propertyNode);
                Q_ASSERT(enumProp);
                comboBox->addItems(enumProp->enumeratedProperties());
                comboBox->blockSignals(false);
                editor = comboBox;
            }
            else
            switch (propertyNode->value().type())
            {
            case QVariant::Bool:
            {
                QCheckBox   *   checkBox = new QCheckBox;
                connect(checkBox,SIGNAL(clicked()),propertyEditorWidget,SIGNAL(valueChanged()));
                editor = checkBox;
                break;
            }
            case QVariant::Int:
            case QVariant::UInt:
            {
                QSpinBox    *   spinBox = new QSpinBox;
                spinBox->setFrame(false);
                connect(spinBox,SIGNAL(valueChanged(int)),propertyEditorWidget,SIGNAL(valueChanged()));
                editor = spinBox;
                break;
            }
            case QVariant::LongLong:
            {
                QLongSpinBox    *   spinBox = new QLongSpinBox;
                spinBox->setFrame(false);
                connect(spinBox,SIGNAL(valueChanged(qint64)),propertyEditorWidget,SIGNAL(valueChanged()));
                editor = spinBox;
                break;
            }
            case QVariant::ULongLong:
            {
                QULongSpinBox    *   spinBox = new QULongSpinBox;
                spinBox->setFrame(false);
                connect(spinBox,SIGNAL(valueChanged(quint64)),propertyEditorWidget,SIGNAL(valueChanged()));
                editor = spinBox;
                break;
            }
            case QVariant::String:
            {
                QLineEdit   *   lineEdit = new QLineEdit;
                lineEdit->setFrame(false);
                connect(lineEdit,SIGNAL(textChanged(QString)),propertyEditorWidget,SIGNAL(valueChanged()));
                connect(lineEdit,SIGNAL(textEdited(QString)),propertyEditorWidget,SIGNAL(valueChanged()));
                connect(propertyEditorWidget,SIGNAL(editButtonClicked()),this,SLOT(showExternalTextEditor()));
                propertyEditorWidget->setEditable(propertyNode->isEditable());
                editor = lineEdit;
                break;
            }
            case QVariant::Double:
            {
                QDoubleSpinBox  *   spinBox = new QDoubleSpinBox;
                spinBox->setFrame(false);
                connect(spinBox,SIGNAL(valueChanged(double)),propertyEditorWidget,SIGNAL(valueChanged()));
                editor = spinBox;
            }
                break;
            case    QVariant::Color:
            {
                QColorWidget    *   colorWidget = new QColorWidget;
                editor = colorWidget;
                connect(colorWidget,SIGNAL(colorChanged()),propertyEditorWidget,SIGNAL(valueChanged()));
                connect(propertyEditorWidget,SIGNAL(editButtonClicked()),this,SLOT(showColorDialog()));
                propertyEditorWidget->setEditable(propertyNode->isEditable());
            }
                break;
            case    QVariant::Font:
            {
                QFontLabel  *   fontLabel = new QFontLabel;
                editor = fontLabel;
                connect(fontLabel,SIGNAL(fontChanged()),propertyEditorWidget,SIGNAL(valueChanged()));
                connect(propertyEditorWidget,SIGNAL(editButtonClicked()),this,SLOT(showFontDialog()));
                propertyEditorWidget->setEditable(propertyNode->isEditable());
            }
                break;
            default:
                break;
            }
        }

        Q_ASSERT(editor);
        propertyEditorWidget->setAutoFillBackground(true);
        propertyEditorWidget->setResatable(propertyNode->isResetable());
        propertyEditorWidget->setWidget(editor);
        editor->setEnabled(propertyNode->isWritable());
        connect(propertyEditorWidget,SIGNAL(valueChanged()),this,SLOT(commitEditorData()));
        connect(propertyEditorWidget,SIGNAL(resetButtonClicked()),propertyNode,SIGNAL(resetProperty()));
        return  propertyEditorWidget;
    }
    return  NULL;
}

void CObjectInspectorTreeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.isValid() && fmodel)
    {
        PropertyTreeNode * propertyNode = index.data(CObjectInspectorTreeModel::TreeNodeRole).value<PropertyTreeNode*>();
        if(!propertyNode || propertyNode==fmodel->rootNode() || index.column()==0) return;

        QPropertyEditWidget *   propertyEditorWidget = qobject_cast<QPropertyEditWidget*>(editor);
        Q_ASSERT(propertyEditorWidget);

        QWidget *   editorWidget = propertyEditorWidget->widget();
        if(!editorWidget)
            return;

        editorWidget->blockSignals(true);
        editorWidget->setUpdatesEnabled(false);

        QVariant    indexValue = index.data(Qt::EditRole);

        if(!propertyNode->isExpandable())
        {
            if(propertyNode->metaProperty().isEnumType())
            {
                QComboBox   *   comboBox = dynamic_cast<QComboBox*>(editorWidget);
                Q_ASSERT(comboBox);
                EnumProperty    *   enumProp = qobject_cast<EnumProperty*>(propertyNode);
                Q_ASSERT(enumProp);
                comboBox->setCurrentIndex(enumProp->indexOfValue());
            }
            else
            switch (indexValue.type())
            {
            case QVariant::Bool:
            {
                QCheckBox   *   checkBox = dynamic_cast<QCheckBox*>(editorWidget);
                Q_ASSERT(checkBox);
                checkBox->setChecked(indexValue.toBool());
            }
                break;
            case QVariant::LongLong:
            {
                QLongSpinBox    *   spinBox = dynamic_cast<QLongSpinBox*>(editorWidget);
                Q_ASSERT(spinBox);
                spinBox->setValue(indexValue.toLongLong());
            }
                break;
            case QVariant::ULongLong:
            {
                QULongSpinBox    *   spinBox = dynamic_cast<QULongSpinBox*>(editorWidget);
                Q_ASSERT(spinBox);
                spinBox->setValue(indexValue.toULongLong());
            }
                break;
            case QVariant::Int:
            {
                QSpinBox    *   spinBox = dynamic_cast<QSpinBox*>(editorWidget);
                Q_ASSERT(spinBox);
                spinBox->setRange(INT_MIN,INT_MAX);
                spinBox->setValue(indexValue.toInt());
            }
                break;
            case QVariant::UInt:
            {
                QSpinBox    *   spinBox = dynamic_cast<QSpinBox*>(editorWidget);
                Q_ASSERT(spinBox);
                spinBox->setRange(0,INT_MAX);
                spinBox->setValue(indexValue.toInt());
            }
                break;
            case    QVariant::String:
            {
                QLineEdit   *   lineEdit = dynamic_cast<QLineEdit*>(editorWidget);
                Q_ASSERT(lineEdit);
                lineEdit->setText(indexValue.toString());
            }
                break;
            case QVariant::Double:
            {
                QDoubleSpinBox    *   spinBox = dynamic_cast<QDoubleSpinBox*>(editorWidget);
                Q_ASSERT(spinBox);
                spinBox->setMinimum(INT_MIN);
                spinBox->setMaximum(INT_MAX);
                spinBox->setValue(indexValue.toFloat());
            }
                break;
            case    QVariant::Color:
            {
                QColorWidget    *   colorWidget = dynamic_cast<QColorWidget*>(editorWidget);
                Q_ASSERT(colorWidget);
                colorWidget->setColor(indexValue.value<QColor>());
            }
                break;
            case    QVariant::Font:
            {
                QFontLabel  *   fontLabel = dynamic_cast<QFontLabel*>(editorWidget);
                Q_ASSERT(fontLabel);
                fontLabel->setCurrentFont(indexValue.value<QFont>());
            }
                break;
            default:
                break;
            }
        }
        else
        {
            QLabel  *   label = dynamic_cast<QLabel*>(editorWidget);
            Q_ASSERT(label);
            label->setText(propertyNode->stringValue());
        }

        editorWidget->setUpdatesEnabled(true);
        editorWidget->blockSignals(false);
    }
}

void CObjectInspectorTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant    indexValue = index.data(Qt::EditRole);
    switch (indexValue.type())
    {
    case QVariant::Color:
    {
        QRect   rect(option.rect);
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(fbrush);
        painter->drawRect(rect);
        painter->setBrush(indexValue.value<QColor>());
        painter->drawRect(rect);
        painter->restore();
    }
        break;
    default:
        QStyledItemDelegate::paint(painter,option,index);
        break;
    }
}

void CObjectInspectorTreeDelegate::setModel(CObjectInspectorTreeModel *Model)
{
    fmodel = Model;
}

void CObjectInspectorTreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid())
    {
        QPropertyEditWidget *   propertyEditor = qobject_cast<QPropertyEditWidget*>(editor);
        Q_ASSERT(propertyEditor);

        PropertyTreeNode    *   propertyNode = index.data(CObjectInspectorTreeModel::TreeNodeRole).value<PropertyTreeNode*>();
        Q_ASSERT(propertyNode);

        if(propertyNode->isExpandable())
            return;

        const   QMetaProperty   &   indexMetaProperty = propertyNode->metaProperty();
        if(indexMetaProperty.isFlagType())
        {
            QCheckBox   *   checkBox = qobject_cast<QCheckBox*>(propertyEditor->widget());
            Q_ASSERT(checkBox);
            model->setData(index,checkBox->isChecked());
            return;
        }

        if(indexMetaProperty.isEnumType())
        {
            QComboBox   *   comboBox = qobject_cast<QComboBox*>(propertyEditor->widget());
            Q_ASSERT(comboBox);
            model->setData(index,comboBox->currentIndex());
            return;
        }

        QVariant::Type t = propertyNode->value().type();
        switch (t)
        {
        case    QVariant::Bool:
        {
            QCheckBox   *   checkBox = qobject_cast<QCheckBox*>(propertyEditor->widget());
            Q_ASSERT(checkBox);
            model->setData(index,checkBox->isChecked());
        }
            break;
        case    QVariant::Int:
        case    QVariant::UInt:
        {
            QSpinBox    *   spinBox = qobject_cast<QSpinBox*>(propertyEditor->widget());
            Q_ASSERT(spinBox);
            model->setData(index,spinBox->value());
        }
            break;
        case    QVariant::LongLong:
        {
            QLongSpinBox    *   spinBox = qobject_cast<QLongSpinBox*>(propertyEditor->widget());
            Q_ASSERT(spinBox);
            model->setData(index,spinBox->value());
        }
            break;
        case    QVariant::ULongLong:
        {
            QULongSpinBox    *   spinBox = qobject_cast<QULongSpinBox*>(propertyEditor->widget());
            Q_ASSERT(spinBox);
            model->setData(index,spinBox->value());
        }
            break;
        case    QVariant::String:
        {
            QLineEdit   *   lineEdit = qobject_cast<QLineEdit*>(propertyEditor->widget());
            Q_ASSERT(lineEdit);
            model->setData(index,lineEdit->text());
        }
            break;
        case    QVariant::Double:
        {
            QDoubleSpinBox  *   spinBox = qobject_cast<QDoubleSpinBox*>(propertyEditor->widget());
            Q_ASSERT(spinBox);
            model->setData(index,spinBox->value());
        }
            break;
        case    QVariant::Color:
        {
            QColorWidget    *   colorWidget = qobject_cast<QColorWidget*>(propertyEditor->widget());
            Q_ASSERT(colorWidget);
            model->setData(index,QVariant::fromValue(colorWidget->color()));
        }
            break;
        case    QVariant::Font:
        {
            QFontLabel  *   fontLabel = qobject_cast<QFontLabel*>(propertyEditor->widget());
            Q_ASSERT(fontLabel);
            model->setData(index,QVariant::fromValue(fontLabel->currentFont()));
        }
            break;
        default:
            break;
        }
    }
}

void CObjectInspectorTreeDelegate::commitEditorData()
{
    QPropertyEditWidget *   senderWidget = qobject_cast<QPropertyEditWidget*>(sender());
    Q_ASSERT(senderWidget);
    emit        commitData(senderWidget);
}

void CObjectInspectorTreeDelegate::showExternalTextEditor()
{
    QPropertyEditWidget *   senderWidget = qobject_cast<QPropertyEditWidget*>(sender());
    Q_ASSERT(senderWidget);

    QLineEdit   *   lineEdit = qobject_cast<QLineEdit*>(senderWidget->widget());
    Q_ASSERT(lineEdit);

    TextInputDialog textDlg;
    textDlg.setText(lineEdit->text());
    if(textDlg.exec()==QDialog::Accepted)
    {
        lineEdit->setText(textDlg.text());
    }
}

void CObjectInspectorTreeDelegate::showFontDialog()
{
    QPropertyEditWidget *   senderWidget = qobject_cast<QPropertyEditWidget*>(sender());
    Q_ASSERT(senderWidget);

    QFontLabel  *   fontLabel = qobject_cast<QFontLabel*>(senderWidget->widget());
    Q_ASSERT(fontLabel);

    QFontDialog    fontDialog;
    fontDialog.setOption(QFontDialog::DontUseNativeDialog);
    fontDialog.setCurrentFont(fontLabel->currentFont());
    if(fontDialog.exec()==QDialog::Accepted)
        fontLabel->setCurrentFont(fontDialog.currentFont());
}

void CObjectInspectorTreeDelegate::showColorDialog()
{
    QPropertyEditWidget *   senderWidget = qobject_cast<QPropertyEditWidget*>(sender());
    Q_ASSERT(senderWidget);

    QColorWidget    *   colorWidget = qobject_cast<QColorWidget*>(senderWidget->widget());
    Q_ASSERT(colorWidget);

    QColorDialog    colorDialog;
    colorDialog.setOption(QColorDialog::DontUseNativeDialog,true);
    colorDialog.setOption(QColorDialog::ShowAlphaChannel);
    colorDialog.setCurrentColor(colorWidget->color());
    if(colorDialog.exec()==QDialog::Accepted)
        colorWidget->setColor(colorDialog.currentColor());
}

