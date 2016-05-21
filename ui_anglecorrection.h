/********************************************************************************
** Form generated from reading UI file 'anglecorrection.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANGLECORRECTION_H
#define UI_ANGLECORRECTION_H

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

class Ui_AngleCorrection
{
public:
    QVBoxLayout *verticalLayout;
    QCustomPlot *correctXPlot;
    QCustomPlot *correctYPlot;
    QPushButton *pushButton;

    void setupUi(QDialog *AngleCorrection)
    {
        if (AngleCorrection->objectName().isEmpty())
            AngleCorrection->setObjectName(QStringLiteral("AngleCorrection"));
        AngleCorrection->resize(646, 478);
        verticalLayout = new QVBoxLayout(AngleCorrection);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        correctXPlot = new QCustomPlot(AngleCorrection);
        correctXPlot->setObjectName(QStringLiteral("correctXPlot"));

        verticalLayout->addWidget(correctXPlot);

        correctYPlot = new QCustomPlot(AngleCorrection);
        correctYPlot->setObjectName(QStringLiteral("correctYPlot"));

        verticalLayout->addWidget(correctYPlot);

        pushButton = new QPushButton(AngleCorrection);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(AngleCorrection);

        QMetaObject::connectSlotsByName(AngleCorrection);
    } // setupUi

    void retranslateUi(QDialog *AngleCorrection)
    {
        AngleCorrection->setWindowTitle(QApplication::translate("AngleCorrection", "Dialog", 0));
        pushButton->setText(QApplication::translate("AngleCorrection", "Save && Close", 0));
    } // retranslateUi

};

namespace Ui {
    class AngleCorrection: public Ui_AngleCorrection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANGLECORRECTION_H
