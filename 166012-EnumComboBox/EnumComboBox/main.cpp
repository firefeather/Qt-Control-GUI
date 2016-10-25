#include "main.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QComboBox *c = NEMO_NEW_ENUM_QCOMBOBOX(ExampleClass, Type, NULL);

    c->show();
    return a.exec();
}
