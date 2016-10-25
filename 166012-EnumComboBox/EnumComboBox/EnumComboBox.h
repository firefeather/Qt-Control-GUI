#pragma once

#include <QObject>
#include <QWidget>
#include <QVariant>
#include <QComboBox>
#include <QDebug>
#include <QMetaEnum>

#define NEMO_DECLARE_ENUM_QCOMBOBOX(ClassName, EnumName) \
    class Nemo##ClassName##EnumName##QComboBox: public QComboBox \
    { \
        Q_OBJECT \
        Q_PROPERTY(ClassName::EnumName value READ value WRITE setValue USER true) \
    public: \
        Nemo##ClassName##EnumName##QComboBox(QWidget *widget = 0) : QComboBox(widget) \
        { \
            populateList(); \
        } \
        ClassName::EnumName value() const \
        { \
            return (ClassName::EnumName)currentIndex(); \
        } \
        void setValue(ClassName::EnumName p) \
        { \
            if((ClassName::EnumName)currentIndex() != p) \
            {\
                setCurrentIndex(int(p)); \
            } \
        } \
    private: \
        void populateList() \
        { \
            const QMetaObject& mo = ClassName::staticMetaObject; \
            int index = mo.indexOfEnumerator(#EnumName); \
            QMetaEnum metaEnum = mo.enumerator(index); \
            for(int i=0; i<metaEnum.keyCount(); ++i) \
                addItem(metaEnum.valueToKey(i)); \
        }\
    };

#define NEMO_NEW_ENUM_QCOMBOBOX(ClassName, EnumName, parent) \
    new Nemo##ClassName##EnumName##QComboBox (parent)

#define nemoRegisterEditorFactory(factory, ClassName, EnumName) \
    factory->registerEditor(QVariant::fromValue((ClassName::EnumName)0).userType(), new QStandardItemEditorCreator<Nemo##ClassName##EnumName##QComboBox>());
