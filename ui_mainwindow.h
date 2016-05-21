/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QPushButton *euler_button;
    QPushButton *optical_button;
    QPushButton *save_button;
    QCustomPlot *customPlot;
    QPushButton *reset_button;
    QPushButton *clear_button;
    QPushButton *close_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(665, 473);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        euler_button = new QPushButton(centralwidget);
        euler_button->setObjectName(QStringLiteral("euler_button"));

        gridLayout->addWidget(euler_button, 1, 3, 1, 1);

        optical_button = new QPushButton(centralwidget);
        optical_button->setObjectName(QStringLiteral("optical_button"));

        gridLayout->addWidget(optical_button, 1, 2, 1, 1);

        save_button = new QPushButton(centralwidget);
        save_button->setObjectName(QStringLiteral("save_button"));

        gridLayout->addWidget(save_button, 1, 1, 1, 1);

        customPlot = new QCustomPlot(centralwidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));

        gridLayout->addWidget(customPlot, 0, 1, 1, 3);

        reset_button = new QPushButton(centralwidget);
        reset_button->setObjectName(QStringLiteral("reset_button"));

        gridLayout->addWidget(reset_button, 2, 2, 1, 1);

        clear_button = new QPushButton(centralwidget);
        clear_button->setObjectName(QStringLiteral("clear_button"));

        gridLayout->addWidget(clear_button, 2, 3, 1, 1);

        close_button = new QPushButton(centralwidget);
        close_button->setObjectName(QStringLiteral("close_button"));

        gridLayout->addWidget(close_button, 2, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 665, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        euler_button->setText(QApplication::translate("MainWindow", "Euler", 0));
        optical_button->setText(QApplication::translate("MainWindow", "Stop Optical Flow", 0));
        save_button->setText(QApplication::translate("MainWindow", "Save", 0));
        reset_button->setText(QApplication::translate("MainWindow", "Reset", 0));
        clear_button->setText(QApplication::translate("MainWindow", "Clear", 0));
        close_button->setText(QApplication::translate("MainWindow", "Close", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
