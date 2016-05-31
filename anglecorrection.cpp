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
    pen.setStyle(Qt::DotLine);
    pen.setColor(Qt::blue);
    pen.setWidth(2);

    QPen pen2;
    pen2.setColor(Qt::blue);
    pen2.setWidth(2);

    //Delta X
    ui->correctXPlot->addGraph();
    ui->correctXPlot->graph(0)->addData(0,0);
    ui->correctXPlot->graph(0)->setPen(pen2);
    ui->correctXPlot->graph(0)->setName("DeltaX");

    pen2.setColor(Qt::red);
    //Delta Y
    ui->correctYPlot->addGraph();
    ui->correctYPlot->graph(0)->addData(0,0);
    ui->correctYPlot->graph(0)->setPen(pen2);
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
    ui->correctXPlot->xAxis->setLabel("Time (1/10 Sec)");
    ui->correctXPlot->xAxis->setLabelFont(QFont("Helvetica",15));
    ui->correctXPlot->yAxis->setLabelFont(QFont("Helvetica",15));
    ui->correctXPlot->xAxis->setTickLabelFont(QFont("Helvetica",12));
    ui->correctXPlot->yAxis->setTickLabelFont(QFont("Helvetica",12));
    ui->correctXPlot->yAxis->setLabel("Distance X (cm)");
    ui->correctXPlot->yAxis->setRange(-50, 50);
    ui->correctYPlot->xAxis->setLabel("Time (1/10 Sec)");
    ui->correctYPlot->yAxis->setLabelFont(QFont("Helvetica",15));
    ui->correctYPlot->xAxis->setLabelFont(QFont("Helvetica",15));
    ui->correctYPlot->xAxis->setTickLabelFont(QFont("Helvetica",12));
    ui->correctYPlot->yAxis->setTickLabelFont(QFont("Helvetica",12));
    ui->correctYPlot->yAxis->setLabel("Distance Y (cm)");
    ui->correctYPlot->yAxis->setRange(-50, 50);

    //Add Legend
    ui->correctXPlot->legend->setVisible(true);
    ui->correctXPlot->legend->setFont(QFont("Helvetica", 15));
    ui->correctYPlot->legend->setVisible(true);
    ui->correctYPlot->legend->setFont(QFont("Helvetica", 15));

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
    QString file_name_y = "./outputs/" + QString::fromStdString(currentTime) + "correctionY.png";
    ui->correctXPlot->savePng(file_name_x);
    ui->correctYPlot->savePng(file_name_y);
}
