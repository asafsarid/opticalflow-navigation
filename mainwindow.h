//#ifndef MAINWINDOW_H
//#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "eulerplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SavePlot();
    void UpdatePlot(double x, double y);

private slots:
    void MakePlot();
    void on_close_button_clicked();

    void on_euler_button_clicked();

    void on_optical_button_clicked();

private:
    QCPCurve *newCurve;
    EulerPlot *ePlot;
    Ui::MainWindow *ui;
};

//#endif // MAINWINDOW_H
