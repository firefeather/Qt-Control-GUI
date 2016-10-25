#include "textinputdialog.h"

TextInputDialog::TextInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextInputDialog)
{
    ui->setupUi(this);
}

TextInputDialog::~TextInputDialog()
{
    delete ui;
}

void TextInputDialog::setText(const QString &Value)
{
    ui->textEdit->setPlainText(Value);
}

QString TextInputDialog::text() const
{
    return  ui->textEdit->document()->toPlainText();
}
