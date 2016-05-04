#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include "globals.h"

using namespace std;

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

    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->xAxis2->setLabel("miau");
    ui->customPlot->yAxis2->setLabel("miau2");
    ui->customPlot->xAxis2->setRange(-70,70);
    ui->customPlot->yAxis2->setRange(-1, 1);

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    //ui->customPlot->axisRect()->setupFullAxesBox();

    //connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

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

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis,ui->customPlot->yAxis);
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  }
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis,ui->customPlot->yAxis);
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  }
  else if (ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis2,ui->customPlot->yAxis2);
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis2->orientation());
  }
  else if (ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis)){
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis2,ui->customPlot->yAxis2);
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis2->orientation());
  }
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}
