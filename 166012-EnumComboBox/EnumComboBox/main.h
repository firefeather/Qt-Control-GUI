#ifndef MAIN_H
#define MAIN_H
#include "EnumComboBox.h"

class ExampleClass : public QObject
{
    Q_OBJECT
public:
    enum Type
    {
        Apple,
        Pear,
        Banana,
        Pineapple
    };

    Q_ENUMS(Type)
};

Q_DECLARE_METATYPE(ExampleClass::Type)
NEMO_DECLARE_ENUM_QCOMBOBOX(ExampleClass, Type)

#endif // MAIN_H
