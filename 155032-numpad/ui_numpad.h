/********************************************************************************
** Form generated from reading UI file 'numpad.ui'
**
** Created: Sun Oct 28 09:45:56 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NUMPAD_H
#define UI_NUMPAD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_numpad
{
public:
    QGridLayout *gridLayout;
    QPushButton *QPB_7;
    QPushButton *QPB_8;
    QPushButton *QPB_9;
    QPushButton *QPB_4;
    QPushButton *QPB_5;
    QPushButton *QPB_6;
    QPushButton *QPB_1;
    QPushButton *QPB_2;
    QPushButton *QPB_3;
    QPushButton *QPB_PLUS_MINUS;
    QPushButton *QPB_0;
    QPushButton *QPB_POINT;

    void setupUi(QWidget *numpad)
    {
        if (numpad->objectName().isEmpty())
            numpad->setObjectName(QString::fromUtf8("numpad"));
        numpad->setWindowModality(Qt::ApplicationModal);
        numpad->resize(248, 328);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(numpad->sizePolicy().hasHeightForWidth());
        numpad->setSizePolicy(sizePolicy);
        numpad->setMinimumSize(QSize(240, 320));
        gridLayout = new QGridLayout(numpad);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setContentsMargins(-1, 4, -1, -1);
        QPB_7 = new QPushButton(numpad);
        QPB_7->setObjectName(QString::fromUtf8("QPB_7"));
        QPB_7->setMinimumSize(QSize(80, 80));
        QFont font;
        font.setPointSize(30);
        QPB_7->setFont(font);

        gridLayout->addWidget(QPB_7, 0, 0, 1, 1);

        QPB_8 = new QPushButton(numpad);
        QPB_8->setObjectName(QString::fromUtf8("QPB_8"));
        QPB_8->setMinimumSize(QSize(80, 80));
        QPB_8->setFont(font);

        gridLayout->addWidget(QPB_8, 0, 1, 1, 1);

        QPB_9 = new QPushButton(numpad);
        QPB_9->setObjectName(QString::fromUtf8("QPB_9"));
        QPB_9->setMinimumSize(QSize(80, 80));
        QPB_9->setFont(font);

        gridLayout->addWidget(QPB_9, 0, 2, 1, 1);

        QPB_4 = new QPushButton(numpad);
        QPB_4->setObjectName(QString::fromUtf8("QPB_4"));
        QPB_4->setMinimumSize(QSize(80, 80));
        QPB_4->setFont(font);

        gridLayout->addWidget(QPB_4, 1, 0, 1, 1);

        QPB_5 = new QPushButton(numpad);
        QPB_5->setObjectName(QString::fromUtf8("QPB_5"));
        QPB_5->setEnabled(true);
        QPB_5->setMinimumSize(QSize(80, 80));
        QPB_5->setFont(font);

        gridLayout->addWidget(QPB_5, 1, 1, 1, 1);

        QPB_6 = new QPushButton(numpad);
        QPB_6->setObjectName(QString::fromUtf8("QPB_6"));
        QPB_6->setMinimumSize(QSize(80, 80));
        QPB_6->setFont(font);

        gridLayout->addWidget(QPB_6, 1, 2, 1, 1);

        QPB_1 = new QPushButton(numpad);
        QPB_1->setObjectName(QString::fromUtf8("QPB_1"));
        QPB_1->setMinimumSize(QSize(80, 80));
        QPB_1->setFont(font);

        gridLayout->addWidget(QPB_1, 2, 0, 1, 1);

        QPB_2 = new QPushButton(numpad);
        QPB_2->setObjectName(QString::fromUtf8("QPB_2"));
        QPB_2->setMinimumSize(QSize(80, 80));
        QPB_2->setFont(font);

        gridLayout->addWidget(QPB_2, 2, 1, 1, 1);

        QPB_3 = new QPushButton(numpad);
        QPB_3->setObjectName(QString::fromUtf8("QPB_3"));
        QPB_3->setMinimumSize(QSize(80, 80));
        QPB_3->setFont(font);

        gridLayout->addWidget(QPB_3, 2, 2, 1, 1);

        QPB_PLUS_MINUS = new QPushButton(numpad);
        QPB_PLUS_MINUS->setObjectName(QString::fromUtf8("QPB_PLUS_MINUS"));
        QPB_PLUS_MINUS->setMinimumSize(QSize(80, 80));
        QPB_PLUS_MINUS->setFont(font);

        gridLayout->addWidget(QPB_PLUS_MINUS, 3, 0, 1, 1);

        QPB_0 = new QPushButton(numpad);
        QPB_0->setObjectName(QString::fromUtf8("QPB_0"));
        QPB_0->setMinimumSize(QSize(80, 80));
        QPB_0->setFont(font);

        gridLayout->addWidget(QPB_0, 3, 1, 1, 1);

        QPB_POINT = new QPushButton(numpad);
        QPB_POINT->setObjectName(QString::fromUtf8("QPB_POINT"));
        QPB_POINT->setMinimumSize(QSize(80, 80));
        QPB_POINT->setFont(font);

        gridLayout->addWidget(QPB_POINT, 3, 2, 1, 1);


        retranslateUi(numpad);

        QMetaObject::connectSlotsByName(numpad);
    } // setupUi

    void retranslateUi(QWidget *numpad)
    {
        numpad->setWindowTitle(QApplication::translate("numpad", "Form", 0, QApplication::UnicodeUTF8));
        numpad->setProperty("current_value", QVariant(QString()));
        QPB_7->setText(QApplication::translate("numpad", "7", 0, QApplication::UnicodeUTF8));
        QPB_8->setText(QApplication::translate("numpad", "8", 0, QApplication::UnicodeUTF8));
        QPB_9->setText(QApplication::translate("numpad", "9", 0, QApplication::UnicodeUTF8));
        QPB_4->setText(QApplication::translate("numpad", "4", 0, QApplication::UnicodeUTF8));
        QPB_5->setText(QApplication::translate("numpad", "5", 0, QApplication::UnicodeUTF8));
        QPB_6->setText(QApplication::translate("numpad", "6", 0, QApplication::UnicodeUTF8));
        QPB_1->setText(QApplication::translate("numpad", "1", 0, QApplication::UnicodeUTF8));
        QPB_2->setText(QApplication::translate("numpad", "2", 0, QApplication::UnicodeUTF8));
        QPB_3->setText(QApplication::translate("numpad", "3", 0, QApplication::UnicodeUTF8));
        QPB_PLUS_MINUS->setText(QApplication::translate("numpad", "+/-", 0, QApplication::UnicodeUTF8));
        QPB_0->setText(QApplication::translate("numpad", "0", 0, QApplication::UnicodeUTF8));
        QPB_POINT->setText(QApplication::translate("numpad", ",", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class numpad: public Ui_numpad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NUMPAD_H
