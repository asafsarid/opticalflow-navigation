/********************************************************************************
** Form generated from reading UI file 'eulerplot.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EULERPLOT_H
#define UI_EULERPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_EulerPlot
{
public:
    QVBoxLayout *verticalLayout;
    QCustomPlot *customEulerPlot;
    QPushButton *pushButton;

    void setupUi(QDialog *EulerPlot)
    {
        if (EulerPlot->objectName().isEmpty())
            EulerPlot->setObjectName(QStringLiteral("EulerPlot"));
        EulerPlot->resize(662, 446);
        verticalLayout = new QVBoxLayout(EulerPlot);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        customEulerPlot = new QCustomPlot(EulerPlot);
        customEulerPlot->setObjectName(QStringLiteral("customEulerPlot"));

        verticalLayout->addWidget(customEulerPlot);

        pushButton = new QPushButton(EulerPlot);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(EulerPlot);

        QMetaObject::connectSlotsByName(EulerPlot);
    } // setupUi

    void retranslateUi(QDialog *EulerPlot)
    {
        EulerPlot->setWindowTitle(QApplication::translate("EulerPlot", "Dialog", 0));
        pushButton->setText(QApplication::translate("EulerPlot", "Save && Close", 0));
    } // retranslateUi

};

namespace Ui {
    class EulerPlot: public Ui_EulerPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EULERPLOT_H
