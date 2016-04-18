#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "globals.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::MakePlot();
    angleCorrectionPlot = new AngleCorrection(this);
    angleCorrectionPlot->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete newCurve;
    delete ePlot;
    delete angleCorrectionPlot;
}

// Initial Plot
void MainWindow::MakePlot()
{
    QVector<double> x, y, t;
    this->newCurve = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(this->newCurve);
    x.append(0);
    y.append(0);
    this->newCurve->setData(x, y);

    ui->customPlot->xAxis->setLabel("X (cm)");
    ui->customPlot->yAxis->setLabel("Y (cm)");
    ui->customPlot->xAxis->setRange(-100, 100);
    ui->customPlot->yAxis->setRange(-100, 100);
    ui->customPlot->replot();

}

// Update Plot - Add X,Y
void MainWindow::UpdatePlot(double x, double y)
{
    this->newCurve->addData(x, y);
    if (x > 90 || y > 90){
        ui->customPlot->yAxis->setRange(-200, 200);
        ui->customPlot->xAxis->setRange(-200, 200);
    }
    if (x > 190 || y > 190){
        ui->customPlot->yAxis->setRange(-400, 400);
        ui->customPlot->xAxis->setRange(-400, 400);
    }
    if (x > 390 || y > 390){
        ui->customPlot->yAxis->setRange(-800, 800);
        ui->customPlot->xAxis->setRange(-800, 800);
    }
    ui->customPlot->replot();
}

// Update The Angle Correction Plot
void MainWindow::AngleCorrectionUpdate(double deltaX, double deltaY, double predX, double predY)
{
    this->angleCorrectionPlot->UpdatePlot(deltaX, deltaY, predX, predY);
}

// Save Plot To File
void MainWindow::SavePlot()
{
    ui->customPlot->savePng("./outputs/"+ QString::fromStdString(currentTime) +"location.png");
}

// Save and Close (will exit program)
void MainWindow::on_close_button_clicked()
{
    this->SavePlot();
    this->close();
    qApp->exit();
}

// Opens Euler Plot
void MainWindow::on_euler_button_clicked()
{
    ePlot = new EulerPlot(this);
    ePlot->show();
}

// Stops Optical Flow Execution
void MainWindow::on_optical_button_clicked()
{
    end_run = 1;
}

// Clear Data From Graph
void MainWindow::on_clear_button_clicked()
{
    this->newCurve->clearData();
}
