#include "anglecorrection.h"
#include "ui_anglecorrection.h"
#include "globals.h"

AngleCorrection::AngleCorrection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AngleCorrection)
{
    ui->setupUi(this);
    AngleCorrection::MakePlot();
}

AngleCorrection::~AngleCorrection()
{
    delete ui;
}

//Initial Plot
void AngleCorrection::MakePlot()
{
    this->time = 0;
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setColor(Qt::blue);

    //Delta X
    ui->correctXPlot->addGraph();
    ui->correctXPlot->graph(0)->addData(0,0);
    ui->correctXPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->correctXPlot->graph(0)->setName("DeltaX");

    //Delta Y
    ui->correctYPlot->addGraph();
    ui->correctYPlot->graph(0)->addData(0,0);
    ui->correctYPlot->graph(0)->setPen(QPen(Qt::red));
    ui->correctYPlot->graph(0)->setName("DeltaY");

    //Predicted X
    ui->correctXPlot->addGraph();
    ui->correctXPlot->graph(1)->addData(0,0);
    ui->correctXPlot->graph(1)->setPen(pen);
    ui->correctXPlot->graph(1)->setName("PredX");

    //Predicted Y
    pen.setColor(Qt::red);
    ui->correctYPlot->addGraph();
    ui->correctYPlot->graph(1)->addData(0,0);
    ui->correctYPlot->graph(1)->setPen(pen);
    ui->correctYPlot->graph(1)->setName("PredY");

    //Set Labels and Ranges
    ui->correctXPlot->xAxis->setLabel("Time (Sec)");
    ui->correctXPlot->yAxis->setLabel("Distance X (cm)");
    ui->correctXPlot->yAxis->setRange(-50, 50);
    ui->correctYPlot->xAxis->setLabel("Time (Sec)");
    ui->correctYPlot->yAxis->setLabel("Distance Y (cm)");
    ui->correctYPlot->yAxis->setRange(-50, 50);

    //Add Legend
    ui->correctXPlot->legend->setVisible(true);
    ui->correctXPlot->legend->setFont(QFont("Helvetica", 9));
    ui->correctYPlot->legend->setVisible(true);
    ui->correctYPlot->legend->setFont(QFont("Helvetica", 9));

    ui->correctXPlot->replot();
    ui->correctYPlot->replot();
}

//Update Plot
void AngleCorrection::UpdatePlot(double deltaX, double deltaY, double predX, double predY)
{
    this->time++;
    ui->correctXPlot->graph(0)->addData(time,deltaX);
    ui->correctYPlot->graph(0)->addData(time,deltaY);
    ui->correctXPlot->graph(1)->addData(time,predX);
    ui->correctYPlot->graph(1)->addData(time,predY);

    //Make plot move with time (100 samples)
    ui->correctXPlot->xAxis->setRange(time+0.25, 100, Qt::AlignRight);
    ui->correctYPlot->xAxis->setRange(time+0.25, 100, Qt::AlignRight);

    ui->correctXPlot->replot();
    ui->correctYPlot->replot();
}

//Save and Close
void AngleCorrection::on_pushButton_clicked()
{
    QString file_name_x = "./outputs/" + QString::fromStdString(currentTime) + "correctionX.png";
    QString file_name_y = "./outputs/" + QString::fromStdString(currentTime) + "correctionX.png";
    ui->correctXPlot->savePng(file_name_x);
    ui->correctYPlot->savePng(file_name_y);
    this->close();
}
