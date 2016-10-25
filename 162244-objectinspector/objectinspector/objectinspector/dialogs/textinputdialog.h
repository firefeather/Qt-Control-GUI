#ifndef TEXTINPUTDIALOG_H
#define TEXTINPUTDIALOG_H

#include <QDialog>
#include "ui_textinputdialog.h"
#include "objectinspector_global.h"

namespace Ui {
    class TextInputDialog;
}

class OBJECTINSPECTORSHARED_EXPORT TextInputDialog : public QDialog
{
    Q_OBJECT
    Ui::TextInputDialog *ui;
public:
    explicit TextInputDialog(QWidget *parent = 0);
    ~TextInputDialog();

    void    setText(const QString & Value);
    QString text() const;
};

#endif // TEXTINPUTDIALOG_H
