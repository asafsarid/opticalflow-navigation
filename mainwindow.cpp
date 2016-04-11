#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "qcustomplot.h"


#include "globals.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::MakePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete newCurve;
}

void MainWindow::MakePlot()
{
    QVector<double> x, y, t;
    this->newCurve = new QCPCurve(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(this->newCurve);
    x.append(0);
    y.append(0);
    this->newCurve->setData(x, y);
//    this->newCurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 1.5));

    ui->customPlot->xAxis->setLabel("X");
    ui->customPlot->yAxis->setLabel("Y");
    ui->customPlot->xAxis->setRange(-400, 400);
    ui->customPlot->yAxis->setRange(-300, 300);
    ui->customPlot->replot();

//    QVector<double> v;
//    QVector<double> t;
//    QFile textFile("./outputs/pitch.txt");
//    if(textFile.open(QIODevice::ReadOnly))
//    {
//        double d;
//        int i = 0;
//        QTextStream textStream(&textFile);
//        while (!textStream.atEnd()) {
//            QString line = textStream.readLine(1);
//            if(textStream.status() == QTextStream::Ok){
//                QStringList lstLine = textStream.split(QRegExp("\\s"));
//                       lstLine.at(0);
//                v.append(d);
//                t.append(i);
//                i++;
//            }
//            else
//                break;
//        }
//    ui->customPlot->addGraph();

//    ui->customPlot->graph(0)->setData(t,v);
//    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
//    ui->customPlot->xAxis->setLabel("Time");
//    ui->customPlot->yAxis->setLabel("Pitch");
//    ui->customPlot->graph(0)->rescaleAxes();
//    ui->customPlot->yAxis->setRange(-180, 180);
//    ui->customPlot->replot();
//    }
}



void MainWindow::UpdatePlot(double x, double y)
{
    this->newCurve->addData(x, y);
    ui->customPlot->replot();
}

void MainWindow::SavePlot()
{
    ui->customPlot->savePng("./outputs/"+ QString::fromStdString(currentTime) +"location.png");
}

void MainWindow::on_close_button_clicked()
{
    this->SavePlot();
    this->close();
    qApp->exit();
}

void MainWindow::on_euler_button_clicked()
{
    ePlot = new EulerPlot(this);
    ePlot->show();
}

void MainWindow::on_optical_button_clicked()
{
    end_run = 1;
}
